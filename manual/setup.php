<?php
error_reporting(E_ALL);
date_default_timezone_set('Asia/Shanghai');

defined('YOD_RUNMODE') or define('YOD_RUNMODE', 3);
defined('YOD_VIEWEXT') or define('YOD_VIEWEXT', '.tpl');
defined('YOD_RUNPATH') or define('YOD_RUNPATH', dirname(__FILE__));
defined('YOD_LOGPATH') or define('YOD_LOGPATH', dirname(__FILE__));

// config
$config = array(
	// db
	'db_dsn' => array(
		'type' => 'pdo',
		'dsn'  => 'sqlite:' . YOD_RUNPATH . '/manual.sdb',
		'prefix' => 'yod_',
		'slaves' => array(
			
		),
	),

	// tpl
	'tpl_data' => array(
		'PUBLIC' => 'Public',

	),
);

// yodphp
class_exists('Yod_Application', false) or require YOD_RUNPATH . '/../master/yodphp/yodphp.php';

/**
 * SetupController
 */
class SetupController extends Yod_Controller
{
	public function indexAction()
	{
		$db = Yod_Database::db();
		$fields = array(
			'id' => 'INTEGER PRIMARY KEY AUTOINCREMENT',
			'node' => 'VARCHAR(32) NOT NULL',
			'prev' => 'VARCHAR(32)',
			'next' => 'VARCHAR(32)',
			'title' => 'VARCHAR(255)',
			'intro' => 'TEXT',
			'content' => 'TEXT',
			'UNIQUE (node)',
		);
		//$db->create($fields, 'manual');

		$m = $this->model('Manual');

		$d = include YOD_RUNPATH . '/manual.php';

		$i = 1;
		foreach ($d as $data) {
			if ($m->find('id = :id', array(':id' => $i), 'id')) {
				$m->save($data, 'id = :id', array(':id' => $i));
			} else {
				$m->save($data);
			}
			$i++;
		}
		
	}
}
