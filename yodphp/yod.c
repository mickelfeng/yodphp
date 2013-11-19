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

/** {{{ char *yod_extpath(ZEND_FILE_LINE_DC TSRMLS_DC)
*/
char *yod_extpath(ZEND_FILE_LINE_DC TSRMLS_DC) {
	zval const_value;
	char *filepath, *extpath;
	size_t filepath_len;

	if (zend_get_constant(ZEND_STRL("YOD_EXTPATH"), &const_value TSRMLS_CC)) {
		extpath = Z_STRVAL(const_value);
		php_printf("zend_get_constant(YOD_EXTPATH)");
	} else {
		filepath = SG(request_info).path_translated;
		filepath_len = php_dirname(filepath, strlen(filepath));
		extpath = estrndup(filepath, filepath_len);
		zend_register_stringl_constant(ZEND_STRS("YOD_EXTPATH"), filepath, filepath_len, CONST_CS, 0 TSRMLS_CC);
		php_printf("zend_register_stringl_constant(YOD_EXTPATH)");
	}
	return extpath;
}
/* }}} */

/** {{{ char *yod_runpath(ZEND_FILE_LINE_DC TSRMLS_DC)
*/
char *yod_runpath(ZEND_FILE_LINE_DC TSRMLS_DC) {
	zval const_value;
	char *filepath, *runpath;
	size_t filepath_len;

	if (zend_get_constant(ZEND_STRL("YOD_RUNPATH"), &const_value TSRMLS_CC)) {
		runpath = Z_STRVAL(const_value);
		php_printf("zend_get_constant(YOD_RUNPATH)");
	} else {
		filepath = SG(request_info).path_translated;
		filepath_len = php_dirname(filepath, strlen(filepath));
		runpath = estrndup(filepath, filepath_len);
		zend_register_stringl_constant(ZEND_STRS("YOD_RUNPATH"), filepath, filepath_len, CONST_CS, 0 TSRMLS_CC);
		php_printf("zend_register_stringl_constant(YOD_RUNPATH)");
	}
	return runpath;
}
/* }}} */


/** {{{ int yod_execute_scripts(char *filepath, zval **result, int type ZEND_FILE_LINE_DC TSRMLS_DC)
 * */
int yod_execute_scripts(char *filepath, zval **result, int type ZEND_FILE_LINE_DC TSRMLS_DC) {
	zend_file_handle file_handle;

	file_handle.type = ZEND_HANDLE_FILENAME;
	file_handle.opened_path = NULL;
	file_handle.free_filename = 0;
	file_handle.filename = filepath;

	return zend_execute_scripts(type TSRMLS_CC, result, 1, &file_handle);
}
/* }}} */

/** {{{ int yod_autoload_register(ZEND_FILE_LINE_DC TSRMLS_DC)
 * */
int yod_autoload_register(ZEND_FILE_LINE_DC TSRMLS_DC) {
	int result = 1;
	zval *autoload, *function, *retval = NULL;
	zval **params[1] = {&autoload};

	MAKE_STD_ZVAL(autoload);
	ZVAL_STRING(autoload, "__autoload", 1);

	MAKE_STD_ZVAL(function);
	ZVAL_STRING(function, "spl_autoload_register", 0);

	zend_fcall_info fci = {
		sizeof(fci),
		EG(function_table),
		function,
		NULL,
		&retval,
		1,
		(zval ***)params,
		NULL,
		1
	};

	if (zend_call_function(&fci, NULL TSRMLS_CC) == FAILURE) {
		result = 0;
	}

	if (retval) {
		zval_ptr_dtor(&retval);
	}
	efree(function);
	zval_ptr_dtor(&autoload);

	return result;
}
/* }}} */

/** {{{ int yod_autoload_class(char *classname, uint classname_len ZEND_FILE_LINE_DC TSRMLS_DC)
*/
int yod_autoload_class(char *classname, uint classname_len ZEND_FILE_LINE_DC TSRMLS_DC) {
	zval filepath;
	char *classfile, *classpath;
	zend_class_entry **pce = NULL;

	classfile = estrndup(classname, classname_len);
	// class name with namespace in PHP 5.3
	if (strstr(classname, "\\")) {
		while (*classfile != '\0') {
			if (*classfile == '\\') {
				*classfile = '_';
			}
			classfile++;
		}
		classfile = classfile - classname_len + 1;
	}

	if (strncmp(classfile, "Yod_", 4) == 0) { // yodphp extends class
		if (strncmp(classfile, "Yod_Db", 6) == 0) {
			spprintf(&classpath, 0, "%s/drivers/%s.class.php", yod_extpath(TSRMLS_CC), classfile + 4);
		} else {
			spprintf(&classpath, 0, "%s/extends/%s.class.php", yod_extpath(TSRMLS_CC), classfile + 4);
		}
	} else {
		if (zend_get_constant(ZEND_STRL("YOD_RUNPATH"), &filepath TSRMLS_CC)) {
			YOD_G(runpath) = Z_STRVAL(filepath);
		}
		if (classname_len > 5 && strncmp(classfile + classname_len - 5, "Model", 5) == 0) {
			spprintf(&classpath, 0, "%s/models/%s.php", yod_runpath(TSRMLS_CC), classfile);
		} else {
			spprintf(&classpath, 0, "%s/extends/%s.php", yod_runpath(TSRMLS_CC), classfile);
		}
	}
	efree(classfile);

	if (VCWD_ACCESS(classpath, F_OK) == 0) {
		yod_execute_scripts(classpath, NULL, ZEND_INCLUDE);
	}
	efree(classpath);

	if (zend_lookup_class_ex(classname, classname_len, 0, &pce TSRMLS_CC) == SUCCESS) {
		return SUCCESS;
	}

	return FAILURE;

}
/* }}} */

/* {{{ ZEND_FUNCTION
*/
ZEND_FUNCTION(__autoload)
{
	char *classname;
	uint classname_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &classname, &classname_len) == FAILURE) {
		return;
	}

	if (yod_autoload_class(classname, classname_len) == FAILURE) {
		RETURN_FALSE;
		return;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ yod_functions[]
*/
zend_function_entry yod_functions[] = {
	ZEND_FE(__autoload, NULL)
	{NULL, NULL, NULL}
};
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(yod_application_construct_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_application_run_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_application_app_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_application_destruct_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

/** {{{ proto public static Yod_Application::__construct($config = null)
*/
PHP_METHOD(yod_application, __construct) {
	zval *yodapp, *config, *request, *object;
	char *filepath, *scandir;
	size_t scandir_len;

	DIR *dir = NULL;
	char dentry[sizeof(struct dirent) + MAXPATHLEN];
	struct dirent *entry = (struct dirent *) &dentry;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z", &config) == FAILURE) {
		return;
	}

	yodapp = zend_read_static_property(yod_application_ce, ZEND_STRL("_app"), 0 TSRMLS_CC);
	if (!ZVAL_IS_NULL(yodapp)) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Only one application can be initialized");
		return;
	}

	object = getThis();

	// config
	if(Z_TYPE_P(config) != IS_ARRAY) {
		if(Z_TYPE_P(config) == IS_STRING) {
			filepath = Z_STRVAL_P(config);
		} else {
			spprintf(&filepath, 0, "%s/configs/config.php", yod_runpath(TSRMLS_CC));
		}
		if (VCWD_ACCESS(filepath, F_OK) == 0) {
			yod_execute_scripts(filepath, &config, ZEND_INCLUDE);
		} else {
			zval_dtor(config);
			array_init(config);
			scandir_len = php_dirname(filepath, strlen(filepath));
			scandir = estrndup(filepath, scandir_len);
			dir = VCWD_OPENDIR(scandir);
			if (dir) {
				while (php_readdir_r(dir, (struct dirent *) dentry, &entry) == 0 && entry) {
					zval *value;
					size_t entry_len = strlen(entry->d_name);

					if (entry_len > 11 && strncmp(entry->d_name + entry_len - 11, ".config.php", 11) == 0) {
						spprintf(&filepath, 0, "%s/%s", scandir, entry->d_name);
						if (VCWD_ACCESS(filepath, F_OK) == 0) {
							yod_execute_scripts(filepath, &value, ZEND_INCLUDE);
							if (entry_len == 15 && strncmp(entry->d_name, "base", 4) == 0) {
								if (Z_TYPE_P(value) == IS_ARRAY) {
									php_array_merge(Z_ARRVAL_P(config), Z_ARRVAL_P(value), 0 TSRMLS_DC);
								}
							} else {
								zval **ppval;
								uint key_len = entry_len - 10;
								char *key = estrndup(entry->d_name, key_len);
								if (zend_hash_find(Z_ARRVAL_P(config), "devel", 6, (void **)&ppval) == SUCCESS) {
									zend_print_zval_r(*ppval, 0 TSRMLS_CC);
								}
								uint temp = zend_inline_hash_func(entry->d_name, entry_len - 10);
								spprintf(&key, 0, "%d\n", temp);
								php_printf(key);

								temp = zend_inline_hash_func("devel", 6);
								spprintf(&key, 0, "%d\n", temp);
								php_printf(key);

								char *devel = "devel";
								temp = zend_inline_hash_func(devel, 6);
								spprintf(&key, 0, "%d\n", temp);
								php_printf(key);

								devel = estrndup(entry->d_name, entry_len - 11);
								spprintf(&devel, 0, "%s", devel);
								php_printf(devel);
								temp = zend_inline_hash_func(devel, entry_len - 10);
								spprintf(&key, 0, "%d\n", temp);
								php_printf(key);

								key = estrndup(entry->d_name, entry_len - 11);
								spprintf(&key, 0, "%s", key);
								if (zend_hash_find(Z_ARRVAL_P(config), key, sizeof(key), (void **)&ppval) == SUCCESS) {
									php_printf("zend_hash_find");
									php_array_merge(Z_ARRVAL_P(value), Z_ARRVAL_PP(ppval), 0 TSRMLS_DC);
									zend_print_zval_r(value, 0 TSRMLS_CC);
								}
								add_assoc_zval_ex(config, key, sizeof(key), value);
								if (zend_hash_find(Z_ARRVAL_P(config), "devel", 6, (void **)&ppval) == SUCCESS) {
									zend_print_zval_r(*ppval, 0 TSRMLS_CC);
								}
								if (zend_hash_find(Z_ARRVAL_P(config), key, sizeof(key), (void **)&ppval) == SUCCESS) {
									zend_print_zval_r(*ppval, 0 TSRMLS_CC);
								}
							}
						}
					}
				}
				closedir(dir);
			}
		}
		
	}
	zend_update_property(yod_application_ce, object, ZEND_STRL("_config"), config TSRMLS_CC);
	zend_print_zval_r(config, 0 TSRMLS_CC);

	// request
	MAKE_STD_ZVAL(request);
	object_init_ex(request, yod_request_ce);
	if (zend_hash_exists(&yod_request_ce->function_table, ZEND_STRS(ZEND_CONSTRUCTOR_FUNC_NAME))) {
		zend_call_method_with_0_params(&request, yod_request_ce, &yod_request_ce->constructor, ZEND_CONSTRUCTOR_FUNC_NAME, NULL);
	}
	zend_update_property(yod_application_ce, object, ZEND_STRL("_request"), request TSRMLS_CC);

	// app
	zend_update_static_property(yod_application_ce, ZEND_STRL("_app"), object TSRMLS_CC);

	//zend_print_zval_r(object, 0 TSRMLS_CC);
}
/* }}} */


/** {{{ proto public static Yod_Application::run()
*/
PHP_METHOD(yod_application, run) {
	zval *running, *object;

	object = getThis();

	running = zend_read_property(yod_application_ce, object, ZEND_STRL("_running"), 1 TSRMLS_CC);
	if (Z_BVAL_P(running)) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "An application instance already running");
		return;
	}

	ZVAL_BOOL(running, 1);
	zend_update_property(yod_application_ce, object, ZEND_STRL("_running"), running TSRMLS_CC);
}
/* }}} */

/** {{{ proto public static Yod_Application::app($config = null)
*/
PHP_METHOD(yod_application, app) {
	zval *yodapp, *config;
	zend_class_entry **pce = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z", &config) == FAILURE) {
		return;
	}

	yodapp = zend_read_static_property(yod_application_ce, ZEND_STRL("_app"), 0 TSRMLS_CC);
	if (yodapp && Z_TYPE_P(yodapp) == IS_OBJECT) {
		RETURN_ZVAL(yodapp, 1, 0);
	}

	object_init_ex(return_value, yod_application_ce);
	if (zend_hash_exists(&yod_application_ce->function_table, ZEND_STRS(ZEND_CONSTRUCTOR_FUNC_NAME))) {
		zend_call_method_with_1_params(&return_value, yod_application_ce, &yod_application_ce->constructor, ZEND_CONSTRUCTOR_FUNC_NAME, NULL, config);
	}
}
/* }}} */

/** {{{ proto public static Yod_Application::__destruct()
*/
PHP_METHOD(yod_application, __destruct) {
	zval *running, *object;

	object = getThis();

	running = zend_read_property(yod_application_ce, object, ZEND_STRL("_running"), 1 TSRMLS_CC);
	if (!Z_BVAL_P(running)) {
		zend_call_method_with_0_params(&object, yod_application_ce, NULL, "run", NULL);
	}
	zval_dtor(running);
}
/* }}} */

/* {{{ yod_application_methods[]
*/
zend_function_entry yod_application_methods[] = {
	PHP_ME(yod_application, __construct,	yod_application_construct_arginfo,	ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(yod_application, run, 	 	 	yod_application_run_arginfo, 		ZEND_ACC_PUBLIC)
	PHP_ME(yod_application, app,			yod_application_app_arginfo,		ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yod_application, __destruct,		yod_application_destruct_arginfo,	ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
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
	yod_globals->runtime = 0;
}
/* }}} */

/** {{{ PHP_MINIT_FUNCTION
*/
PHP_MINIT_FUNCTION(yod)
{
	REGISTER_STRINGL_CONSTANT("YOD_VERSION", YOD_VERSION, sizeof(YOD_VERSION) - 1, CONST_PERSISTENT | CONST_CS);

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

	zend_declare_property_long(yod_controller_ce, ZEND_STRL("_forward"), 0, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC TSRMLS_CC);
	zend_declare_property_null(yod_controller_ce, ZEND_STRL("_name"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_controller_ce, ZEND_STRL("_action"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_controller_ce, ZEND_STRL("_request"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_controller_ce, ZEND_STRL("_view"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_bool(yod_controller_ce, ZEND_STRL("_exited"), 0, ZEND_ACC_PROTECTED TSRMLS_CC);
	
	// Yod_Action
	zend_class_entry action_ce;

	INIT_CLASS_ENTRY(action_ce, "Yod_Action", yod_action_methods);
	yod_action_ce = zend_register_internal_class_ex(&action_ce, yod_controller_ce, NULL TSRMLS_CC);

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

	// spl_autoload_register
	yod_autoload_register(TSRMLS_CC);

	return SUCCESS;
}
/* }}} */

/** {{{ PHP_RSHUTDOWN_FUNCTION
*/
PHP_RSHUTDOWN_FUNCTION(yod)
{
	zval runpath;

	if (zend_get_constant(ZEND_STRL("YOD_RUNPATH"), &runpath TSRMLS_CC)) {
		zend_call_method_with_0_params(NULL, yod_application_ce, NULL, "app", NULL);
	}
	zval_dtor(&runpath);

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
	ZEND_MOD_REQUIRED("spl")
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
