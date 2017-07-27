/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2016 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_xaking.h"

/* If you declare any globals in php_xaking.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(xaking)
*/

/* True global resources - no need for thread safety here */
static int le_xaking;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini 
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("xaking.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_xaking_globals, xaking_globals)
    STD_PHP_INI_ENTRY("xaking.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_xaking_globals, xaking_globals)
PHP_INI_END()

 }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_xaking_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_xaking_compiled)
{
	char *arg = NULL;
	size_t arg_len, len;
	zend_string *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	strg = strpprintf(0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "xaking", arg);

	RETURN_STR(strg);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/
/*function xaking_init_queue();*/
/*function xaking_init_threads(int $num);*/
PHP_FUNCTION(xaking_init_threads)
{
	char *arg = NULL;
	size_t arg_len, len;
	zend_string *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	strg = strpprintf(0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "xaking", arg);

	RETURN_STR(strg);
}
/*function xaking_gotask(callable $func, mixed $arg1 ...*/
PHP_FUNCTION(xaking_gotask)
{
	int i, status, argc = ZEND_NUM_ARGS();
    zval *args = NULL;
    zval retval;

#ifndef FAST_ZPP
    php_printf("use no FAST_ZPP\n");
        if (zend_parse_parameters(ZEND_NUM_ARGS(), "+", &args, &argc) == FAILURE) {
            RETURN_FALSE;
        }
#else
        ZEND_PARSE_PARAMETERS_START(1, -1)
            Z_PARAM_VARIADIC('+', args, argc)
        ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);
#endif

    if (argc < 1) {
        //WRONG_PARAM_COUNT_WITH_RETVAL(NULL);
        WRONG_PARAM_COUNT;
    }

    zval *thefunc = &(args[0]);
    php_printf("you will call: %s argc=%d is_callable=%d\n", Z_STRVAL(*thefunc), argc, zend_is_callable(thefunc, IS_CALLABLE_CHECK_NO_ACCESS, NULL));
    if (!zend_is_callable(thefunc, IS_CALLABLE_CHECK_NO_ACCESS, NULL)) {
        php_printf("the function is not callable:%s", Z_STRVAL(*thefunc));
        RETURN_FALSE;
    }

    for (i=1; i<argc; i++) {
        convert_to_string_ex(&args[i]);
        php_printf("arg[%d] type=%d len=%d val=%s \n", i, Z_TYPE(args[i]), Z_STRLEN(args[i]), Z_STRVAL(args[i]));
    }

    // call user function
    status = call_user_function_ex(EG(function_table), NULL, thefunc, &retval, argc-1, (argc>1?args+1:NULL), 0, NULL);
    if (status == SUCCESS && !Z_ISUNDEF(retval)) {
        php_printf("the function success:%d\n", status);
        //zval_ptr_dtor(value);
        //ZVAL_COPY_VALUE(value, &retval);
    } else {
        php_printf("the function failed:%d\n", status);
        //zval_ptr_dtor(value);
        //ZVAL_NULL(value);
    }
    php_printf("the call over: %s argc=%d status=%d\n", Z_STRVAL(*thefunc), argc);
    // do not free from Z_PARAM_VARIADIC or 
    //efree(args);
	//RETURN_STR(strg);
    RETURN_TRUE;
}

/* {{{ php_xaking_init_globals
 */
/* Uncomment this function if you have INI entries 
static void php_xaking_init_globals(zend_xaking_globals *xaking_globals)
{
	xaking_globals->global_value = 0;
	xaking_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(xaking)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
    //php_error(E_WARNING, "the xaking warning when minit mypid=%d", getpid());
    
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(xaking)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
    //php_error(E_WARNING, "the xaking warning when mshutdown");
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(xaking)
{
#if defined(COMPILE_DL_XAKING) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
    //php_error(E_WARNING, "the xaking warning when rinit mypid=%d", getpid());
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(xaking)
{
    //php_error(E_WARNING, "the xaking warning when rshutdown");
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(xaking)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "xaking support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ xaking_functions[]
 *
 * Every user visible function must have an entry in xaking_functions[].
 */
const zend_function_entry xaking_functions[] = {
	PHP_FE(confirm_xaking_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(xaking_init_threads,	NULL)		/* For testing, remove later. */
	PHP_FE(xaking_gotask,	NULL)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in xaking_functions[] */
};
/* }}} */

/* {{{ xaking_module_entry
 */
zend_module_entry xaking_module_entry = {
	STANDARD_MODULE_HEADER,
	"xaking",
	xaking_functions,
	PHP_MINIT(xaking),
	PHP_MSHUTDOWN(xaking),
	PHP_RINIT(xaking),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(xaking),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(xaking),
	PHP_XAKING_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_XAKING
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(xaking)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
