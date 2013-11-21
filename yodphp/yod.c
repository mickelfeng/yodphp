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

/** {{{ double yod_runtime(TSRMLS_DC)
*/
double yod_runtime(TSRMLS_DC) {
	zval const_value;
	double runtime;

	if (zend_get_constant(ZEND_STRL("YOD_RUNTIME"), &const_value TSRMLS_CC)) {
		runtime = Z_DVAL(const_value);
	} else {
		runtime = YOD_G(runtime);
		zend_register_double_constant(ZEND_STRS("YOD_RUNTIME"), runtime, CONST_CS, 0 TSRMLS_CC);
	}
	return runtime;
}
/* }}} */

/** {{{ int yod_forward(TSRMLS_DC)
*/
int yod_forward(TSRMLS_DC) {
	zval const_value;
	int forward;

	if (zend_get_constant(ZEND_STRL("YOD_FORWARD"), &const_value TSRMLS_CC)) {
		forward = Z_LVAL(const_value);
	} else {
		forward = YOD_FORWARD;
		zend_register_long_constant(ZEND_STRS("YOD_FORWARD"), forward, CONST_CS, 0 TSRMLS_CC);
	}
	return forward;
}
/* }}} */

/** {{{ char *yod_charset(TSRMLS_DC)
*/
char *yod_charset(TSRMLS_DC) {
	zval const_value;
	char *charset;

	if (zend_get_constant(ZEND_STRL("YOD_CHARSET"), &const_value TSRMLS_CC)) {
		charset = Z_STRVAL(const_value);
	} else {
		charset = YOD_CHARSET;
		zend_register_string_constant(ZEND_STRS("YOD_CHARSET"), charset, CONST_CS, 0 TSRMLS_CC);
	}
	return charset;
}
/* }}} */

/** {{{ char *yod_pathvar(TSRMLS_DC)
*/
char *yod_pathvar(TSRMLS_DC) {
	zval const_value;
	char *pathvar;

	if (zend_get_constant(ZEND_STRL("YOD_PATHVAR"), &const_value TSRMLS_CC)) {
		pathvar = Z_STRVAL(const_value);
	} else {
		pathvar = YOD_PATHVAR;
		zend_register_string_constant(ZEND_STRS("YOD_PATHVAR"), pathvar, CONST_CS, 0 TSRMLS_CC);
	}
	return pathvar;
}
/* }}} */

/** {{{ char *yod_extpath(TSRMLS_DC)
*/
char *yod_extpath(TSRMLS_DC) {
	zval const_value;
	char *extpath;
	size_t extpath_len;

	if (zend_get_constant(ZEND_STRL("YOD_EXTPATH"), &const_value TSRMLS_CC)) {
		extpath = Z_STRVAL(const_value);
	} else {
		extpath_len = strlen(SG(request_info).path_translated);
		extpath = estrndup(SG(request_info).path_translated, extpath_len);
		extpath_len = php_dirname(extpath, extpath_len);
		zend_register_stringl_constant(ZEND_STRS("YOD_EXTPATH"), extpath, extpath_len, CONST_CS, 0 TSRMLS_CC);
	}
	return extpath;
}
/* }}} */

/** {{{ char *yod_runpath(TSRMLS_DC)
*/
char *yod_runpath(TSRMLS_DC) {
	zval const_value;
	char *runpath;
	size_t runpath_len;

	if (zend_get_constant(ZEND_STRL("YOD_RUNPATH"), &const_value TSRMLS_CC)) {
		runpath = Z_STRVAL(const_value);
	} else {
		runpath_len = strlen(SG(request_info).path_translated);
		runpath = estrndup(SG(request_info).path_translated, runpath_len);
		runpath_len = php_dirname(runpath, runpath_len);
		zend_register_stringl_constant(ZEND_STRS("YOD_RUNPATH"), runpath, runpath_len, CONST_CS, 0 TSRMLS_CC);
	}
	return runpath;
}
/* }}} */

/** {{{ int yod_execute_scripts(char *filepath, zval **result, int dtor TSRMLS_DC)
*/
int yod_execute_scripts(char *filepath, zval **result, int dtor TSRMLS_DC) {
	zend_file_handle file_handle;
	zend_op_array 	*op_array;
	char realpath[MAXPATHLEN];

	if (!VCWD_REALPATH(filepath, realpath)) {
		return 0;
	}

	file_handle.filename = filepath;
	file_handle.free_filename = 0;
	file_handle.type = ZEND_HANDLE_FILENAME;
	file_handle.opened_path = NULL;
	file_handle.handle.fp = NULL;

	op_array = zend_compile_file(&file_handle, ZEND_INCLUDE TSRMLS_CC);

	if (op_array && file_handle.handle.stream.handle) {
		int dummy = 1;

		if (!file_handle.opened_path) {
			file_handle.opened_path = filepath;
		}

		zend_hash_add(&EG(included_files), file_handle.opened_path, strlen(file_handle.opened_path)+1, (void *)&dummy, sizeof(int), NULL);
	}
	zend_destroy_file_handle(&file_handle TSRMLS_CC);

	if (op_array) {
		YOD_STORE_EG_ENVIRON();

		EG(return_value_ptr_ptr) = result;
		EG(active_op_array) 	 = op_array;

#if ((PHP_MAJOR_VERSION == 5) && (PHP_MINOR_VERSION > 2)) || (PHP_MAJOR_VERSION > 5)
		if (!EG(active_symbol_table)) {
			zend_rebuild_symbol_table(TSRMLS_C);
		}
#endif
		zend_execute(op_array TSRMLS_CC);

		destroy_op_array(op_array TSRMLS_CC);
		efree(op_array);
		if (!EG(exception) && dtor) {
			if (EG(return_value_ptr_ptr) && *EG(return_value_ptr_ptr)) {
				zval_ptr_dtor(EG(return_value_ptr_ptr));
			}
		}
		YOD_RESTORE_EG_ENVIRON();
	    return 1;
	}

	return 0;
}
/* }}} */

/** {{{ int yod_autoload_register(TSRMLS_DC)
 * */
int yod_autoload_register(TSRMLS_DC) {
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

/** {{{ int yod_autoload_class(char *classname, uint classname_len TSRMLS_DC)
*/
int yod_autoload_class(char *classname, uint classname_len TSRMLS_DC) {
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
		if (classname_len > 5 && strncmp(classfile + classname_len - 5, "Model", 5) == 0) {
			spprintf(&classpath, 0, "%s/models/%s.php", yod_runpath(TSRMLS_CC), classfile);
		} else {
			spprintf(&classpath, 0, "%s/extends/%s.php", yod_runpath(TSRMLS_CC), classfile);
		}
	}
	efree(classfile);

	if (VCWD_ACCESS(classpath, F_OK) == 0) {
		yod_execute_scripts(classpath, NULL, 1 TSRMLS_CC);
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

ZEND_BEGIN_ARG_INFO_EX(yod_application_config_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_application_import_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, alias)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_application_app_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_application_destruct_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()


/** {{{ zval *yod_application_app(zval *config TSRMLS_DC)
*/
zval *yod_application_app(zval *config TSRMLS_DC) {
	zval *object;

	object = zend_read_static_property(yod_application_ce, ZEND_STRL("_app"), 0 TSRMLS_CC);
	if (Z_TYPE_P(object) == IS_OBJECT) {
		return object;
	}

	return yod_application_instance(NULL, config TSRMLS_CC);
}
/* }}} */

/** {{{ void yod_application_init_config(yod_application_t *object)
*/
void yod_application_init_config(yod_application_t *object, zval *config) {
	zval *result;
	char *filepath;
	size_t filepath_len;

	DIR *dir = NULL;
	char dentry[sizeof(struct dirent) + MAXPATHLEN];
	struct dirent *entry = (struct dirent *) &dentry;

	if(Z_TYPE_P(config) == IS_ARRAY) {
		zend_update_property(yod_application_ce, object, ZEND_STRL("_config"), config TSRMLS_CC);
	} else {

		if(Z_TYPE_P(config) == IS_STRING) {
			filepath = Z_STRVAL_P(config);
		} else {
			spprintf(&filepath, 0, "%s/configs/config.php", yod_runpath(TSRMLS_CC));
		}
		if (VCWD_ACCESS(filepath, F_OK) == 0) {
			yod_execute_scripts(filepath, &result, 0 TSRMLS_CC);
			zend_update_property(yod_application_ce, object, ZEND_STRL("_config"), result TSRMLS_CC);
		} else {
			MAKE_STD_ZVAL(result);
			array_init(result);

			filepath_len = php_dirname(filepath, strlen(filepath));
			dir = VCWD_OPENDIR(filepath);
			if (dir) {
				while (php_readdir_r(dir, (struct dirent *) dentry, &entry) == 0 && entry) {
					zval *value;
					char *filename;
					size_t entry_len = strlen(entry->d_name);

					if (entry_len > 11 && strncmp(entry->d_name + entry_len - 11, ".config.php", 11) == 0) {
						spprintf(&filename, 0, "%s/%s", filepath, entry->d_name);
						if (VCWD_ACCESS(filename, F_OK) == 0) {
							yod_execute_scripts(filename, &value, 0 TSRMLS_CC);
							if (entry_len == 15 && strncmp(entry->d_name, "base", 4) == 0) {
								if (Z_TYPE_P(value) == IS_ARRAY) {
									php_array_merge(Z_ARRVAL_P(result), Z_ARRVAL_P(value), 0 TSRMLS_DC);
								}
							} else {
								zval **ppval;
								uint key_len = entry_len - 11;
								char *key = estrndup(entry->d_name, key_len);

								if (zend_hash_find(Z_ARRVAL_P(result), key, key_len + 1, (void **)&ppval) == SUCCESS) {
									if (Z_TYPE_PP(ppval) == IS_ARRAY) {
										php_array_merge(Z_ARRVAL_P(value), Z_ARRVAL_PP(ppval), 0 TSRMLS_DC);
									}
									zval_ptr_dtor(ppval);
								}
								add_assoc_zval_ex(result, key, key_len + 1, value);
								efree(key);
							}
						}
					}
					if (value) {
						zval_dtor(value);
					}
					
				}
				closedir(dir);
			}
			zend_update_property(yod_application_ce, object, ZEND_STRL("_config"), result TSRMLS_CC);
		}
		efree(filepath);
	}
}
/* }}} */

/** {{{ zval *yod_request_instance(zval *this_ptr, zval *route TSRMLS_DC)
*/
zval *yod_request_instance(yod_request_t *this_ptr, zval *route TSRMLS_DC) {
	yod_request_t *object;

	if (this_ptr) {
		object = this_ptr;
	} else {
		MAKE_STD_ZVAL(object);
		object_init_ex(object, yod_request_ce);
	}

	return object;
}
/* }}} */

/** {{{ zval *yod_application_instance(zval *this_ptr, zval *config TSRMLS_DC)
*/
zval *yod_application_instance(zval *this_ptr, zval *config TSRMLS_DC) {
	zval *object, *request, *imports;

	object = zend_read_static_property(yod_application_ce, ZEND_STRL("_app"), 0 TSRMLS_CC);
	if (!ZVAL_IS_NULL(object)) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Only one application can be initialized");
		return;
	}

	if (this_ptr) {
		object = this_ptr;
	} else {
		MAKE_STD_ZVAL(object);
		object_init_ex(object, yod_application_ce);
	}

	// config
	yod_application_init_config(object, config);

	// request
	request = yod_request_instance(NULL, NULL);
	zend_update_property(yod_application_ce, object, ZEND_STRL("_request"), request TSRMLS_CC);
	zval_ptr_dtor(&request);

	// imports
	MAKE_STD_ZVAL(imports);
	array_init(imports);
	zend_update_property(yod_application_ce, object, ZEND_STRL("_imports"), imports TSRMLS_CC);

	// app
	zend_update_static_property(yod_application_ce, ZEND_STRL("_app"), object TSRMLS_CC);

	return object;
}
/* }}} */

/** {{{ proto public Yod_Application::__construct($config = null)
*/
PHP_METHOD(yod_application, __construct) {
	zval *config;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z", &config) == FAILURE) {
		return;
	}

	(void)yod_application_instance(getThis(), config TSRMLS_CC);
}
/* }}} */

/** {{{ proto public Yod_Application::run()
*/
PHP_METHOD(yod_application, run) {
	zval *running, *request, *object;

	object = getThis();

	running = zend_read_property(yod_application_ce, object, ZEND_STRL("_running"), 1 TSRMLS_CC);
	if (Z_BVAL_P(running)) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "An application instance already running");
		return;
	}

	ZVAL_BOOL(running, 1);
	zend_update_property(yod_application_ce, object, ZEND_STRL("_running"), running TSRMLS_CC);

	request = zend_read_property(yod_application_ce, object, ZEND_STRL("_request"), 1 TSRMLS_CC);
	if (Z_TYPE_P(request) == IS_OBJECT) {
		zend_call_method_with_0_params(&request, yod_request_ce, NULL, "dispatch", NULL);
	}
}
/* }}} */

/** {{{ proto public Yod_Application::config($name = null)
*/
PHP_METHOD(yod_application, config) {
	zval *name, *config, *object;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z", &name) == FAILURE) {
		return;
	}

	object = getThis();

	config = zend_read_property(yod_application_ce, object, ZEND_STRL("_config"), 1 TSRMLS_CC);

	if (Z_TYPE_P(config) != IS_ARRAY) {
		RETURN_NULL();
	}

	if (ZEND_NUM_ARGS() == 0) {
		RETURN_ZVAL(config, 1, 0);
	} else if (Z_TYPE_P(name) == IS_STRING) {
		zval **ppval;
		char *skey, *token;

		if (zend_hash_find(Z_ARRVAL_P(config), Z_STRVAL_P(name), Z_STRLEN_P(name) + 1, (void **)&ppval) == SUCCESS) {
			RETURN_ZVAL(*ppval, 1, 1);
		} else {
			ZVAL_ZVAL(return_value, config, 1, 0);
			skey = php_strtok_r(Z_STRVAL_P(name), ".", &token);
			while (skey) {
				if (zend_hash_find(Z_ARRVAL_P(return_value), skey, strlen(skey) + 1, (void **)&ppval) == SUCCESS){
					ZVAL_ZVAL(return_value, *ppval, 1, 1);
				} else {
					RETURN_NULL();
				}
				skey = php_strtok_r(NULL, ".", &token);
			}
		}
		zval_ptr_dtor(ppval);
	} else {
		RETURN_NULL();
	}
}
/* }}} */

/** {{{ proto public Yod_Application::import($alias)
*/
PHP_METHOD(yod_application, import) {
	zval *alias, *imports, *object, **ppval;
	char *classfile, *classname, *classpath;
	size_t classfile_len, classname_len;
	zend_class_entry **pce = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &alias) == FAILURE) {
		return;
	}

	if (!alias || Z_TYPE_P(alias) != IS_STRING) {
		RETURN_FALSE;
	}

	classfile = estrndup(Z_STRVAL_P(alias), Z_STRLEN_P(alias));
	classfile_len = 0;
	
	while (*classfile != '\0') {
		if (*classfile == '.' || *classfile == '\\') {
			*classfile = '/';
		}
		classfile++;
		classfile_len++;
	}
	classfile = classfile - classfile_len;

	php_basename(classfile, classfile_len, NULL, 0, &classname, &classname_len TSRMLS_CC);

	object = getThis();

	imports = zend_read_property(yod_application_ce, object, ZEND_STRL("_imports"), 1 TSRMLS_CC);
	if (zend_hash_find(Z_ARRVAL_P(imports), Z_STRVAL_P(alias), Z_STRLEN_P(alias) + 1, (void **)&ppval) == FAILURE) {
		
		spprintf(&classpath, 0, "%s/extends/%s.class.php", yod_runpath(TSRMLS_CC), classfile);

		if (VCWD_ACCESS(classpath, F_OK) == 0) {
			yod_execute_scripts(classpath, NULL, 1 TSRMLS_CC);
		}

		add_assoc_string_ex(imports, Z_STRVAL_P(alias), Z_STRLEN_P(alias) + 1, classpath, 1);
		zend_update_property(yod_application_ce, object, ZEND_STRL("_imports"), imports TSRMLS_CC);
	}

	if (zend_lookup_class_ex(classname, classname_len, 0, &pce TSRMLS_CC) == SUCCESS) {
		RETURN_TRUE;
	}
	RETURN_FALSE;
}
/* }}} */

/** {{{ proto public static Yod_Application::app($config = null)
*/
PHP_METHOD(yod_application, app) {
	zval *config;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z", &config) == FAILURE) {
		return;
	}

	return_value = yod_application_app(config);
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
}
/* }}} */

/* {{{ yod_application_methods[]
*/
zend_function_entry yod_application_methods[] = {
	PHP_ME(yod_application, __construct,	yod_application_construct_arginfo,	ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(yod_application, run,			yod_application_run_arginfo,		ZEND_ACC_PUBLIC)
	PHP_ME(yod_application, config,			yod_application_config_arginfo,		ZEND_ACC_PUBLIC)
	PHP_ME(yod_application, import,			yod_application_import_arginfo,		ZEND_ACC_PUBLIC)
	PHP_ME(yod_application, app,			yod_application_app_arginfo,		ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yod_application, __destruct,		yod_application_destruct_arginfo,	ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	{NULL, NULL, NULL}
};
/* }}} */

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

/** {{{ proto public Yod_Request::__construct($route = null)
*/
PHP_METHOD(yod_request, __construct) {
	zval *route, *method, *params;

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

	zend_declare_property_null(yod_database_ce, ZEND_STRL("_db"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC TSRMLS_CC);
	zend_declare_property_null(yod_database_ce, ZEND_STRL("_config"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_database_ce, ZEND_STRL("_driver"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_database_ce, ZEND_STRL("_linkid"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_database_ce, ZEND_STRL("_linkids"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_database_ce, ZEND_STRL("_result"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_database_ce, ZEND_STRL("_lastquery"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_bool(yod_database_ce, ZEND_STRL("_pconnect"), 0, ZEND_ACC_PROTECTED TSRMLS_CC);

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
	REGISTER_DOUBLE_CONSTANT("YOD_RUNTIME", YOD_G(runtime), CONST_CS);

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
		//yod_application_app();
	}
	zval_dtor(&runpath);

/*
	double runtime;
	struct timeval tp = {0};

	php_printf("<hr>");

	if (gettimeofday(&tp, NULL)) {
		runtime	= 0;	
	} else {
		runtime	= (double)(tp.tv_sec + tp.tv_usec / MICRO_IN_SEC);
	}

	runtime = (runtime - YOD_G(runtime)) * 1000;
	php_printf("%f\n", runtime);
*/
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
