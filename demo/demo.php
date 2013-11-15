<?php
//set_time_limit(0);
error_reporting(E_ALL);
date_default_timezone_set('Asia/Shanghai');

define('YOD_PATHVAR', 'r');
define('YOD_RUNPATH', dirname(__FILE__) . '/app');
class_exists('Yod_Application', false) or require YOD_RUNPATH . '/../../yodphp/yodphp.php';

class DemoController extends Yod_Controller
{
	public function indexAction()
	{
		echo '<pre>';
		print_r($this);
	}

	public function testerAction()
	{
		echo '<pre>';
		print_r($_GET);
	}

	public function errorAction()
	{
		echo '<pre>';
		print_r(Yod_Application::app());
	}
}
