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
#include "ext/standard/info.h"
#include "ext/standard/php_string.h"

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#include "php_yod.h"

#define MICRO_IN_SEC 1000000.00

zend_class_entry *yod_application_ce;
zend_class_entry *yod_request_ce;
zend_class_entry *yod_controller_ce;
zend_class_entry *yod_action_ce;
zend_class_entry *yod_model_ce;
zend_class_entry *yod_database_ce;

ZEND_DECLARE_MODULE_GLOBALS(yod);

/* {{{ yod_application_methods[]
*/
zend_function_entry yod_application_methods[] = {
	{NULL, NULL, NULL}
};
/* }}} */

/** {{{ yod_request_methods[]
*/
zend_function_entry yod_request_methods[] = {
  {NULL, NULL, NULL}
};
/* }}} */

/** {{{ yod_controller_methods[]
*/
zend_function_entry yod_controller_methods[] = {
  {NULL, NULL, NULL}
};
/* }}} */

/** {{{ yod_action_methods[]
*/
zend_function_entry yod_action_methods[] = {
  {NULL, NULL, NULL}
};
/* }}} */

/** {{{ yod_model_methods[]
*/
zend_function_entry yod_model_methods[] = {
  {NULL, NULL, NULL}
};
/* }}} */

/** {{{ yod_database_methods[]
*/
zend_function_entry yod_database_methods[] = {
  {NULL, NULL, NULL}
};
/* }}} */

/** {{{ PHP_GINIT_FUNCTION
*/
PHP_GINIT_FUNCTION(yod)
{
	yod_globals->runtime	= 0;
}
/* }}} */

/** {{{ PHP_MINIT_FUNCTION
*/
PHP_MINIT_FUNCTION(yod)
{
	REGISTER_STRINGL_CONSTANT("YOD_VERSION", YOD_VERSION,	sizeof(YOD_VERSION) - 1,	CONST_PERSISTENT | CONST_CS);

	// Yod_Application
	zend_class_entry application_ce;

	INIT_CLASS_ENTRY(application_ce, "Yod_Application", yod_application_methods);
	yod_application_ce = zend_register_internal_class(&application_ce TSRMLS_CC);

	zend_declare_property_null(yod_application_ce, ZEND_STRL("_app"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC TSRMLS_CC);
	zend_declare_property_null(yod_application_ce, ZEND_STRL("_config"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_application_ce, ZEND_STRL("_request"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_application_ce, ZEND_STRL("_imports"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_bool(yod_application_ce, ZEND_STRL("_running"), 0, ZEND_ACC_PROTECTED TSRMLS_CC);

	// Yod_Request
	zend_class_entry request_ce;

	INIT_CLASS_ENTRY(request_ce, "Yod_Request", yod_request_methods);
	yod_request_ce = zend_register_internal_class(&request_ce TSRMLS_CC);

	zend_declare_property_bool(yod_request_ce, ZEND_STRL("_routed"), 0, ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_request_ce, ZEND_STRL("controller"), ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(yod_request_ce, ZEND_STRL("action"), ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(yod_request_ce, ZEND_STRL("params"), ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(yod_request_ce, ZEND_STRL("method"), ZEND_ACC_PUBLIC TSRMLS_CC);
	
	// Yod_Controller
	zend_class_entry controller_ce;

	INIT_CLASS_ENTRY(controller_ce, "Yod_Controller", yod_controller_methods);
	yod_controller_ce = zend_register_internal_class(&controller_ce TSRMLS_CC);

	zend_declare_property_long(controller_ce, ZEND_STRL("_forward"), 0, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC TSRMLS_CC);
	zend_declare_property_null(controller_ce, ZEND_STRL("_name"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(controller_ce, ZEND_STRL("_action"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(controller_ce, ZEND_STRL("_request"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(controller_ce, ZEND_STRL("_view"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_bool(controller_ce, ZEND_STRL("_exited"), 0, ZEND_ACC_PROTECTED TSRMLS_CC);
	
	// Yod_Action
	zend_class_entry action_ce;

	INIT_CLASS_ENTRY(action_ce, "Yod_Action", yod_action_methods);
	yod_action_ce = zend_register_internal_class_ex(&action_ce, controller_ce, NULL TSRMLS_CC);

	// Yod_Model
	zend_class_entry model_ce;

	INIT_CLASS_ENTRY(model_ce, "Yod_Model", yod_model_methods);
	yod_model_ce = zend_register_internal_class(&model_ce TSRMLS_CC);

	zend_declare_property_null(yod_model_ce, ZEND_STRL("_model"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC TSRMLS_CC);
	zend_declare_property_null(yod_model_ce, ZEND_STRL("_db"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_model_ce, ZEND_STRL("_dsn"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_model_ce, ZEND_STRL("_name"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_model_ce, ZEND_STRL("_table"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_model_ce, ZEND_STRL("_prefix"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_model_ce, ZEND_STRL("_query"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_model_ce, ZEND_STRL("_params"), ZEND_ACC_PROTECTED TSRMLS_CC);

	// Yod_Database
	zend_class_entry database_ce;

	INIT_CLASS_ENTRY(database_ce, "Yod_Database", yod_database_methods);
	yod_database_ce = zend_register_internal_class(&database_ce TSRMLS_CC);

	zend_declare_property_null(yod_model_ce, ZEND_STRL("_db"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC TSRMLS_CC);
	zend_declare_property_null(yod_model_ce, ZEND_STRL("_config"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_model_ce, ZEND_STRL("_driver"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_model_ce, ZEND_STRL("_linkid"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_model_ce, ZEND_STRL("_linkids"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_model_ce, ZEND_STRL("_result"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_model_ce, ZEND_STRL("_lastquery"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_bool(yod_model_ce, ZEND_STRL("_pconnect"), 0, ZEND_ACC_PROTECTED TSRMLS_CC);

	return SUCCESS;
}
/* }}} */

/** {{{ PHP_MSHUTDOWN_FUNCTION
*/
PHP_MSHUTDOWN_FUNCTION(yod)
{
	return SUCCESS;
}
/* }}} */

/** {{{ PHP_RINIT_FUNCTION
*/
PHP_RINIT_FUNCTION(yod)
{
	struct timeval tp = {0};

	if (gettimeofday(&tp, NULL)) {
		YOD_G(runtime)		= 0;	
	} else {
		YOD_G(runtime)		= (double)(tp.tv_sec + tp.tv_usec / MICRO_IN_SEC);
	}

#if ((PHP_MAJOR_VERSION == 5) && (PHP_MINOR_VERSION < 4))
	YOD_G(buffer)			= NULL;
	YOD_G(owrite_handler)	= NULL;
	YOD_G(buf_nesting)		= 0;
#endif

	efree(path);

	/* rinit components */
	PHP_RINIT(application)(INIT_FUNC_ARGS_PASSTHRU);

	return SUCCESS;
}
/* }}} */

/** {{{ PHP_RSHUTDOWN_FUNCTION
*/
PHP_RSHUTDOWN_FUNCTION(yod)
{
	return SUCCESS;
}
/* }}} */

/** {{{ PHP_MINFO_FUNCTION
*/
PHP_MINFO_FUNCTION(yod)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "yod support", "enabled");
	php_info_print_table_row(2, "Version", YOD_VERSION);
	//php_info_print_table_row(2, "Supports", YOD_SUPPORT_URL);
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}
/* }}} */

/** {{{ DL support
 */
#ifdef COMPILE_DL_YOD
ZEND_GET_MODULE(yod)
#endif
/* }}} */

/** {{{ module depends
 */
#if ZEND_MODULE_API_NO >= 20050922
zend_module_dep yod_deps[] = {
	//ZEND_MOD_REQUIRED("spl")
	{NULL, NULL, NULL}
};
#endif
/* }}} */

/** {{{ yod_module_entry
*/
zend_module_entry yod_module_entry = {
#if ZEND_MODULE_API_NO >= 20050922
	STANDARD_MODULE_HEADER_EX, NULL,
	yod_deps,
#else
	STANDARD_MODULE_HEADER,
#endif
	"yod",
	yod_functions,
	PHP_MINIT(yod),
	PHP_MSHUTDOWN(yod),
	PHP_RINIT(yod),
	PHP_RSHUTDOWN(yod),
	PHP_MINFO(yod),
	YOD_VERSION,
	PHP_MODULE_GLOBALS(yod),
	PHP_GINIT(yod),
	NULL,
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
