<?php
//set_time_limit(0);
error_reporting(E_ALL);
date_default_timezone_set('Asia/Shanghai');

define('YOD_PATHVAR', 'r');
define('YOD_RUNPATH', dirname(__FILE__) . '/app');

// config
$config = array(
	'db_dsn' => array(
		'type' => 'pdo',
		'dsn'  => 'mysql:host=localhost;port=3306;dbname=test',
		'host' => 'localhost',
		'user' => 'root',
		'pass' => '',
		'dbname' => 'test',
		'prefix' => 'yod_',
		'slaves' => array(

		),
	),
);

class_exists('Yod_Application', false) or require YOD_RUNPATH . '/../../yodphp/yodphp.php';

class DemoController extends Yod_Controller
{
	public function indexAction()
	{
		$model = Yod_Model::getInstance();
		$demo = $this->model('demo');

		echo '<pre>';
		print_r($model);

		$this->display();
	}

	public function testerAction()
	{
		echo '<pre>';
		print_r($_GET);
	}

	public function widgetAction()
	{
		$this->widget('demo/test');
	}

	public function errorAction()
	{
		echo '<pre>';
		print_r(Yod_Application::app());
	}
}

class DemoModel extends Yod_Model
{

}