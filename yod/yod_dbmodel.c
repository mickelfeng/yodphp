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
#include "yod_model.h"
#include "yod_dbmodel.h"

zend_class_entry *yod_dbmodel_ce;

/** {{{ ARG_INFO
*/
ZEND_BEGIN_ARG_INFO_EX(yod_dbmodel_construct_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbmodel_getinstance_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbmodel_table_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, table)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbmodel_find_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, where)
	ZEND_ARG_INFO(0, params)
	ZEND_ARG_INFO(0, select)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbmodel_findall_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, where)
	ZEND_ARG_INFO(0, params)
	ZEND_ARG_INFO(0, select)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbmodel_count_arginfo, 0, 0, 0)
	ZEND_ARG_INFO(0, where)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbmodel_save_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, data)
	ZEND_ARG_INFO(0, where)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbmodel_remove_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, where)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbmodel_select_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, select)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbmodel_from_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, table)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbmodel_join_arginfo, 0, 0, 2)
	ZEND_ARG_INFO(0, table)
	ZEND_ARG_INFO(0, where)
	ZEND_ARG_INFO(0, mode)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbmodel_where_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, where)
	ZEND_ARG_INFO(0, params)
	ZEND_ARG_INFO(0, mode)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbmodel_group_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, group)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbmodel_having_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, having)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbmodel_order_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, order)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbmodel_limit_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, limit)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbmodel_union_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, union)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbmodel_comment_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, comment)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbmodel_params_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbmodel_parsequery_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, query)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(yod_dbmodel_initquery_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ yod_dbmodel_t *yod_dbmodel_construct(yod_dbmodel_t *object, char *name, uint name_len, zval *config TSRMLS_DC)
*/
yod_dbmodel_t *yod_dbmodel_construct(yod_dbmodel_t *object, char *name, uint name_len, zval *config TSRMLS_DC) {
	yod_dbmodel_t *retval;
	zval *p_model;

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbmodel_construct(%s)", name ? name : "");
#endif

	if (object) {
		retval = object;
	} else {
		MAKE_STD_ZVAL(retval);
		object_init_ex(retval, yod_dbmodel_ce);
	}

	(void)yod_model_construct(retval, name, name_len, config TSRMLS_CC);

	yod_dbmodel_initquery(retval, NULL TSRMLS_CC);

	p_model = zend_read_static_property(yod_dbmodel_ce, ZEND_STRL("_model"), 0 TSRMLS_CC);
	if (!p_model || Z_TYPE_P(p_model) != IS_ARRAY) {
		MAKE_STD_ZVAL(p_model);
		array_init(p_model);
	}
	add_assoc_zval_ex(p_model, name, name_len + 1, retval);
	zend_update_static_property(yod_dbmodel_ce, ZEND_STRL("_model"), p_model TSRMLS_CC);

	return retval;
}
/* }}} */

/** {{{ int yod_dbmodel_getinstance(char *name, uint name_len, zval *config, zval *retval TSRMLS_DC)
*/
int yod_dbmodel_getinstance(char *name, uint name_len, zval *config, zval *retval TSRMLS_DC) {
	yod_model_t *object;
	zval *p_model, *p_name, **ppval;
	char *classname, *classpath;
	uint classname_len;
	zend_class_entry **pce = NULL;

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbmodel_getinstance(%s)", name ? name : "");
#endif

	if (name_len == 0) {
		classname_len = spprintf(&classname, 0, "Yod_DbModel");
	} else {
		classname_len = spprintf(&classname, 0, "%sModel", name);
	}

	p_model = zend_read_static_property(yod_dbmodel_ce, ZEND_STRL("_model"), 0 TSRMLS_CC);
	if (p_model && Z_TYPE_P(p_model) == IS_ARRAY) {
		if (zend_hash_find(Z_ARRVAL_P(p_model), name, name_len + 1, (void **)&ppval) == SUCCESS) {
			if (retval) {
				ZVAL_ZVAL(retval, *ppval, 1, 0);
			}
			return 1;
		}
	} else {
		MAKE_STD_ZVAL(p_model);
		array_init(p_model);
	}

	if (!config) {
		MAKE_STD_ZVAL(config);
		ZVAL_NULL(config);
	}

	MAKE_STD_ZVAL(object);
	if (name_len > 0) {
		spprintf(&classpath, 0, "%s/models/%s.class.php", yod_runpath(TSRMLS_CC), classname);

		if (VCWD_ACCESS(classpath, F_OK) == 0) {
			yod_include(classpath, NULL, 1 TSRMLS_CC);
			if (zend_lookup_class_ex(classname, classname_len, 0, &pce TSRMLS_CC) == SUCCESS) {
				object_init_ex(object, *pce);
				if (zend_hash_exists(&(*pce)->function_table, ZEND_STRS(ZEND_CONSTRUCTOR_FUNC_NAME))) {
					MAKE_STD_ZVAL(p_name);
					ZVAL_STRINGL(p_name, name, name_len, 1);
					zend_call_method_with_2_params(&object, *pce, &(*pce)->constructor, ZEND_CONSTRUCTOR_FUNC_NAME, NULL, p_name, config);
					zval_ptr_dtor(&p_name);
				}
			} else {
				php_error_docref(NULL TSRMLS_CC, E_ERROR, "Class '%s' not found", classname);
				return 0;
			}
		} else {
			if (zend_lookup_class_ex(classname, classname_len, 0, &pce TSRMLS_CC) == SUCCESS) {
				object_init_ex(object, *pce);
				if (zend_hash_exists(&(*pce)->function_table, ZEND_STRS(ZEND_CONSTRUCTOR_FUNC_NAME))) {
					MAKE_STD_ZVAL(p_name);
					ZVAL_STRINGL(p_name, name, name_len, 1);
					zend_call_method_with_2_params(&object, *pce, &(*pce)->constructor, ZEND_CONSTRUCTOR_FUNC_NAME, NULL, p_name, config);
					zval_ptr_dtor(&p_name);
				}
			} else {
				object = yod_dbmodel_construct(NULL, name, name_len, config TSRMLS_CC);
			}
		}
	} else {
		object = yod_dbmodel_construct(NULL, name, name_len, config TSRMLS_CC);
	}

	add_assoc_zval_ex(p_model, name, name_len + 1, object);
	zend_update_static_property(yod_dbmodel_ce, ZEND_STRL("_model"), p_model TSRMLS_CC);

	if (retval) {
		ZVAL_ZVAL(retval, object, 1, 0);
	}
	return 1;
}
/* }}} */

/** {{{ int yod_dbmodel_find(yod_dbmodel_t *object, char *where, uint where_len, zval *params, zval *select, zval *retval TSRMLS_DC)
*/
static int yod_dbmodel_find(yod_dbmodel_t *object, char *where, uint where_len, zval *params, zval *select, zval *retval TSRMLS_DC) {

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbmodel_find(%s)", where ? where : "");
#endif

}
/* }}} */

/** {{{ int yod_dbmodel_findall(yod_dbmodel_t *object, char *where, uint where_len, zval *params, zval *select, zval *retval TSRMLS_DC)
*/
static int yod_dbmodel_findall(yod_dbmodel_t *object, char *where, uint where_len, zval *params, zval *select, zval *retval TSRMLS_DC) {

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbmodel_findall(%s)", where ? where : "");
#endif

}
/* }}} */

/** {{{ int yod_dbmodel_count(yod_dbmodel_t *object, char *where, uint where_len, zval *params, zval *retval TSRMLS_DC)
*/
static int yod_dbmodel_count(yod_dbmodel_t *object, char *where, uint where_len, zval *params, zval *retval TSRMLS_DC) {

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbmodel_count(%s)", where ? where : "");
#endif

}
/* }}} */

/** {{{ int yod_dbmodel_save(yod_dbmodel_t *object, zval *data, char *where, uint where_len, zval *params, zval *retval TSRMLS_DC)
*/
static int yod_dbmodel_save(yod_dbmodel_t *object, zval *data, char *where, uint where_len, zval *params, zval *retval TSRMLS_DC) {

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbmodel_save(%s)", where ? where : "");
#endif

}
/* }}} */

/** {{{ int yod_dbmodel_remove(yod_dbmodel_t *object, char *where, uint where_len, zval *params, zval *retval TSRMLS_DC)
*/
static int yod_dbmodel_remove(yod_dbmodel_t *object, char *where, uint where_len, zval *params, zval *retval TSRMLS_DC) {

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbmodel_remove(%s)", where ? where : "");
#endif

}
/* }}} */

/** {{{ int yod_dbmodel_select(yod_dbmodel_t *object, zval *select TSRMLS_DC)
*/
static int yod_dbmodel_select(yod_dbmodel_t *object, zval *select TSRMLS_DC) {
	zval *query, **ppval;
	char *fields = "";
	uint fields_len = 0;
	HashPosition pos;

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbmodel_select()");
#endif

	if (!object) {
		return 0;
	}

	query = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_query"), 1 TSRMLS_CC);
	if (!query || Z_TYPE_P(query) != IS_ARRAY) {
		yod_dbmodel_initquery(object, query TSRMLS_CC);
	}

	if (Z_TYPE_P(query) == IS_ARRAY) {
		if (zend_hash_find(Z_ARRVAL_P(query), ZEND_STRS("SELECT"), (void **)&ppval) == FAILURE) {
			return 0;
		}

		add_assoc_stringl(query, "SELECT", "*", 1, 1);
		if (select) {
			if (Z_TYPE_P(select) == IS_ARRAY) {
				zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(select), &pos);
				while (zend_hash_get_current_data_ex(Z_ARRVAL_P(select), (void **)&ppval, &pos) == SUCCESS) {
					char *str_key = NULL;
					uint key_len;
					int key_type;
					ulong num_key;

					if (Z_TYPE_PP(ppval) != IS_STRING) {
						continue;
					}

					key_type = zend_hash_get_current_key_ex(Z_ARRVAL_P(select), &str_key, &key_len, &num_key, 0, &pos);
					if (key_type == HASH_KEY_IS_STRING) {
						fields_len = spprintf(&fields, 0, "%s%s AS %s, ", fields, Z_STRVAL_PP(ppval), str_key);
					} else {
						fields_len = spprintf(&fields, 0, "%s%s, ", fields, Z_STRVAL_PP(ppval));
					}

					zend_hash_move_forward_ex(Z_ARRVAL_P(select), &pos);
				}
				if (fields_len > 0) {
					fields_len = fields_len - 2;
					*(fields + fields_len) = '\0';
				}
				add_assoc_stringl(query, "SELECT", fields, fields_len, 0);
			} else if (Z_TYPE_P(select) == IS_STRING && Z_STRLEN_P(select)) {
				add_assoc_stringl(query, "SELECT", Z_STRVAL_P(select), Z_STRLEN_P(select), 1);
			}
		}

		zend_update_property(Z_OBJCE_P(object), object, ZEND_STRL("_query"), query TSRMLS_CC);
		return 1;
	}

	return 0;
}
/* }}} */

/** {{{ int yod_dbmodel_from(yod_dbmodel_t *object, char *table, uint table_len TSRMLS_DC)
*/
static int yod_dbmodel_from(yod_dbmodel_t *object, char *table, uint table_len TSRMLS_DC) {
	zval *query, *prefix, **ppval;

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbmodel_from()");
#endif

	if (!object) {
		return 0;
	}

	query = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_query"), 1 TSRMLS_CC);
	if (!query || Z_TYPE_P(query) != IS_ARRAY) {
		yod_dbmodel_initquery(object, query TSRMLS_CC);
	}

	if (Z_TYPE_P(query) == IS_ARRAY) {
		if (zend_hash_find(Z_ARRVAL_P(query), ZEND_STRS("FROM"), (void **)&ppval) == FAILURE) {
			return 0;
		}


		if (table_len) {
			prefix = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_prefix"), 1 TSRMLS_CC);
			if (prefix && Z_TYPE_P(prefix) == IS_STRING && Z_STRLEN_P(prefix)) {
				table_len = spprintf(&table, 0, "%s%s", Z_STRVAL_P(prefix), table);
			}
			add_assoc_stringl(query, "FROM", table, table_len, 1);

			zend_update_property(Z_OBJCE_P(object), object, ZEND_STRL("_query"), query TSRMLS_CC);
			return 1;
		}
	}

	return 0;
}
/* }}} */

/** {{{ int yod_dbmodel_join(yod_dbmodel_t *object, char *table, uint table_len, char *where, uint where_len, char *mode, uint mode_len TSRMLS_DC)
*/
static int yod_dbmodel_join(yod_dbmodel_t *object, char *table, uint table_len, char *where, uint where_len, char *mode, uint mode_len TSRMLS_DC) {
	zval *query, *prefix, **ppval;
	char *squery = "";
	uint squery_len = 0;
	long nelem;

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbmodel_join()");
#endif

	if (!object) {
		return 0;
	}

	query = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_query"), 1 TSRMLS_CC);
	if (!query || Z_TYPE_P(query) != IS_ARRAY) {
		yod_dbmodel_initquery(object, query TSRMLS_CC);
	}

	if (Z_TYPE_P(query) == IS_ARRAY) {
		if (zend_hash_find(Z_ARRVAL_P(query), ZEND_STRS("JOIN"), (void **)&ppval) == FAILURE) {
			return 0;
		}

		if (table_len) {
			Z_ADDREF_PP(ppval);

			if (Z_TYPE_PP(ppval) != IS_ARRAY) {
				ALLOC_INIT_ZVAL(*ppval);
				array_init(*ppval);
			}

			nelem = zend_hash_num_elements(Z_ARRVAL_PP(ppval)) + 2;

			prefix = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_prefix"), 1 TSRMLS_CC);
			if (prefix && Z_TYPE_P(prefix) == IS_STRING && Z_STRLEN_P(prefix)) {
				squery_len = spprintf(&squery, 0, "%s%sJOIN %s%s AS t%d%s%s", (mode ? mode : "LEFT"), (mode_len ? " " : ""), Z_STRVAL_P(prefix), table, nelem, (where_len ? " ON " : ""), (where_len ? where : ""));
			} else {
				squery_len = spprintf(&squery, 0, "%s%sJOIN %s AS t%d%s%s", (mode ? mode : "LEFT"), (mode_len ? " " : ""), table, nelem, (where_len ? " ON " : ""), (where_len ? where : ""));
			}
			add_next_index_stringl(*ppval, squery, squery_len, 1);
			add_assoc_zval(query, "JOIN", *ppval);

			zend_update_property(Z_OBJCE_P(object), object, ZEND_STRL("_query"), query TSRMLS_CC);
			return 1;
		}
	}

	return 0;
}
/* }}} */

/** {{{ int yod_dbmodel_where(yod_dbmodel_t *object, char *where, uint where_len, zval *params, char *mode, uint mode_len TSRMLS_DC)
*/
static int yod_dbmodel_where(yod_dbmodel_t *object, char *where, uint where_len, zval *params, char *mode, uint mode_len TSRMLS_DC) {

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbmodel_where()");
#endif

}
/* }}} */

/** {{{ int yod_dbmodel_group(yod_dbmodel_t *object, char *group, uint group_len TSRMLS_DC)
*/
static int yod_dbmodel_group(yod_dbmodel_t *object, char *group, uint group_len TSRMLS_DC) {

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbmodel_group()");
#endif

}
/* }}} */

/** {{{ int yod_dbmodel_having(yod_dbmodel_t *object, char *having, uint having_len TSRMLS_DC)
*/
static int yod_dbmodel_having(yod_dbmodel_t *object, char *having, uint having_len TSRMLS_DC) {

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbmodel_having()");
#endif

}
/* }}} */

/** {{{ int yod_dbmodel_order(yod_dbmodel_t *object, char *order, uint order_len TSRMLS_DC)
*/
static int yod_dbmodel_order(yod_dbmodel_t *object, char *order, uint order_len TSRMLS_DC) {

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbmodel_order()");
#endif

}
/* }}} */

/** {{{ int yod_dbmodel_limit(yod_dbmodel_t *object, char *limit, uint limit_len TSRMLS_DC)
*/
static int yod_dbmodel_limit(yod_dbmodel_t *object, char *limit, uint limit_len TSRMLS_DC) {

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbmodel_limit()");
#endif

}
/* }}} */

/** {{{ int yod_dbmodel_union(yod_dbmodel_t *object, char *unions, uint unions_len TSRMLS_DC)
*/
static int yod_dbmodel_union(yod_dbmodel_t *object, char *unions, uint unions_len TSRMLS_DC) {

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbmodel_union()");
#endif

}
/* }}} */

/** {{{ int yod_dbmodel_comment(yod_dbmodel_t *object, char *comment, uint comment_len TSRMLS_DC)
*/
static int yod_dbmodel_comment(yod_dbmodel_t *object, char *comment, uint comment_len TSRMLS_DC) {

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbmodel_comment()");
#endif

}
/* }}} */

/** {{{ int yod_dbmodel_params(yod_dbmodel_t *object, zval *params TSRMLS_DC)
*/
static int yod_dbmodel_params(yod_dbmodel_t *object, zval *params TSRMLS_DC) {

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbmodel_params()");
#endif

}
/* }}} */

/** {{{ int yod_dbmodel_parsequery(yod_dbmodel_t *object, zval *query, zval *retval TSRMLS_DC)
*/
static int yod_dbmodel_parsequery(yod_dbmodel_t *object, zval *query, zval *retval TSRMLS_DC) {
	zval  *prefix, *table, **ppval;
	char *from, *squery = "";
	uint from_len, squery_len;
	HashPosition pos;

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbmodel_parsequery()");
#endif

	if (!object) {
		if (retval) {
			ZVAL_BOOL(retval, 0);
		}
		return 0;
	}

	if (!query || Z_TYPE_P(query) != IS_ARRAY) {
		query = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_query"), 1 TSRMLS_CC);
	}
	
	if (!query || Z_TYPE_P(query) != IS_ARRAY) {
		if (retval) {
			ZVAL_BOOL(retval, 0);
		}
		return 0;
	}

	if (zend_hash_find(Z_ARRVAL_P(query), ZEND_STRS("FROM"), (void **)&ppval) == FAILURE ||
		Z_TYPE_PP(ppval) != IS_STRING || Z_STRLEN_PP(ppval) == 0
	) {
		table = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_table"), 1 TSRMLS_CC);
		if (!table || Z_TYPE_P(table) != IS_STRING || Z_STRLEN_P(table) == 0) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "You have an error in your SQL syntax: table name is empty");
			if (retval) {
				ZVAL_BOOL(retval, 0);
			}
			return 0;
		}

		prefix = zend_read_property(Z_OBJCE_P(object), object, ZEND_STRL("_prefix"), 1 TSRMLS_CC);
		if (prefix && Z_TYPE_P(prefix) == IS_STRING) {
			from_len = spprintf(&from, 0, "%s%s AS t1", Z_STRVAL_P(prefix), Z_STRVAL_P(table));
		} else {
			from_len = spprintf(&from, 0, "%s AS t1", Z_STRVAL_P(table));
		}
		
		add_assoc_stringl(query, "FROM", from, from_len, 1);
	}

	zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(query), &pos);
	while (zend_hash_get_current_data_ex(Z_ARRVAL_P(query), (void **)&ppval, &pos) == SUCCESS) {
		char *str_key = NULL, *name = NULL;
		uint key_len, name_len;
		ulong num_key;

		if (!ppval || Z_TYPE_PP(ppval) == IS_NULL) {
			continue;
		}

		if (zend_hash_get_current_key_ex(Z_ARRVAL_P(query), &str_key, &key_len, &num_key, 0, &pos) == HASH_KEY_IS_STRING) {
			if (Z_TYPE_PP(ppval) == IS_ARRAY) {
				if (key_len == 6 && strncmp(str_key, "UNION", 5) == 0) {
					zval *value;
					MAKE_STD_ZVAL(value);
					yod_dbmodel_parsequery(object, *ppval, value TSRMLS_CC);
					if (value && Z_TYPE_P(value) == IS_STRING && Z_STRLEN_P(value)) {
						squery_len = spprintf(&squery, 0, "%s UNION %s", squery, Z_STRVAL_P(value));
					}
				} else if (key_len == 5 && strncmp(str_key, "JOIN", 4) == 0) {
					zval **value;
					HashPosition hpos;

					zend_hash_internal_pointer_reset_ex(Z_ARRVAL_PP(ppval), &hpos);
					while (zend_hash_get_current_data_ex(Z_ARRVAL_PP(ppval), (void **)&value, &hpos) == SUCCESS) {
						if (value && Z_TYPE_PP(value) == IS_STRING) {
							squery_len = spprintf(&squery, 0, "%s %s", squery, Z_STRVAL_PP(value));
						}
						zend_hash_move_forward_ex(Z_ARRVAL_PP(ppval), &hpos);
					}
				}
			} else if (Z_TYPE_PP(ppval) == IS_STRING && Z_STRLEN_PP(ppval)) {
				squery_len = spprintf(&squery, 0, "%s %s %s", squery, str_key, Z_STRVAL_PP(ppval));
			}
		}
		
		zend_hash_move_forward_ex(Z_ARRVAL_P(query), &pos);
	}

	if (squery_len == 0) {
		if (retval) {
			ZVAL_BOOL(retval, 0);
		}
		return 0;
	}

	if (retval) {
		ZVAL_STRINGL(retval, squery, squery_len, 0);
	}
	return 1;
}
/* }}} */

/** {{{ int yod_dbmodel_initquery(yod_dbmodel_t *object, zval *retval TSRMLS_DC)
*/
int yod_dbmodel_initquery(yod_dbmodel_t *object, zval *retval TSRMLS_DC) {
	zval *query, *join, *params;

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbmodel_initquery()");
#endif
	
	if (!object) {
		if (retval) {
			ZVAL_NULL(retval);
		}
		return 0;
	}

	MAKE_STD_ZVAL(query);
	array_init(query);
	add_assoc_stringl(query, "SELECT", "", 0, 1);
	add_assoc_stringl(query, "FROM", "", 0, 1);
	MAKE_STD_ZVAL(join);
	array_init(join);
	add_assoc_zval(query, "JOIN", join);
	add_assoc_stringl(query, "WHERE", "", 0, 1);
	add_assoc_stringl(query, "GROUP BY", "", 0, 1);
	add_assoc_stringl(query, "HAVING", "", 0, 1);
	add_assoc_stringl(query, "ORDER BY", "", 0, 1);
	add_assoc_stringl(query, "LIMIT", "", 0, 1);
	add_assoc_stringl(query, "UNION", "", 0, 1);
	add_assoc_stringl(query, "COMMENT", "", 0, 1);
	zend_update_property(Z_OBJCE_P(object), object, ZEND_STRL("_query"), query TSRMLS_CC);
	
	MAKE_STD_ZVAL(params);
	array_init(params);
	zend_update_property(Z_OBJCE_P(object), object, ZEND_STRL("_params"), params TSRMLS_CC);
	zval_ptr_dtor(&params);

	if (retval) {
		ZVAL_ZVAL(retval, query, 1, 0);
	} else {
		zval_ptr_dtor(&query);
	}

	return 1;
}
/* }}} */

/** {{{ proto public Yod_DbModel::__construct($name = '', $config = '')
*/
PHP_METHOD(yod_dbmodel, __construct) {
	zval *config = NULL;
	char *name = NULL;
	uint name_len = 0;
	

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|sz", &name, &name_len, &config) == FAILURE) {
		return;
	}

	(void)yod_dbmodel_construct(getThis(), name, name_len, config TSRMLS_CC);
}
/* }}} */

/** {{{ proto public Yod_DbModel::getInstance($name, $config = '')
*/
PHP_METHOD(yod_dbmodel, getInstance) {
	char *name = NULL;
	uint name_len = 0;
	zval *config = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|z", &name, &name_len, &config) == FAILURE) {
		return;
	}

	yod_dbmodel_getinstance(name, name_len, config, return_value TSRMLS_CC);
}
/* }}} */

/** {{{ proto public Yod_DbModel::table($table)
*/
PHP_METHOD(yod_dbmodel, table) {
	yod_dbmodel_t *object;
	char *table = NULL;
	uint table_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &table, &table_len) == FAILURE) {
		return;
	}

	object = getThis();

	if (table_len) {
		zend_update_property_stringl(Z_OBJCE_P(object), object, ZEND_STRL("_table"), table, table_len TSRMLS_CC);
	}
}
/* }}} */

/** {{{ proto public Yod_DbModel::find($where = '', $params = array(), $select = '*')
*/
PHP_METHOD(yod_dbmodel, find) {
	zval *params = NULL, *select = NULL;
	char *where = NULL;
	uint where_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|szz", &where, &where_len, &params, &select) == FAILURE) {
		return;
	}

	yod_dbmodel_find(getThis(), where, where_len, params, select, return_value TSRMLS_CC);
}
/* }}} */

/** {{{ proto public Yod_DbModel::findAll($where = '', $params = array(), $select = '*')
*/
PHP_METHOD(yod_dbmodel, findAll) {
	zval *params = NULL, *select = NULL;
	char *where = NULL;
	uint where_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|szz", &where, &where_len, &params, &select) == FAILURE) {
		return;
	}

	yod_dbmodel_findall(getThis(), where, where_len, params, select, return_value TSRMLS_CC);
}
/* }}} */

/** {{{ proto public Yod_DbModel::count($where = '', $params = array())
*/
PHP_METHOD(yod_dbmodel, count) {
	zval *params = NULL, *select = NULL;
	char *where = NULL;
	uint where_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|sz", &where, &where_len, &params) == FAILURE) {
		return;
	}

	yod_dbmodel_count(getThis(), where, where_len, params, return_value TSRMLS_CC);
}
/* }}} */

/** {{{ proto public Yod_DbModel::save($data, $where = '', $params = array())
*/
PHP_METHOD(yod_dbmodel, save) {
	zval *data = NULL, *params = NULL;
	char *where = NULL;
	uint where_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|sz", &data, &where, &where_len, &params) == FAILURE) {
		return;
	}

	yod_dbmodel_save(getThis(), data, where, where_len, params, return_value TSRMLS_CC);
}
/* }}} */

/** {{{ proto public Yod_DbModel::remove($where, $params = array())
*/
PHP_METHOD(yod_dbmodel, remove) {
	zval *params = NULL;
	char *where = NULL;
	uint where_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &where, &where_len, &params) == FAILURE) {
		return;
	}

	yod_dbmodel_remove(getThis(), where, where_len, params, return_value TSRMLS_CC);
}
/* }}} */

/** {{{ proto public Yod_DbModel::select($select)
*/
PHP_METHOD(yod_dbmodel, select) {
	yod_dbmodel_t *object;
	zval *select = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &select) == FAILURE) {
		return;
	}

	object = getThis();

	yod_dbmodel_select(object, select TSRMLS_CC);

	RETURN_ZVAL(object, 1, 0);
}
/* }}} */

/** {{{ proto public Yod_DbModel::from($table)
*/
PHP_METHOD(yod_dbmodel, from) {
	yod_dbmodel_t *object;
	char *table = NULL;
	uint table_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &table, &table_len) == FAILURE) {
		return;
	}

	object = getThis();

	yod_dbmodel_from(object, table, table_len TSRMLS_CC);

	RETURN_ZVAL(object, 1, 0);
}
/* }}} */

/** {{{ proto public Yod_DbModel::join($table, $where = '', $mode = 'LEFT')
*/
PHP_METHOD(yod_dbmodel, join) {
	yod_dbmodel_t *object;
	char *table = NULL, *where = NULL, *mode = NULL;
	uint table_len = 0, where_len = 0, mode_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss|s", &table, &table_len, &where, &where_len, &mode, &mode_len) == FAILURE) {
		return;
	}

	object = getThis();

	yod_dbmodel_join(object, table, table_len, where, where_len, mode, mode_len TSRMLS_CC);

	RETURN_ZVAL(object, 1, 0);
}
/* }}} */

/** {{{ proto public Yod_DbModel::where($where, $params = array(), $mode = 'AND')
*/
PHP_METHOD(yod_dbmodel, where) {
	yod_dbmodel_t *object;
	zval *params = NULL;
	char *where = NULL, *mode = NULL;
	uint where_len = 0, mode_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|zs", &where, &where_len, &params, &mode, &mode_len) == FAILURE) {
		return;
	}

	object = getThis();

	yod_dbmodel_where(object, where, where_len, params, mode, mode_len TSRMLS_CC);

	RETURN_ZVAL(object, 1, 0);
}
/* }}} */

/** {{{ proto public Yod_DbModel::group($group)
*/
PHP_METHOD(yod_dbmodel, group) {
	yod_dbmodel_t *object;
	char *group = NULL;
	uint group_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &group, &group_len) == FAILURE) {
		return;
	}

	object = getThis();

	yod_dbmodel_group(object, group, group_len TSRMLS_CC);

	RETURN_ZVAL(object, 1, 0);
}
/* }}} */

/** {{{ proto public Yod_DbModel::having($having)
*/
PHP_METHOD(yod_dbmodel, having) {
	yod_dbmodel_t *object;
	char *having = NULL;
	uint having_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &having, &having_len) == FAILURE) {
		return;
	}

	object = getThis();

	yod_dbmodel_having(object, having, having_len TSRMLS_CC);

	RETURN_ZVAL(object, 1, 0);
}
/* }}} */

/** {{{ proto public Yod_DbModel::order($order)
*/
PHP_METHOD(yod_dbmodel, order) {
	yod_dbmodel_t *object;
	char *order = NULL;
	uint order_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &order, &order_len) == FAILURE) {
		return;
	}

	object = getThis();

	yod_dbmodel_order(object, order, order_len TSRMLS_CC);

	RETURN_ZVAL(object, 1, 0);
}
/* }}} */

/** {{{ proto public Yod_DbModel::limit($limit)
*/
PHP_METHOD(yod_dbmodel, limit) {
	yod_dbmodel_t *object;
	char *limit = NULL;
	uint limit_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &limit, &limit_len) == FAILURE) {
		return;
	}

	object = getThis();

	yod_dbmodel_limit(object, limit, limit_len TSRMLS_CC);

	RETURN_ZVAL(object, 1, 0);
}
/* }}} */

/** {{{ proto public Yod_DbModel::union($union)
*/
PHP_METHOD(yod_dbmodel, union) {
	yod_dbmodel_t *object;
	char *unions = NULL;
	uint unions_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &unions, &unions_len) == FAILURE) {
		return;
	}

	object = getThis();

	yod_dbmodel_union(object, unions, unions_len TSRMLS_CC);

	RETURN_ZVAL(object, 1, 0);
}
/* }}} */

/** {{{ proto public Yod_DbModel::comment($comment)
*/
PHP_METHOD(yod_dbmodel, comment) {
	yod_dbmodel_t *object;
	char *comment = NULL;
	uint comment_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &comment, &comment_len) == FAILURE) {
		return;
	}

	object = getThis();

	yod_dbmodel_comment(object, comment, comment_len TSRMLS_CC);

	RETURN_ZVAL(object, 1, 0);
}
/* }}} */

/** {{{ proto public Yod_DbModel::params($params)
*/
PHP_METHOD(yod_dbmodel, params) {
	yod_dbmodel_t *object;
	zval *params = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &params) == FAILURE) {
		return;
	}

	object = getThis();

	yod_dbmodel_params(object, params TSRMLS_CC);

	RETURN_ZVAL(object, 1, 0);
}
/* }}} */

/** {{{ proto public Yod_DbModel::parseQuery($query = null)
*/
PHP_METHOD(yod_dbmodel, parseQuery) {
	zval *query = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z", &query) == FAILURE) {
		return;
	}

	yod_dbmodel_parsequery(getThis(), query, return_value TSRMLS_CC);
}
/* }}} */

/** {{{ proto public Yod_DbModel::initQuery()
*/
PHP_METHOD(yod_dbmodel, initQuery) {
	yod_dbmodel_t *object;

	object = getThis();

	yod_dbmodel_initquery(object, NULL TSRMLS_CC);

	RETURN_ZVAL(object, 1, 0);
}
/* }}} */

/** {{{ yod_dbmodel_methods[]
*/
zend_function_entry yod_dbmodel_methods[] = {
	PHP_ME(yod_dbmodel, __construct,	yod_dbmodel_construct_arginfo,		ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(yod_dbmodel, getInstance,	yod_dbmodel_getinstance_arginfo,	ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(yod_dbmodel, table,			yod_dbmodel_find_arginfo,			ZEND_ACC_PUBLIC)
	PHP_ME(yod_dbmodel, find,			yod_dbmodel_find_arginfo,			ZEND_ACC_PUBLIC)
	PHP_ME(yod_dbmodel, findAll,		yod_dbmodel_findall_arginfo,		ZEND_ACC_PUBLIC)
	PHP_ME(yod_dbmodel, count,			yod_dbmodel_count_arginfo,			ZEND_ACC_PUBLIC)
	PHP_ME(yod_dbmodel, save,			yod_dbmodel_save_arginfo,			ZEND_ACC_PUBLIC)
	PHP_ME(yod_dbmodel, remove,			yod_dbmodel_remove_arginfo,			ZEND_ACC_PUBLIC)
	PHP_ME(yod_dbmodel, select,			yod_dbmodel_select_arginfo,			ZEND_ACC_PUBLIC)
	PHP_ME(yod_dbmodel, from,			yod_dbmodel_from_arginfo,			ZEND_ACC_PUBLIC)
	PHP_ME(yod_dbmodel, join,			yod_dbmodel_join_arginfo,			ZEND_ACC_PUBLIC)
	PHP_ME(yod_dbmodel, where,			yod_dbmodel_where_arginfo,			ZEND_ACC_PUBLIC)
	PHP_ME(yod_dbmodel, group,			yod_dbmodel_group_arginfo,			ZEND_ACC_PUBLIC)
	PHP_ME(yod_dbmodel, having,			yod_dbmodel_having_arginfo,			ZEND_ACC_PUBLIC)
	PHP_ME(yod_dbmodel, order,			yod_dbmodel_order_arginfo,			ZEND_ACC_PUBLIC)
	PHP_ME(yod_dbmodel, limit,			yod_dbmodel_limit_arginfo,			ZEND_ACC_PUBLIC)
	PHP_ME(yod_dbmodel, union,			yod_dbmodel_union_arginfo,			ZEND_ACC_PUBLIC)
	PHP_ME(yod_dbmodel, comment,		yod_dbmodel_comment_arginfo,		ZEND_ACC_PUBLIC)
	PHP_ME(yod_dbmodel, params,			yod_dbmodel_params_arginfo,			ZEND_ACC_PUBLIC)
	PHP_ME(yod_dbmodel, parseQuery,		yod_dbmodel_parsequery_arginfo,		ZEND_ACC_PROTECTED)
	PHP_ME(yod_dbmodel, initQuery,		yod_dbmodel_initquery_arginfo,		ZEND_ACC_PROTECTED)
	{NULL, NULL, NULL}
};
/* }}} */

/** {{{ PHP_MINIT_FUNCTION
*/
PHP_MINIT_FUNCTION(yod_dbmodel) {
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "Yod_DbModel", yod_dbmodel_methods);
	yod_dbmodel_ce = zend_register_internal_class_ex(&ce, yod_model_ce, NULL TSRMLS_CC);

	zend_declare_property_null(yod_dbmodel_ce, ZEND_STRL("_model"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC TSRMLS_CC);
	zend_declare_property_null(yod_dbmodel_ce, ZEND_STRL("_query"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(yod_dbmodel_ce, ZEND_STRL("_params"), ZEND_ACC_PROTECTED TSRMLS_CC);

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
