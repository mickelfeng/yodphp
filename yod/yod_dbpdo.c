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

/** {{{ static int yod_dbpdo_connect(yod_dbpdo_t *object, zval *config, long linknum, zval *retval TSRMLS_DC)
*/
static int yod_dbpdo_connect(yod_dbpdo_t *object, zval *config, long linknum, zval *retval TSRMLS_DC) {
	zval *dbconfig, *linkids, *linkid, *argv[4], *query, **ppval;
	zval persist, errmode, warning;
	zend_class_entry **pce = NULL;
	char *squery;
	uint squery_len;
	

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbpdo_construct()");
#endif

	if (!object) {
		if (retval) {
			ZVAL_BOOL(retval, 0);
		}
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
			zend_update_property(Z_OBJCE_P(object), object, ZEND_STRL("_linkid"), *ppval TSRMLS_CC);
			if (retval) {
				ZVAL_ZVAL(retval, *ppval, 1, 0);
			}
			return 1;
		}
	} else {
		MAKE_STD_ZVAL(linkids);
		array_init(linkids);
	}

	if (Z_TYPE_P(dbconfig) == IS_ARRAY) {
		if (zend_hash_find(Z_ARRVAL_P(dbconfig), ZEND_STRS("dsn"), (void **)&ppval) == FAILURE ||
			Z_TYPE_PP(ppval) != IS_STRING || Z_STRLEN_PP(ppval) == 0
		) {
			php_error_docref(NULL TSRMLS_CC, E_ERROR, "Database DSN configure error");
			if (retval) {
				ZVAL_BOOL(retval, 0);
			}
			return 0;
		}

		if (zend_lookup_class_ex(ZEND_STRL("PDO"), 0, &pce TSRMLS_CC) == SUCCESS) {
			MAKE_STD_ZVAL(linkid);
			object_init_ex(linkid, *pce);

			// argv
			MAKE_STD_ZVAL(argv[0]);
			MAKE_STD_ZVAL(argv[1]);
			MAKE_STD_ZVAL(argv[2]);
			MAKE_STD_ZVAL(argv[3]);

			// argv.dsn
			ZVAL_STRINGL(argv[0] , Z_STRVAL_PP(ppval), Z_STRLEN_PP(ppval), 1);

			// argv.user
			if (zend_hash_find(Z_ARRVAL_P(dbconfig), ZEND_STRS("user"), (void **)&ppval) == SUCCESS &&
				Z_TYPE_PP(ppval) == IS_STRING && Z_STRLEN_PP(ppval)
			) {
				ZVAL_STRINGL(argv[1], Z_STRVAL_PP(ppval), Z_STRLEN_PP(ppval), 1);
			} else {
				ZVAL_STRING(argv[1], "", 1);
			}

			// argv.pass
			if (zend_hash_find(Z_ARRVAL_P(dbconfig), ZEND_STRS("pass"), (void **)&ppval) == SUCCESS &&
				Z_TYPE_PP(ppval) == IS_STRING && Z_STRLEN_PP(ppval)
			) {
				ZVAL_STRINGL(argv[2], Z_STRVAL_PP(ppval), Z_STRLEN_PP(ppval), 1);
			} else {
				ZVAL_STRING(argv[2], "", 1);
			}

			// argv.options
			if (zend_hash_find(Z_ARRVAL_P(dbconfig), ZEND_STRS("options"), (void **)&ppval) == SUCCESS &&
				Z_TYPE_PP(ppval) == IS_ARRAY
			) {
				ZVAL_ZVAL(argv[3], *ppval, 1, 1);
			} else {
				array_init(argv[3]);
			}

			// pconnect
			if (zend_hash_find(Z_ARRVAL_P(dbconfig), ZEND_STRS("pconnect"), (void **)&ppval) == SUCCESS &&
				Z_TYPE_PP(ppval) == IS_BOOL && Z_BVAL_PP(ppval)
			) {
#ifndef ZEND_FETCH_CLASS_SILENT
				if (zend_get_constant_ex(ZEND_STRL("PDO::ATTR_PERSISTENT"), &persist, NULL TSRMLS_CC)) {
#else
				if (zend_get_constant_ex(ZEND_STRL("PDO::ATTR_PERSISTENT"), &persist, NULL, ZEND_FETCH_CLASS_SILENT TSRMLS_CC)) {
#endif
					add_index_bool(argv[3], Z_LVAL(persist), 1);
				} else {
					add_index_bool(argv[3], 12, 1);
				}
			}

			yod_call_method(linkid, ZEND_STRL(ZEND_CONSTRUCTOR_FUNC_NAME), NULL, 4, argv[0], argv[1], argv[2], argv[3] TSRMLS_DC);

			zval_ptr_dtor(&argv[0]);
			zval_ptr_dtor(&argv[1]);
			zval_ptr_dtor(&argv[2]);
			zval_ptr_dtor(&argv[3]);
			
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
#ifndef ZEND_FETCH_CLASS_SILENT
				if (!zend_get_constant_ex(ZEND_STRL("PDO::ATTR_ERRMODE"), &errmode, NULL TSRMLS_CC)) {
#else
				if (!zend_get_constant_ex(ZEND_STRL("PDO::ATTR_ERRMODE"), &errmode, NULL, ZEND_FETCH_CLASS_SILENT TSRMLS_CC)) {
#endif
					INIT_PZVAL(&errmode);
					ZVAL_LONG(&errmode, 3);
				}
#ifndef ZEND_FETCH_CLASS_SILENT
				if (!zend_get_constant_ex(ZEND_STRL("PDO::ERRMODE_WARNING"), &warning, NULL TSRMLS_CC)) {
#else
				if (!zend_get_constant_ex(ZEND_STRL("PDO::ERRMODE_WARNING"), &warning, NULL, ZEND_FETCH_CLASS_SILENT TSRMLS_CC)) {
#endif
					INIT_PZVAL(&warning);
					ZVAL_LONG(&warning, 1);
				}
				zend_call_method_with_2_params(&linkid, Z_OBJCE_P(linkid), NULL, "setattribute", NULL, &errmode, &warning);
			}

			add_index_zval(linkids, linknum, linkid);
			zend_update_property(Z_OBJCE_P(object), object, ZEND_STRL("_linkids"), linkids TSRMLS_CC);
			zend_update_property(Z_OBJCE_P(object), object, ZEND_STRL("_linkid"), linkid TSRMLS_CC);

			if (retval) {
				ZVAL_ZVAL(retval, linkid, 1, 0);
			}
		} else {
			php_error_docref(NULL TSRMLS_CC, E_ERROR, "Class 'PDO' not found");
			if (retval) {
				ZVAL_BOOL(retval, 0);
			}
			zval_ptr_dtor(&dbconfig);
			return 0;
		}
	}
	zval_ptr_dtor(&dbconfig);
	
	return 1;
}
/* }}} */

/** {{{ int yod_dbpdo_execute(yod_dbpdo_t *object, zval *query, zval *params, int affected, zval *retval TSRMLS_DC)
*/
int yod_dbpdo_execute(yod_dbpdo_t *object, zval *query, zval *params, int affected, zval *retval TSRMLS_DC) {
	zval *config, *linkid, *result, *bindparams, *pzval, **ppval;
	HashPosition pos;

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbpdo_execute()");
#endif

	if (!query || Z_TYPE_P(query) != IS_STRING || !object) {
		if (retval) {
			ZVAL_BOOL(retval, 0);
		}
		return 0;
	}

	config = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_config"), 1 TSRMLS_CC);
	yod_dbpdo_connect(object, config, 0, NULL TSRMLS_CC);

	zend_update_property(Z_OBJCE_P(object), object, ZEND_STRL("_lastquery"), query TSRMLS_CC);

	linkid = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_linkid"), 1 TSRMLS_CC);
	if (linkid && Z_TYPE_P(linkid) == IS_OBJECT) {
		if (!params || Z_TYPE_P(params) != IS_ARRAY) {
			zend_call_method_with_1_params(&linkid, Z_OBJCE_P(linkid), NULL, "exec", &pzval, query);
			if (pzval) {
				if (retval) {
					if (params && Z_TYPE_P(params) == IS_BOOL) {
						affected = Z_BVAL_P(params);
					}
					if (affected) {
						ZVAL_ZVAL(retval, pzval, 1, 0);
					} else {
						ZVAL_BOOL(retval, 1);
					}
				}
				return 1;
			}
		} else {
			zend_call_method_with_1_params(&linkid, Z_OBJCE_P(linkid), NULL, "prepare", &result, query);
			if (result && Z_TYPE_P(result) == IS_OBJECT) {
				MAKE_STD_ZVAL(bindparams);
				array_init(bindparams);
				zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(params), &pos);
				while (zend_hash_get_current_data_ex(Z_ARRVAL_P(params), (void **)&ppval, &pos) == SUCCESS) {
					zval *value = NULL;
					char *str_key = NULL;
					uint key_len, name_len;
					ulong num_key;

					if (zend_hash_get_current_key_ex(Z_ARRVAL_P(params), &str_key, &key_len, &num_key, 0, &pos) == HASH_KEY_IS_STRING) {
						if (strstr(Z_STRVAL_P(query), str_key)) {
							MAKE_STD_ZVAL(value);
							ZVAL_ZVAL(value, *ppval, 1, 0);
							add_assoc_zval_ex(bindparams, str_key, key_len, value);
						}
					}
					zend_hash_move_forward_ex(Z_ARRVAL_P(params), &pos);
				}
				zend_call_method_with_1_params(&result, Z_OBJCE_P(result), NULL, "execute", &pzval, bindparams);
				zval_ptr_dtor(&bindparams);

				if (affected) {
					zend_call_method_with_0_params(&result, Z_OBJCE_P(result), NULL, "rowcount", &pzval);
				}
				zend_update_property(Z_OBJCE_P(object), object, ZEND_STRL("_result"), result TSRMLS_CC);
				if (pzval) {
					if (retval) {
						ZVAL_ZVAL(retval, pzval, 1, 0);
					}
					zval_ptr_dtor(&pzval);
					return 1;
				}
			} else {
				php_error_docref(NULL TSRMLS_CC, E_WARNING, "Call to a member function execute() on a non-object");
			}
		}
	} else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Call to a member function execute() on a non-object");
	}

	if (retval) {
		ZVAL_BOOL(retval, 0);
	}
	return 0;
}
/* }}} */

/** {{{ int yod_dbpdo_query(yod_dbpdo_t *object, zval *query, zval *params, zval *retval TSRMLS_DC)
*/
int yod_dbpdo_query(yod_dbpdo_t *object, zval *query, zval *params, zval *retval TSRMLS_DC) {
	zval *config, *linkid, *result, *bindparams, *pzval, **ppval;
	HashPosition pos;

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbpdo_query()");
#endif

	if (!query || Z_TYPE_P(query) != IS_STRING || !object) {
		if (retval) {
			ZVAL_BOOL(retval, 0);
		}
		return 0;
	}

	config = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_config"), 1 TSRMLS_CC);
	yod_dbpdo_connect(object, config, 0, NULL TSRMLS_CC);

	zend_update_property(Z_OBJCE_P(object), object, ZEND_STRL("_lastquery"), query TSRMLS_CC);

	linkid = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_linkid"), 1 TSRMLS_CC);
	if (linkid && Z_TYPE_P(linkid) == IS_OBJECT) {
		if (!params || Z_TYPE_P(params) != IS_ARRAY) {
			zend_call_method_with_1_params(&linkid, Z_OBJCE_P(linkid), NULL, "query", &result, query);
			zend_update_property(Z_OBJCE_P(object), object, ZEND_STRL("_result"), result TSRMLS_CC);
			if (result) {
				if (retval) {
					ZVAL_ZVAL(retval, result, 1, 0);
				}
				return 1;
			}
		} else {
			zend_call_method_with_1_params(&linkid, Z_OBJCE_P(linkid), NULL, "prepare", &result, query);
			if (result && Z_TYPE_P(result) == IS_OBJECT) {
				MAKE_STD_ZVAL(bindparams);
				array_init(bindparams);
				zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(params), &pos);
				while (zend_hash_get_current_data_ex(Z_ARRVAL_P(params), (void **)&ppval, &pos) == SUCCESS) {
					zval *value = NULL;
					char *str_key = NULL;
					uint key_len, name_len;
					ulong num_key;

					if (zend_hash_get_current_key_ex(Z_ARRVAL_P(params), &str_key, &key_len, &num_key, 0, &pos) == HASH_KEY_IS_STRING) {
						if (strstr(Z_STRVAL_P(query), str_key)) {
							MAKE_STD_ZVAL(value);
							ZVAL_ZVAL(value, *ppval, 1, 0);
							add_assoc_zval_ex(bindparams, str_key, key_len, value);
						}
					}
					zend_hash_move_forward_ex(Z_ARRVAL_P(params), &pos);
				}
				zend_call_method_with_1_params(&result, Z_OBJCE_P(result), NULL, "execute", &pzval, bindparams);
				zval_ptr_dtor(&bindparams);

				zend_update_property(Z_OBJCE_P(object), object, ZEND_STRL("_result"), result TSRMLS_CC);
				if (result) {
					if (retval) {
						ZVAL_ZVAL(retval, result, 1, 0);
					}
					return 1;
				}
			} else {
				php_error_docref(NULL TSRMLS_CC, E_WARNING, "Call to a member function query() on a non-object");
			}
		}
	} else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Call to a member function query() on a non-object");
	}

	if (retval) {
		ZVAL_BOOL(retval, 0);
	}
	return 0;
}
/* }}} */

/** {{{ int yod_dbpdo_fetch(yod_dbpdo_t *object, zval *result, zval *retval TSRMLS_DC)
*/
int yod_dbpdo_fetch(yod_dbpdo_t *object, zval *result, zval *retval TSRMLS_DC) {
	zval assoc, *pzval;

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbpdo_fetch()");
#endif

	if (!object) {
		if (retval) {
			ZVAL_BOOL(retval, 0);
		}
		return 0;
	}

	if (!result) {
		result = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_result"), 1 TSRMLS_CC);
	}

#ifndef ZEND_FETCH_CLASS_SILENT
	if (!zend_get_constant_ex(ZEND_STRL("PDO::FETCH_ASSOC"), &assoc, NULL TSRMLS_CC)) {
#else
	if (!zend_get_constant_ex(ZEND_STRL("PDO::FETCH_ASSOC"), &assoc, NULL, ZEND_FETCH_CLASS_SILENT TSRMLS_CC)) {
#endif
		INIT_PZVAL(&assoc);
		ZVAL_LONG(&assoc, 2);
	}

	if (result && Z_TYPE_P(result) == IS_OBJECT) {
		zend_call_method_with_1_params(&result, Z_OBJCE_P(result), NULL, "fetch", &pzval, &assoc);
		if (pzval) {
			if (retval) {
				ZVAL_ZVAL(retval, pzval, 1, 1);
			}
			return 1;
		}
	} else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Call to a member function fetch() on a non-object");
	}

	if (retval) {
		ZVAL_BOOL(retval, 0);
	}
	return 0;
}
/* }}} */

/** {{{ int yod_dbpdo_fetchall(yod_dbpdo_t *object, zval *result, zval *retval TSRMLS_DC)
*/
int yod_dbpdo_fetchall(yod_dbpdo_t *object, zval *result, zval *retval TSRMLS_DC) {
	zval assoc, *pzval;

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbpdo_fetchall()");
#endif

	if (!object) {
		if (retval) {
			ZVAL_BOOL(retval, 0);
		}
		return 0;
	}

	if (!result) {
		result = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_result"), 1 TSRMLS_CC);
	}

#ifndef ZEND_FETCH_CLASS_SILENT
	if (!zend_get_constant_ex(ZEND_STRL("PDO::FETCH_ASSOC"), &assoc, NULL TSRMLS_CC)) {
#else
	if (!zend_get_constant_ex(ZEND_STRL("PDO::FETCH_ASSOC"), &assoc, NULL, ZEND_FETCH_CLASS_SILENT TSRMLS_CC)) {
#endif
		INIT_PZVAL(&assoc);
		ZVAL_LONG(&assoc, 2);
	}

	if (result && Z_TYPE_P(result) == IS_OBJECT) {
		zend_call_method_with_1_params(&result, Z_OBJCE_P(result), NULL, "fetchall", &pzval, &assoc);
		if (pzval) {
			if (retval) {
				ZVAL_ZVAL(retval, pzval, 1, 1);
			}
			return 1;
		}
	} else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Call to a member function fetchAll() on a non-object");
	}

	if (retval) {
		ZVAL_BOOL(retval, 0);
	}
	return 0;
}
/* }}} */

/** {{{ int yod_dbpdo_free(yod_dbpdo_t *object, zval *result TSRMLS_DC)
*/
int yod_dbpdo_free(yod_dbpdo_t *object, zval *result TSRMLS_DC) {

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbpdo_free()");
#endif

	if (!object) {
		return 0;
	}

	if (result) {
		ZVAL_NULL(result);
	}

	result = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_result"), 1 TSRMLS_CC);
	if (result) {
		ZVAL_NULL(result);
	}

	return 1;
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
	yod_dbpdo_t *object;
	zval *query, *prefix, *table = NULL;
	zval *result, *data, **value, **ppval;
	char *squery;
	uint squery_len;
	HashPosition pos;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &table) == FAILURE) {
		return;
	}

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbpdo_fields()");
#endif

	object = getThis();

	array_init(return_value);

	squery_len = spprintf(&squery, 0, "SHOW COLUMNS FROM ");
	prefix = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_prefix"), 1 TSRMLS_CC);
	if (prefix && Z_TYPE_P(prefix) == IS_STRING) {
		squery_len = spprintf(&squery, 0, "%s%s", squery, Z_STRVAL_P(prefix));
	}

	if (table && Z_TYPE_P(table) == IS_STRING) {
		squery_len = spprintf(&squery, 0, "%s%s", squery, Z_STRVAL_P(table));
	}

	MAKE_STD_ZVAL(query);
	ZVAL_STRINGL(query, squery, squery_len, 1);
	MAKE_STD_ZVAL(result);
	yod_dbpdo_query(object, query, NULL, result TSRMLS_CC);
	if (result) {
		MAKE_STD_ZVAL(data);
		yod_dbpdo_fetchall(object, result, data TSRMLS_CC);
		if (data && Z_TYPE_P(data) == IS_ARRAY) {
			zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(data), &pos);
			while (zend_hash_get_current_data_ex(Z_ARRVAL_P(data), (void **)&value, &pos) == SUCCESS) {
				if (value && Z_TYPE_PP(value) == IS_ARRAY) {
					zval *field;
					MAKE_STD_ZVAL(field);
					array_init(field);
					// name
					add_assoc_null_ex(field, ZEND_STRS("name"));
					// type
					if (zend_hash_find(Z_ARRVAL_PP(value), ZEND_STRS("Type"), (void**)&ppval) == SUCCESS) {
						add_assoc_zval_ex(field, ZEND_STRS("type"), *ppval);
					}
					// notnull
					if (zend_hash_find(Z_ARRVAL_PP(value), ZEND_STRS("Null"), (void**)&ppval) == SUCCESS) {
						if (Z_TYPE_PP(ppval) == IS_STRING && Z_STRLEN_PP(ppval) == 0) {
							add_assoc_bool_ex(field, ZEND_STRS("notnull"), 1);
						} else {
							add_assoc_bool_ex(field, ZEND_STRS("notnull"), 0);
						}
					}
					// default
					if (zend_hash_find(Z_ARRVAL_PP(value), ZEND_STRS("Default"), (void**)&ppval) == SUCCESS) {
						add_assoc_zval_ex(field, ZEND_STRS("default"), *ppval);
					}
					// primary
					if (zend_hash_find(Z_ARRVAL_PP(value), ZEND_STRS("Key"), (void**)&ppval) == SUCCESS) {
						if (Z_TYPE_PP(ppval) == IS_STRING && strcasecmp(Z_STRVAL_PP(ppval), "pri") == 0) {
							add_assoc_bool_ex(field, ZEND_STRS("primary"), 1);
						} else {
							add_assoc_bool_ex(field, ZEND_STRS("primary"), 0);
						}
					}
					// autoinc
					if (zend_hash_find(Z_ARRVAL_PP(value), ZEND_STRS("Extra"), (void**)&ppval) == SUCCESS) {
						if (Z_TYPE_PP(ppval) == IS_STRING && strcasecmp(Z_STRVAL_PP(ppval), "auto_increment") == 0) {
							add_assoc_bool_ex(field, ZEND_STRS("autoinc"), 1);
						} else {
							add_assoc_bool_ex(field, ZEND_STRS("autoinc"), 0);
						}
					}
					if (zend_hash_find(Z_ARRVAL_PP(value), ZEND_STRS("Field"), (void**)&ppval) == SUCCESS) {
						add_assoc_zval_ex(field, ZEND_STRS("name"), *ppval);
						add_assoc_zval_ex(return_value, Z_STRVAL_PP(ppval), Z_STRLEN_PP(ppval) + 1, field);
					}
				}
				zend_hash_move_forward_ex(Z_ARRVAL_P(data), &pos);
			}
		}
	}
	efree(squery);
}
/* }}} */

/** {{{ proto public Yod_DbPdo::execute($query, $params = array(), $affected = true)
*/
PHP_METHOD(yod_dbpdo, execute) {
	zval *query = NULL, *params = NULL;
	int affected = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|zb", &query, &params, &affected) == FAILURE) {
		return;
	}

	yod_dbpdo_execute(getThis(), query, params, affected, return_value TSRMLS_DC);
}
/* }}} */

/** {{{ proto public Yod_DbPdo::query($query, $params = array())
*/
PHP_METHOD(yod_dbpdo, query) {
	zval *query = NULL, *params = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|z", &query, &params) == FAILURE) {
		return;
	}

	yod_dbpdo_query(getThis(), query, params, return_value TSRMLS_DC);
}
/* }}} */

/** {{{ proto public Yod_DbPdo::fetch($result = null)
*/
PHP_METHOD(yod_dbpdo, fetch) {
	zval *result = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z", &result) == FAILURE) {
		return;
	}

	yod_dbpdo_fetch(getThis(), result, return_value TSRMLS_DC);
}
/* }}} */

/** {{{ proto public Yod_DbPdo::fetchAll($result = null)
*/
PHP_METHOD(yod_dbpdo, fetchAll) {
	zval *result = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z", &result) == FAILURE) {
		return;
	}

	yod_dbpdo_fetchall(getThis(), result, return_value TSRMLS_DC);
}
/* }}} */

/** {{{ proto public Yod_DbPdo::transaction()
*/
PHP_METHOD(yod_dbpdo, transaction) {
	yod_dbpdo_t *object;
	zval *config, *linkid, *retval;

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbpdo_transaction()");
#endif

	object = getThis();

	zend_update_property_bool(Z_OBJCE_P(object), object, ZEND_STRL("_locked"), 1 TSRMLS_CC);
	config = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_config"), 1 TSRMLS_CC);
	yod_dbpdo_connect(object, config, 0, NULL TSRMLS_CC);
	linkid = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_linkid"), 1 TSRMLS_CC);
	if (linkid && Z_TYPE_P(linkid) == IS_OBJECT) {
		zend_call_method_with_0_params(&linkid, Z_OBJCE_P(linkid), NULL, "begintransaction", &retval);
		if (retval) {
			RETURN_ZVAL(retval, 1, 1);
		}
	} else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Call to a member function begintransaction() on a non-object");
	}

	RETURN_FALSE;
}
/* }}} */

/** {{{ proto public Yod_DbPdo::commit()
*/
PHP_METHOD(yod_dbpdo, commit) {
	yod_dbpdo_t *object;
	zval *config, *linkid, *retval;

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbpdo_commit()");
#endif

	object = getThis();

	zend_update_property_bool(Z_OBJCE_P(object), object, ZEND_STRL("_locked"), 0 TSRMLS_CC);
	config = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_config"), 1 TSRMLS_CC);
	yod_dbpdo_connect(object, config, 0, NULL TSRMLS_CC);
	linkid = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_linkid"), 1 TSRMLS_CC);
	if (linkid && Z_TYPE_P(linkid) == IS_OBJECT) {
		zend_call_method_with_0_params(&linkid, Z_OBJCE_P(linkid), NULL, "commit", &retval);
		if (retval) {
			RETURN_ZVAL(retval, 1, 1);
		}
	} else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Call to a member function commit() on a non-object");
	}

	RETURN_FALSE;
}
/* }}} */

/** {{{ proto public Yod_DbPdo::rollback()
*/
PHP_METHOD(yod_dbpdo, rollback) {
	yod_dbpdo_t *object;
	zval *config, *linkid, *retval;

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbpdo_rollback()");
#endif

	object = getThis();

	zend_update_property_bool(Z_OBJCE_P(object), object, ZEND_STRL("_locked"), 0 TSRMLS_CC);
	config = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_config"), 1 TSRMLS_CC);
	yod_dbpdo_connect(object, config, 0, NULL TSRMLS_CC);
	linkid = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_linkid"), 1 TSRMLS_CC);
	if (linkid && Z_TYPE_P(linkid) == IS_OBJECT) {
		zend_call_method_with_0_params(&linkid, Z_OBJCE_P(linkid), NULL, "rollback", &retval);
		if (retval) {
			RETURN_ZVAL(retval, 1, 1);
		}
	} else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Call to a member function rollBack() on a non-object");
	}

	RETURN_FALSE;
}
/* }}} */

/** {{{ proto public Yod_DbPdo::insertid()
*/
PHP_METHOD(yod_dbpdo, insertid) {
	yod_dbpdo_t *object;
	zval *linkid, *retval;

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbpdo_insertid()");
#endif

	object = getThis();

	linkid = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_linkid"), 1 TSRMLS_CC);
	if (linkid && Z_TYPE_P(linkid) == IS_OBJECT) {
		zend_call_method_with_0_params(&linkid, Z_OBJCE_P(linkid), NULL, "lastinsertid", &retval);
		if (retval) {
			RETURN_ZVAL(retval, 1, 1);
		}
	} else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Call to a member function lastInsertId() on a non-object");
	}

	RETURN_NULL();
}
/* }}} */

/** {{{ proto public Yod_DbPdo::quote($string)
*/
PHP_METHOD(yod_dbpdo, quote) {
	yod_dbpdo_t *object;
	zval *linkid, *string, *retval;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &string) == FAILURE) {
		return;
	}

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbpdo_quote()");
#endif

	object = getThis();

	linkid = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_linkid"), 1 TSRMLS_CC);
	if (linkid && Z_TYPE_P(linkid) == IS_OBJECT) {
		zend_call_method_with_1_params(&linkid, Z_OBJCE_P(linkid), NULL, "quote", &retval, string);
		if (retval) {
			RETURN_ZVAL(retval, 1, 1);
		}
	} else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Call to a member function quote() on a non-object");
	}

	RETURN_NULL();
}
/* }}} */

/** {{{ proto public Yod_DbPdo::free($result = null)
*/
PHP_METHOD(yod_dbpdo, free) {
	zval *result = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z", &result) == FAILURE) {
		return;
	}

	yod_dbpdo_free(getThis(), result TSRMLS_DC);
}
/* }}} */

/** {{{ proto public Yod_DbPdo::close()
*/
PHP_METHOD(yod_dbpdo, close) {
	yod_dbpdo_t *object;
	zval *linkids, *linkid, **ppval;
	HashPosition pos;

#if PHP_YOD_DEBUG
	//yod_debugf("yod_dbpdo_close()");
#endif

	object = getThis();
	
	linkids = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_linkids"), 1 TSRMLS_CC);
	if (linkids && Z_TYPE_P(linkids) == IS_ARRAY) {
		zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(linkids), &pos);
		while (zend_hash_get_current_data_ex(Z_ARRVAL_P(linkids), (void **)&ppval, &pos) == SUCCESS) {
			if (ppval) {
				zval_ptr_dtor(ppval);
			}
			zend_hash_move_forward_ex(Z_ARRVAL_P(linkids), &pos);
		}
		zend_hash_destroy(Z_ARRVAL_P(linkids));
		efree(Z_ARRVAL_P(linkids));
	}

	linkid = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_linkid"), 1 TSRMLS_CC);
	if (linkid) {
		zval_ptr_dtor(&linkid);
	}
}
/* }}} */

/** {{{ proto public Yod_DbPdo::errno()
*/
PHP_METHOD(yod_dbpdo, errNo) {
	yod_dbpdo_t *object;
	zval *linkid, *errcode;

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbpdo_errno()");
#endif

	object = getThis();

	linkid = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_linkid"), 1 TSRMLS_CC);
	if (!linkid || Z_TYPE_P(linkid) != IS_OBJECT) {
		RETURN_FALSE;
	}

	zend_call_method_with_0_params(&linkid, Z_OBJCE_P(linkid), NULL, "errorcode", &errcode);
	if (errcode && Z_TYPE_P(errcode) == IS_LONG) {
		RETURN_LONG(Z_LVAL_P(errcode));
	}

	RETURN_LONG(0);
}
/* }}} */

/** {{{ proto public Yod_DbPdo::error()
*/
PHP_METHOD(yod_dbpdo, error) {
	yod_dbpdo_t *object;
	zval *linkid, *errinfo, **ppval;

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbpdo_error()");
#endif

	object = getThis();
	
	linkid = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_linkid"), 1 TSRMLS_CC);
	if (!linkid || Z_TYPE_P(linkid) != IS_OBJECT) {
		RETURN_FALSE;
	}

	zend_call_method_with_0_params(&linkid, Z_OBJCE_P(linkid), NULL, "errorinfo", &errinfo);
	if (errinfo && Z_TYPE_P(errinfo) == IS_ARRAY) {
		if (zend_hash_index_find(Z_ARRVAL_P(errinfo), 2, (void**)&ppval) == SUCCESS && Z_TYPE_PP(ppval) == IS_STRING) {
			RETURN_STRINGL(Z_STRVAL_PP(ppval), Z_STRLEN_PP(ppval), 1);
		}
	}

	RETURN_FALSE;
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
	PHP_ME(yod_dbpdo, errNo,		yod_dbpdo_errno_arginfo,			ZEND_ACC_PUBLIC)
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
