<?php
//set_time_limit(0);
error_reporting(E_ALL);
date_default_timezone_set('Asia/Shanghai');

defined('YOD_RUNPATH') or define('YOD_RUNPATH', dirname(__FILE__) . '/app');
class_exists('Yod_Application', false) or require YOD_RUNPATH . '/../../yodphp/yodphp.php';
