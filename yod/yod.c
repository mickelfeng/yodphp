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

/*
#include "Zend/zend_alloc.h"
#include "Zend/zend_interfaces.h"
#include "ext/standard/info.h"
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
	char *buffer;

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
		vspprintf(&buffer, 0, format, args);
		va_end(args);

		spprintf(&buffer, 0, "[%s %06d] (%dk) %s\n", datetime, tp.tv_usec, zend_memory_usage(1 TSRMLS_CC) / 1024, buffer);
/*
		spprintf(&buffer, 0, "%s%s\n", buffer, YOD_DOTLINE);

		spprintf(&buffer, 0, "%s%s\n", buffer, YOD_DIVLINE);
*/
		add_next_index_string(YOD_G(debugs), buffer, 1);
		efree(buffer);
	}
}
/* }}} */

/** {{{ void yod_debugl(char *sline)
*/
void yod_debugl(char *sline) {
	char *buffer;

	spprintf(&buffer, 0, "%s\n", sline ? sline : YOD_DOTLINE);
	add_next_index_string(YOD_G(debugs), buffer, 1);
	efree(buffer);
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
	double retval;

	if (zend_get_constant(ZEND_STRL("YOD_RUNTIME"), &runtime TSRMLS_CC)) {
		retval = Z_DVAL(runtime);
	} else {
		retval = YOD_G(runtime);
		zend_register_double_constant(ZEND_STRS("YOD_RUNTIME"), retval, CONST_CS, 0 TSRMLS_CC);
	}

	return retval;
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
	char *retval;

	if (zend_get_constant(ZEND_STRL("YOD_CHARSET"), &charset TSRMLS_CC)) {
		retval = Z_STRVAL(charset);
	} else {
		retval = estrndup(YOD_CHARSET, strlen(YOD_CHARSET));
		zend_register_string_constant(ZEND_STRS("YOD_CHARSET"), retval, CONST_CS, 0 TSRMLS_CC);
	}

#if PHP_YOD_DEBUG
	yod_debugf("yod_charset():%s", retval);
#endif

	return retval;
}
/* }}} */

/** {{{ char *yod_pathvar(TSRMLS_DC)
*/
char *yod_pathvar(TSRMLS_DC) {
	zval pathvar;
	char *retval;

	if (zend_get_constant(ZEND_STRL("YOD_PATHVAR"), &pathvar TSRMLS_CC)) {
		retval = Z_STRVAL(pathvar);
	} else {
		retval = estrndup(YOD_PATHVAR, strlen(YOD_PATHVAR));
		zend_register_string_constant(ZEND_STRS("YOD_PATHVAR"), retval, CONST_CS, 0 TSRMLS_CC);
	}

#if PHP_YOD_DEBUG
	yod_debugf("yod_pathvar():%s", retval);
#endif

	return retval;
}
/* }}} */

/** {{{ char *yod_extpath(TSRMLS_DC)
*/
char *yod_extpath(TSRMLS_DC) {
	zval extpath;
	char *retval;
	size_t retval_len;

	if (zend_get_constant(ZEND_STRL("YOD_EXTPATH"), &extpath TSRMLS_CC)) {
		retval = Z_STRVAL(extpath);
	} else {
		retval_len = strlen(SG(request_info).path_translated);
		retval = estrndup(SG(request_info).path_translated, retval_len);
		retval_len = php_dirname(retval, retval_len);
		zend_register_stringl_constant(ZEND_STRS("YOD_EXTPATH"), retval, retval_len, CONST_CS, 0 TSRMLS_CC);
	}
	
#if PHP_YOD_DEBUG
	yod_debugf("yod_extpath():%s", retval);
#endif

	return retval;
}
/* }}} */

/** {{{ char *yod_runpath(TSRMLS_DC)
*/
char *yod_runpath(TSRMLS_DC) {
	zval runpath;
	char *retval;
	size_t retval_len;

	if (zend_get_constant(ZEND_STRL("YOD_RUNPATH"), &runpath TSRMLS_CC)) {
		retval = Z_STRVAL(runpath);
	} else {
		retval_len = strlen(SG(request_info).path_translated);
		retval = estrndup(SG(request_info).path_translated, retval_len);
		retval_len = php_dirname(retval, retval_len);
		zend_register_stringl_constant(ZEND_STRS("YOD_RUNPATH"), retval, retval_len, CONST_CS, 0 TSRMLS_CC);
	}

#if PHP_YOD_DEBUG
	yod_debugf("yod_runpath():%s", retval);
#endif

	return retval;
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

		EG(return_value_ptr_ptr) = retval;
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

/** {{{ static int yod_autoload_register(TSRMLS_DC)
 * */
static int yod_autoload_register(TSRMLS_DC) {
	int result = 1;
	zval *autoload, *function, *retval = NULL;
	zval **params[1] = {&autoload};

	MAKE_STD_ZVAL(autoload);
	ZVAL_STRING(autoload, "yod_autoload", 1);

	MAKE_STD_ZVAL(function);
	ZVAL_STRING(function, "spl_autoload_register", 0);

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

	if (zend_call_function(&fci, NULL TSRMLS_CC) == FAILURE) {
		result = 0;
	}

	if (retval) {
		zval_ptr_dtor(&retval);
	}
	efree(function);
	zval_ptr_dtor(&autoload);

	return result;
}
/* }}} */

/** {{{ static int yod_autoload(char *classname, uint classname_len TSRMLS_DC)
*/
static int yod_autoload(char *classname, uint classname_len TSRMLS_DC) {
	zend_class_entry **pce = NULL;
	char *classfile, *classpath;
	zval *retval;

	if (!YOD_G(yodapp)) return;

	classfile = estrndup(classname, classname_len);
	// class name with namespace in PHP 5.3
	if (strstr(classname, "\\")) {
		while (*classfile != '\0') {
			if (*classfile == '\\') {
				*classfile = '_';
			}
			classfile++;
		}
		classfile = classfile - classname_len + 1;
	}

	if (strncmp(classfile, "Yod_", 4) == 0) { // yodphp extends class
		if (strncmp(classfile, "Yod_Db", 6) == 0) {
			spprintf(&classpath, 0, "%s/drivers/%s.class.php", yod_extpath(TSRMLS_CC), classfile + 4);
		} else {
			spprintf(&classpath, 0, "%s/extends/%s.class.php", yod_extpath(TSRMLS_CC), classfile + 4);
		}
	} else {
		if (classname_len > 5 && strncmp(classfile + classname_len - 5, "Model", 5) == 0) {
			spprintf(&classpath, 0, "%s/models/%s.php", yod_runpath(TSRMLS_CC), classfile);
		} else {
			spprintf(&classpath, 0, "%s/extends/%s.php", yod_runpath(TSRMLS_CC), classfile);
		}
	}
	efree(classfile);

	if (VCWD_ACCESS(classpath, F_OK) == 0) {
		yod_include(classpath, &retval, 1 TSRMLS_CC);
	}
	
#if PHP_YOD_DEBUG
	yod_debugf("yod_autoload(%s):%s", classname, classpath);
#endif

	efree(classpath);

#if PHP_API_VERSION < 20100412
	if (zend_lookup_class_ex(classname, classname_len, 0, &pce TSRMLS_CC) == SUCCESS) {
#else
	if (zend_lookup_class_ex(classname, classname_len, NULL, 0, &pce TSRMLS_CC) == SUCCESS) {
#endif
		return SUCCESS;
	}

	return FAILURE;

}
/* }}} */

/* {{{ ZEND_FUNCTION
*/
ZEND_FUNCTION(yod_autoload)
{
	char *classname;
	uint classname_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &classname, &classname_len) == FAILURE) {
		return;
	}

	if (yod_autoload(classname, classname_len) == FAILURE) {
		RETURN_FALSE;
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ yod_functions[]
*/
zend_function_entry yod_functions[] = {
	ZEND_FE(yod_autoload, NULL)
	{NULL, NULL, NULL}
};
/* }}} */

/** {{{ PHP_GINIT_FUNCTION
*/
PHP_GINIT_FUNCTION(yod)
{
	yod_globals->runtime	= 0;
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

	if (gettimeofday(&tp, NULL)) {
		YOD_G(runtime)		= 0;	
	} else {
		YOD_G(runtime)		= (double)(tp.tv_sec + tp.tv_usec / MICRO_IN_SEC);
	}
	REGISTER_DOUBLE_CONSTANT("YOD_RUNTIME", YOD_G(runtime), CONST_CS);

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

	// spl_autoload_register
/*
	zval *autoload;
	MAKE_STD_ZVAL(autoload);
	ZVAL_STRING(autoload, "yod_autoload", 1);
	zend_call_method_with_1_params(NULL, NULL, NULL, "spl_autoload_register", NULL, autoload);
*/
	yod_autoload_register(TSRMLS_CC);

	return SUCCESS;
}
/* }}} */

/** {{{ PHP_RSHUTDOWN_FUNCTION
*/
PHP_RSHUTDOWN_FUNCTION(yod)
{
	zval runpath;

	if (zend_get_constant(ZEND_STRL("YOD_RUNPATH"), &runpath TSRMLS_CC)) {
		if (!YOD_G(yodapp) && !YOD_G(exited)) {
			SG(headers_sent) = 0;

			yod_application_app(NULL);
			yod_application_run(TSRMLS_CC);

			if (!SG(headers_sent)) {
				sapi_send_headers(TSRMLS_C);
			}
		}
	}

	if (YOD_G(yodapp)) {
		zval_ptr_dtor(&YOD_G(yodapp));
		YOD_G(yodapp) = NULL;

#if PHP_YOD_DEBUG
		yod_debugs(TSRMLS_CC);
#endif

	}

#if PHP_YOD_DEBUG
	if (YOD_G(debugs)) {
		zval_ptr_dtor(&YOD_G(debugs));
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
