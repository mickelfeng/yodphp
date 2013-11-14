<?php
//set_time_limit(0);
error_reporting(E_ALL);
ini_set('display_errors', 'On');
date_default_timezone_set('Asia/Shanghai');

define('APP_PATH', dirname(__FILE__) . '/app');
require APP_PATH . '/../../yodphp/yodphp.php';

$yodapp = new Yod_Application();
$yodapp->run();
