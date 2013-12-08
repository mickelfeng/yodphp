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
#include "yod_database.h"
#include "yod_dbpdo.h"


zend_class_entry *yod_dbpdo_ce;

/** {{{ ARG_INFO
 */
ZEND_BEGIN_ARG_INFO_EX(yod_dbpdo_construct_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbpdo_db_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbpdo_getinstance_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbpdo_config_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbpdo_create_arginfo, 0, 0, 2)
	ZEND_ARG_INFO(0, fields)
	ZEND_ARG_INFO(0, table)
	ZEND_ARG_INFO(0, extend)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbpdo_insert_arginfo, 0, 0, 2)
	ZEND_ARG_INFO(0, data)
	ZEND_ARG_INFO(0, table)
	ZEND_ARG_INFO(0, replace)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbpdo_update_arginfo, 0, 0, 2)
	ZEND_ARG_INFO(0, data)
	ZEND_ARG_INFO(0, table)
	ZEND_ARG_INFO(0, where)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbpdo_delete_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, table)
	ZEND_ARG_INFO(0, where)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbpdo_select_arginfo, 0, 0, 2)
	ZEND_ARG_INFO(0, select)
	ZEND_ARG_INFO(0, table)
	ZEND_ARG_INFO(0, where)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbpdo_lastquery_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbpdo_dbconfig_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, config)
	ZEND_ARG_INFO(0, linknum)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbpdo_connect_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, config)
	ZEND_ARG_INFO(0, linknum)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbpdo_fields_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, table)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbpdo_execute_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, query)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbpdo_query_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, query)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbpdo_fetch_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, result)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbpdo_fetchall_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, result)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbpdo_transaction_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbpdo_commit_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbpdo_rollback_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbpdo_insertid_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbpdo_quote_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, string)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbpdo_free_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, result)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbpdo_close_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbpdo_errno_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbpdo_error_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ proto public Yod_DbPdo::__construct($config)
*/
PHP_METHOD(yod_dbpdo, __construct) {
	zval *config = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &config) == FAILURE) {
		return;
	}

}
/* }}} */

/** {{{ proto public Yod_DbPdo::connect($config = null, $linknum = 0)
*/
PHP_METHOD(yod_dbpdo, connect) {

}
/* }}} */

/** {{{ proto public Yod_DbPdo::fields($table)
*/
PHP_METHOD(yod_dbpdo, fields) {

}
/* }}} */

/** {{{ proto public Yod_DbPdo::execute($query, $params = array())
*/
PHP_METHOD(yod_dbpdo, execute) {

}
/* }}} */

/** {{{ proto public Yod_DbPdo::query($query, $params = array())
*/
PHP_METHOD(yod_dbpdo, query) {

}
/* }}} */

/** {{{ proto public Yod_DbPdo::fetch($result = null)
*/
PHP_METHOD(yod_dbpdo, fetch) {

}
/* }}} */

/** {{{ proto public Yod_DbPdo::fetchAll($result = null)
*/
PHP_METHOD(yod_dbpdo, fetchAll) {

}
/* }}} */

/** {{{ proto public Yod_DbPdo::transaction()
*/
PHP_METHOD(yod_dbpdo, transaction) {

}
/* }}} */

/** {{{ proto public Yod_DbPdo::commit()
*/
PHP_METHOD(yod_dbpdo, commit) {

}
/* }}} */

/** {{{ proto public Yod_DbPdo::rollback()
*/
PHP_METHOD(yod_dbpdo, rollback) {

}
/* }}} */

/** {{{ proto public Yod_DbPdo::insertid()
*/
PHP_METHOD(yod_dbpdo, insertid) {

}
/* }}} */

/** {{{ proto public Yod_DbPdo::quote($string)
*/
PHP_METHOD(yod_dbpdo, quote) {

}
/* }}} */

/** {{{ proto public Yod_DbPdo::free($result = null)
*/
PHP_METHOD(yod_dbpdo, free) {

}
/* }}} */

/** {{{ proto public Yod_DbPdo::close()
*/
PHP_METHOD(yod_dbpdo, close) {

}
/* }}} */

/** {{{ proto public Yod_DbPdo::errno()
*/
PHP_METHOD(yod_dbpdo, errno) {

}
/* }}} */

/** {{{ proto public Yod_DbPdo::error()
*/
PHP_METHOD(yod_dbpdo, error) {

}
/* }}} */

/** {{{ yod_dbpdo_methods[]
*/
zend_function_entry yod_dbpdo_methods[] = {
	PHP_ME(yod_dbpdo, __construct,	yod_dbpdo_construct_arginfo,		ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(yod_dbpdo, connect,		yod_dbpdo_connect_arginfo,			ZEND_ACC_PUBLIC)
	PHP_ME(yod_dbpdo, fields,		yod_dbpdo_fields_arginfo,			ZEND_ACC_PUBLIC)
	PHP_ME(yod_dbpdo, execute,		yod_dbpdo_execute_arginfo,			ZEND_ACC_PUBLIC)
	PHP_ME(yod_dbpdo, query,		yod_dbpdo_query_arginfo,			ZEND_ACC_PUBLIC)
	PHP_ME(yod_dbpdo, fetch,		yod_dbpdo_fetch_arginfo,			ZEND_ACC_PUBLIC)
	PHP_ME(yod_dbpdo, fetchAll,		yod_dbpdo_fetchall_arginfo,			ZEND_ACC_PUBLIC)
	PHP_ME(yod_dbpdo, transaction,	yod_dbpdo_transaction_arginfo,		ZEND_ACC_PUBLIC)
	PHP_ME(yod_dbpdo, commit,		yod_dbpdo_commit_arginfo,			ZEND_ACC_PUBLIC)
	PHP_ME(yod_dbpdo, rollback,		yod_dbpdo_rollback_arginfo,			ZEND_ACC_PUBLIC)
	PHP_ME(yod_dbpdo, insertid,		yod_dbpdo_insertid_arginfo,			ZEND_ACC_PUBLIC)
	PHP_ME(yod_dbpdo, quote,		yod_dbpdo_quote_arginfo,			ZEND_ACC_PUBLIC)
	PHP_ME(yod_dbpdo, free,			yod_dbpdo_free_arginfo,				ZEND_ACC_PUBLIC)
	PHP_ME(yod_dbpdo, close,		yod_dbpdo_close_arginfo,			ZEND_ACC_PUBLIC)
	PHP_ME(yod_dbpdo, errno,		yod_dbpdo_errno_arginfo,			ZEND_ACC_PUBLIC)
	PHP_ME(yod_dbpdo, error,		yod_dbpdo_error_arginfo,			ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};
/* }}} */

/** {{{ PHP_MINIT_FUNCTION
*/
PHP_MINIT_FUNCTION(yod_dbpdo) {
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "Yod_DbPdo", yod_dbpdo_methods);
	yod_dbpdo_ce = zend_register_internal_class_ex(&ce, yod_database_ce, NULL TSRMLS_CC);

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
