--TEST--
Check for yod database
--SKIPIF--
<?php
if (!extension_loaded("yod") || defined('YOD_RUNMODE') || !class_exists('PDO', false)) {
	print "skip";
} else {
	try {
		$config = include dirname(__FILE__) . '/configs/db_dsn.config.php';
		new PDO($config['dsn'], $config['user'], $config['pass']);
	} catch (Exception $e) {
		print "skip";
	}
}
?>
--FILE--
<?php
error_reporting(E_ALL);
date_default_timezone_set('Asia/Shanghai');

define('TESTS_PATH', dirname(__FILE__));
include TESTS_PATH . '/clean.php';

is_dir(TESTS_PATH . '/configs') or mkdir(TESTS_PATH . '/configs');
file_put_contents(TESTS_PATH . '/configs/db_dsn.config.php', <<<PHP
<?php
return array(
	'type' => 'pdo',
	'dsn' => 'mysql:host=localhost;port=3306;dbname=test',
	'host' => 'localhost',
	'user' => 'root',
	'pass' => '123456',
	'dbname' => 'test',
	'prefix' => 'yod_',
	'slaves' => array(
		
	),
);

PHP
);

file_put_contents(TESTS_PATH . '/configs/tpl_data.config.php', <<<PHP
<?php
return array(
	'_PUBLIC_' => '/Public/',
);

PHP
);

define('YOD_RUNPATH', dirname(__FILE__));

class IndexController extends Yod_Controller
{
	public function indexAction()
	{
		$db = Yod_Database::db();

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
		$create = $db->create($fields, 'tests', $extend);
		echo "create:"; var_dump($create);

		$tests = $this->model('Tests');

		$data = array(
			'title' => 'Tests',
			'content' => 'Yod PHP Framework',
			'created' => 1234567890,
		);
		$save = $tests->save($data);
		echo "save:"; var_dump($save);

		$demo = $this->model('Demo');

		$data['updated'] = 1234567891;
		$save = $demo->table('tests')->where('id = :id', array(':id' => 1))->save($data);
		echo "save:"; var_dump($save);
/*
		$data = $demo->from('tests')->where('id = :id', array(':id' => 1))->find();
		echo "find:"; print_r($data);

		$data = $demo->from('tests')->where('id = :id', array(':id' => 1))->findAll();
		echo "findAll:"; print_r($data);

		echo "remove:" . $demo->table('tests')->where('id = :id', array(':id' => 1))->remove();

		$find = $demo->from('tests')->where('id = :id', array(':id' => 1))->find();
		echo "\nfind:"; var_dump($find);
*/
		$execute = $db->execute('DROP TABLE yod_tests');
		echo "execute:"; var_dump($execute);

	}
}

class DemoModel extends Yod_DbModel
{

}
?>
--EXPECTF--
111Array
(
    [id] => 1
    [title] => Tests
    [content] => Yod PHP Framework
    [updated] => 1234567891
    [created] => 1234567890
    [status] => 0
)
1bool(false)
1
