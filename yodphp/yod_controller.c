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
#include "php_ini.h"
#include "main/SAPI.h"
#include "Zend/zend_alloc.h"
#include "Zend/zend_interfaces.h"
#include "ext/standard/info.h"
#include "ext/standard/php_string.h"
#include "ext/standard/php_math.h"

#include "php_yod.h"
#include "yod_application.h"
#include "yod_request.h"
#include "yod_controller.h"
#include "yod_action.h"

zend_class_entry *yod_controller_ce;

/** {{{ ARG_INFO
 */
ZEND_BEGIN_ARG_INFO_EX(yod_controller_construct_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, route)
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
	ZEND_ARG_INFO(0, prefix)
	ZEND_ARG_INFO(0, config)
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

/** {{{ yod_controller_run(yod_controller_t *object TSRMLS_DC)
*/
void yod_controller_run(yod_controller_t *object TSRMLS_DC) {
	zval *name, *action, *request, *params, *target;
	char *method, *classname, *classpath;
	size_t method_len, classname_len;
	zend_class_entry **pce = NULL;

	action = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_action"), 1 TSRMLS_CC);
	if (action && Z_TYPE_P(action) == IS_STRING) {
		zend_str_tolower(Z_STRVAL_P(action), Z_STRLEN_P(action));
		method_len = spprintf(&method, 0, "%saction", Z_STRVAL_P(action));
		classname_len = spprintf(&classname, 0, "%sAction", Z_STRVAL_P(action));
		zend_update_property(Z_OBJCE_P(object), object, ZEND_STRL("_action"), action TSRMLS_CC);
	} else {
		method_len = 11;
		method = "indexaction";
		classname_len = 11;
		classname = "indexAction";
		zend_update_property_string(Z_OBJCE_P(object), object, ZEND_STRL("_action"), "index" TSRMLS_CC);
	}

	request = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_request"), 1 TSRMLS_CC);
	params = zend_read_property(yod_request_ce, request, ZEND_STRL("params"), 1 TSRMLS_CC);

	if (zend_hash_exists(&(Z_OBJCE_P(object)->function_table), method, method_len + 1)) {
		zend_call_method(&object, Z_OBJCE_P(object), NULL, method, method_len, NULL, 1, params, NULL TSRMLS_CC);
	} else {
		name = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_name"), 1 TSRMLS_CC);
		if (name && Z_TYPE_P(name) == IS_STRING) {
			zend_str_tolower(Z_STRVAL_P(name), Z_STRLEN_P(name));
			zend_update_property(Z_OBJCE_P(object), object, ZEND_STRL("_name"), name TSRMLS_CC);
			spprintf(&classpath, 0, "%s/actions/%s/%s.php", yod_runpath(TSRMLS_CC), Z_STRVAL_P(name), classname);
		} else {
			zend_update_property_string(Z_OBJCE_P(object), object, ZEND_STRL("_name"), "index" TSRMLS_CC);
			spprintf(&classpath, 0, "%s/actions/index/%s.php", yod_runpath(TSRMLS_CC), classname);
		}
		
		if (VCWD_ACCESS(classpath, F_OK) == 0) {
			yod_execute_scripts(classpath, NULL, 1 TSRMLS_CC);
			if (zend_lookup_class_ex(classname, classname_len, 0, &pce TSRMLS_CC) == SUCCESS) {
				object_init_ex(target, *pce);
				if (zend_hash_exists(&(*pce)->function_table, ZEND_STRS(ZEND_CONSTRUCTOR_FUNC_NAME))) {
					zend_call_method_with_1_params(&target, *pce, &(*pce)->constructor, ZEND_CONSTRUCTOR_FUNC_NAME, NULL, request);
					zend_call_method_with_1_params(&target, *pce, NULL, "run", NULL, params);
				}
			} else {
				php_error_docref(NULL TSRMLS_CC, E_ERROR, "Class '%s' not found", classname);
			}
		} else {
			if (zend_hash_exists(&(Z_OBJCE_P(object))->function_table, ZEND_STRS("erroraction"))) {
				zend_call_method_with_1_params(&object, Z_OBJCE_P(object), NULL, "erroraction", NULL, params);
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
	zval *name, *view, *tpl_data;
	char *cname, *tpl_path;

	if (!object) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "Cannot instantiate abstract class Yod_Controller");
		return;
	}

	name = zend_read_property(yod_request_ce, request, ZEND_STRL("controller"), 1 TSRMLS_CC);
	if (name && Z_TYPE_P(name) == IS_STRING) {
		cname = estrndup(Z_STRVAL_P(name), Z_STRLEN_P(name));
		zend_str_tolower(cname, Z_STRLEN_P(name));
		zend_update_property_string(Z_OBJCE_P(object), object, ZEND_STRL("_name"), cname TSRMLS_CC);
	}

	if (action_len) {
		zend_update_property_stringl(Z_OBJCE_P(object), object, ZEND_STRL("_action"), action, action_len TSRMLS_CC);
	} else {
		zval *action = zend_read_property(yod_request_ce, request, ZEND_STRL("action"), 1 TSRMLS_CC);
		if (action && Z_TYPE_P(action) == IS_STRING) {
			zend_str_tolower(Z_STRVAL_P(action), Z_STRLEN_P(action));
			zend_update_property(Z_OBJCE_P(object), object, ZEND_STRL("_action"), action TSRMLS_CC);
		}
	}

	zend_update_property(Z_OBJCE_P(object), object, ZEND_STRL("_request"), request TSRMLS_CC);

	MAKE_STD_ZVAL(tpl_data);
	array_init(tpl_data);
	spprintf(&tpl_path, 0, "%s/views", yod_runpath(TSRMLS_CC));
	MAKE_STD_ZVAL(view);
	array_init(view);
	add_assoc_zval(view, "tpl_data", tpl_data);
	add_assoc_string(view, "tpl_path", tpl_path, 1);
	add_assoc_string(view, "tpl_file", "", 1);
	zend_update_property(Z_OBJCE_P(object), object, ZEND_STRL("_view"), view TSRMLS_CC);
	zval_ptr_dtor(&view);
	efree(tpl_path);

	if (zend_hash_exists(&(Z_OBJCE_P(object)->function_table), ZEND_STRS("init"))) {
		zend_call_method_with_0_params(&object, Z_OBJCE_P(object), NULL, "init", NULL);
	}

	if (instanceof_function(Z_OBJCE_P(object), yod_action_ce TSRMLS_CC)) {
		zend_call_method_with_0_params(&object, Z_OBJCE_P(object), NULL, "run", NULL);
	} else {
		yod_controller_run(object TSRMLS_CC);
	}
}
/* }}} */

/** {{{ proto public Yod_Controller::__construct($request, $action = null)
*/
PHP_METHOD(yod_controller, __construct) {
	zval *request = NULL;
	char *action = NULL;
	uint action_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|s", &request, &action, action_len) == FAILURE) {
		return;
	}

	yod_controller_construct(getThis(), request, action, action_len TSRMLS_CC);
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
	yod_controller_run(getThis() TSRMLS_CC);
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

	yod_application_config(name, name_len, return_value TSRMLS_CC);
}
/* }}} */

/** {{{ proto protected Yod_Controller::import($alias)
*/
PHP_METHOD(yod_controller, import) {
	char *alias = NULL;
	uint alias_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &alias, &alias_len) == FAILURE) {
		return;
	}

	if (yod_application_import(alias, alias_len TSRMLS_CC)) {
		RETURN_TRUE;
	}
	RETURN_FALSE;
}
/* }}} */

/** {{{ proto protected Yod_Controller::model($name = '', $prefix = '', $config = '')
*/
PHP_METHOD(yod_controller, model) {
	
}
/* }}} */

/** {{{ proto protected Yod_Controller::assign($name, $value = null)
*/
PHP_METHOD(yod_controller, assign) {
	
}
/* }}} */

/** {{{ proto protected Yod_Controller::render($view = null, $data = array())
*/
PHP_METHOD(yod_controller, render) {
	
}
/* }}} */

/** {{{ proto protected Yod_Controller::display($view = null, $data = array())
*/
PHP_METHOD(yod_controller, display) {
	
}
/* }}} */

/** {{{ proto protected Yod_Controller::forward($route, $exited = true)
*/
PHP_METHOD(yod_controller, forward) {
	
}
/* }}} */

/** {{{ proto protected Yod_Controller::redirect($url, $code = 302)
*/
PHP_METHOD(yod_controller, redirect) {
	
}
/* }}} */

/** {{{ proto protected Yod_Controller::error404($html = null)
*/
PHP_METHOD(yod_controller, error404) {
	
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
	zend_declare_property_bool(yod_controller_ce, ZEND_STRL("_exited"), 0, ZEND_ACC_PROTECTED TSRMLS_CC);

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
