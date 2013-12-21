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

#include "php_yod.h"
#include "yod_application.h"
#include "yod_request.h"

zend_class_entry *yod_application_ce;

/** {{{ ARG_INFO
 */
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
	ZEND_ARG_INFO(0, classext)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_application_app_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_application_autorun_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_application_autoload_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, classname)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_application_destruct_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ static int yod_application_init_autoload(TSRMLS_DC)
 * */
static int yod_application_init_autoload(TSRMLS_DC) {
	zval *param1, *function, *retval = NULL;
	zval **params[1] = {&param1};

#if PHP_YOD_DEBUG
	yod_debugf("yod_application_init_autoload()");
#endif

	MAKE_STD_ZVAL(param1);
	array_init(param1);
	add_next_index_string(param1, YOD_APP_CNAME, 1);
	add_next_index_string(param1, "autoload", 1);

	MAKE_STD_ZVAL(function);
	ZVAL_STRING(function, "spl_autoload_register", 1);

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

	zend_call_function(&fci, NULL TSRMLS_CC);

	zval_ptr_dtor(&function);
	zval_ptr_dtor(&param1);

	if (retval) {
		zval_ptr_dtor(&retval);
		return 1;
	}
	return 0;
}
/* }}} */

/** {{{ static void yod_application_init_config(yod_application_t *object, zval *config)
*/
static void yod_application_init_config(yod_application_t *object, zval *config) {
	zval *retval, **ppval;
	char *filepath;
	size_t filepath_len;

	DIR *dir = NULL;
	char dentry[sizeof(struct dirent) + MAXPATHLEN];
	struct dirent *entry = (struct dirent *) &dentry;

#if PHP_YOD_DEBUG
	yod_debugf("yod_application_init_config()");
#endif

	if(config && Z_TYPE_P(config) == IS_ARRAY) {
		zend_update_property(yod_application_ce, object, ZEND_STRL("_config"), config TSRMLS_CC);
		return;
	}

	if(config && Z_TYPE_P(config) == IS_STRING) {
		spprintf(&filepath, 0, "%s", Z_STRVAL_P(config));
	} else {
		spprintf(&filepath, 0, "%s/configs/config.php", yod_runpath(TSRMLS_CC));
	}

	if (VCWD_ACCESS(filepath, F_OK) == 0) {
		yod_include(filepath, &retval, 0 TSRMLS_CC);
	} else {
		MAKE_STD_ZVAL(retval);
		array_init(retval);

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
						yod_include(filename, &value, 0 TSRMLS_CC);
						if (Z_TYPE_P(value) == IS_ARRAY) {
							if (entry_len == 15 && strncmp(entry->d_name, "base", 4) == 0) {
								php_array_merge(Z_ARRVAL_P(retval), Z_ARRVAL_P(value), 0 TSRMLS_DC);
							} else {
								uint key_len = entry_len - 11;
								char *key = estrndup(entry->d_name, key_len);

								if (zend_hash_find(Z_ARRVAL_P(retval), key, key_len + 1, (void **)&ppval) == SUCCESS &&
									Z_TYPE_PP(ppval) == IS_ARRAY
								) {
									php_array_merge(Z_ARRVAL_P(value), Z_ARRVAL_PP(ppval), 0 TSRMLS_DC);
								}
								add_assoc_zval_ex(retval, key, key_len + 1, value);
								efree(key);
							}
						}
					}
					efree(filename);
				}
				
			}
			closedir(dir);
		}
	}
	efree(filepath);

	if (zend_hash_find(&EG(symbol_table), "config", sizeof("config"), (void **) &ppval) == SUCCESS &&
		Z_TYPE_PP(ppval) == IS_ARRAY
	) {
		php_array_merge(Z_ARRVAL_P(retval), Z_ARRVAL_PP(ppval), 0 TSRMLS_DC);
	}

	zend_update_property(yod_application_ce, object, ZEND_STRL("_config"), retval TSRMLS_CC);
	zval_ptr_dtor(&retval);
}
/* }}} */

/** {{{ static void yod_application_construct(yod_application_t *object, zval *config TSRMLS_DC)
*/
static void yod_application_construct(yod_application_t *object, zval *config TSRMLS_DC) {
	yod_request_t *request;
	zval *imports, runmode;

#if PHP_YOD_DEBUG
	yod_debugf("yod_application_construct()");
#endif

	if (YOD_G(yodapp)) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Only one application can be initialized");
		return;
	}

	if (object) {
		YOD_G(yodapp) = object;
	} else {
		MAKE_STD_ZVAL(YOD_G(yodapp));
		object_init_ex(YOD_G(yodapp), yod_application_ce);
	}

	// runmode
	if (zend_get_constant(ZEND_STRL("YOD_RUNMODE"), &runmode TSRMLS_CC)) {
		convert_to_long(&runmode);
		EG(error_reporting) = Z_LVAL(runmode);
		zval_dtor(&runmode);
	}

	// autoload
	yod_application_init_autoload(TSRMLS_CC);

	// config
	yod_application_init_config(YOD_G(yodapp), config);

	// request
	request = yod_request_construct(NULL, NULL, 0 TSRMLS_CC);
	zend_update_property(yod_application_ce, YOD_G(yodapp), ZEND_STRL("_request"), request TSRMLS_CC);
	zval_ptr_dtor(&request);

	// imports
	MAKE_STD_ZVAL(imports);
	array_init(imports);
	zend_update_property(yod_application_ce, YOD_G(yodapp), ZEND_STRL("_imports"), imports TSRMLS_CC);
	zval_ptr_dtor(&imports);

	// app
	zend_update_static_property(yod_application_ce, ZEND_STRL("_app"), YOD_G(yodapp) TSRMLS_CC);
}
/* }}} */

/** {{{ void yod_application_run(TSRMLS_DC)
*/
void yod_application_run(TSRMLS_DC) {
	yod_request_t *request;

#if PHP_YOD_DEBUG
	yod_debugf("yod_application_run()");
#endif

	if (YOD_G(running)) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "An application instance already running");
		return;
	}
	YOD_G(running) = 1;

	zend_update_property_bool(yod_application_ce, YOD_G(yodapp), ZEND_STRL("_running"), 1 TSRMLS_CC);

	request = zend_read_property(yod_application_ce, YOD_G(yodapp), ZEND_STRL("_request"), 1 TSRMLS_CC);
	yod_request_dispatch(request TSRMLS_CC);
}
/* }}} */

/** {{{ int yod_application_config(char *name, uint name_len, zval *result TSRMLS_DC)
*/
int yod_application_config(char *name, uint name_len, zval *result TSRMLS_DC) {
	zval *config;

#if PHP_YOD_DEBUG
	yod_debugf("yod_application_config(%s)", name ? name : "");
#endif

	if (!YOD_G(yodapp)) {
		ZVAL_NULL(result);
		return 0;
	}

	config = zend_read_property(yod_application_ce, YOD_G(yodapp), ZEND_STRL("_config"), 1 TSRMLS_CC);
	if (Z_TYPE_P(config) != IS_ARRAY) {
		ZVAL_NULL(result);
		return 0;
	}

	if (name_len == 0) {
		ZVAL_ZVAL(result, config, 1, 0);
		return 1;
	} else {
		zval **ppval;
		char *skey, *token;

		if (zend_hash_find(Z_ARRVAL_P(config), name, name_len + 1, (void **)&ppval) == SUCCESS) {
			ZVAL_ZVAL(result, *ppval, 1, 0);
			return 1;
		} else {
			ZVAL_ZVAL(result, config, 1, 0);
			skey = php_strtok_r(name, ".", &token);
			while (skey) {
				if (zend_hash_find(Z_ARRVAL_P(result), skey, strlen(skey) + 1, (void **)&ppval) == SUCCESS) {
					ZVAL_ZVAL(result, *ppval, 1, 1);
				} else {
					ZVAL_NULL(result);
					return 0;
				}
				skey = php_strtok_r(NULL, ".", &token);
			}
		}
		zval_ptr_dtor(ppval);
	}
	return 1;
}
/* }}} */

/** {{{ int yod_application_import(char *alias, uint alias_len, char *classext, uint classext_len TSRMLS_DC)
*/
int yod_application_import(char *alias, uint alias_len, char *classext, uint classext_len TSRMLS_DC) {
	zval *p_imports, *pzval, **ppval;
	char *classfile, *classname, *classpath;
	uint classfile_len, classname_len;
	zend_class_entry **pce = NULL;

#if PHP_YOD_DEBUG
	yod_debugf("yod_application_import(%s)", alias ? alias : "");
#endif

	if (!YOD_G(yodapp) || alias_len == 0) {
		return 0;
	}

	classfile = estrndup(alias, alias_len);
	classfile_len = 0;
	
	while (*classfile != '\0') {
		if (*classfile == '.' || *classfile == '\\') {
			*classfile = '/';
		}
		classfile++;
		classfile_len++;
	}

	while (*classfile == '/') {
		classfile--;
		classfile_len--;
	}

	classfile = classfile - classfile_len;

	while (*classfile == '/') {
		classfile++;
		classfile_len--;
	}

	php_basename(classfile, classfile_len, NULL, 0, &classname, &classname_len TSRMLS_CC);

	p_imports = zend_read_property(yod_application_ce, YOD_G(yodapp), ZEND_STRL("_imports"), 1 TSRMLS_CC);
	if (zend_hash_find(Z_ARRVAL_P(p_imports), alias, alias_len + 1, (void **)&ppval) == FAILURE) {
		if (classext_len) {
			spprintf(&classpath, 0, "%s/extends/%s%s", yod_runpath(TSRMLS_CC), classfile, classext);
		} else {
			spprintf(&classpath, 0, "%s/extends/%s.class.php", yod_runpath(TSRMLS_CC), classfile);
		}

		if (VCWD_ACCESS(classpath, F_OK) == 0) {
			yod_include(classpath, NULL, 1 TSRMLS_CC);
		}
		efree(classpath);

		add_assoc_string_ex(p_imports, alias, alias_len + 1, classpath, 1);
		zend_update_property(yod_application_ce, YOD_G(yodapp), ZEND_STRL("_imports"), p_imports TSRMLS_CC);
	}
	efree(classfile);

#if PHP_API_VERSION < 20100412
	if (zend_lookup_class_ex(classname, classname_len, 0, &pce TSRMLS_CC) == SUCCESS) {
#else
	if (zend_lookup_class_ex(classname, classname_len, NULL, 0, &pce TSRMLS_CC) == SUCCESS) {
#endif
		efree(classname);
		return 1;
	}
	efree(classname);
	return 0;
}
/* }}} */

/** {{{ void yod_application_app(zval *config TSRMLS_DC)
*/
void yod_application_app(zval *config TSRMLS_DC) {

#if PHP_YOD_DEBUG
	yod_debugf("yod_application_app()");
#endif

	if (!YOD_G(yodapp)) {
		yod_application_construct(NULL, config TSRMLS_CC);
	}
}
/* }}} */

/** {{{ static void yod_application_autorun(TSRMLS_DC)
*/
static void yod_application_autorun(TSRMLS_DC) {
	zval runpath;

#if PHP_YOD_DEBUG
	yod_debugf("yod_application_autorun()");
#endif

	if (zend_get_constant(ZEND_STRL("YOD_RUNPATH"), &runpath TSRMLS_CC)) {
		if (!YOD_G(yodapp) && !YOD_G(exited)) {
			SG(headers_sent) = 0;

			yod_application_app(NULL TSRMLS_CC);
			yod_application_run(TSRMLS_CC);

			if (!SG(headers_sent)) {
				sapi_send_headers(TSRMLS_C);
			}
		}
		zval_dtor(&runpath);
	}
}
/* }}} */

/** {{{ static int yod_application_autoload(char *classname, uint classname_len TSRMLS_DC)
*/
static int yod_application_autoload(char *classname, uint classname_len TSRMLS_DC) {
	zend_class_entry **pce = NULL;
	char *classfile, *classpath;
	zval *retval;

	if (!YOD_G(yodapp)) return 0;

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
			spprintf(&classpath, 0, "%s/extends/%s.class.php", yod_runpath(TSRMLS_CC), classfile);
		}
	}
	efree(classfile);

	if (VCWD_ACCESS(classpath, F_OK) == 0) {
		yod_include(classpath, &retval, 1 TSRMLS_CC);
	}
	
#if PHP_YOD_DEBUG
	yod_debugf("yod_application_autoload(%s):%s", classname, classpath);
#endif

	efree(classpath);

#if PHP_API_VERSION < 20100412
	if (zend_lookup_class_ex(classname, classname_len, 0, &pce TSRMLS_CC) == SUCCESS) {
#else
	if (zend_lookup_class_ex(classname, classname_len, NULL, 0, &pce TSRMLS_CC) == SUCCESS) {
#endif
		return 1;
	}
	return 0;
}
/* }}} */

/** {{{ proto public Yod_Application::__construct($config = null)
*/
PHP_METHOD(yod_application, __construct) {
	zval *config = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z", &config) == FAILURE) {
		return;
	}

	yod_application_construct(getThis(), config TSRMLS_CC);
}
/* }}} */

/** {{{ proto public Yod_Application::run()
*/
PHP_METHOD(yod_application, run) {
	yod_application_run(TSRMLS_CC);
}
/* }}} */

/** {{{ proto public Yod_Application::config($name = null)
*/
PHP_METHOD(yod_application, config) {
	char *name = NULL;
	uint name_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &name, &name_len) == FAILURE) {
		return;
	}

	yod_application_config(name, name_len, return_value TSRMLS_CC);
}
/* }}} */

/** {{{ proto public Yod_Application::import($alias, $classext = '.class.php')
*/
PHP_METHOD(yod_application, import) {
	char *alias = NULL, *classext = NULL;
	uint alias_len = 0, classext_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s", &alias, &alias_len, &classext, &classext_len) == FAILURE) {
		return;
	}

	if (yod_application_import(alias, alias_len, classext, classext_len TSRMLS_CC)) {
		RETURN_TRUE;
	}
	RETURN_FALSE;
}
/* }}} */

/** {{{ proto public static Yod_Application::app($config = null)
*/
PHP_METHOD(yod_application, app) {
	zval *config = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z", &config) == FAILURE) {
		return;
	}

	yod_application_app(config);

	RETURN_ZVAL(YOD_G(yodapp), 1, 0);
}
/* }}} */

/** {{{ proto public static Yod_Application::autorun()
*/
PHP_METHOD(yod_application, autorun) {

	yod_application_autorun(TSRMLS_CC);
}
/* }}} */

/** {{{ proto public static Yod_Application::autoload($classname)
*/
PHP_METHOD(yod_application, autoload) {
	char *classname;
	uint classname_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &classname, &classname_len) == FAILURE) {
		return;
	}

	if (yod_application_autoload(classname, classname_len TSRMLS_CC)) {
		RETURN_TRUE;
	}
	RETURN_FALSE;
}
/* }}} */

/** {{{ proto public Yod_Application::__destruct()
*/
PHP_METHOD(yod_application, __destruct) {

	if (!YOD_G(running) && !YOD_G(exited)) {
		yod_application_run(TSRMLS_CC);
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
	PHP_ME(yod_application, autorun,		yod_application_autorun_arginfo,	ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yod_application, autoload,		yod_application_autoload_arginfo,	ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yod_application, __destruct,		yod_application_destruct_arginfo,	ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	{NULL, NULL, NULL}
};
/* }}} */

/** {{{ PHP_MINIT_FUNCTION
*/
PHP_MINIT_FUNCTION(yod_application) {
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "Yod_Application", yod_application_methods);
	yod_application_ce = zend_register_internal_class(&ce TSRMLS_CC);
	yod_application_ce->ce_flags |= ZEND_ACC_FINAL_CLASS;

	zend_declare_property_null(yod_application_ce, ZEND_STRL("_app"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC TSRMLS_CC);
	zend_declare_property_null(yod_application_ce, ZEND_STRL("_config"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_application_ce, ZEND_STRL("_request"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_application_ce, ZEND_STRL("_imports"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_bool(yod_application_ce, ZEND_STRL("_running"), 0, ZEND_ACC_PROTECTED TSRMLS_CC);

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
