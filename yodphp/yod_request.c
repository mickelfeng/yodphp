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
	zval *routed;

	if (!request || Z_TYPE_P(request) != IS_OBJECT) {
		return 0;
	}
	YOD_G(routed) = 1;

	ALLOC_ZVAL(routed);
	ZVAL_BOOL(routed, 1);
	zend_update_property(yod_request_ce, request, ZEND_STRL("_routed"), routed TSRMLS_CC);

	return 1;
}
/* }}} */

/** {{{ int yod_request_dispatch(yod_request_t *request TSRMLS_DC)
*/
int yod_request_dispatch(yod_request_t *request TSRMLS_DC) {
	zval *routed;
	
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
	zend_update_property(yod_request_ce, object, ZEND_STRL("_method"), method TSRMLS_CC);
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
	
}
/* }}} */

/** {{{ proto public Yod_Request::dispatch()
*/
PHP_METHOD(yod_request, dispatch) {
	
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
