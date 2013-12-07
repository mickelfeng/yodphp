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
#include "yod_application.h"
#include "yod_model.h"
#include "yod_database.h"

zend_class_entry *yod_database_ce;

/** {{{ ARG_INFO
 */
ZEND_BEGIN_ARG_INFO_EX(yod_database_construct_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_database_db_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_database_getinstance_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_database_config_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_database_create_arginfo, 0, 0, 2)
	ZEND_ARG_INFO(0, fields)
	ZEND_ARG_INFO(0, table)
	ZEND_ARG_INFO(0, extend)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_database_insert_arginfo, 0, 0, 2)
	ZEND_ARG_INFO(0, data)
	ZEND_ARG_INFO(0, table)
	ZEND_ARG_INFO(0, replace)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_database_update_arginfo, 0, 0, 2)
	ZEND_ARG_INFO(0, data)
	ZEND_ARG_INFO(0, table)
	ZEND_ARG_INFO(0, where)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_database_delete_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, table)
	ZEND_ARG_INFO(0, where)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_database_select_arginfo, 0, 0, 2)
	ZEND_ARG_INFO(0, select)
	ZEND_ARG_INFO(0, table)
	ZEND_ARG_INFO(0, where)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_database_lastquery_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_database_dbconfig_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, config)
	ZEND_ARG_INFO(0, linknum)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_database_connect_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, config)
	ZEND_ARG_INFO(0, linknum)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_database_fields_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, table)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_database_execute_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, query)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_database_query_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, query)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_database_fetch_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, result)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_database_fetchall_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, result)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_database_transaction_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_database_commit_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_database_rollback_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_database_insertid_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_database_quote_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, string)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_database_free_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, result)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_database_close_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ yod_database_t *yod_database_construct(yod_database_t *object, zval *config TSRMLS_DC)
*/
yod_database_t *yod_database_construct(yod_database_t *object, zval *config TSRMLS_DC) {
	yod_database_t *retval;


#if PHP_YOD_DEBUG
	yod_debugf("yod_database_construct()");
#endif

	if (object) {
		retval = object;
	} else {
		MAKE_STD_ZVAL(retval);
		object_init_ex(retval, yod_database_ce);
	}

	if (config) {
		zend_update_property(Z_OBJCE_P(retval), retval, ZEND_STRL("_config"), config TSRMLS_CC);
	}

	return retval;
}
/* }}} */

/** {{{ int yod_database_getinstance(zval *config, zval *result TSRMLS_DC)
*/
int yod_database_getinstance(zval *config, zval *result TSRMLS_DC) {

#if PHP_YOD_DEBUG
	yod_debugf("yod_database_getinstance()");
#endif

}
/* }}} */

/** {{{ char *yod_database_config(yod_database_t *object, char *name, uint name_len, zval *value TSRMLS_DC)
*/
zval *yod_database_config(yod_database_t *object, char *name, uint name_len, zval *value TSRMLS_DC) {
	zval *retval;

#if PHP_YOD_DEBUG
	yod_debugf("yod_database_config(%s)", name ? name : "");
#endif

	MAKE_STD_ZVAL(retval);
	ZVAL_NULL(retval);

	return retval;
}
/* }}} */

/** {{{ proto public Yod_Database::__construct($config)
*/
PHP_METHOD(yod_database, __construct) {
	zval *config = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &config) == FAILURE) {
		return;
	}

	(void)yod_database_construct(getThis(), config TSRMLS_CC);
}
/* }}} */

/** {{{ proto public Yod_Database::db($config = 'db_dsn')
*/
PHP_METHOD(yod_database, db) {

}
/* }}} */

/** {{{ proto public Yod_Database::getInstance($config = 'db_dsn')
*/
PHP_METHOD(yod_database, getInstance) {

}
/* }}} */

/** {{{ proto public Yod_Database::config($name = '', $value = null)
*/
PHP_METHOD(yod_database, config) {

}
/* }}} */

/** {{{ proto public Yod_Database::create($fields, $table, $extend = '')
*/
PHP_METHOD(yod_database, create) {

}
/* }}} */

/** {{{ proto public Yod_Database::insert($data, $table, $replace=false)
*/
PHP_METHOD(yod_database, insert) {

}
/* }}} */

/** {{{ proto public Yod_Database::update($data, $table, $where = null, $params = array())
*/
PHP_METHOD(yod_database, update) {

}
/* }}} */

/** {{{ proto public Yod_Database::delete($table, $where = null, $params = array())
*/
PHP_METHOD(yod_database, delete) {

}
/* }}} */

/** {{{ proto public Yod_Database::select($select, $table, $where = null, $params = array())
*/
PHP_METHOD(yod_database, select) {

}
/* }}} */

/** {{{ proto public Yod_Database::lastQuery()
*/
PHP_METHOD(yod_database, lastQuery) {

}
/* }}} */

/** {{{ proto public Yod_Database::dbconfig($config, $linknum = 0)
*/
PHP_METHOD(yod_database, dbconfig) {

}
/* }}} */

/** {{{ proto public Yod_Database::__destruct()
*/
PHP_METHOD(yod_database, __destruct) {

}
/* }}} */

/** {{{ yod_database_methods[]
*/
zend_function_entry yod_database_methods[] = {
	PHP_ME(yod_database, __construct,		yod_database_construct_arginfo,		ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(yod_database, db,				yod_database_db_arginfo,			ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yod_database, getInstance,		yod_database_getinstance_arginfo,	ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yod_database, config,			yod_database_config_arginfo,		ZEND_ACC_PUBLIC)
	PHP_ME(yod_database, create,			yod_database_create_arginfo,		ZEND_ACC_PUBLIC)
	PHP_ME(yod_database, insert,			yod_database_insert_arginfo,		ZEND_ACC_PUBLIC)
	PHP_ME(yod_database, update,			yod_database_update_arginfo,		ZEND_ACC_PUBLIC)
	PHP_ME(yod_database, delete,			yod_database_delete_arginfo,		ZEND_ACC_PUBLIC)
	PHP_ME(yod_database, select,			yod_database_select_arginfo,		ZEND_ACC_PUBLIC)
	PHP_ME(yod_database, lastQuery,			yod_database_lastquery_arginfo,		ZEND_ACC_PUBLIC)
	PHP_ME(yod_database, dbconfig,			yod_database_dbconfig_arginfo,		ZEND_ACC_PROTECTED)
	ZEND_ABSTRACT_ME(yod_database, connect,		yod_database_connect_arginfo)
	ZEND_ABSTRACT_ME(yod_database, fields,		yod_database_fields_arginfo)
	ZEND_ABSTRACT_ME(yod_database, execute,		yod_database_execute_arginfo)
	ZEND_ABSTRACT_ME(yod_database, query,		yod_database_query_arginfo)
	ZEND_ABSTRACT_ME(yod_database, fetch,		yod_database_fetch_arginfo)
	ZEND_ABSTRACT_ME(yod_database, fetchAll,	yod_database_fetchall_arginfo)
	ZEND_ABSTRACT_ME(yod_database, transaction,	yod_database_transaction_arginfo)
	ZEND_ABSTRACT_ME(yod_database, commit,		yod_database_commit_arginfo)
	ZEND_ABSTRACT_ME(yod_database, rollback,	yod_database_rollback_arginfo)
	ZEND_ABSTRACT_ME(yod_database, insertid,	yod_database_insertid_arginfo)
	ZEND_ABSTRACT_ME(yod_database, quote,		yod_database_quote_arginfo)
	ZEND_ABSTRACT_ME(yod_database, free,		yod_database_free_arginfo)
	ZEND_ABSTRACT_ME(yod_database, close,		yod_database_close_arginfo)
	PHP_ME(yod_database, __destruct,		NULL,	ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	{NULL, NULL, NULL}
};
/* }}} */

/** {{{ PHP_MINIT_FUNCTION
*/
PHP_MINIT_FUNCTION(yod_database) {
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "Yod_Database", yod_database_methods);
	yod_database_ce = zend_register_internal_class(&ce TSRMLS_CC);

	zend_declare_property_null(yod_database_ce, ZEND_STRL("_db"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC TSRMLS_CC);
	zend_declare_property_null(yod_database_ce, ZEND_STRL("_config"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_database_ce, ZEND_STRL("_driver"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_database_ce, ZEND_STRL("_prefix"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_database_ce, ZEND_STRL("_result"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_database_ce, ZEND_STRL("_linkid"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_database_ce, ZEND_STRL("_linkids"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_bool(yod_database_ce, ZEND_STRL("_locked"), 0, ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_string(yod_database_ce, ZEND_STRL("_lastquery"), "", ZEND_ACC_PROTECTED TSRMLS_CC);

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
