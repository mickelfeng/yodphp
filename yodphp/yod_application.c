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
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_application_app_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_application_destruct_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ void yod_application_init_config(yod_application_t *object, zval *config)
*/
void yod_application_init_config(yod_application_t *object, zval *config) {
	zval *result;
	char *filepath;
	size_t filepath_len;

	DIR *dir = NULL;
	char dentry[sizeof(struct dirent) + MAXPATHLEN];
	struct dirent *entry = (struct dirent *) &dentry;

	if(config && Z_TYPE_P(config) == IS_ARRAY) {
		zend_update_property(yod_application_ce, object, ZEND_STRL("_config"), config TSRMLS_CC);
		return;
	}

	if(config && Z_TYPE_P(config) == IS_STRING) {
		filepath = Z_STRVAL_P(config);
	} else {
		spprintf(&filepath, 0, "%s/configs/config.php", yod_runpath(TSRMLS_CC));
	}

	if (VCWD_ACCESS(filepath, F_OK) == 0) {
		yod_execute_scripts(filepath, &result, 0 TSRMLS_CC);
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
		
		efree(filepath);

	}
	zend_update_property(yod_application_ce, object, ZEND_STRL("_config"), result TSRMLS_CC);

}
/* }}} */

/** {{{ void yod_application_instance(yod_application_t *this_ptr, zval *config TSRMLS_DC)
*/
void yod_application_instance(yod_application_t *this_ptr, zval *config TSRMLS_DC) {
	yod_application_t *object;
	yod_request_t *request;
	zval *imports;

	if (YOD_G(yodapp)) {
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
	zval_ptr_dtor(&imports);

	// app
	zend_update_static_property(yod_application_ce, ZEND_STRL("_app"), object TSRMLS_CC);
	YOD_G(yodapp) = object;
}
/* }}} */

/** {{{ void yod_application_run(TSRMLS_DC)
*/
void yod_application_run(TSRMLS_DC) {
	yod_application_t *object;
	yod_request_t *request;

	if (YOD_G(running)) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "An application instance already running");
		return;
	}
	YOD_G(running) = 1;

	object = YOD_G(yodapp);

	zend_update_property_bool(yod_application_ce, object, ZEND_STRL("_running"), 1 TSRMLS_CC);
	
	request = zend_read_property(yod_application_ce, object, ZEND_STRL("_request"), 1 TSRMLS_CC);
	yod_request_dispatch(request TSRMLS_CC);
}
/* }}} */

/** {{{ int yod_application_config(zval *name, zval *result TSRMLS_DC)
*/
int yod_application_config(zval *name, zval *result TSRMLS_DC) {
	yod_application_t *object;
	zval *config;

	if (!YOD_G(yodapp)) {
		ZVAL_NULL(result);
		return 0;
	}

	object = YOD_G(yodapp);
	
	config = zend_read_property(yod_application_ce, object, ZEND_STRL("_config"), 1 TSRMLS_CC);

	if (Z_TYPE_P(config) != IS_ARRAY) {
		ZVAL_NULL(result);
		return 0;
	}

	if (!name) {
		ZVAL_ZVAL(result, config, 1, 0);
		return 1;
	} else if (Z_TYPE_P(name) == IS_STRING) {
		zval **ppval;
		char *skey, *token;

		if (zend_hash_find(Z_ARRVAL_P(config), Z_STRVAL_P(name), Z_STRLEN_P(name) + 1, (void **)&ppval) == SUCCESS) {
			ZVAL_ZVAL(result, *ppval, 1, 1);
			return 1;
		} else {
			ZVAL_ZVAL(result, config, 1, 0);
			skey = php_strtok_r(Z_STRVAL_P(name), ".", &token);
			while (skey) {
				if (zend_hash_find(Z_ARRVAL_P(result), skey, strlen(skey) + 1, (void **)&ppval) == SUCCESS){
					ZVAL_ZVAL(result, *ppval, 1, 1);
				} else {
					zval_ptr_dtor(ppval);
					ZVAL_NULL(result);
					return 0;
				}
				skey = php_strtok_r(NULL, ".", &token);
			}
		}
		zval_ptr_dtor(ppval);
	} else {
		ZVAL_NULL(result);
		return 0;
	}
	return 1;
}
/* }}} */

/** {{{ int yod_application_import(zval *alias TSRMLS_DC)
*/
int yod_application_import(zval *alias TSRMLS_DC) {
	yod_application_t *object;
	zval *imports, **ppval;
	char *classfile, *classname, *classpath;
	size_t classfile_len, classname_len;
	zend_class_entry **pce = NULL;

	if (!alias || Z_TYPE_P(alias) != IS_STRING || !YOD_G(yodapp)) {
		return 0;
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

	object = YOD_G(yodapp);

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
		return 1;
	}
	return 0;
}
/* }}} */

/** {{{ void yod_application_app(zval *config TSRMLS_DC)
*/
void yod_application_app(zval *config TSRMLS_DC) {

	if (!YOD_G(yodapp)) {
		yod_application_instance(NULL, config TSRMLS_CC);
	}
}
/* }}} */

/** {{{ proto public Yod_Application::__construct($config = null)
*/
PHP_METHOD(yod_application, __construct) {
	zval *config = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z", &config) == FAILURE) {
		return;
	}

	yod_application_instance(getThis(), config TSRMLS_CC);
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
	zval *name = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z", &name) == FAILURE) {
		return;
	}

	yod_application_config(name, return_value TSRMLS_CC);
}
/* }}} */

/** {{{ proto public Yod_Application::import($alias)
*/
PHP_METHOD(yod_application, import) {
	zval *alias = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &alias) == FAILURE) {
		return;
	}

	if (yod_application_import(alias TSRMLS_CC)) {
		RETURN_TRUE;
	}
	RETURN_FALSE;
}
/* }}} */

/** {{{ proto public static Yod_Application::app($config = null)
*/
PHP_METHOD(yod_application, app) {
	zval *object, *config = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z", &config) == FAILURE) {
		return;
	}

	yod_application_app(config);
	RETURN_ZVAL(YOD_G(yodapp), 1, 0);
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
