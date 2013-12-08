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

/** {{{ static long yod_dbpdo_errno(yod_dbpdo_t *object TSRMLS_DC)
*/
static long yod_dbpdo_errno(yod_dbpdo_t *object TSRMLS_DC) {
	zval *linkid, *retval;
	long errcode = 0;

	if (!object) {
		return 1;
	}

	linkid = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_linkid"), 1 TSRMLS_CC);
	if (!linkid) {
		return 1;
	}

	zend_call_method_with_0_params(&linkid, Z_OBJCE_P(linkid), NULL, "errorCode", &retval);
	if (retval && Z_TYPE_P(retval) == IS_LONG) {
		errcode = Z_LVAL_P(retval);
	}

	return errcode;
}
/* }}} */

/** {{{ static char *yod_dbpdo_errer(yod_dbpdo_t *object TSRMLS_DC)
*/
static char *yod_dbpdo_errer(yod_dbpdo_t *object TSRMLS_DC) {
	zval *linkid, *retval, **ppval;
	char *errer = "";
/*
	if (!object) {
		errer = "Class 'DbPdo' not found";
	}
	
	linkid = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_linkid"), 1 TSRMLS_CC);
	if (!linkid) {
		errer = "Class 'PDO' not found";
	}

	zend_call_method_with_0_params(&linkid, Z_OBJCE_P(linkid), NULL, "errorInfo", &retval);
	if (retval && Z_TYPE_P(retval) == IS_ARRAY) {
		if (zend_hash_index_find(Z_ARRVAL_P(retval), 2, (void**)&ppval) == SUCCESS &&
			Z_TYPE_PP(ppval) == IS_STRING
		) {
			errer = estrndup(Z_STRVAL_PP(ppval), Z_STRLEN_PP(ppval));
		}
	}
*/
	return errer;
}
/* }}} */

/** {{{ int yod_dbpdo_connect(yod_dbpdo_t *object, zval *config, long linknum, zval *result TSRMLS_DC)
*/
int yod_dbpdo_connect(yod_dbpdo_t *object, zval *config, long linknum, zval *result TSRMLS_DC) {
	zval *dbconfig, *linkids, *linkid, *query, **ppval;
	zval *ctor, *argv[4], retval;
	zval persist, errmode, warning;
	zend_class_entry **pce = NULL;
	char *squery;
	uint squery_len;
	

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbpdo_construct()");
#endif

	if (!object) {
		ZVAL_BOOL(result, 0);
		return 0;
	}

	MAKE_STD_ZVAL(dbconfig);
	yod_database_dbconfig(object, config, linknum, dbconfig TSRMLS_CC);
	if (Z_TYPE_P(dbconfig) == IS_ARRAY) {
		if (zend_hash_find(Z_ARRVAL_P(dbconfig), ZEND_STRS("linknum"), (void **)&ppval) == SUCCESS &&
			Z_TYPE_P(dbconfig) == IS_LONG
		) {
			linknum = Z_LVAL_PP(ppval);
		} else {
			linknum = 0;
		}
	}

	linkids = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_linkids"), 1 TSRMLS_CC);
	if (linkids && Z_TYPE_P(linkids) == IS_ARRAY) {
		if (zend_hash_index_find(Z_ARRVAL_P(linkids), linknum, (void **)&ppval) == SUCCESS) {
			ZVAL_ZVAL(result, *ppval, 1, 0);
			return 1;
		}
	} else {
		MAKE_STD_ZVAL(linkids);
		array_init(linkids);
	}

	if (Z_TYPE_P(dbconfig) == IS_ARRAY) {
		zend_print_zval_r(dbconfig, 0 TSRMLS_CC);

		if (zend_hash_find(Z_ARRVAL_P(dbconfig), ZEND_STRS("dsn"), (void **)&ppval) == FAILURE ||
			Z_TYPE_PP(ppval) != IS_STRING || Z_STRLEN_PP(ppval) == 0
		) {
			php_error_docref(NULL TSRMLS_CC, E_ERROR, "Database DSN configure error");
			ZVAL_BOOL(result, 0);
			return 0;
		}

		if (zend_lookup_class_ex(ZEND_STRL("PDO"), 0, &pce TSRMLS_CC) == SUCCESS) {
			MAKE_STD_ZVAL(linkid);
			object_init_ex(linkid, *pce);

			// ctor
			MAKE_STD_ZVAL(ctor);
			ZVAL_STRING(ctor, ZEND_CONSTRUCTOR_FUNC_NAME, 1);

			// argv
			MAKE_STD_ZVAL(argv[0]);
			MAKE_STD_ZVAL(argv[1]);
			MAKE_STD_ZVAL(argv[2]);
			MAKE_STD_ZVAL(argv[3]);

			ZVAL_STRINGL(argv[0] , Z_STRVAL_PP(ppval), Z_STRLEN_PP(ppval), 1);

			if (zend_hash_find(Z_ARRVAL_P(dbconfig), ZEND_STRS("user"), (void **)&ppval) == SUCCESS &&
				Z_TYPE_PP(ppval) == IS_STRING && Z_STRLEN_PP(ppval)
			) {
				ZVAL_STRINGL(argv[1], Z_STRVAL_PP(ppval), Z_STRLEN_PP(ppval), 1);
			} else {
				ZVAL_STRING(argv[1], "", 1);
			}

			if (zend_hash_find(Z_ARRVAL_P(dbconfig), ZEND_STRS("pass"), (void **)&ppval) == SUCCESS &&
				Z_TYPE_PP(ppval) == IS_STRING && Z_STRLEN_PP(ppval)
			) {
				ZVAL_STRINGL(argv[2], Z_STRVAL_PP(ppval), Z_STRLEN_PP(ppval), 1);
			} else {
				ZVAL_STRING(argv[2], "", 1);
			}

			if (zend_hash_find(Z_ARRVAL_P(dbconfig), ZEND_STRS("options"), (void **)&ppval) == SUCCESS &&
				Z_TYPE_PP(ppval) == IS_ARRAY
			) {
				ZVAL_ZVAL(argv[3], *ppval, 1, 1);
			} else {
				array_init(argv[3]);
			}

			if (zend_hash_find(Z_ARRVAL_P(dbconfig), ZEND_STRS("pconnect"), (void **)&ppval) == SUCCESS &&
				Z_TYPE_PP(ppval) == IS_BOOL && Z_BVAL_PP(ppval)
			) {
				if (zend_get_constant(ZEND_STRL("PDO::ATTR_PERSISTENT"), &persist TSRMLS_CC)) {
					if (Z_TYPE(persist) == IS_STRING) {
						add_assoc_bool_ex(argv[3], Z_STRVAL(persist), Z_STRLEN(persist), 1);
					} else if (Z_TYPE(persist) == IS_LONG) {
						add_index_bool(argv[3], Z_LVAL(persist), 1);
					}
				}
			}

			if (call_user_function(NULL, &linkid, ctor, &retval, 4, argv TSRMLS_CC) == FAILURE) {
				zval_ptr_dtor(&argv[0]);
				zval_ptr_dtor(&argv[1]);
				zval_ptr_dtor(&argv[2]);
				zval_ptr_dtor(&argv[3]);

				php_error_docref(NULL TSRMLS_CC, E_ERROR, "Error calling PDO::__construct()");
				ZVAL_BOOL(result, 0);
				return 0;
			}

			// charset
			if (zend_hash_find(Z_ARRVAL_P(dbconfig), ZEND_STRS("charset"), (void **)&ppval) == SUCCESS &&
				Z_TYPE_PP(ppval) == IS_STRING || Z_STRLEN_PP(ppval)
			) {
				squery_len = spprintf(&squery, 0, "SET NAMES %s", Z_STRVAL_PP(ppval));
			} else {
				squery_len = spprintf(&squery, 0, "SET NAMES utf8");
			}

			MAKE_STD_ZVAL(query);
			ZVAL_STRINGL(query, squery, squery_len, 1);
			zend_call_method_with_1_params(&linkid, Z_OBJCE_P(linkid), NULL, "exec", NULL, query);	
			zval_ptr_dtor(&query);

			if (EG(error_reporting)) {
				if (zend_get_constant(ZEND_STRL("PDO::ATTR_ERRMODE"), &errmode TSRMLS_CC) &&
					zend_get_constant(ZEND_STRL("PDO::ERRMODE_WARNING"), &warning TSRMLS_CC)
				) {
					zend_call_method_with_2_params(&linkid, Z_OBJCE_P(linkid), NULL, "setAttribute", NULL, &errmode, &warning);
				}
			}

			add_index_zval(linkids, linknum, linkid);
			zend_update_property(Z_OBJCE_P(object), object, ZEND_STRL("_linkids"), linkids TSRMLS_CC);
			zend_update_property(Z_OBJCE_P(object), object, ZEND_STRL("_linkid"), linkid TSRMLS_CC);

			ZVAL_ZVAL(result, linkid, 1, 0);

			zval_ptr_dtor(&argv[0]);
			zval_ptr_dtor(&argv[1]);
			zval_ptr_dtor(&argv[2]);
			zval_ptr_dtor(&argv[3]);
			zval_dtor(&retval);
		} else {
			php_error_docref(NULL TSRMLS_CC, E_ERROR, "Class 'PDO' not found");
			ZVAL_BOOL(result, 0);
			return 0;
		}

		zend_print_zval_r(dbconfig, 0 TSRMLS_CC);
	}
}
/* }}} */

/** {{{ proto public Yod_DbPdo::__construct($config)
*/
PHP_METHOD(yod_dbpdo, __construct) {
	yod_dbpdo_t *object;
	zval *config = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &config) == FAILURE) {
		return;
	}

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbpdo_construct()");
#endif

	object = getThis();

	yod_database_construct(object, config TSRMLS_DC);

	zend_update_property_string(Z_OBJCE_P(object), object, ZEND_STRL("_driver"), Z_OBJCE_P(object)->name TSRMLS_CC);
}
/* }}} */

/** {{{ proto public Yod_DbPdo::connect($config = null, $linknum = 0)
*/
PHP_METHOD(yod_dbpdo, connect) {
	zval *config = NULL;
	long linknum = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|zl", &config, &linknum) == FAILURE) {
		return;
	}

	yod_dbpdo_connect(getThis(), config, linknum, return_value TSRMLS_CC);
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
