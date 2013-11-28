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

#include "php_yod.h"
#include "yod_controller.h"
#include "yod_widget.h"

zend_class_entry *yod_widget_ce;

/** {{{ ARG_INFO
*/
ZEND_BEGIN_ARG_INFO_EX(yod_widget_construct_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, route)
	ZEND_ARG_INFO(0, action)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_widget_run_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ static void yod_controller_construct(yod_controller_t *object, yod_request_t *request, char *action, uint action_len, zval *params TSRMLS_DC)
*/
static void yod_controller_construct(yod_controller_t *object, yod_request_t *request, char *action, uint action_len, zval *params TSRMLS_DC) {

}
/* }}} */

/** {{{ proto public Yod_Widget::__construct($request, $action = null, $params = null)
*/
PHP_METHOD(yod_widget, __construct) {
	yod_request_t *request = NULL;
	zval *params = NULL;
	char *action = NULL;
	uint action_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|sz", &request, &action, action_len, &params) == FAILURE) {
		return;
	}

	yod_widget_construct(getThis(), request, action, action_len, params TSRMLS_CC);
}
/* }}} */

/** {{{ proto protected Yod_Widget::run()
*/
PHP_METHOD(yod_widget, run) {
	
}
/* }}} */

/** {{{ yod_widget_methods[]
*/
zend_function_entry yod_widget_methods[] = {
	PHP_ME(yod_widget, __construct,		yod_widget_construct_arginfo,	ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(yod_widget, run,				yod_widget_run_arginfo,		ZEND_ACC_PROTECTED)
	{NULL, NULL, NULL}
};
/* }}} */

/** {{{ PHP_MINIT_FUNCTION
*/
PHP_MINIT_FUNCTION(yod_widget) {
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "Yod_Widget", yod_widget_methods);
	yod_widget_ce = zend_register_internal_class_ex(&ce, yod_controller_ce, NULL TSRMLS_CC);
	yod_widget_ce->ce_flags |= ZEND_ACC_IMPLICIT_ABSTRACT_CLASS;
	
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
