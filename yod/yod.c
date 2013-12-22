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
#include "Zend/zend_interfaces.h"
#include "ext/standard/php_smart_str.h"

/*
#include "Zend/zend_alloc.h"
#include "ext/standard/info.h"
#include "ext/standard/php_var.h"
#include "ext/standard/php_string.h"
#include "ext/standard/php_math.h"
*/


#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#include "php_yod.h"
#include "yod_application.h"
#include "yod_request.h"
#include "yod_controller.h"
#include "yod_action.h"
#include "yod_widget.h"
#include "yod_model.h"
#include "yod_database.h"

#define MICRO_IN_SEC 1000000.00

ZEND_DECLARE_MODULE_GLOBALS(yod);

#if PHP_YOD_DEBUG

/** {{{ void yod_debugf(const char *format,...)
*/
void yod_debugf(const char *format,...) {
	struct timeval tp = {0};
	va_list args;
	char *buffer, *buffer1;

	struct tm *ta, tmbuf;
	time_t curtime;
	char *datetime, asctimebuf[52];
	uint datetime_len;

	TSRMLS_FETCH();

	time(&curtime);
	ta = php_localtime_r(&curtime, &tmbuf);
	datetime = php_asctime_r(ta, asctimebuf);
	datetime_len = strlen(datetime);
	datetime[datetime_len - 1] = 0;

	if (!gettimeofday(&tp, NULL)) {
		va_start(args, format);
		vspprintf(&buffer1, 0, format, args);
		va_end(args);

		spprintf(&buffer, 0, "[%s %06d] (%dk) %s\n", datetime, tp.tv_usec, zend_memory_usage(1 TSRMLS_CC) / 1024, buffer1);
/*
		spprintf(&buffer, 0, "%s%s\n", buffer, YOD_DOTLINE);

		spprintf(&buffer, 0, "%s%s\n", buffer, YOD_DIVLINE);
*/
		add_next_index_string(YOD_G(debugs), buffer, 1);

		efree(buffer1);
		efree(buffer);
	}
}
/* }}} */

/** {{{ void yod_debugl(char *sline TSRMLS_DC)
*/
void yod_debugl(char *sline TSRMLS_DC) {
	char *buffer;

	if (sline) {
		switch (sline[0]) {
			case '-' :
				spprintf(&buffer, 0, "%s\n", YOD_DOTLINE);
				break;
			case '=' :
				spprintf(&buffer, 0, "%s\n", YOD_DIVLINE);
				break;
			default :
				spprintf(&buffer, 0, "%s\n", sline ? sline : YOD_DOTLINE);
		}
	} else {
		spprintf(&buffer, 0, "%s\n", YOD_DOTLINE);
	}
	add_next_index_string(YOD_G(debugs), buffer, 1);
	efree(buffer);
}
/* }}} */

/** {{{ void yod_debugz(zval *pzval, int dump TSRMLS_DC) {
*/
void yod_debugz(zval *pzval, int dump TSRMLS_DC) {
	if (dump) {
		php_var_dump(pzval, 0 TSRMLS_CC);
	} else {
		zend_print_zval_r(pzval, 0 TSRMLS_CC);
	}
}
/* }}} */

/** {{{ void yod_debugs(TSRMLS_DC)
*/
void yod_debugs(TSRMLS_DC) {
	double runtime;
	struct timeval tp = {0};
	zval **ppzval;

	if (YOD_G(exited)) {
		return;
	}
	
	if (gettimeofday(&tp, NULL)) {
		runtime	= 0;	
	} else {
		runtime	= (double)(tp.tv_sec + tp.tv_usec / MICRO_IN_SEC);
	}

	runtime = (runtime - YOD_G(runtime)) * 1000;

	if (SG(request_info).request_method) {
		php_printf("\n<pre><hr><font color=\"red\">Yod is running in debug mode</font>\n%s\n", YOD_DOTLINE);
	} else {
		php_printf("\n%s\nYod is running in debug mode:\n%s\n", YOD_DIVLINE, YOD_DOTLINE);
	}

	zend_hash_internal_pointer_reset(Z_ARRVAL_P(YOD_G(debugs)));
	while (zend_hash_get_current_data(Z_ARRVAL_P(YOD_G(debugs)), (void **) &ppzval) == SUCCESS) {
		if (Z_TYPE_PP(ppzval) == IS_STRING) {
			PHPWRITE(Z_STRVAL_PP(ppzval), Z_STRLEN_PP(ppzval));
		}
		zend_hash_move_forward(Z_ARRVAL_P(YOD_G(debugs)));
	}

	php_printf("%s\n[%fms]\n", YOD_DOTLINE, runtime);
}
/* }}} */

#endif

/** {{{ void yod_do_exit(TSRMLS_DC)
*/
void yod_do_exit(TSRMLS_DC) {

#if PHP_YOD_DEBUG
	yod_debugf("yod_do_exit()");
	yod_debugs(TSRMLS_CC);
#endif

	YOD_G(exited) = 1;
	zend_set_memory_limit(PG(memory_limit));
	zend_objects_store_mark_destructed(&EG(objects_store) TSRMLS_CC);
	zend_bailout();
}
/* }}} */

/** {{{ int yod_call_method(zval *object, char *func, int func_len, zval **result, int pcount, zval *arg1, zval *arg2, zval *arg3, zval *arg4 TSRMLS_DC)
*/
int yod_call_method(zval *object, char *func, int func_len, zval **result, int pcount, zval *arg1, zval *arg2, zval *arg3, zval *arg4 TSRMLS_DC)
{
	zval *method, *argv[4], *pzval, retval;

#if PHP_YOD_DEBUG
	if (object) {
		yod_debugf("yod_call_method(%s, %s)", Z_OBJCE_P(object)->name, func ? func : "");
	} else {
		yod_debugf("yod_call_method(%s)", func ? func : "");
	}
#endif

	MAKE_STD_ZVAL(argv[0]);
	MAKE_STD_ZVAL(argv[1]);
	MAKE_STD_ZVAL(argv[2]);
	MAKE_STD_ZVAL(argv[3]);

	if (arg1) {
		ZVAL_ZVAL(argv[0], arg1, 1, 0);
	} else {
		ZVAL_NULL(argv[0]);
	}
	if (arg2) {
		ZVAL_ZVAL(argv[1], arg2, 1, 0);
	} else {
		ZVAL_NULL(argv[1]);
	}
	if (arg3) {
		ZVAL_ZVAL(argv[2], arg3, 1, 0);
	} else {
		ZVAL_NULL(argv[2]);
	}
	if (arg4) {
		ZVAL_ZVAL(argv[3], arg4, 1, 0);
	} else {
		ZVAL_NULL(argv[3]);
	}

	MAKE_STD_ZVAL(method);
	ZVAL_STRINGL(method, func,func_len, 1);

	if (call_user_function(NULL, &object, method, &retval, pcount, argv TSRMLS_CC) == FAILURE) {
		if (result) {
			ZVAL_BOOL(*result, 0);
		}
		zval_ptr_dtor(&argv[0]);
		zval_ptr_dtor(&argv[1]);
		zval_ptr_dtor(&argv[2]);
		zval_ptr_dtor(&argv[3]);
		zval_ptr_dtor(&method);
		zval_dtor(&retval);

		php_error_docref(NULL TSRMLS_CC, E_ERROR, "Error calling %s::%s()", Z_OBJCE_P(object)->name, func);
		return 0;
	}
	
	if (result) {
		pzval = &retval;
		ZVAL_ZVAL(*result, pzval, 1, 0);
	}
	zval_ptr_dtor(&argv[0]);
	zval_ptr_dtor(&argv[1]);
	zval_ptr_dtor(&argv[2]);
	zval_ptr_dtor(&argv[3]);
	zval_ptr_dtor(&method);
	zval_dtor(&retval);

	return 1;
}
/* }}} */

/** {{{ double yod_runtime(TSRMLS_DC)
*/
double yod_runtime(TSRMLS_DC) {
	zval runtime;

	if (zend_get_constant(ZEND_STRL("YOD_RUNTIME"), &runtime TSRMLS_CC)) {
		YOD_G(runtime) = Z_DVAL(runtime);
	} else {
		zend_register_double_constant(ZEND_STRS("YOD_RUNTIME"), YOD_G(runtime), CONST_CS, 0 TSRMLS_CC);
	}

	return YOD_G(runtime);
}
/* }}} */

/** {{{ long yod_forward(TSRMLS_DC)
*/
long yod_forward(TSRMLS_DC) {
	zval forward;
	long retval;

	if (zend_get_constant(ZEND_STRL("YOD_FORWARD"), &forward TSRMLS_CC)) {
		retval = Z_LVAL(forward);
	} else {
		retval = YOD_FORWARD;
		zend_register_long_constant(ZEND_STRS("YOD_FORWARD"), retval, CONST_CS, 0 TSRMLS_CC);
	}
	
#if PHP_YOD_DEBUG
	yod_debugf("yod_forward():%d", retval);
#endif
	
	return retval;
}
/* }}} */

/** {{{ char *yod_charset(TSRMLS_DC)
*/
char *yod_charset(TSRMLS_DC) {
	zval charset;

	if (!YOD_G(charset)) {
		if (zend_get_constant(ZEND_STRL("YOD_CHARSET"), &charset TSRMLS_CC)) {
			YOD_G(charset) = Z_STRVAL(charset);
		} else {
			INIT_ZVAL(charset);
			ZVAL_STRING(&charset, YOD_CHARSET, 1);
			YOD_G(charset) = estrndup(YOD_CHARSET, strlen(YOD_CHARSET));
			zend_register_string_constant(ZEND_STRS("YOD_CHARSET"), Z_STRVAL(charset), CONST_CS, 0 TSRMLS_CC);
		}
	}

#if PHP_YOD_DEBUG
	yod_debugf("yod_charset():%s", YOD_G(charset));
#endif

	return YOD_G(charset);
}
/* }}} */

/** {{{ char *yod_pathvar(TSRMLS_DC)
*/
char *yod_pathvar(TSRMLS_DC) {
	zval pathvar;

	if (!YOD_G(pathvar)) {
		if (zend_get_constant(ZEND_STRL("YOD_PATHVAR"), &pathvar TSRMLS_CC)) {
			YOD_G(pathvar) = Z_STRVAL(pathvar);
		} else {
			INIT_ZVAL(pathvar);
			ZVAL_STRING(&pathvar, YOD_PATHVAR, 1);
			YOD_G(pathvar) = estrndup(YOD_PATHVAR, strlen(YOD_PATHVAR));
			zend_register_string_constant(ZEND_STRS("YOD_PATHVAR"), Z_STRVAL(pathvar), CONST_CS, 0 TSRMLS_CC);
		}
	}

#if PHP_YOD_DEBUG
	yod_debugf("yod_pathvar():%s", YOD_G(pathvar));
#endif

	return YOD_G(pathvar);
}
/* }}} */

/** {{{ char *yod_extpath(TSRMLS_DC)
*/
char *yod_extpath(TSRMLS_DC) {
	zval extpath;
	uint extpath_len;

	if (!YOD_G(extpath)) {
		if (zend_get_constant(ZEND_STRL("YOD_EXTPATH"), &extpath TSRMLS_CC)) {
			YOD_G(extpath) = Z_STRVAL(extpath);
		} else {
			INIT_ZVAL(extpath);
			extpath_len = strlen(SG(request_info).path_translated);
			YOD_G(extpath) = estrndup(SG(request_info).path_translated, extpath_len);
			extpath_len = php_dirname(YOD_G(extpath), extpath_len);
			ZVAL_STRINGL(&extpath, YOD_G(extpath), extpath_len, 1);
			zend_register_stringl_constant(ZEND_STRS("YOD_EXTPATH"), Z_STRVAL(extpath), extpath_len, CONST_CS, 0 TSRMLS_CC);
		}
	}
	
#if PHP_YOD_DEBUG
	yod_debugf("yod_extpath():%s", YOD_G(extpath));
#endif

	return YOD_G(extpath);
}
/* }}} */

/** {{{ char *yod_runpath(TSRMLS_DC)
*/
char *yod_runpath(TSRMLS_DC) {
	zval runpath;
	uint runpath_len;

	if (!YOD_G(runpath)) {
		if (zend_get_constant(ZEND_STRL("YOD_RUNPATH"), &runpath TSRMLS_CC)) {
			YOD_G(runpath) = Z_STRVAL(runpath);
		} else {
			INIT_ZVAL(runpath);
			runpath_len = strlen(SG(request_info).path_translated);
			YOD_G(runpath) = estrndup(SG(request_info).path_translated, runpath_len);
			runpath_len = php_dirname(YOD_G(runpath), runpath_len);
			ZVAL_STRINGL(&runpath, YOD_G(runpath), runpath_len, 1);
			zend_register_stringl_constant(ZEND_STRS("YOD_RUNPATH"), Z_STRVAL(runpath), runpath_len, CONST_CS, 0 TSRMLS_CC);
		}
	}

#if PHP_YOD_DEBUG
	yod_debugf("yod_runpath():%s", YOD_G(runpath));
#endif

	return YOD_G(runpath);
}
/* }}} */

/** {{{ int yod_include(char *filepath, zval **retval, int dtor TSRMLS_DC)
*/
int yod_include(char *filepath, zval **retval, int dtor TSRMLS_DC) {
	zend_file_handle file_handle;
	zend_op_array 	*op_array;
	char realpath[MAXPATHLEN];
	zval *pzval = NULL;

	if (!VCWD_REALPATH(filepath, realpath)) {
		return 0;
	}

#if PHP_YOD_DEBUG
	yod_debugf("yod_include(%s)", filepath);
#endif

	file_handle.filename = filepath;
	file_handle.free_filename = 0;
	file_handle.type = ZEND_HANDLE_FILENAME;
	file_handle.opened_path = NULL;
	file_handle.handle.fp = NULL;

	op_array = zend_compile_file(&file_handle, ZEND_INCLUDE TSRMLS_CC);

	if (op_array && file_handle.handle.stream.handle) {
		int dummy = 1;

		if (!file_handle.opened_path) {
			file_handle.opened_path = filepath;
		}

		zend_hash_add(&EG(included_files), file_handle.opened_path, strlen(file_handle.opened_path)+1, (void *)&dummy, sizeof(int), NULL);
	}
	zend_destroy_file_handle(&file_handle TSRMLS_CC);

	if (op_array) {
		YOD_STORE_EG_ENVIRON();

		EG(return_value_ptr_ptr) = retval ? retval : &pzval;
		EG(active_op_array) 	 = op_array;

#if ((PHP_MAJOR_VERSION == 5) && (PHP_MINOR_VERSION > 2)) || (PHP_MAJOR_VERSION > 5)
		if (!EG(active_symbol_table)) {
			zend_rebuild_symbol_table(TSRMLS_C);
		}
#endif
		zend_try {
			zend_execute(op_array TSRMLS_CC);

			destroy_op_array(op_array TSRMLS_CC);
			efree(op_array);
			
			if (!EG(exception) && dtor) {
				if (EG(return_value_ptr_ptr) && *EG(return_value_ptr_ptr)) {
					zval_ptr_dtor(EG(return_value_ptr_ptr));
				}
			}
		} zend_end_try();

		YOD_RESTORE_EG_ENVIRON();

	    return 1;
	}
	return 0;
}
/* }}} */

/** {{{ static int yod_autorun_register(TSRMLS_DC)
 * */
static int yod_autorun_register(TSRMLS_DC) {
	zval *param1, *function, *retval = NULL;
	zval **params[1] = {&param1};

	MAKE_STD_ZVAL(param1);
	array_init(param1);
	add_next_index_string(param1, YOD_APP_CNAME, 1);
	add_next_index_string(param1, "autorun", 1);

	MAKE_STD_ZVAL(function);
	ZVAL_STRING(function, "register_shutdown_function", 1);

	zend_fcall_info fci = {
		sizeof(fci),
		EG(function_table),
		function,
		NULL,
		&retval,
		1,
		(zval ***)params,
		NULL,
		1
	};

	zend_call_function(&fci, NULL TSRMLS_CC);

	zval_ptr_dtor(&function);
	zval_ptr_dtor(&param1);

	if (retval) {
		zval_ptr_dtor(&retval);
		return 1;
	}
	return 0;
}
/* }}} */

/* {{{ yod_functions[]
*/
zend_function_entry yod_functions[] = {
	{NULL, NULL, NULL}
};
/* }}} */

/** {{{ PHP_GINIT_FUNCTION
*/
PHP_GINIT_FUNCTION(yod)
{
	yod_globals->runtime	= 0;
	yod_globals->charset	= NULL;
	yod_globals->pathvar	= NULL;
	yod_globals->extpath	= NULL;
	yod_globals->runpath	= NULL;
	yod_globals->yodapp		= NULL;
	yod_globals->exited		= 0;
	yod_globals->routed		= 0;
	yod_globals->running	= 0;
	yod_globals->forward	= 0;

#if PHP_YOD_DEBUG
	yod_globals->debugs		= NULL;
#endif
}
/* }}} */

/** {{{ PHP_MINIT_FUNCTION
*/
PHP_MINIT_FUNCTION(yod)
{
	REGISTER_STRINGL_CONSTANT("YOD_VERSION", YOD_VERSION, sizeof(YOD_VERSION) - 1, CONST_PERSISTENT | CONST_CS);

	/* startup class */
	PHP_MINIT(yod_application)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(yod_request)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(yod_controller)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(yod_action)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(yod_widget)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(yod_model)(INIT_FUNC_ARGS_PASSTHRU);
//	PHP_MINIT(yod_dbmodel)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(yod_database)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(yod_dbpdo)(INIT_FUNC_ARGS_PASSTHRU);

	return SUCCESS;
}
/* }}} */

/** {{{ PHP_MSHUTDOWN_FUNCTION
*/
PHP_MSHUTDOWN_FUNCTION(yod)
{
	return SUCCESS;
}
/* }}} */

/** {{{ PHP_RINIT_FUNCTION
*/
PHP_RINIT_FUNCTION(yod)
{
	struct timeval tp = {0};

	// runtime
	if (gettimeofday(&tp, NULL)) {
		YOD_G(runtime)		= 0;	
	} else {
		YOD_G(runtime)		= (double)(tp.tv_sec + tp.tv_usec / MICRO_IN_SEC);
	}
	REGISTER_DOUBLE_CONSTANT("YOD_RUNTIME", YOD_G(runtime), CONST_CS);

	YOD_G(charset)			= NULL;
	YOD_G(pathvar)			= NULL;
	YOD_G(extpath)			= NULL;
	YOD_G(runpath)			= NULL;
	YOD_G(yodapp)			= NULL;
	YOD_G(exited)			= 0;
	YOD_G(routed)			= 0;
	YOD_G(running)			= 0;
	YOD_G(forward)			= 0;

#if PHP_YOD_DEBUG
	REGISTER_LONG_CONSTANT("YOD_RUNMODE", E_ALL, CONST_CS);
	MAKE_STD_ZVAL(YOD_G(debugs));
	array_init(YOD_G(debugs));
#endif

/*
	zval *params;
	MAKE_STD_ZVAL(params);
	array_init(params);
	add_next_index_string(params, YOD_APP_CNAME, 1);
	add_next_index_string(params, "autorun", 1);
	zend_call_method_with_1_params(NULL, NULL, NULL, "register_shutdown_function", NULL, params);
	zval_ptr_dtor(&params);
*/
	yod_autorun_register(TSRMLS_CC);

	return SUCCESS;
}
/* }}} */

/** {{{ PHP_RSHUTDOWN_FUNCTION
*/
PHP_RSHUTDOWN_FUNCTION(yod)
{
	if (YOD_G(yodapp)) {
		zval_ptr_dtor(&YOD_G(yodapp));
		YOD_G(yodapp) = NULL;

#if PHP_YOD_DEBUG
		yod_debugs(TSRMLS_CC);
#endif

	}

	if (YOD_G(charset)) {
		efree(YOD_G(charset));
		YOD_G(charset) = NULL;
	}
	
	if (YOD_G(pathvar)) {
		efree(YOD_G(pathvar));
		YOD_G(pathvar) = NULL;
	}

	if (YOD_G(extpath)) {
		efree(YOD_G(extpath));
		YOD_G(extpath) = NULL;
	}

	if (YOD_G(runpath)) {
		efree(YOD_G(runpath));
		YOD_G(runpath) = NULL;
	}

#if PHP_YOD_DEBUG
	if (YOD_G(debugs)) {
		zval_ptr_dtor(&YOD_G(debugs));
		YOD_G(debugs) = NULL;
	}
#endif

	return SUCCESS;
}
/* }}} */

/** {{{ PHP_MINFO_FUNCTION
*/
PHP_MINFO_FUNCTION(yod)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "yod support", "enabled");
	php_info_print_table_row(2, "Version", YOD_VERSION);
	//php_info_print_table_row(2, "Supports", YOD_SUPPORT_URL);
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}
/* }}} */

/** {{{ DL support
 */
#ifdef COMPILE_DL_YOD
ZEND_GET_MODULE(yod)
#endif
/* }}} */

/** {{{ module depends
 */
#if ZEND_MODULE_API_NO >= 20050922
zend_module_dep yod_deps[] = {
	ZEND_MOD_REQUIRED("spl")
	{NULL, NULL, NULL}
};
#endif
/* }}} */

/** {{{ yod_module_entry
*/
zend_module_entry yod_module_entry = {
#if ZEND_MODULE_API_NO >= 20050922
	STANDARD_MODULE_HEADER_EX, NULL,
	yod_deps,
#else
	STANDARD_MODULE_HEADER,
#endif
	"yod",
	yod_functions,
	PHP_MINIT(yod),
	PHP_MSHUTDOWN(yod),
	PHP_RINIT(yod),
	PHP_RSHUTDOWN(yod),
	PHP_MINFO(yod),
	YOD_VERSION,
	PHP_MODULE_GLOBALS(yod),
	PHP_GINIT(yod),
	NULL,
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
