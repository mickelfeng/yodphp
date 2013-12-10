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
		'pass' => '123456',
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
		'pass' => '123456',
		'dbname' => 'test',
		'prefix' => 'yod_',
		'slaves' => array(
			'dsn' => 'mysql:host=192.168.8.1;port=3306;dbname=test',
			'host' => '192.168.8.1',
			array(
				'host' => '192.168.8.1',
			),
			array(
				'host' => '192.168.8.2',
			),
		),
	),
	'db_dsn2' => array(
		'dsn'  => 'mysql:host=localhost;port=3306;dbname=test',
		'host' => 'localhost',
		'user' => 'root',
		'pass' => '123456',
		'dbname' => 'test',
		'prefix' => 'yod_',
		'slaves' => array(

		),
	),
);

class_exists('Yod_Application', false) or require dirname(__FILE__) . '/yodphp.php';

class DemoController extends Yod_Controller
{
	public function indexAction()
	{
		echo '<pre>';
		$model = Yod_Model::getInstance(null);

		print_r($model);
		//$demoUser = $this->model('DemoUser');

		//$this->display();
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

	public function pdoAction()
	{
		echo '<pre>';
		//print_r(Yod_Application::app());
		$db = Yod_Database::db();
		//$db->connect();
		echo '<br>db:'; print_r($db);
/*
		echo '<br>transaction:'; echo $db->transaction();
		$query = 'UPDATE yod_demo SET updated = '.time().' WHERE id = 2';
		echo '<br>execute:'; echo $db->execute($query);
		$query = 'UPDATE yod_demo SET updated = 1 WHERE id = :id';
		$params = array(':id' => 2);
		echo '<br>execute:'; echo $db->execute($query, $params);
		echo '<br>commit:'; echo $db->commit();

		echo '<br>transaction:'; echo $db->transaction();
		$query = 'UPDATE yod_demo SET updated = :updated WHERE id = :id';
		$params = array(':updated' => time(), ':id' => 2);
		echo '<br>execute:'; echo $db->execute($query, $params);
		echo '<br>rollback:'; echo $db->rollback();

		$query = 'SELECT * FROM yod_demo WHERE id = :id';
		$params = array(':id' => 2);
		$db->query($query, $params);
		echo '<br>fetch:'; print_r($db->fetch());

		$query = 'SELECT * FROM yod_demo';
		$result = $db->query($query);
		$data = $db->fetchAll($result);
		//echo '<br>fetchAll:'; print_r($data);

		$fields = $db->fields('demo');
		//echo '<br>fields:'; print_r($fields);
*/
		echo '<br>errno:'; echo $db->errno();
		echo '<br>error:'; echo $db->error();
		echo '<br>quote:'; $db->quote("'quote'");
		echo '<br>close:'; echo $db->close();
	}

	public function dbAction()
	{
		echo '<pre>';

		$db = Yod_Database::db();
		echo '<br>db:'; print_r($db);

		$db1 = Yod_Database::db('db_dsn1');
		echo '<br>db1:'; print_r($db1);
		$db1->config('slaves', array('host' => '192.168.8.1'), array('host' => '192.168.8.2'), array('host' => '192.168.8.3'));
		$db1->config('slaves', null);

		$fields = array(
			'id' => 'int(11) NOT NULL AUTO_INCREMENT COMMENT \'ID\'',
			'title' => 'varchar(255) NOT NULL COMMENT \'标题\'',
			'content' => 'text DEFAULT NULL COMMENT \'内容\'',
			'updated' => 'int(11) NOT NULL DEFAULT \'0\' COMMENT \'更新时间\'',
			'created' => 'int(11) NOT NULL DEFAULT \'0\' COMMENT \'创建时间\'',
			'status' => 'tinyint(2) NOT NULL DEFAULT \'0\' COMMENT \'状态\'',
			'PRIMARY' => 'KEY (`id`)',
		);
		$extend = 'ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT=\'Demo\' AUTO_INCREMENT=1';

		//$db1->create($fields, 'demo', $extend);

		$data = array(
			'title' => 'Demo',
			'content' => base64_encode(json_encode(var_export($this, true))),
			'created' => time(),
		);

		//echo $db->insert($data, 'demo');
		//$db1->insert($data, 'demo');

		$data = array(
			'title' => 'Demo',
			'content' => base64_encode(time()),
			'updated' => time(),
		);

		//$db1->update($data, 'demo', 'id = :id', array(':id' => 1));

		//$db1->delete('demo', 'id = :id', array(':id' => 1));

		//$db1->select('*', 'demo', 'id = :id', array(':id' => 2));

		echo '<br>fetchAll:'; print_r($db1->fetchAll());
		echo '<br>db1:'; print_r($db1);

		$db2 = Yod_Database::db('db_dsn2');
		echo '<br>db2:'; print_r($db2);

		$db3 = Yod_Database::db('db_dsn2');
		echo '<br>db3:'; print_r($db3);

		$db4 = Yod_Database::db('db_dsn2');
		echo '<br>db4:'; print_r($db4);

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