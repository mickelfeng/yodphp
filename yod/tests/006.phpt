--TEST--
Check for yod model
--SKIPIF--
<?php if (!extension_loaded("yod") || defined('YOD_RUNMODE')) print "skip"; ?>
--FILE--
<?php
error_reporting(E_ALL);
date_default_timezone_set('Asia/Shanghai');

define('YOD_RUNPATH', dirname(__FILE__));

$config = array(
	//db
	'db_dsn' => array(
		'type' => 'pdo',
		'dsn' => 'mysql:host=localhost;port=3306;dbname=test',
		'host' => 'localhost',
		'user' => 'root',
		'pass' => '123456',
		'dbname' => 'test',
		'prefix' => 'yod_',
		'slaves' => array(

		),
	),
	//tpl
	'tpl_data' => array(
		'_PUBLIC_' => '/Public/',
	),
);

class IndexController extends Yod_Controller
{
	public function indexAction()
	{
		$tests = $this->model('Tests');
		$tests->yodphp();

		$hello = $this->model('Hello');
		$hello->show();
	}
}

class HelloModel extends Yod_DbModel
{
	public function show()
	{
		echo 'Hello World!';
	}
}
?>
--EXPECTF--
Yodphp Hello World!
