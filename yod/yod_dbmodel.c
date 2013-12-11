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

/** {{{ int yod_dbmodel_getinstance(char *name, uint name_len, zval *config, zval *retval TSRMLS_DC)
*/
int yod_dbmodel_getinstance(char *name, uint name_len, zval *config, zval *retval TSRMLS_DC) {
	yod_model_t *object;
	zval *p_model, **ppval;
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

	spprintf(&classpath, 0, "%s/models/%s.class.php", yod_runpath(TSRMLS_CC), classname);

	MAKE_STD_ZVAL(object);
	if (VCWD_ACCESS(classpath, F_OK) == 0) {
		yod_include(classpath, NULL, 1 TSRMLS_CC);
		if (zend_lookup_class_ex(classname, classname_len, 0, &pce TSRMLS_CC) == SUCCESS) {
			object_init_ex(object, *pce);
			object = yod_model_construct(object, name, name_len, config TSRMLS_CC);
		} else {
			php_error_docref(NULL TSRMLS_CC, E_ERROR, "Class '%s' not found", classname);
			return 0;
		}
	} else {
		if (zend_lookup_class_ex(classname, classname_len, 0, &pce TSRMLS_CC) == SUCCESS) {
			object_init_ex(object, *pce);
			object = yod_model_construct(object, name, name_len, config TSRMLS_CC);
		} else {
			object_init_ex(object, yod_dbmodel_ce);
			object = yod_model_construct(NULL, name, name_len, config TSRMLS_CC);
		}
	}

	add_assoc_zval_ex(p_model, name, name_len + 1, object);
	zend_update_static_property(yod_dbmodel_ce, ZEND_STRL("_model"), p_model TSRMLS_CC);

	if (retval) {
		ZVAL_ZVAL(retval, object, 1, 0);
	}
	return 1;
}
/* }}} */

/** {{{ int yod_dbmodel_find(char *where, uint where_len, zval *params, zval *select, zval *retval TSRMLS_DC)
*/
static int yod_dbmodel_find(char *where, uint where_len, zval *params, zval *select, zval *retval TSRMLS_DC) {

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbmodel_find(%s)", where ? where : "");
#endif

}
/* }}} */

/** {{{ int yod_dbmodel_findall(char *where, uint where_len, zval *params, zval *select, zval *retval TSRMLS_DC)
*/
static int yod_dbmodel_findall(char *where, uint where_len, zval *params, zval *select, zval *retval TSRMLS_DC) {

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbmodel_findall(%s)", where ? where : "");
#endif

}
/* }}} */

/** {{{ int yod_dbmodel_count(char *where, uint where_len, zval *params, zval *retval TSRMLS_DC)
*/
static int yod_dbmodel_count(char *where, uint where_len, zval *params, zval *retval TSRMLS_DC) {

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbmodel_count(%s)", where ? where : "");
#endif

}
/* }}} */

/** {{{ int yod_dbmodel_save(zval *data, char *where, uint where_len, zval *params, zval *retval TSRMLS_DC)
*/
static int yod_dbmodel_save(zval *data, char *where, uint where_len, zval *params, zval *retval TSRMLS_DC) {

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbmodel_save(%s)", where ? where : "");
#endif

}
/* }}} */

/** {{{ int yod_dbmodel_remove(char *where, uint where_len, zval *params, zval *retval TSRMLS_DC)
*/
static int yod_dbmodel_remove(char *where, uint where_len, zval *params, zval *retval TSRMLS_DC) {

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbmodel_remove(%s)", where ? where : "");
#endif

}
/* }}} */

/** {{{ int yod_dbmodel_parsequery(yod_dbmodel_t *object, zval *retval TSRMLS_DC)
*/
static int yod_dbmodel_parsequery(yod_dbmodel_t *object, zval *retval TSRMLS_DC) {
#if PHP_YOD_DEBUG
	yod_debugf("yod_dbmodel_parsequery()");
#endif

}
/* }}} */

/** {{{ int yod_dbmodel_initquery(yod_dbmodel_t *object, zval *retval TSRMLS_DC)
*/
static int yod_dbmodel_initquery(yod_dbmodel_t *object, zval *retval TSRMLS_DC) {
	zval *query, *params;

#if PHP_YOD_DEBUG
	yod_debugf("yod_dbmodel_initquery()");
#endif
	
	if (!object) {
		return 0;
	}

	MAKE_STD_ZVAL(query);
	array_init(query);
	add_assoc_null(query, "SELECT");
	add_assoc_null(query, "FROM");
	add_assoc_null(query, "JOIN");
	add_assoc_null(query, "WHERE");
	add_assoc_null(query, "GROUP BY");
	add_assoc_null(query, "HAVING");
	add_assoc_null(query, "ORDER BY");
	add_assoc_null(query, "LIMIT");
	add_assoc_null(query, "UNION");
	add_assoc_null(query, "COMMENT");
	zend_update_property(Z_OBJCE_P(object), object, ZEND_STRL("_query"), query TSRMLS_CC);

	MAKE_STD_ZVAL(params);
	array_init(params);
	zend_update_property(Z_OBJCE_P(object), object, ZEND_STRL("_params"), params TSRMLS_CC);

	if (retval) {
		ZVAL_ZVAL(retval, object, 1, 0);
	}

	return 1;
}
/* }}} */

/** {{{ proto public Yod_DbModel::__construct($name = '', $config = '')
*/
PHP_METHOD(yod_dbmodel, __construct) {
	yod_dbmodel_t *object;
	zval *p_model, *config = NULL;
	char *name = "";
	uint name_len = 0;
	

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|sz", &name, &name_len, &config) == FAILURE) {
		return;
	}

	object = getThis();

	(void)yod_model_construct(object, name, name_len, config TSRMLS_CC);

	(void)yod_dbmodel_initquery(object, NULL TSRMLS_CC);

	p_model = zend_read_static_property(yod_dbmodel_ce, ZEND_STRL("_model"), 0 TSRMLS_CC);
	if (!p_model || Z_TYPE_P(p_model) != IS_ARRAY) {
		MAKE_STD_ZVAL(p_model);
		array_init(p_model);
	}
	add_assoc_zval_ex(p_model, name, name_len + 1, object);
	zend_update_static_property(yod_dbmodel_ce, ZEND_STRL("_model"), p_model TSRMLS_CC);
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

	yod_dbmodel_find(where, where_len, params, select, return_value TSRMLS_CC);
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

	yod_dbmodel_findall(where, where_len, params, select, return_value TSRMLS_CC);
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

	yod_dbmodel_count(where, where_len, params, return_value TSRMLS_CC);
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

	yod_dbmodel_save(data, where, where_len, params, return_value TSRMLS_CC);
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

	yod_dbmodel_remove(where, where_len, params, return_value TSRMLS_CC);
}
/* }}} */

/** {{{ proto public Yod_DbModel::select($select)
*/
PHP_METHOD(yod_dbmodel, select) {

}
/* }}} */

/** {{{ proto public Yod_DbModel::from($table)
*/
PHP_METHOD(yod_dbmodel, from) {

}
/* }}} */

/** {{{ proto public Yod_DbModel::join($table, $where, $mode = 'LEFT')
*/
PHP_METHOD(yod_dbmodel, join) {

}
/* }}} */

/** {{{ proto public Yod_DbModel::where($where, $params = array(), $mode = 'AND')
*/
PHP_METHOD(yod_dbmodel, where) {

}
/* }}} */

/** {{{ proto public Yod_DbModel::group($group)
*/
PHP_METHOD(yod_dbmodel, group) {

}
/* }}} */

/** {{{ proto public Yod_DbModel::having($having)
*/
PHP_METHOD(yod_dbmodel, having) {

}
/* }}} */

/** {{{ proto public Yod_DbModel::order($order)
*/
PHP_METHOD(yod_dbmodel, order) {

}
/* }}} */

/** {{{ proto public Yod_DbModel::limit($limit)
*/
PHP_METHOD(yod_dbmodel, limit) {

}
/* }}} */

/** {{{ proto public Yod_DbModel::union($union)
*/
PHP_METHOD(yod_dbmodel, union) {

}
/* }}} */

/** {{{ proto public Yod_DbModel::comment($comment)
*/
PHP_METHOD(yod_dbmodel, comment) {

}
/* }}} */

/** {{{ proto public Yod_DbModel::params($params)
*/
PHP_METHOD(yod_dbmodel, params) {

}
/* }}} */

/** {{{ proto public Yod_DbModel::parseQuery($query = null)
*/
PHP_METHOD(yod_dbmodel, parseQuery) {
	yod_dbmodel_parsequery(getThis(), return_value TSRMLS_CC);
}
/* }}} */

/** {{{ proto public Yod_DbModel::initQuery()
*/
PHP_METHOD(yod_dbmodel, initQuery) {
	yod_dbmodel_initquery(getThis(), return_value TSRMLS_CC);
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
