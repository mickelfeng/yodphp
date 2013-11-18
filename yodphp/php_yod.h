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

#ifndef PHP_YOD_H
#define PHP_YOD_H

extern zend_module_entry yod_module_entry;
#define phpext_yod_ptr &yod_module_entry

#ifdef PHP_WIN32
#define PHP_YOD_API __declspec(dllexport)
#ifndef _MSC_VER
#define _MSC_VER 1600
#endif
#else
#define PHP_YOD_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#ifdef ZTS
#define YOD_G(v) TSRMG(yod_globals_id, zend_yod_globals *, v)
#else
#define YOD_G(v) (yod_globals.v)
#endif

#define YOD_VERSION						"1.1.0"

#if ((PHP_MAJOR_VERSION == 5) && (PHP_MINOR_VERSION == 2)) || (PHP_MAJOR_VERSION > 5)
#define Z_SET_REFCOUNT_P(pz, rc)	  (pz)->refcount = rc
#define Z_SET_REFCOUNT_PP(ppz, rc)	  Z_SET_REFCOUNT_P(*(ppz), rc)
#define Z_ADDREF_P	 ZVAL_ADDREF
#define Z_REFCOUNT_P ZVAL_REFCOUNT
#define Z_DELREF_P	 ZVAL_DELREF
#endif

#define yod_capplication_t	zval

#define YOD_ME(c, m, a, f) {m, PHP_MN(c), a, (zend_uint) (sizeof(a)/sizeof(struct _zend_arg_info)-1), f},

extern PHPAPI void php_var_dump(zval **struc, int level TSRMLS_DC);
extern PHPAPI void php_debug_zval_dump(zval **struc, int level TSRMLS_DC);

ZEND_BEGIN_MODULE_GLOBALS(yod)
  double   runtime; 
#if ((PHP_MAJOR_VERSION == 5) && (PHP_MINOR_VERSION < 4))
  uint		buf_nesting;
  void		*buffer;
	void		*owrite_handler;
#endif
ZEND_END_MODULE_GLOBALS(yod)

PHP_MINIT_FUNCTION(yod);
PHP_MSHUTDOWN_FUNCTION(yod);
PHP_RINIT_FUNCTION(yod);
PHP_RSHUTDOWN_FUNCTION(yod);
PHP_MINFO_FUNCTION(yod);

extern ZEND_DECLARE_MODULE_GLOBALS(yod);

#endif
/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */