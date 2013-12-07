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
	'db_dsn1' => array(
		'type' => 'mysqli',
		'dsn'  => 'mysql:host=localhost;port=3306;dbname=test',
		'host' => 'localhost',
		'user' => 'root',
		'pass' => '',
		'dbname' => 'test',
		'prefix' => 'yod_',
		'slaves' => array(

		),
	),
	'db_dsn2' => array(
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

class_exists('Yod_Application', false) or require YOD_EXTPATH . '/yodphp.php';

class DemoController extends Yod_Controller
{
	public function indexAction()
	{
		$model = $this->model('DemoUser');

		$this->display();
	}

	public function testerAction()
	{
		echo '<pre>';
		print_r($this->config('db_dsn21'));
	}

	public function widgetAction()
	{
		$this->widget('demo/test');
	}

	public function dbAction()
	{
		echo '<pre>';

		$db = Yod_Database::db();
		echo 'db:'; print_r($db);

		$db1 = Yod_Database::db('db_dsn1');
		echo 'db1:'; print_r($db1);

		$db2 = Yod_Database::db('db_dsn2');
		echo 'db2:'; print_r($db2);

		$db3 = Yod_Database::db('db_dsn2');
		echo 'db3:'; print_r($db3);

		$db4 = Yod_Database::db('db_dsn2');
		echo 'db4:'; print_r($db4);

	}

	public function errorAction()
	{
		echo '<pre>';
		print_r(Yod_Application::app());
	}
}

class DemoUserModel extends Yod_Model
{
	protected function init()
	{
		echo '<pre>';
		$m_info = $this->model('DemoInfo');
		print_r($m_info);
	}
}

class DemoInfoModel extends Yod_Model
{
	protected function init()
	{
		echo '<pre>';
		$m_text = $this->model('DemoText');
		echo $m_text->lastQuery();
		print_r($m_text);
	}
}