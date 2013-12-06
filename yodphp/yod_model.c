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
#include "Zend/zend_interfaces.h"

#include "php_yod.h"
#include "yod_application.h"
#include "yod_model.h"
#include "yod_database.h"

zend_class_entry *yod_model_ce;

/** {{{ ARG_INFO
*/
ZEND_BEGIN_ARG_INFO_EX(yod_model_construct_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_model_init_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_model_getinstance_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_model_find_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, where)
	ZEND_ARG_INFO(0, params)
	ZEND_ARG_INFO(0, select)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_model_findall_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, where)
	ZEND_ARG_INFO(0, params)
	ZEND_ARG_INFO(0, select)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_model_count_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, where)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_model_save_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, data)
	ZEND_ARG_INFO(0, where)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_model_remove_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, where)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_model_lastquery_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_model_config_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_model_import_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, alias)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_model_model_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ yod_model_t *yod_model_construct(yod_model_t *object, char *name, uint name_len, zval *config TSRMLS_DC)
*/
yod_model_t *yod_model_construct(yod_model_t *object, char *name, uint name_len, zval *config TSRMLS_DC) {
	yod_model_t *retval;
	zval *p_name, *p_table, *p_dsn;
	char *table;
	uint table_len;

#if PHP_YOD_DEBUG
	yod_debugf("yod_model_construct(%s)", name ? name : "");
#endif

	if (object) {
		retval = object;
	} else {
		MAKE_STD_ZVAL(retval);
		object_init_ex(retval, yod_model_ce);
	}

#if PHP_YOD_DEBUG
	yod_debugf("yod_model_init()");
#endif

	if (zend_hash_exists(&(Z_OBJCE_P(retval)->function_table), ZEND_STRS("init"))) {
		zend_call_method_with_0_params(&retval, Z_OBJCE_P(retval), NULL, "init", NULL);
	}

	if (name_len == 0) {
		p_name = zend_read_property(Z_OBJCE_P(retval), retval, ZEND_STRL("_name"), 1 TSRMLS_CC);
		if (p_name && Z_TYPE_P(p_name) == IS_STRING && Z_STRLEN_P(p_name) > 0) {
			name_len = Z_STRLEN_P(p_name);
			name = estrndup(Z_STRVAL_P(p_name), name_len);
		} else {
			name_len = strlen(Z_OBJCE_P(retval)->name) - 5;
			if (name_len > 0 && strncmp(Z_OBJCE_P(retval)->name, "Yod_", 4)) {
				name = estrndup(Z_OBJCE_P(retval)->name, name_len);
			} else {
				name_len = 0;
				name = "";
			}
		}
	}
	zend_update_property_stringl(Z_OBJCE_P(retval), retval, ZEND_STRL("_name"), name, name_len TSRMLS_CC);

	p_table = zend_read_property(Z_OBJCE_P(retval), retval, ZEND_STRL("_table"), 1 TSRMLS_CC);
	if (!p_table || Z_TYPE_P(p_table) != IS_STRING || Z_STRLEN_P(p_table) == 0) {
		if (name_len > 0) {
			table = emalloc(name_len + 1);
			*table = tolower(*name);
			table_len = 1;
			table++;
			name++;
			while (*name != '\0') {
				if (isupper(*name)) {
					*table = '_';
					table_len++;
					table++;
				}
				*table = tolower(*name);
				table_len++;
				table++;
				name++;
			}
			*table = '\0';
			table = table - table_len;
			zend_update_property_stringl(Z_OBJCE_P(retval), retval, ZEND_STRL("_table"), table, table_len TSRMLS_CC);
			efree(table);
		}
	}

	if (!config) {
		p_dsn = zend_read_property(Z_OBJCE_P(retval), retval, ZEND_STRL("_dsn"), 1 TSRMLS_CC);
		if (p_dsn && Z_TYPE_P(p_dsn) == IS_STRING || Z_STRLEN_P(p_dsn) > 0) {
			MAKE_STD_ZVAL(config);
			yod_application_config(Z_STRVAL_P(p_dsn), Z_STRLEN_P(p_dsn), config);
		}
	}

/*
	if ($this->_db = Yod_Database::getInstance($config)) {
		$this->_prefix = $this->_db->config('prefix');
	}
*/

/*
	if (EG(called_scope)) {
		//RETURN_STRINGL(EG(called_scope)->name, EG(called_scope)->name_length, 1);
		php_printf("called_scope:%s\n", EG(called_scope)->name);
	}
	if (retval) {
		php_printf("this:%s\n", Z_OBJCE_P(retval)->name);
	}
*/
	return retval;
}
/* }}} */

/** {{{ void yod_model_getinstance(char *name, uint name_len, zval *config, zval *result TSRMLS_DC)
*/
void yod_model_getinstance(char *name, uint name_len, zval *config, zval *result TSRMLS_DC) {
	yod_model_t *object;
	zval *p_model, **ppval;
	char *classname, *classpath;
	uint classname_len;
	zend_class_entry **pce = NULL;

#if PHP_YOD_DEBUG
	yod_debugf("yod_model_getinstance(%s)", name ? name : "");
#endif

	classname_len = spprintf(&classname, 0, "%sModel", name);

	p_model = zend_read_static_property(yod_model_ce, ZEND_STRL("_model"), 0 TSRMLS_CC);
	if (p_model && Z_TYPE_P(p_model) == IS_ARRAY) {
		if (zend_hash_find(Z_ARRVAL_P(p_model), name, name_len + 1, (void **)&ppval) == SUCCESS) {
		}
	} else {
		MAKE_STD_ZVAL(p_model);
		array_init(p_model);
	}

	spprintf(&classpath, 0, "%s/models/%s.class.php", yod_runpath(TSRMLS_CC), classname);

	MAKE_STD_ZVAL(object);
	if (VCWD_ACCESS(classpath, F_OK) == 0) {
		yod_include(classpath, NULL, 1 TSRMLS_CC);
		if (zend_lookup_class_ex(classname, classname_len, 0, &pce TSRMLS_CC) == SUCCESS) {
			object_init_ex(object, *pce);
			object = yod_model_construct(object, name, name_len, config TSRMLS_CC);
		} else {
			php_error_docref(NULL TSRMLS_CC, E_ERROR, "Class '%s' not found", classname);
		}
	} else {
		if (zend_lookup_class_ex(classname, classname_len, 0, &pce TSRMLS_CC) == SUCCESS) {
			object_init_ex(object, *pce);
			object = yod_model_construct(object, name, name_len, config TSRMLS_CC);
		} else {
			object = yod_model_construct(NULL, name, name_len, config TSRMLS_CC);
		}
	}

	ZVAL_ZVAL(result, object, 1, 1);

	add_assoc_zval_ex(p_model, name, name_len + 1, result);
	zend_update_static_property(yod_model_ce, ZEND_STRL("_model"), p_model TSRMLS_CC);
}
/* }}} */

/** {{{ proto public Yod_Model::__construct($name = '', $config = '')
*/
PHP_METHOD(yod_model, __construct) {
	char *name = NULL;
	uint name_len = 0;
	zval *config = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|sz", &name, &name_len, &config) == FAILURE) {
		return;
	}

	(void)yod_model_construct(getThis(), name, name_len, config TSRMLS_CC);
}
/* }}} */

/** {{{ proto public Yod_Model::init()
*/
PHP_METHOD(yod_model, init) {

}
/* }}} */

/** {{{ proto public Yod_Model::getInstance($name, $config = '')
*/
PHP_METHOD(yod_model, getInstance) {
	char *name = NULL;
	uint name_len = 0;
	zval *config = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|z", &name, &name_len, &config) == FAILURE) {
		return;
	}

	yod_model_getinstance(name, name_len, config, return_value TSRMLS_CC);
}
/* }}} */

/** {{{ proto public Yod_Model::find($where = '', $params = array(), $select = '*')
*/
PHP_METHOD(yod_model, find) {

}
/* }}} */

/** {{{ proto public Yod_Model::findAll($where = '', $params = array(), $select = '*')
*/
PHP_METHOD(yod_model, findAll) {

}
/* }}} */

/** {{{ proto public Yod_Model::count($where = '', $params = array())
*/
PHP_METHOD(yod_model, count) {

}
/* }}} */

/** {{{ proto public Yod_Model::save($data, $where = '', $params = array())
*/
PHP_METHOD(yod_model, save) {

}
/* }}} */

/** {{{ proto public Yod_Model::remove($where, $params = array())
*/
PHP_METHOD(yod_model, remove) {

}
/* }}} */

/** {{{ proto public Yod_Model::lastQuery()
*/
PHP_METHOD(yod_model, lastQuery) {

}
/* }}} */

/** {{{ proto public Yod_Model::config($name = null)
*/
PHP_METHOD(yod_model, config) {
	char *name = NULL;
	uint name_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &name, &name_len) == FAILURE) {
		return;
	}

	(void)yod_application_config(name, name_len, return_value TSRMLS_CC);
}
/* }}} */

/** {{{ proto public Yod_Model::import($alias)
*/
PHP_METHOD(yod_model, import) {
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

/** {{{ proto public Yod_Model::model($name = '', $config = '', $dbmod = false)
*/
PHP_METHOD(yod_model, model) {
	zval *config = NULL;
	char *name = NULL;
	uint name_len = 0;
	int dbmod = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|zb", &name, &name_len, &config, &dbmod) == FAILURE) {
		return;
	}

#if PHP_YOD_DEBUG
	yod_debugf("yod_model_model(%s)", name ? name : "");
#endif

	if (name_len == 0) {
		RETURN_ZVAL(getThis(), 1, 0);
	}

	if (config && Z_TYPE_P(config) == IS_BOOL) {
		dbmod = Z_BVAL_P(config);
		ZVAL_NULL(config);
	}
	
	if (dbmod) {
		yod_dbmodel_getinstance(name, name_len, config, return_value TSRMLS_CC);
	} else {
		yod_model_getinstance(name, name_len, config, return_value TSRMLS_CC);
	}
}
/* }}} */

/** {{{ proto public Yod_Model::__destruct()
*/
PHP_METHOD(yod_model, __destruct) {

}
/* }}} */

/** {{{ yod_model_methods[]
*/
zend_function_entry yod_model_methods[] = {
	PHP_ME(yod_model, __construct,		yod_model_construct_arginfo,	ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(yod_model, init,				yod_model_init_arginfo,			ZEND_ACC_PROTECTED)
	PHP_ME(yod_model, getInstance,		yod_model_getinstance_arginfo,	ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yod_model, find,				yod_model_find_arginfo,			ZEND_ACC_PUBLIC)
	PHP_ME(yod_model, findAll,			yod_model_findall_arginfo,		ZEND_ACC_PUBLIC)
	PHP_ME(yod_model, count,			yod_model_count_arginfo,		ZEND_ACC_PUBLIC)
	PHP_ME(yod_model, save,				yod_model_save_arginfo,			ZEND_ACC_PUBLIC)
	PHP_ME(yod_model, remove,			yod_model_remove_arginfo,		ZEND_ACC_PUBLIC)
	PHP_ME(yod_model, lastQuery,		yod_model_lastquery_arginfo,	ZEND_ACC_PUBLIC)
	PHP_ME(yod_model, config,			yod_model_config_arginfo,		ZEND_ACC_PROTECTED)
	PHP_ME(yod_model, import,			yod_model_import_arginfo,		ZEND_ACC_PROTECTED)
	PHP_ME(yod_model, model,			yod_model_model_arginfo,		ZEND_ACC_PROTECTED)
	PHP_ME(yod_model, __destruct,		NULL,	ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	{NULL, NULL, NULL}
};
/* }}} */

/** {{{ PHP_MINIT_FUNCTION
*/
PHP_MINIT_FUNCTION(yod_model) {
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "Yod_Model", yod_model_methods);
	yod_model_ce = zend_register_internal_class(&ce TSRMLS_CC);

	zend_declare_property_null(yod_model_ce, ZEND_STRL("_model"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC TSRMLS_CC);
	zend_declare_property_null(yod_model_ce, ZEND_STRL("_db"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_string(yod_model_ce, ZEND_STRL("_dsn"), "db_dsn", ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_model_ce, ZEND_STRL("_name"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_model_ce, ZEND_STRL("_table"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_model_ce, ZEND_STRL("_prefix"), ZEND_ACC_PROTECTED TSRMLS_CC);

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
