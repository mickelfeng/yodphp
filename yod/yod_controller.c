/*
  +----------------------------------------------------------------------+
  | Yod Framework as PHP extension										 |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,		 |
  | that is bundled with this package in the file LICENSE, and is		 |
  | available through the world-wide-web at the following url:			 |
  | http://www.php.net/license/3_01.txt									 |
  | If you did not receive a copy of the PHP license and are unable to	 |
  | obtain it through the world-wide-web, please send a note to			 |
  | license@php.net so we can mail you a copy immediately.				 |
  +----------------------------------------------------------------------+
  | Author: Baoqiang Su  <zmrnet@qq.com>								 |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "main/SAPI.h"
#include "Zend/zend_interfaces.h"
#include "ext/standard/php_string.h"

#include "php_yod.h"
#include "yod_application.h"
#include "yod_request.h"
#include "yod_controller.h"
#include "yod_action.h"
#include "yod_widget.h"
#include "yod_model.h"
#include "yod_dbmodel.h"

zend_class_entry *yod_controller_ce;

/** {{{ ARG_INFO
 */
ZEND_BEGIN_ARG_INFO_EX(yod_controller_construct_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, route)
	ZEND_ARG_INFO(0, action)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_controller_init_arginfo, 0, 0, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_controller_run_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_controller_config_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_controller_import_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, alias)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_controller_model_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, config)
	ZEND_ARG_INFO(0, dbmod)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_controller_assign_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_controller_render_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, view)
	ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_controller_display_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, view)
	ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_controller_widget_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, route)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_controller_forward_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, route)
	ZEND_ARG_INFO(0, exited)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_controller_redirect_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, url)
	ZEND_ARG_INFO(0, code)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_controller_error404_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, html)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ static int yod_valid_varname(char *varname, int varname_len)
*/
static int yod_valid_varname(char *varname, int varname_len) {
	int i, ch;

	if (!varname || !varname_len) {
		return 0;
	}
	
	/* These are allowed as first char: [a-zA-Z_\x7f-\xff] */
	ch = (int)((unsigned char *)varname)[0];
	if (varname[0] != '_' &&
		(ch < 65  /* A    */ || /* Z    */ ch > 90)  &&
		(ch < 97  /* a    */ || /* z    */ ch > 122) &&
		(ch < 127 /* 0x7f */ || /* 0xff */ ch > 255)
	) {
		return 0;
	}

	/* And these as the rest: [a-zA-Z0-9_\x7f-\xff] */
	if (varname_len > 1) {
		for (i = 1; i < varname_len; i++) {
			ch = (int)((unsigned char *)varname)[i];
			if (varname[i] != '_' &&
				(ch < 48  /* 0    */ || /* 9    */ ch > 57)  &&
				(ch < 65  /* A    */ || /* Z    */ ch > 90)  &&
				(ch < 97  /* a    */ || /* z    */ ch > 122) &&
				(ch < 127 /* 0x7f */ || /* 0xff */ ch > 255)
			) {
				return 0;
			}
		}
	}
	return 1;
}
/* }}} */

/** {{{ static int yod_extract(zval *array TSRMLS_DC)
*/
static int yod_extract(zval *array TSRMLS_DC) {
	zval **data, *value;
	char *key;
	uint key_len;
	ulong num_key;
	HashPosition pos;

	if (!array || Z_TYPE_P(array) != IS_ARRAY) {
		return 0;
	}

	if (!EG(active_symbol_table)) {
		zend_rebuild_symbol_table(TSRMLS_C);
	}

	zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(array), &pos);
	while (zend_hash_get_current_data_ex(Z_ARRVAL_P(array), (void **)&data, &pos) == SUCCESS) {

		if (zend_hash_get_current_key_ex(Z_ARRVAL_P(array), &key, &key_len, &num_key, 0, &pos) != HASH_KEY_IS_STRING) {
			continue;
		}

		if (key_len == sizeof("GLOBALS")-1 && !strcmp(key, "GLOBALS")) {
			continue;
		}

		if (key_len == sizeof("this")-1  && !strcmp(key, "this") && EG(scope) && EG(scope)->name_length != 0) {
			continue;
		}

		if (key_len && yod_valid_varname(key, key_len)) {
			MAKE_STD_ZVAL(value);
			*value = **data;
			zval_copy_ctor(value);
			ZEND_SET_SYMBOL_WITH_LENGTH(EG(active_symbol_table), key, key_len, value, 1, 0);
		}

		zend_hash_move_forward_ex(Z_ARRVAL_P(array), &pos);
	}
}
/* }}} */

/** {{{ static void yod_controller_run(yod_controller_t *object TSRMLS_DC)
*/
static void yod_controller_run(yod_controller_t *object TSRMLS_DC) {
	yod_request_t *request;
	zval *p_name, *p_action, *p_params, *target;
	char *method, *classname, *classpath;
	size_t method_len, classname_len;
	zend_class_entry **pce = NULL;
	
#if PHP_YOD_DEBUG
	yod_debugf("yod_controller_run()");
#endif

	p_action = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_action"), 1 TSRMLS_CC);
	if (p_action && Z_TYPE_P(p_action) == IS_STRING) {
		zend_str_tolower(Z_STRVAL_P(p_action), Z_STRLEN_P(p_action));
		method_len = spprintf(&method, 0, "%saction", Z_STRVAL_P(p_action));
		classname_len = spprintf(&classname, 0, "%sAction", Z_STRVAL_P(p_action));
		zend_update_property(Z_OBJCE_P(object), object, ZEND_STRL("_action"), p_action TSRMLS_CC);
	} else {
		method_len = 11;
		method = "indexaction";
		classname_len = 11;
		classname = "indexAction";
		zend_update_property_string(Z_OBJCE_P(object), object, ZEND_STRL("_action"), "index" TSRMLS_CC);
	}

	request = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_request"), 1 TSRMLS_CC);
	p_params = zend_read_property(yod_request_ce, request, ZEND_STRL("params"), 1 TSRMLS_CC);

	if (zend_hash_exists(&(Z_OBJCE_P(object)->function_table), method, method_len + 1)) {
		zend_call_method(&object, Z_OBJCE_P(object), NULL, method, method_len, NULL, 1, p_params, NULL TSRMLS_CC);
	} else {
		p_name = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_name"), 1 TSRMLS_CC);
		if (p_name && Z_TYPE_P(p_name) == IS_STRING) {
			zend_str_tolower(Z_STRVAL_P(p_name), Z_STRLEN_P(p_name));
			zend_update_property(Z_OBJCE_P(object), object, ZEND_STRL("_name"), p_name TSRMLS_CC);
			spprintf(&classpath, 0, "%s/actions/%s/%s.php", yod_runpath(TSRMLS_CC), Z_STRVAL_P(p_name), classname);
		} else {
			zend_update_property_string(Z_OBJCE_P(object), object, ZEND_STRL("_name"), "index" TSRMLS_CC);
			spprintf(&classpath, 0, "%s/actions/index/%s.php", yod_runpath(TSRMLS_CC), classname);
		}
		
		if (VCWD_ACCESS(classpath, F_OK) == 0) {
			yod_include(classpath, NULL, 1 TSRMLS_CC);
			if (zend_lookup_class_ex(classname, classname_len, 0, &pce TSRMLS_CC) == SUCCESS) {
				object_init_ex(target, *pce);
				if (zend_hash_exists(&(*pce)->function_table, ZEND_STRS(ZEND_CONSTRUCTOR_FUNC_NAME))) {
					zend_call_method_with_1_params(&target, *pce, &(*pce)->constructor, ZEND_CONSTRUCTOR_FUNC_NAME, NULL, request);
					zend_call_method_with_1_params(&target, *pce, NULL, "run", NULL, p_params);
				}
			} else {
				php_error_docref(NULL TSRMLS_CC, E_ERROR, "Class '%s' not found", classname);
			}
		} else {
			if (zend_hash_exists(&(Z_OBJCE_P(object))->function_table, ZEND_STRS("erroraction"))) {
				zend_call_method_with_1_params(&object, Z_OBJCE_P(object), NULL, "erroraction", NULL, p_params);
			} else {
				yod_request_erroraction(request TSRMLS_CC);
			}
		}

		efree(classpath);
	}

	efree(classname);
	efree(method);
}
/* }}} */

/** {{{ void yod_controller_construct(yod_controller_t *object, yod_request_t *request, char *action, uint action_len TSRMLS_DC)
*/
void yod_controller_construct(yod_controller_t *object, yod_request_t *request, char *action, uint action_len TSRMLS_DC) {
	zval *p_name, *p_action, *tpl_view, *tpl_data;
	char *name, *tpl_path;

	if (!object) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "Cannot instantiate abstract class Yod_Controller");
		return;
	}

#if PHP_YOD_DEBUG
	if (instanceof_function(Z_OBJCE_P(object), yod_action_ce TSRMLS_CC)) {
		yod_debugf("yod_action_construct()");
	} else {
		yod_debugf("yod_controller_construct()");
	}
#endif

	p_name = zend_read_property(yod_request_ce, request, ZEND_STRL("controller"), 1 TSRMLS_CC);
	if (p_name && Z_TYPE_P(p_name) == IS_STRING) {
		name = estrndup(Z_STRVAL_P(p_name), Z_STRLEN_P(p_name));
		zend_str_tolower(name, Z_STRLEN_P(p_name));
		zend_update_property_string(Z_OBJCE_P(object), object, ZEND_STRL("_name"), name TSRMLS_CC);
	}

	if (action_len) {
		zend_update_property_stringl(Z_OBJCE_P(object), object, ZEND_STRL("_action"), action, action_len TSRMLS_CC);
	} else {
		p_action = zend_read_property(yod_request_ce, request, ZEND_STRL("action"), 1 TSRMLS_CC);
		if (p_action && Z_TYPE_P(p_action) == IS_STRING) {
			zend_str_tolower(Z_STRVAL_P(p_action), Z_STRLEN_P(p_action));
			zend_update_property(Z_OBJCE_P(object), object, ZEND_STRL("_action"), p_action TSRMLS_CC);
		}
	}

	zend_update_property(Z_OBJCE_P(object), object, ZEND_STRL("_request"), request TSRMLS_CC);

	MAKE_STD_ZVAL(tpl_data);
	array_init(tpl_data);
	spprintf(&tpl_path, 0, "%s/views", yod_runpath(TSRMLS_CC));
	MAKE_STD_ZVAL(tpl_view);
	array_init(tpl_view);
	add_assoc_zval(tpl_view, "tpl_data", tpl_data);
	add_assoc_string(tpl_view, "tpl_path", tpl_path, 1);
	add_assoc_string(tpl_view, "tpl_file", "", 1);
	zend_update_property(Z_OBJCE_P(object), object, ZEND_STRL("_view"), tpl_view TSRMLS_CC);
	zval_ptr_dtor(&tpl_view);
	efree(tpl_path);

#if PHP_YOD_DEBUG
	if (instanceof_function(Z_OBJCE_P(object), yod_action_ce TSRMLS_CC)) {
		yod_debugf("yod_action_init()");
	} else {
		yod_debugf("yod_controller_init()");
	}
#endif

	if (zend_hash_exists(&(Z_OBJCE_P(object)->function_table), ZEND_STRS("init"))) {
		zend_call_method_with_0_params(&object, Z_OBJCE_P(object), NULL, "init", NULL);
	}

#if PHP_YOD_DEBUG
	if (instanceof_function(Z_OBJCE_P(object), yod_action_ce TSRMLS_CC)) {
		yod_debugf("yod_action_run()");
	}
#endif

	if (instanceof_function(Z_OBJCE_P(object), yod_action_ce TSRMLS_CC)) {
		zend_call_method_with_0_params(&object, Z_OBJCE_P(object), NULL, "run", NULL);
	} else {
		yod_controller_run(object TSRMLS_CC);
	}

}
/* }}} */

/** {{{ static int yod_controller_assign(yod_controller_t *object, zval *name, zval *value TSRMLS_DC)
*/
static int yod_controller_assign(yod_controller_t *object, zval *name, zval *value TSRMLS_DC) {
	zval **data, **tpl_data;
	zval *tpl_view;
	char *key;
	uint key_len;
	ulong num_key;
	HashPosition pos;

#if PHP_YOD_DEBUG
	if (instanceof_function(Z_OBJCE_P(object), yod_widget_ce TSRMLS_CC)) {
		yod_debugf("yod_widget_assign()");
	} else if (instanceof_function(Z_OBJCE_P(object), yod_action_ce TSRMLS_CC)) {
		yod_debugf("yod_action_assign()");
	} else {
		yod_debugf("yod_controller_assign()");
	}
#endif

	if (!name) {
		return 0;
	}

	tpl_view = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_view"), 1 TSRMLS_CC);
	if (!tpl_view || Z_TYPE_P(tpl_view) != IS_ARRAY) {
		return 0;
	}

	if (zend_hash_find(Z_ARRVAL_P(tpl_view), "tpl_data", sizeof("tpl_data"), (void**) &tpl_data) == SUCCESS) {
		if (!tpl_data || Z_TYPE_PP(tpl_data) != IS_ARRAY) {
			MAKE_STD_ZVAL(*tpl_data);
			array_init(*tpl_data);
		}
	}

	if (Z_TYPE_P(name) == IS_ARRAY) {
		zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(name), &pos);
		while (zend_hash_get_current_data_ex(Z_ARRVAL_P(name), (void **)&data, &pos) == SUCCESS) {
			zval_add_ref(data);
			switch (zend_hash_get_current_key_ex(Z_ARRVAL_P(name), &key, &key_len, &num_key, 0, &pos)) {
				case HASH_KEY_IS_LONG:
					add_index_zval(*tpl_data, num_key, *data);
					break;
				case HASH_KEY_IS_STRING:
					add_assoc_zval_ex(*tpl_data, key, key_len, *data);
					break;
			}
			zend_hash_move_forward_ex(Z_ARRVAL_P(name), &pos);
		}
	} else if (Z_TYPE_P(name) == IS_STRING) {
		if (!value) {
			MAKE_STD_ZVAL(value);
			ZVAL_NULL(value);
		}
		zval_add_ref(&value);
		add_assoc_zval_ex(*tpl_data, Z_STRVAL_P(name), Z_STRLEN_P(name) + 1, value);
	}

	return 1;
}
/* }}} */


/** {{{ static int yod_controller_render(yod_controller_t *object, zval *response, char *view, uint view_len, zval *data TSRMLS_DC)
*/
static int yod_controller_render(yod_controller_t *object, zval *response, char *view, uint view_len, zval *data TSRMLS_DC) {
	zend_class_entry *scope;

	zval *p_action, *p_name, *tpl_view, *buffer;
	zval **tpl_path, **tpl_data;

	char *tpl_file, *key;
	uint tpl_file_len, key_len;

	ulong num_key;
	HashPosition pos;

#if PHP_YOD_DEBUG
	if (instanceof_function(Z_OBJCE_P(object), yod_widget_ce TSRMLS_CC)) {
		yod_debugf("yod_widget_render(%s)", view ? view : "");
	} else if (instanceof_function(Z_OBJCE_P(object), yod_action_ce TSRMLS_CC)) {
		yod_debugf("yod_action_render(%s)", view ? view : "");
	} else {
		yod_debugf("yod_controller_render(%s)", view ? view : "");
	}
#endif

	if (view && view_len) {
		view = estrndup(view, view_len);
	} else {
		p_action = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_action"), 1 TSRMLS_CC);
		if (p_action || Z_TYPE_P(p_action) == IS_STRING) {
			view_len = Z_STRLEN_P(p_action);
			view = Z_STRVAL_P(p_action);
		}
	}
	if (view && view_len) {
		view = php_str_to_str(view, view_len, "..", 2, "", 0, &view_len);
		view = php_str_to_str(view, view_len, "\\", 1, "/", 1, &view_len);
		while (strstr(view, "//")) {
			view = php_str_to_str(view, view_len, "//", 2, "/", 1, &view_len);
		}
	} else {
		view_len = 5;
		view = "index";
	}

	if (view[0] != '/') {
		p_name = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_name"), 1 TSRMLS_CC);
		if (p_name && Z_TYPE_P(p_name) == IS_STRING) {
			view_len = spprintf(&view, 0, "/%s/%s", Z_STRVAL_P(p_name), view);
		} else {
			view_len = spprintf(&view, 0, "/%s", view);
		}
	}

	zend_str_tolower(view, view_len);

	tpl_view = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_view"), 1 TSRMLS_CC);
	if (!tpl_view || Z_TYPE_P(tpl_view) != IS_ARRAY) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unavailable property %s::$_view", Z_OBJCE_P(object)->name);
		ZVAL_NULL(response);
		return 0;
	}

	if (zend_hash_find(Z_ARRVAL_P(tpl_view), "tpl_path", sizeof("tpl_path"), (void**) &tpl_path) == SUCCESS) {
		if (!tpl_path || Z_TYPE_PP(tpl_path) != IS_STRING) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unavailable property %s::$_view", Z_OBJCE_P(object)->name);
			ZVAL_NULL(response);
			return 0;
		}
	}

	tpl_file_len = spprintf(&tpl_file, 0, "%s%s.php", Z_STRVAL_PP(tpl_path), view);
	add_assoc_stringl(tpl_view, "tpl_file", tpl_file, tpl_file_len, 1);

	// tpl_data
	if (zend_hash_find(Z_ARRVAL_P(tpl_view), "tpl_data", sizeof("tpl_data"), (void**) &tpl_data) == SUCCESS) {
		if (tpl_data && Z_TYPE_PP(tpl_data) == IS_ARRAY) {
			(void)yod_extract(*tpl_data TSRMLS_DC);
		}
	}

	if (data && Z_TYPE_P(data) == IS_ARRAY) {
		(void)yod_extract(data TSRMLS_DC);
	}

	// response
	if (VCWD_ACCESS(tpl_file, F_OK) == 0) {
		if (php_start_ob_buffer(NULL, 0, 1 TSRMLS_CC) != SUCCESS) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "ob_start failed");
			ZVAL_NULL(response);
			return 0;
		}

		scope = EG(scope);
		EG(scope) = Z_OBJCE_P(object);

		yod_include(tpl_file, NULL, 1 TSRMLS_CC);
		
		EG(scope) = scope;

		if (php_ob_get_buffer(response TSRMLS_CC) != SUCCESS) {
			ZVAL_NULL(response);
		}

		if (OG(ob_nesting_level)) {
			php_end_ob_buffer(0, 0 TSRMLS_CC);
		}

		return 1;
	} else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "View '%s' not found", tpl_file);
		ZVAL_NULL(response);
		return 0;
	}

}
/* }}} */

/** {{{ static int yod_controller_display(yod_controller_t *object, char *view, size_t view_len, zval *data TSRMLS_DC)
*/
static int yod_controller_display(yod_controller_t *object, char *view, size_t view_len, zval *data TSRMLS_DC) {
	zval *response = NULL;

#if PHP_YOD_DEBUG
	if (instanceof_function(Z_OBJCE_P(object), yod_widget_ce TSRMLS_CC)) {
		yod_debugf("yod_widget_display(%s)", view ? view : "");
	} else if (instanceof_function(Z_OBJCE_P(object), yod_action_ce TSRMLS_CC)) {
		yod_debugf("yod_action_display(%s)", view ? view : "");
	} else {
		yod_debugf("yod_controller_display(%s)", view ? view : "");
	}
#endif

	if (!SG(headers_sent)) {
		sapi_header_line ctr = {0};
		ctr.line_len = spprintf(&(ctr.line), 0, "Content-type: text/html; charset=%s", yod_charset(TSRMLS_CC));
		sapi_header_op(SAPI_HEADER_REPLACE, &ctr TSRMLS_CC);
		efree(ctr.line);
	}

	MAKE_STD_ZVAL(response);
	if (yod_controller_render(object, response, view, view_len, data TSRMLS_CC)) {
		if (response && Z_TYPE_P(response) == IS_STRING) {
			PHPWRITE(Z_STRVAL_P(response), Z_STRLEN_P(response));
		}
		return 1;
	}

	return 0;
}
/* }}} */

/** {{{ static void yod_controller_widget(yod_controller_t *object, char *route, uint route_len TSRMLS_DC)
*/
static void yod_controller_widget(yod_controller_t *object, char *route, uint route_len TSRMLS_DC) {
	yod_request_t *request;

	zval *params, *target, *pzval;
	zval *ctor, *argv[3], retval;

	char *widget, *action, *classpath;
	char *classname, *key, *value, *token;
	uint classname_len, key_len;

	zend_class_entry **pce = NULL;


#if PHP_YOD_DEBUG
	if (instanceof_function(Z_OBJCE_P(object), yod_widget_ce TSRMLS_CC)) {
		yod_debugf("yod_widget_widget(%s)", route ? route : "");
	} else if (instanceof_function(Z_OBJCE_P(object), yod_action_ce TSRMLS_CC)) {
		yod_debugf("yod_action_widget(%s)", route ? route : "");
	} else {
		yod_debugf("yod_controller_widget(%s)", route ? route : "");
	}
#endif

	request = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_request"), 1 TSRMLS_CC);

	// route
	route = estrndup(route, route_len);
	route = php_str_to_str(route, route_len, "\\", 1, "/", 1, &route_len);
	while (strstr(route, "//")) {
		route = php_str_to_str(route, route_len, "//", 2, "/", 1, &route_len);
	}
	if (*(route + route_len - 1) == '/') {
		*(route + route_len - 1) = '\0';
		route_len--;
	}
	while (*route == '/') {
		route_len--;
		route++;
	}

	// widget
	widget = php_strtok_r(route, "/", &token);
	if (widget) {
		zend_str_tolower(widget, strlen(widget));
		*widget = toupper(*widget);
	} else {
		widget = "Index";
	}

	action = php_strtok_r(NULL, "/", &token);
	if (action) {
		zend_str_tolower(action, strlen(action));
	} else {
		action = "index";
	}

	// params
	MAKE_STD_ZVAL(params);
	array_init(params);

	while (key = php_strtok_r(NULL, "/", &token)) {
		key_len = strlen(key);
		if (key_len) {
			MAKE_STD_ZVAL(pzval);
			value = php_strtok_r(NULL, "/", &token);
			if (value && strlen(value)) {
				ZVAL_STRING(pzval, value, 1);
			} else {
				ZVAL_NULL(pzval);
			}
			zend_hash_update(Z_ARRVAL_P(params), key, key_len + 1, (void **)&pzval, sizeof(zval *), NULL);
			if (HASH_OF(PG(http_globals)[TRACK_VARS_GET])) {
				zend_hash_update(HASH_OF(PG(http_globals)[TRACK_VARS_GET]), key, key_len + 1, (void **)&pzval, sizeof(zval *), NULL);
			}
		}
	}

	// ctor
	MAKE_STD_ZVAL(ctor);
	ZVAL_STRING(ctor, ZEND_CONSTRUCTOR_FUNC_NAME, 1);

	// argv
	MAKE_STD_ZVAL(argv[0]);
	MAKE_STD_ZVAL(argv[1]);
	MAKE_STD_ZVAL(argv[2]);

	ZVAL_ZVAL(argv[0] , request, 1, 1);
	ZVAL_STRING(argv[1], action, 1);
	ZVAL_ZVAL(argv[2] , params, 1, 1);

	MAKE_STD_ZVAL(target);
	classname_len = spprintf(&classname, 0, "%sWidget", widget);
	if (zend_lookup_class_ex(classname, classname_len, 0, &pce TSRMLS_CC) == SUCCESS) {
		object_init_ex(target, *pce);
		if (zend_hash_exists(&(*pce)->function_table, ZEND_STRS(ZEND_CONSTRUCTOR_FUNC_NAME))) {
			if (call_user_function(NULL, &target, ctor, &retval, 3, argv TSRMLS_CC) == FAILURE) {
				php_error_docref(NULL TSRMLS_CC, E_ERROR, "Error calling %s::__construct()", classname);
			}
		}
	} else {
		spprintf(&classpath, 0, "%s/widgets/%sWidget.php", yod_runpath(TSRMLS_CC), widget);
		if (VCWD_ACCESS(classpath, F_OK) == 0) {
			yod_include(classpath, NULL, 1 TSRMLS_CC);
			if (zend_lookup_class_ex(classname, classname_len, 0, &pce TSRMLS_CC) == SUCCESS) {
				object_init_ex(target, *pce);
				if (zend_hash_exists(&(*pce)->function_table, ZEND_STRS(ZEND_CONSTRUCTOR_FUNC_NAME))) {
					if (call_user_function(NULL, &target, ctor, &retval, 3, argv TSRMLS_CC) == FAILURE) {
						php_error_docref(NULL TSRMLS_CC, E_ERROR, "Error calling %s::__construct()", classname);
					}
				}
			} else {
				php_error_docref(NULL TSRMLS_CC, E_ERROR, "Class '%s' not found", classname);
			}
		} else {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Widget '%s' not found", classname);
		}
	}

	zval_ptr_dtor(&argv[0]);
	zval_ptr_dtor(&argv[1]);
	zval_ptr_dtor(&argv[2]);
	zval_dtor(&retval);
}
/* }}} */

/** {{{ static void yod_controller_forward(yod_controller_t *object, char *route, uint route_len, int exited TSRMLS_DC)
*/
static void yod_controller_forward(yod_controller_t *object, char *route, uint route_len, int exited TSRMLS_DC) {
	yod_request_t *request;

#if PHP_YOD_DEBUG
	if (instanceof_function(Z_OBJCE_P(object), yod_widget_ce TSRMLS_CC)) {
		yod_debugf("yod_widget_forward(%s)", route ? route : "");
	} else if (instanceof_function(Z_OBJCE_P(object), yod_action_ce TSRMLS_CC)) {
		yod_debugf("yod_action_forward(%s)", route ? route : "");
	} else {
		yod_debugf("yod_controller_forward(%s)", route ? route : "");
	}
#endif

	zend_update_static_property_long(yod_controller_ce, ZEND_STRL("_forward"), YOD_G(forward) + 1 TSRMLS_CC);
	if (YOD_G(forward)++ > yod_forward(TSRMLS_CC)) {	
		return;
	}

	if (!strstr(route, "/")) {
		zend_update_property_string(Z_OBJCE_P(object), object, ZEND_STRL("_action"), route TSRMLS_CC);
		yod_controller_run(object TSRMLS_CC);
	} else {
		request = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_request"), 1 TSRMLS_CC);
		yod_request_construct(request, route, route_len TSRMLS_CC);
		yod_request_dispatch(request TSRMLS_CC);
	}

	if (exited) {
		YOD_G(exited) = 1;
		zend_set_memory_limit(PG(memory_limit));
		zend_objects_store_mark_destructed(&EG(objects_store) TSRMLS_CC);
		zend_bailout();
	}

}
/* }}} */

/** {{{ proto public Yod_Controller::__construct($request, $action = null)
*/
PHP_METHOD(yod_controller, __construct) {
	yod_request_t *request = NULL;
	char *action = NULL;
	uint action_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|s", &request, &action, action_len) == FAILURE) {
		return;
	}

	(void)yod_controller_construct(getThis(), request, action, action_len TSRMLS_CC);
}
/* }}} */

/** {{{ proto protected Yod_Controller::init()
*/
PHP_METHOD(yod_controller, init) {
	
}
/* }}} */

/** {{{ proto protected Yod_Controller::run()
*/
PHP_METHOD(yod_controller, run) {
	(void)yod_controller_run(getThis() TSRMLS_CC);
}
/* }}} */

/** {{{ proto protected Yod_Controller::config($name = null)
*/
PHP_METHOD(yod_controller, config) {
	char *name = NULL;
	uint name_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &name, &name_len) == FAILURE) {
		return;
	}

	(void)yod_application_config(name, name_len, return_value TSRMLS_CC);
}
/* }}} */

/** {{{ proto protected Yod_Controller::import($alias, $isclass = true)
*/
PHP_METHOD(yod_controller, import) {
	char *alias = NULL;
	uint alias_len = 0;
	int isclass = 1;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|b", &alias, &alias_len, &isclass) == FAILURE) {
		return;
	}

	if (yod_application_import(alias, alias_len, isclass TSRMLS_CC)) {
		RETURN_TRUE;
	}
	RETURN_FALSE;
}
/* }}} */

/** {{{ proto protected Yod_Controller::model($name = '', $config = '', $dbmod = false)
*/
PHP_METHOD(yod_controller, model) {
	yod_controller_t *object;
	zval *z_name = NULL, *config = NULL, *p_name, *retval;
	char *name = NULL;
	uint name_len = 0;
	int dbmod = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|zzb", &z_name, &config, &dbmod) == FAILURE) {
		return;
	}

#if PHP_YOD_DEBUG
	yod_debugf("yod_controller_model()");
#endif

	object = getThis();

	if (z_name) {
		if (Z_TYPE_P(z_name) == IS_BOOL) {
			dbmod = Z_BVAL_P(z_name);
		} else if (Z_TYPE_P(z_name) == IS_STRING) {
			name_len = Z_STRLEN_P(z_name);
			name = estrndup(Z_STRVAL_P(z_name), name_len);
		}
	}
	if (config && Z_TYPE_P(config) == IS_BOOL) {
		dbmod = Z_BVAL_P(config);
		ZVAL_NULL(config);
	}

	if (name_len == 0) {
		p_name = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_name"), 1 TSRMLS_CC);
		if (p_name && Z_TYPE_P(p_name) == IS_STRING) {
			name_len = Z_STRLEN_P(p_name);
			name = estrndup(Z_STRVAL_P(p_name), name_len);
			*name = toupper(*name);
		}
	}

	if (dbmod) {
		yod_dbmodel_getinstance(name, name_len, config, return_value TSRMLS_CC);
	} else {
		yod_model_getinstance(name, name_len, config, return_value TSRMLS_CC);
	}
	if(name){
		efree(name);
	}
}
/* }}} */

/** {{{ proto protected Yod_Controller::assign($name, $value = null)
*/
PHP_METHOD(yod_controller, assign) {
	zval *name = NULL, *value = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|z", &name, &value) == FAILURE) {
		return;
	}

	return_value = getThis();

	(void)yod_controller_assign(return_value, name, value TSRMLS_CC);
}
/* }}} */

/** {{{ proto protected Yod_Controller::render($view = null, $data = array())
*/
PHP_METHOD(yod_controller, render) {
	zval *data = NULL;
	char *view = NULL;
	uint view_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|sz", &view, &view_len, &data) == FAILURE) {
		return;
	}

	(void)yod_controller_render(getThis(), return_value, view, view_len, data TSRMLS_CC);

}
/* }}} */

/** {{{ proto protected Yod_Controller::display($view = null, $data = array())
*/
PHP_METHOD(yod_controller, display) {
	zval *data = NULL;
	char *view = NULL;
	uint view_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|sz", &view, &view_len, &data) == FAILURE) {
		return;
	}

	(void)yod_controller_display(getThis(), view, view_len, data TSRMLS_CC);

}
/* }}} */

/** {{{ proto protected Yod_Controller::widget($route)
*/
PHP_METHOD(yod_controller, widget) {
	char *route;
	uint route_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &route, &route_len) == FAILURE) {
		return;
	}

	(void)yod_controller_widget(getThis(), route, route_len TSRMLS_CC);
}
/* }}} */

/** {{{ proto protected Yod_Controller::forward($route, $exited = true)
*/
PHP_METHOD(yod_controller, forward) {
	char *route;
	uint route_len;
	int exited = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|b", &route, &route_len, &exited) == FAILURE) {
		return;
	}

	(void)yod_controller_forward(getThis(), route, route_len, exited TSRMLS_CC);
}
/* }}} */

/** {{{ proto protected Yod_Controller::redirect($url, $code = 302)
*/
PHP_METHOD(yod_controller, redirect) {
	char *url;
	uint url_len;
	long code = 302;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|l", &url, &url_len, &code) == FAILURE) {
		return;
	}

	if (!SG(headers_sent)) {
		sapi_header_line ctr = {0};

		ctr.response_code = code;
		ctr.line_len = spprintf(&(ctr.line), 0, "Location: %s", url);
		sapi_header_op(SAPI_HEADER_REPLACE, &ctr TSRMLS_CC);
		sapi_send_headers(TSRMLS_C);
		efree(ctr.line);
	}
}
/* }}} */

/** {{{ proto protected Yod_Controller::error404($html = null)
*/
PHP_METHOD(yod_controller, error404) {
	yod_controller_t *object;
	yod_request_t *request;
	zval *html = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z", &html) == FAILURE) {
		return;
	}

	object = getThis();

	request = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_request"), 1 TSRMLS_CC);

	(void)yod_request_error404(request, html TSRMLS_CC);
}
/* }}} */

/** {{{ proto public Yod_Controller::__destruct()
*/
PHP_METHOD(yod_controller, __destruct) {
	
}
/* }}} */

/** {{{ yod_controller_methods[]
*/
zend_function_entry yod_controller_methods[] = {
	PHP_ME(yod_controller, __construct,		yod_controller_construct_arginfo,	ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(yod_controller, init,			yod_controller_init_arginfo,		ZEND_ACC_PROTECTED)
	PHP_ME(yod_controller, run,				yod_controller_run_arginfo,			ZEND_ACC_PROTECTED)
	PHP_ME(yod_controller, config,			yod_controller_config_arginfo,		ZEND_ACC_PROTECTED)
	PHP_ME(yod_controller, import,			yod_controller_import_arginfo,		ZEND_ACC_PROTECTED)
	PHP_ME(yod_controller, model,			yod_controller_model_arginfo,		ZEND_ACC_PROTECTED)
	PHP_ME(yod_controller, assign,			yod_controller_assign_arginfo,		ZEND_ACC_PROTECTED)
	PHP_ME(yod_controller, render,			yod_controller_render_arginfo,		ZEND_ACC_PROTECTED)
	PHP_ME(yod_controller, display,			yod_controller_display_arginfo,		ZEND_ACC_PROTECTED)
	PHP_ME(yod_controller, widget,			yod_controller_widget_arginfo,		ZEND_ACC_PROTECTED)
	PHP_ME(yod_controller, forward,			yod_controller_forward_arginfo,		ZEND_ACC_PROTECTED)
	PHP_ME(yod_controller, redirect,		yod_controller_redirect_arginfo,	ZEND_ACC_PROTECTED)
	PHP_ME(yod_controller, error404,		yod_controller_error404_arginfo,	ZEND_ACC_PROTECTED)
	PHP_ME(yod_controller, __destruct,		NULL,	ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	{NULL, NULL, NULL}
};
/* }}} */


/** {{{ PHP_MINIT_FUNCTION
*/
PHP_MINIT_FUNCTION(yod_controller) {
	zend_class_entry ce;
	zval *view;

	INIT_CLASS_ENTRY(ce, "Yod_Controller", yod_controller_methods);
	yod_controller_ce = zend_register_internal_class(&ce TSRMLS_CC);
	yod_controller_ce->ce_flags |= ZEND_ACC_IMPLICIT_ABSTRACT_CLASS;

	zend_declare_property_long(yod_controller_ce, ZEND_STRL("_forward"), 0, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC TSRMLS_CC);
	zend_declare_property_null(yod_controller_ce, ZEND_STRL("_name"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_controller_ce, ZEND_STRL("_action"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_controller_ce, ZEND_STRL("_request"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_controller_ce, ZEND_STRL("_view"), ZEND_ACC_PROTECTED TSRMLS_CC);

	return SUCCESS;
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
