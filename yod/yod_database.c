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
#include "ext/standard/md5.h"
#include "ext/standard/php_var.h"
#include "ext/standard/php_rand.h"
#include "ext/standard/php_smart_str.h"

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

/** {{{ char *yod_database_md5(char *str, uint len TSRMLS_DC)
*/
char *yod_database_md5(char *str, uint len TSRMLS_DC) {
	PHP_MD5_CTX context;
	unsigned char digest[16];
	char md5str[33];
	char *retval;

	md5str[0] = '\0';
	PHP_MD5Init(&context);
	PHP_MD5Update(&context, str, len);
	PHP_MD5Final(digest, &context);
	make_digest(md5str, digest);

	retval = estrndup(&md5str[0], sizeof(md5str));

	return 	retval;
}
/* }}} */

/** {{{ char *yod_database_md5hash(zval *data TSRMLS_DC)
*/
char *yod_database_md5hash(zval **data TSRMLS_DC) {
	php_serialize_data_t var_hash;
	smart_str buf = {0};
	
	PHP_MD5_CTX context;
	unsigned char digest[16];
	char md5str[33];
	char *retval;

	// serialize
	PHP_VAR_SERIALIZE_INIT(var_hash);
	php_var_serialize(&buf, data, &var_hash TSRMLS_CC);
	PHP_VAR_SERIALIZE_DESTROY(var_hash);

	retval = yod_database_md5(buf.c, buf.len);

	return 	retval;
}
/* }}} */

/** {{{ yod_database_t *yod_database_construct(yod_database_t *object, zval *config TSRMLS_DC)
*/
yod_database_t *yod_database_construct(yod_database_t *object, zval *config TSRMLS_DC) {
	yod_database_t *retval;
	zval *linkids, **ppval;


#if PHP_YOD_DEBUG
	yod_debugf("yod_database_construct()");
#endif

	if (object) {
		retval = object;
	} else {
		MAKE_STD_ZVAL(retval);
		object_init_ex(retval, yod_database_ce);
	}

	// linkids
	MAKE_STD_ZVAL(linkids);
	array_init(linkids);
	zend_update_property(Z_OBJCE_P(retval), retval, ZEND_STRL("_linkids"), linkids TSRMLS_CC);
	zval_ptr_dtor(&linkids);

	if (config) {
		zend_update_property(Z_OBJCE_P(retval), retval, ZEND_STRL("_config"), config TSRMLS_CC);

		if (Z_TYPE_P(config) == IS_ARRAY &&
			zend_hash_find(Z_ARRVAL_P(config), ZEND_STRS("prefix"), (void **)&ppval) == SUCCESS &&
			Z_TYPE_PP(ppval) == IS_STRING
		) {
			zend_update_property_stringl(Z_OBJCE_P(retval), retval, ZEND_STRL("_prefix"), Z_STRVAL_PP(ppval), Z_STRLEN_PP(ppval) TSRMLS_CC);
		}
	}

	return retval;
}
/* }}} */

/** {{{ int yod_database_getinstance(zval *config, zval *result TSRMLS_DC)
*/
int yod_database_getinstance(zval *config, zval *result TSRMLS_DC) {
	yod_database_t *object;
	zval *p_db, **ppval;
	char *classname, *classpath, *md5hash;
	uint classname_len;
	zend_class_entry **pce = NULL;


#if PHP_YOD_DEBUG
	yod_debugf("yod_database_getinstance()");
#endif

	if (!config) {
		MAKE_STD_ZVAL(config);
		yod_application_config(ZEND_STRL("db_dsn"), config TSRMLS_CC);
	} else if (Z_TYPE_P(config) == IS_STRING) {
		yod_application_config(Z_STRVAL_P(config), Z_STRLEN_P(config), config TSRMLS_CC);
	}

	if (Z_TYPE_P(config) != IS_ARRAY) {
		ZVAL_BOOL(result, 0);
		return 0;
	}

	if (zend_hash_find(Z_ARRVAL_P(config), ZEND_STRS("type"), (void **)&ppval) == FAILURE ||
		Z_TYPE_PP(ppval) != IS_STRING
	) {
		add_assoc_string_ex(config, ZEND_STRS("type"), "pdo", 1);
		classname_len = spprintf(&classname, 0, "Yod_DbPdo");
	} else {
		char *dbtype = estrndup(Z_STRVAL_PP(ppval), Z_STRLEN_PP(ppval));
		if (Z_STRLEN_PP(ppval) == 3 && strncmp(dbtype, "pdo", 3) == 0 ) {
			classname_len = spprintf(&classname, 0, "Yod_DbPdo");
		} else {
			*dbtype = toupper((unsigned char) *dbtype);
			while (*dbtype != '\0') {
				if (isspace((int) *(unsigned char *)dbtype++)) {
					*dbtype = toupper((unsigned char) *dbtype);
				}
			}
			dbtype = dbtype - Z_STRLEN_PP(ppval);
			classname_len = spprintf(&classname, 0, "Yod_Db%s", dbtype);
		}
		efree(dbtype);
	}

	md5hash = yod_database_md5hash(&config);

	p_db = zend_read_static_property(yod_database_ce, ZEND_STRL("_db"), 0 TSRMLS_CC);
	if (p_db && Z_TYPE_P(p_db) == IS_ARRAY) {
		if (zend_hash_find(Z_ARRVAL_P(p_db), md5hash, strlen(md5hash) + 1, (void **)&ppval) == SUCCESS) {
			ZVAL_ZVAL(result, *ppval, 1, 0);
			return 1;
		}
	} else {
		MAKE_STD_ZVAL(p_db);
		array_init(p_db);
	}

	if (zend_lookup_class_ex(classname, classname_len, 0, &pce TSRMLS_CC) == FAILURE) {
		spprintf(&classpath, 0, "%s/drivers/%s.class.php", yod_extpath(TSRMLS_CC), classname + 4);

		if (VCWD_ACCESS(classpath, F_OK) == 0) {
			yod_include(classpath, NULL, 1 TSRMLS_CC);
		}
	}

	MAKE_STD_ZVAL(object);
	if (zend_lookup_class_ex(classname, classname_len, 0, &pce TSRMLS_CC) == SUCCESS) {
		object_init_ex(object, *pce);
		if (zend_hash_exists(&(*pce)->function_table, ZEND_STRS(ZEND_CONSTRUCTOR_FUNC_NAME))) {
			zend_call_method_with_1_params(&object, *pce, &(*pce)->constructor, ZEND_CONSTRUCTOR_FUNC_NAME, NULL, config);
		}
	} else {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "Class '%s' not found", classname);
		ZVAL_BOOL(result, 0);
		return 0;
	}

	add_assoc_zval_ex(p_db, md5hash, strlen(md5hash) + 1, object);
	zend_update_static_property(yod_database_ce, ZEND_STRL("_db"), p_db TSRMLS_CC);

	ZVAL_ZVAL(result, object, 1, 0);

	return 1;
}
/* }}} */

/** {{{ char *yod_database_config(yod_database_t *object, char *name, uint name_len, zval *value TSRMLS_DC)
*/
zval *yod_database_config(yod_database_t *object, char *name, uint name_len, zval *value TSRMLS_DC) {
	zval *p_config, *retval = NULL;
	zval *pzval, **ppval;

#if PHP_YOD_DEBUG
	yod_debugf("yod_database_config(%s)", name ? name : "");
#endif

	if (!object) {
		return;
	}

	p_config = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_config"), 1 TSRMLS_CC);
	if (!p_config || Z_TYPE_P(p_config) != IS_ARRAY) {
		MAKE_STD_ZVAL(p_config);
		array_init(p_config);
	}

	MAKE_STD_ZVAL(retval);
	if (name_len) {
		if (value) {
			MAKE_STD_ZVAL(pzval);
			ZVAL_ZVAL(pzval, value, 1, 0);
			add_assoc_zval_ex(p_config, name, name_len + 1, pzval);
			zend_update_property(Z_OBJCE_P(object), object, ZEND_STRL("_db"), p_config TSRMLS_CC);
		} else {
			if (zend_hash_find(Z_ARRVAL_P(p_config), name, name_len + 1, (void **)&ppval) == SUCCESS) {
				ZVAL_ZVAL(retval, *ppval, 1, 1);
				return retval;
			}
		}
	}

	ZVAL_NULL(retval);
	return retval;
}
/* }}} */

/** {{{ int yod_database_dbconfig(yod_database_t *object, zval *config, long linknum, zval* result TSRMLS_DC)
*/
int yod_database_dbconfig(yod_database_t *object, zval *config, long linknum, zval* result TSRMLS_DC) {
	zval *p_config, *p_locked;
	zval **slaves, **ppval;
	long hash_num, rand_num;
	HashPosition pos;

#if PHP_YOD_DEBUG
	yod_debugf("yod_database_dbconfig()");
#endif

	if (!object) {
		ZVAL_NULL(result);
		return 0;
	}

	p_config = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_config"), 1 TSRMLS_CC);
	if (!config) {
		if (p_config && Z_TYPE_P(p_config) == IS_ARRAY) {
			ZVAL_ZVAL(result, p_config, 1, 0);
		} else {
			ZVAL_NULL(result);
			return 0;
		}
	} else {
		ZVAL_ZVAL(result, config, 1, 0);
	}

	p_locked = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_locked"), 1 TSRMLS_CC);
	if (p_locked && Z_TYPE_P(p_locked) == IS_BOOL && Z_BVAL_P(p_locked)) {
		linknum = 0;
	}

	if (linknum == 1) {
		if (zend_hash_find(Z_ARRVAL_P(config), ZEND_STRS("slaves"), (void **)&slaves) == FAILURE ||
			Z_TYPE_PP(slaves) != IS_ARRAY
		) {
			linknum = 0;
		} else {
			if (zend_hash_find(Z_ARRVAL_PP(slaves), ZEND_STRS("dsn"), (void **)&ppval) == SUCCESS) {
				php_array_merge(Z_ARRVAL_P(result), Z_ARRVAL_PP(slaves), 0 TSRMLS_DC);
			} else {
				hash_num = zend_hash_num_elements(Z_ARRVAL_PP(slaves));
				zend_hash_internal_pointer_reset_ex(Z_ARRVAL_PP(slaves), &pos);
				while (zend_hash_get_current_data_ex(Z_ARRVAL_PP(slaves), (void **)&ppval, &pos) == SUCCESS &&
					Z_TYPE_PP(ppval) == IS_ARRAY
				) {
					rand_num = php_rand(TSRMLS_C);
					if ((double) (rand_num / (PHP_RAND_MAX + 1.0)) < (double) 1 / (double) hash_num) {
						php_array_merge(Z_ARRVAL_P(result), Z_ARRVAL_PP(ppval), 0 TSRMLS_DC);
						break;
					}
					zend_hash_move_forward_ex(Z_ARRVAL_PP(slaves), &pos);
				}
			}
		}
	}

	if (Z_TYPE_P(result) == IS_ARRAY) {
		add_assoc_long(result, "linknum", linknum);
	}

	return 1;
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
	zval *config = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z", &config) == FAILURE) {
		return;
	}

	yod_database_getinstance(config, return_value TSRMLS_CC);
}
/* }}} */

/** {{{ proto public Yod_Database::getInstance($config = 'db_dsn')
*/
PHP_METHOD(yod_database, getInstance) {
	zval *config = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z", &config) == FAILURE) {
		return;
	}

	yod_database_getinstance(config, return_value TSRMLS_CC);
}
/* }}} */

/** {{{ proto public Yod_Database::config($name = '', $value = null)
*/
PHP_METHOD(yod_database, config) {
	yod_database_t *object;
	zval *value = NULL, *retval = NULL;
	char *name = NULL;
	uint name_len = 0;
	int argc, i;
	zval ***args;

#if PHP_YOD_DEBUG
	yod_debugf("yod_database_config()");
#endif

	object = getThis();

	argc = ZEND_NUM_ARGS();
	switch (argc) {
		case 0:
			retval = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_config"), 1 TSRMLS_CC);
			if (retval) {
				RETURN_ZVAL(retval, 1, 0);
			}
			break;
		case 1:
		case 2:
			if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|z", &name, &name_len, &value) == FAILURE) {
				return;
			}
			if (argc == 1) {
				retval = yod_database_config(object, name, name_len, NULL TSRMLS_CC);
				if (retval) {
					RETURN_ZVAL(retval, 1, 1);
				}
			} else {
				(void)yod_database_config(object, name, name_len, value TSRMLS_CC);
			}
			break;
		default :
			args = (zval ***)safe_emalloc(argc, sizeof(zval **), 0);
			if (zend_get_parameters_array_ex(argc, args) == FAILURE || Z_TYPE_PP(args[0]) != IS_STRING) {
				efree(args);
				RETURN_NULL();
			}
			name_len = Z_STRLEN_PP(args[0]);
			name = Z_STRVAL_PP(args[0]);

			MAKE_STD_ZVAL(value);
			array_init(value);
			for (i=1; i<argc; i++) {
				add_next_index_zval(value, *args[i]);
			}

			(void)yod_database_config(object, name, name_len, value TSRMLS_CC);
	}
	RETURN_NULL();
}
/* }}} */

/** {{{ proto public Yod_Database::create($fields, $table, $extend = '')
*/
PHP_METHOD(yod_database, create) {
	yod_database_t *object;
	zval *fields = NULL, *prefix, *query, **ppval;
	char *table = NULL, *extend = NULL, *squery, *values = "";
	uint table_len =0, extend_len = 0, squery_len, values_len = 0;
	HashPosition pos;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zs|s", &fields, &table, &table_len, &extend, &extend_len) == FAILURE) {
		return;
	}

#if PHP_YOD_DEBUG
	yod_debugf("yod_database_create()");
#endif

	object = getThis();

	if (!fields || Z_TYPE_P(fields) != IS_ARRAY || table_len == 0 || !object) {
		RETURN_FALSE;
	}

	zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(fields), &pos);
	while (zend_hash_get_current_data_ex(Z_ARRVAL_P(fields), (void **)&ppval, &pos) == SUCCESS && Z_TYPE_PP(ppval) == IS_STRING) {
		char *str_key = NULL;
		uint key_len;
		int key_type;
		ulong num_key;

		key_type = zend_hash_get_current_key_ex(Z_ARRVAL_P(fields), &str_key, &key_len, &num_key, 0, &pos);
		if (key_type == HASH_KEY_IS_STRING) {
			values_len = spprintf(&values, 0, "%s%s %s, ", values, str_key, Z_STRVAL_PP(ppval));
		} else {
			values_len = spprintf(&values, 0, "%s%s, ", values, Z_STRVAL_PP(ppval));
		}
		zend_hash_move_forward_ex(Z_ARRVAL_P(fields), &pos);
	}
	if (values_len > 0) {
		values_len = values_len - 2;
		*(values + values_len) = '\0';
	}

	prefix = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_prefix"), 1 TSRMLS_CC);
	if (prefix && Z_TYPE_P(prefix) == IS_STRING) {
		squery_len = spprintf(&squery, 0, "CREATE TABLE %s%s (%s) %s", Z_STRVAL_P(prefix), table, values, (extend ? extend : ""));
	} else {
		squery_len = spprintf(&squery, 0, "CREATE TABLE %s (%s) %s", table, values, (extend ? extend : ""));
	}

#if PHP_YOD_DEBUG
	yod_debugl(YOD_DOTLINE);
	yod_debugf(squery);
	yod_debugl(YOD_DOTLINE);
	yod_debugf("yod_database_execute()");
#endif

	MAKE_STD_ZVAL(query);
	ZVAL_STRINGL(query, squery, squery_len, 1);
	zend_call_method_with_1_params(&object, Z_OBJCE_P(object), NULL, "execute", &return_value, query);	
	zval_ptr_dtor(&query);
}
/* }}} */

/** {{{ proto public Yod_Database::insert($data, $table, $replace=false)
*/
PHP_METHOD(yod_database, insert) {
	yod_database_t *object;
	zval *data = NULL, *prefix, *query, *params, **ppval;
	char *table = NULL, *squery, *fields = "", *values = "";
	uint table_len =0, squery_len, fields_len = 0, values_len = 0;
	int replace = 0;
	HashPosition pos;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zs|b", &data, &table, &table_len, &replace) == FAILURE) {
		return;
	}

#if PHP_YOD_DEBUG
	yod_debugf("yod_database_insert()");
#endif

	object = getThis();

	if (!data || Z_TYPE_P(data) != IS_ARRAY || table_len == 0 || !object) {
		RETURN_FALSE;
	}

	MAKE_STD_ZVAL(params);
	array_init(params);

	zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(data), &pos);
	while (zend_hash_get_current_data_ex(Z_ARRVAL_P(data), (void **)&ppval, &pos) == SUCCESS) {
		char *str_key = NULL, *name = NULL;
		uint key_len, name_len;
		ulong num_key;

		if (zend_hash_get_current_key_ex(Z_ARRVAL_P(data), &str_key, &key_len, &num_key, 0, &pos) == HASH_KEY_IS_STRING) {
			name_len = spprintf(&name, 0, ":%s", yod_database_md5(str_key, key_len));
			fields_len = spprintf(&fields, 0, "%s%s, ", fields, str_key);
			values_len = spprintf(&values, 0, "%s%s, ", values, name);
			add_assoc_zval_ex(params, name, name_len + 1, *ppval);
		}
		
		zend_hash_move_forward_ex(Z_ARRVAL_P(data), &pos);
	}
	if (fields_len > 0) {
		fields_len = fields_len - 2;
		*(fields + fields_len) = '\0';
	}
	if (values_len > 0) {
		values_len = values_len - 2;
		*(values + values_len) = '\0';
	}

	prefix = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_prefix"), 1 TSRMLS_CC);
	if (prefix && Z_TYPE_P(prefix) == IS_STRING) {
		squery_len = spprintf(&squery, 0, "%s INTO %s%s (%s) VALUES (%s)", (replace ? "REPLACE" : "INSERT"), Z_STRVAL_P(prefix), table, fields, values);
	} else {
		squery_len = spprintf(&squery, 0, "%s INTO %s (%s) VALUES (%s)", (replace ? "REPLACE" : "INSERT"), table, fields, values);
	}
	
#if PHP_YOD_DEBUG
	yod_debugl(YOD_DOTLINE);
	yod_debugf(squery);
	yod_debugl(YOD_DOTLINE);
	yod_debugf("yod_database_execute()");
#endif

	MAKE_STD_ZVAL(query);
	ZVAL_STRINGL(query, squery, squery_len, 1);
	zend_call_method_with_2_params(&object, Z_OBJCE_P(object), NULL, "execute", &return_value, query, params);	
	zval_ptr_dtor(&query);
}
/* }}} */

/** {{{ proto public Yod_Database::update($data, $table, $where = null, $params = array())
*/
PHP_METHOD(yod_database, update) {
	yod_database_t *object;
	zval *data = NULL, *prefix, *query, *params, **ppval;
	char *table = NULL, *where = NULL, *squery, *update = "";
	uint table_len =0, where_len =0, squery_len, update_len = 0;
	int replace = 0;
	HashPosition pos;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zs|sz", &data, &table, &table_len, &where, &where_len, &params) == FAILURE) {
		return;
	}

#if PHP_YOD_DEBUG
	yod_debugf("yod_database_insert()");
#endif

	object = getThis();

	if (!data || Z_TYPE_P(data) != IS_ARRAY || table_len == 0 || !object) {
		RETURN_FALSE;
	}

	if (!params || Z_TYPE_P(params) != IS_ARRAY) {
		MAKE_STD_ZVAL(params);
		array_init(params);
	}

	zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(data), &pos);
	while (zend_hash_get_current_data_ex(Z_ARRVAL_P(data), (void **)&ppval, &pos) == SUCCESS) {
		char *str_key = NULL, *name = NULL;
		uint key_len, name_len;
		ulong num_key;

		if (zend_hash_get_current_key_ex(Z_ARRVAL_P(data), &str_key, &key_len, &num_key, 0, &pos) == HASH_KEY_IS_STRING) {
			name_len = spprintf(&name, 0, ":%s", yod_database_md5(str_key, key_len));
			update_len = spprintf(&update, 0, "%s%s = %s, ", update, str_key, name);
			add_assoc_zval_ex(params, name, name_len + 1, *ppval);
		}
		zend_hash_move_forward_ex(Z_ARRVAL_P(data), &pos);
	}
	if (update_len > 0) {
		update_len = update_len - 2;
		*(update + update_len) = '\0';
	}

	prefix = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_prefix"), 1 TSRMLS_CC);
	if (prefix && Z_TYPE_P(prefix) == IS_STRING) {
		squery_len = spprintf(&squery, 0, "UPDATE %s%s SET %s%s%s", Z_STRVAL_P(prefix), table, update, (where ? " WHERE " : ""), (where ? where : ""));
	} else {
		squery_len = spprintf(&squery, 0, "UPDATE %s SET %s%s%s", table, update, (where ? " WHERE " : ""), (where ? where : ""));
	}
	
#if PHP_YOD_DEBUG
	yod_debugl(YOD_DOTLINE);
	yod_debugf(squery);
	yod_debugl(YOD_DOTLINE);
	yod_debugf("yod_database_execute()");
#endif

	MAKE_STD_ZVAL(query);
	ZVAL_STRINGL(query, squery, squery_len, 1);
	zend_call_method_with_2_params(&object, Z_OBJCE_P(object), NULL, "execute", &return_value, query, params);	
	zval_ptr_dtor(&query);
}
/* }}} */

/** {{{ proto public Yod_Database::delete($table, $where = null, $params = array())
*/
PHP_METHOD(yod_database, delete) {
	yod_database_t *object;
	zval *prefix, *query, *params, **ppval;
	char *table = NULL, *where = NULL, *squery;
	uint table_len =0, where_len =0, squery_len;
	int replace = 0;
	HashPosition pos;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|sz", &table, &table_len, &where, &where_len, &params) == FAILURE) {
		return;
	}

#if PHP_YOD_DEBUG
	yod_debugf("yod_database_delete()");
#endif

	object = getThis();

	if (table_len == 0 || !object) {
		RETURN_FALSE;
	}

	if (!params || Z_TYPE_P(params) != IS_ARRAY) {
		MAKE_STD_ZVAL(params);
		array_init(params);
	}

	prefix = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_prefix"), 1 TSRMLS_CC);
	if (prefix && Z_TYPE_P(prefix) == IS_STRING) {
		squery_len = spprintf(&squery, 0, "DELETE FROM %s%s%s%s", Z_STRVAL_P(prefix), table, (where ? " WHERE " : ""), (where ? where : ""));
	} else {
		squery_len = spprintf(&squery, 0, "DELETE FROM %s%s%s", table, (where ? " WHERE " : ""), (where ? where : ""));
	}
	
#if PHP_YOD_DEBUG
	yod_debugl(YOD_DOTLINE);
	yod_debugf(squery);
	yod_debugl(YOD_DOTLINE);
	yod_debugf("yod_database_execute()");
#endif

	MAKE_STD_ZVAL(query);
	ZVAL_STRINGL(query, squery, squery_len, 1);
	zend_call_method_with_2_params(&object, Z_OBJCE_P(object), NULL, "execute", &return_value, query, params);	
	zval_ptr_dtor(&query);
}
/* }}} */

/** {{{ proto public Yod_Database::select($select, $table, $where = null, $params = array())
*/
PHP_METHOD(yod_database, select) {
	yod_database_t *object;
	zval *select = NULL, *params = NULL, *prefix, *query, **ppval;
	char *table = NULL, *where = NULL, *squery, *fields = "";
	uint table_len =0, where_len = 0, squery_len, fields_len = 0;
	HashPosition pos;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zs|sz", &select, &table, &table_len, &where, &where_len, &params) == FAILURE) {
		return;
	}

#if PHP_YOD_DEBUG
	yod_debugf("yod_database_select()");
#endif

	object = getThis();

	if (table_len == 0 || !object) {
		RETURN_FALSE;
	}

	if (!params || Z_TYPE_P(params) != IS_ARRAY) {
		MAKE_STD_ZVAL(params);
		array_init(params);
	}
	
	if (select) {
		if (Z_TYPE_P(select) == IS_ARRAY) {
			zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(select), &pos);
			while (zend_hash_get_current_data_ex(Z_ARRVAL_P(select), (void **)&ppval, &pos) == SUCCESS && Z_TYPE_PP(ppval) == IS_STRING) {
				char *str_key = NULL;
				uint key_len;
				int key_type;
				ulong num_key;

				key_type = zend_hash_get_current_key_ex(Z_ARRVAL_P(select), &str_key, &key_len, &num_key, 0, &pos);
				if (key_type == HASH_KEY_IS_STRING) {
					fields_len = spprintf(&fields, 0, "%s%s AS %s, ", fields, str_key, Z_STRVAL_PP(ppval));
				} else {
					fields_len = spprintf(&fields, 0, "%s%s, ", fields, Z_STRVAL_PP(ppval));
				}
				zend_hash_move_forward_ex(Z_ARRVAL_P(select), &pos);
			}
			if (fields_len > 0) {
				fields_len = fields_len - 2;
				*(fields + fields_len) = '\0';
			}
		} else if (Z_TYPE_P(select) == IS_STRING) {
			fields_len = spprintf(&fields, 0, "%s", Z_STRVAL_P(select));
		}
	}

	prefix = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_prefix"), 1 TSRMLS_CC);
	if (prefix && Z_TYPE_P(prefix) == IS_STRING) {
		squery_len = spprintf(&squery, 0, "SELECT %s FROM %s%s%s%s", fields, Z_STRVAL_P(prefix), table, (where ? " WHERE " : ""), (where ? where : ""));
	} else {
		squery_len = spprintf(&squery, 0, "SELECT %s FROM %s%s%s", fields, table, (where ? " WHERE " : ""), (where ? where : ""));
	}

#if PHP_YOD_DEBUG
	yod_debugl(YOD_DOTLINE);
	yod_debugf(squery);
	yod_debugl(YOD_DOTLINE);
	yod_debugf("yod_query_execute()");
#endif

	MAKE_STD_ZVAL(query);
	ZVAL_STRINGL(query, squery, squery_len, 1);
	zend_call_method_with_2_params(&object, Z_OBJCE_P(object), NULL, "query", &return_value, query, params);	
	zval_ptr_dtor(&query);
}
/* }}} */

/** {{{ proto public Yod_Database::lastQuery()
*/
PHP_METHOD(yod_database, lastQuery) {
	yod_database_t *object;
	zval *retval;

	object = getThis();
	retval = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_lastquery"), 1 TSRMLS_CC);
	if (retval) {
		RETURN_ZVAL(retval, 1, 0);
	}
	RETURN_NULL();
}
/* }}} */

/** {{{ proto public Yod_Database::dbconfig($config, $linknum = 0)
*/
PHP_METHOD(yod_database, dbconfig) {
	zval *config;
	long linknum = 0;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|l", &config, &linknum) == FAILURE) {
		return;
	}

	yod_database_dbconfig(getThis(), config, linknum, return_value TSRMLS_CC);
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
	zend_declare_property_string(yod_database_ce, ZEND_STRL("_prefix"), "", ZEND_ACC_PROTECTED TSRMLS_CC);
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