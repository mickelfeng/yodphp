<?php
//set_time_limit(0);
error_reporting(E_ALL);
date_default_timezone_set('Asia/Shanghai');

defined('YOD_PATHVAR') or define('YOD_PATHVAR', 'r');
defined('YOD_RUNPATH') or define('YOD_RUNPATH', dirname(__FILE__) . '/testapp');
defined('YOD_EXTPATH') or define('YOD_EXTPATH', dirname(__FILE__) . '/../yodphp');
defined('YOD_LOGFILE') or define('YOD_LOGFILE', dirname(__FILE__) . '/debugs/' . date('YmdHis') . '.log');

defined('APP_WEBROOT') or define('APP_WEBROOT', '/index.php');

class_exists('Yod_Application', false) or require dirname(__FILE__) . '/../yodphp/yodphp.php';
