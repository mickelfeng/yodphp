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
#include "yod_debug.h"

zend_class_entry *yod_action_ce;

/** {{{ ARG_INFO
*/
ZEND_BEGIN_ARG_INFO_EX(yod_debug_run_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ proto protected Yod_Debug::run()
*/
PHP_METHOD(yod_action, run) {
	
}
/* }}} */

/** {{{ yod_debug_methods[]
*/
zend_function_entry yod_debug_methods[] = {
	PHP_ME(yod_debug, run,		yod_debug_run_arginfo,		ZEND_ACC_PROTECTED)
	{NULL, NULL, NULL}
};
/* }}} */

/** {{{ PHP_MINIT_FUNCTION
*/
PHP_MINIT_FUNCTION(yod_debug) {
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "Yod_Debug", yod_debug_methods);
	yod_debug_ce = zend_register_internal_class(&ce TSRMLS_CC);
	
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
