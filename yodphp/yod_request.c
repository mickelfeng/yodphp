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

zend_class_entry *yod_request_ce;

/** {{{ ARG_INFO
 */
ZEND_BEGIN_ARG_INFO_EX(yod_request_construct_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, route)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_request_route_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, route)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_request_dispatch_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_request_erroraction_arginfo, 0, 0, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_request_error404_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, html)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ int yod_request_route(yod_request_t *request, zval *route TSRMLS_DC)
*/
int yod_request_route(yod_request_t *request, zval *route TSRMLS_DC) {
	HashTable *_SERVER, *_GET;
	zval *method, *params, *pzval, **argv, **ppval;
	char *controller, *action, *token;
	char *key, *value, *route_str = "";
	uint key_len, route_len = 0;

	if (!request || Z_TYPE_P(request) != IS_OBJECT) {
		return 0;
	}

	if (!PG(http_globals)[TRACK_VARS_SERVER]) {
		zend_is_auto_global(ZEND_STRL("_SERVER") TSRMLS_CC);
	}
	_SERVER = HASH_OF(PG(http_globals)[TRACK_VARS_SERVER]);

	if (!route || Z_TYPE_P(route) != IS_STRING) {

		method = zend_read_property(yod_request_ce, request, ZEND_STRL("method"), 1 TSRMLS_CC);
		if (method && Z_TYPE_P(method) == IS_STRING) {
			if (strncasecmp(Z_STRVAL_P(method), "cli", 3) == 0) {
				if((zend_hash_find(_SERVER, ZEND_STRS("argv"), (void **) &argv) != FAILURE ||
					zend_hash_find(&EG(symbol_table), ZEND_STRS("argv"), (void **) &argv) != FAILURE) &&
					Z_TYPE_PP(argv) == IS_ARRAY &&
					zend_hash_index_find(Z_ARRVAL_PP(argv), 1, (void**)&ppval) == SUCCESS &&
					Z_TYPE_PP(ppval) == IS_STRING
				){
					route_len = Z_STRLEN_PP(ppval);
					route_str = Z_STRVAL_PP(ppval);
				}
			} else {
				char *pathvar = yod_pathvar(TSRMLS_CC);
				size_t pathvar_len = strlen(pathvar);

				if (HASH_OF(PG(http_globals)[TRACK_VARS_GET]) &&
					zend_hash_find(HASH_OF(PG(http_globals)[TRACK_VARS_GET]), pathvar, pathvar_len + 1, (void **) &ppval) != FAILURE &&
					Z_TYPE_PP(ppval) == IS_STRING
				) {
					route_str = Z_STRVAL_PP(ppval);
					route_len = Z_STRLEN_PP(ppval);
				} else if (zend_hash_find(_SERVER, ZEND_STRS("PATH_INFO"), (void **) &ppval) != FAILURE &&
					Z_TYPE_PP(ppval) == IS_STRING
				) {
					route_len = Z_STRLEN_PP(ppval);
					route_str = Z_STRVAL_PP(ppval);
				}
			}
		}
	} else {
		route_len = Z_STRLEN_P(route);
		route_str = Z_STRVAL_P(route);
	}

	while (*route_str == '/' || *route_str == '\\') {
		route_len--;
		route_str++;
	}

	// params
	MAKE_STD_ZVAL(params);
	array_init(params);

	// controller
	controller = php_strtok_r(route_str, "/", &token);
	if (!controller) {
		zend_update_property_string(yod_request_ce, request, ZEND_STRL("controller"), "Index" TSRMLS_CC);
	} else {
		zend_str_tolower(controller, strlen(controller));
		*controller = toupper(*controller);
		zend_update_property_string(yod_request_ce, request, ZEND_STRL("controller"), controller TSRMLS_CC);

		// action
		action = php_strtok_r(NULL, "/", &token);
		if (!action) {
			zend_update_property_string(yod_request_ce, request, ZEND_STRL("action"), "index" TSRMLS_CC);
		} else {
			zend_str_tolower(action, strlen(action));
			zend_update_property_string(yod_request_ce, request, ZEND_STRL("action"), action TSRMLS_CC);

			// params
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
		}
	}

	zend_update_property(yod_request_ce, request, ZEND_STRL("params"), params TSRMLS_CC);

	zend_update_property_bool(yod_request_ce, request, ZEND_STRL("_routed"), 1 TSRMLS_CC);
	YOD_G(routed) = 1;

	return 1;
}
/* }}} */

/** {{{ int yod_request_dispatch(yod_request_t *request TSRMLS_DC)
*/
int yod_request_dispatch(yod_request_t *request TSRMLS_DC) {
	zval *routed;

#if PHP_YOD_DEBUG
	php_printf("yod_request_dispatch()\n");
#endif

	if (!request || Z_TYPE_P(request) != IS_OBJECT) {
		return 0;
	}

	if (!YOD_G(routed)) {
		yod_request_route(request, NULL TSRMLS_CC);
	}
	return 1;
}
/* }}} */

/** {{{ yod_request_t *yod_request_instance(yod_request_t *this_ptr, zval *route TSRMLS_DC)
*/
yod_request_t *yod_request_instance(yod_request_t *this_ptr, zval *route TSRMLS_DC) {
	yod_request_t *object;
	zval *method;

#if PHP_YOD_DEBUG
	php_printf("yod_request_instance()\n");
#endif

	if (this_ptr) {
		object = this_ptr;
	} else {
		MAKE_STD_ZVAL(object);
		object_init_ex(object, yod_request_ce);
	}

	MAKE_STD_ZVAL(method);
    if (SG(request_info).request_method) {
        ZVAL_STRING(method, (char *)SG(request_info).request_method, 1);
    } else if (strncasecmp(sapi_module.name, "cli", 3)) {
        ZVAL_STRING(method, "Unknow", 1);
    } else {
        ZVAL_STRING(method, "Cli", 1);
    }
	zend_update_property(yod_request_ce, object, ZEND_STRL("method"), method TSRMLS_CC);
	zval_ptr_dtor(&method);

	if (route) {
		yod_request_route(object, route);
	}

	return object;
}
/* }}} */

/** {{{ proto public Yod_Request::__construct($route = null)
*/
PHP_METHOD(yod_request, __construct) {
	zval *route = NULL, *method, *params;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z", &route) == FAILURE) {
		return;
	}

	(void)yod_request_instance(getThis(), route TSRMLS_CC);
}
/* }}} */

/** {{{ proto public Yod_Request::route($route = null)
*/
PHP_METHOD(yod_request, route) {
	zval *route = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z", &route) == FAILURE) {
		return;
	}

	yod_request_route(getThis(), NULL TSRMLS_CC);
}
/* }}} */

/** {{{ proto public Yod_Request::dispatch()
*/
PHP_METHOD(yod_request, dispatch) {
	yod_request_dispatch(getThis() TSRMLS_CC);
}
/* }}} */

/** {{{ proto public Yod_Request::erroraction()
*/
PHP_METHOD(yod_request, erroraction) {
	
}
/* }}} */

/** {{{ proto public Yod_Request::error404($html = null)
*/
PHP_METHOD(yod_request, error404) {
	
}
/* }}} */

/** {{{ proto public Yod_Request::__destruct()
*/
PHP_METHOD(yod_request, __destruct) {
	
}
/* }}} */

/** {{{ yod_request_methods[]
*/
zend_function_entry yod_request_methods[] = {
	PHP_ME(yod_request, __construct,	yod_request_construct_arginfo,		ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(yod_request, route,			yod_request_route_arginfo,			ZEND_ACC_PUBLIC)
	PHP_ME(yod_request, dispatch,		yod_request_dispatch_arginfo,		ZEND_ACC_PUBLIC)
	PHP_ME(yod_request, erroraction,	yod_request_erroraction_arginfo,	ZEND_ACC_PUBLIC)
	PHP_ME(yod_request, error404,		yod_request_error404_arginfo,		ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yod_request, __destruct,		NULL,	ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	{NULL, NULL, NULL}
};
/* }}} */

/** {{{ PHP_MINIT_FUNCTION
*/
PHP_MINIT_FUNCTION(yod_request) {
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "Yod_Request", yod_request_methods);
	yod_request_ce = zend_register_internal_class(&ce TSRMLS_CC);

	zend_declare_property_bool(yod_request_ce, ZEND_STRL("_routed"), 0, ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_request_ce, ZEND_STRL("controller"), ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(yod_request_ce, ZEND_STRL("action"), ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(yod_request_ce, ZEND_STRL("params"), ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(yod_request_ce, ZEND_STRL("method"), ZEND_ACC_PUBLIC TSRMLS_CC);

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
