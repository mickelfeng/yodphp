<?php
/**
 * DatabaseController
 */
class DatabaseController extends Yod_Controller
{
	public function indexAction()
	{
		$this->assign('title', 'Yod_Database');

		$this->display();
	}

	public function dbAction()
	{
		$this->assign('title', 'Yod_Database::db()');

		$db = Yod_Database::db();

		$this->display('db', array('db' => $db));
	}

	public function getinstanceAction()
	{
		$this->assign('title', 'Yod_Database::getInstance()');

		$db = Yod_Database::getInstance();

		$this->display('db', array('db' => $db));
	}
	
	public function configAction()
	{
		$this->assign('title', 'Yod_Database::getInstance()');

		$db = Yod_Database::getInstance();

		$db->config('type', 'mysql');

		$this->display('config', array('db' => $db));
	}

	public function createAction()
	{
		$this->assign('title', 'Yod_Database::create()');

		$db = Yod_Database::db('db_mysql');

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
		$db->create($fields, 'test', $extend);

		$this->display('create', array('db' => $db));

		$db->execute('DROP TABLE `yod_test`');
	}

	public function insertAction()
	{
		$this->assign('title', 'Yod_Database::insert()');

		$db = Yod_Database::db('db_mysql');

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
		$db->create($fields, 'test', $extend);

		$data = array(
			'title' => 'Yodphp',
			'content' => 'Yod PHP Framework',
			'created' => time(),
		);
		$this->assign('result', $db->insert($data, 'test'));

		$this->display('insert', array('db' => $db));

		$db->execute('DROP TABLE `yod_test`');
	}

	public function updateAction()
	{
		$this->assign('title', 'Yod_Database::update()');

		$db = Yod_Database::db('db_mysql');

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
		$db->create($fields, 'test', $extend);

		$data = array(
			'title' => 'Yodphp',
			'content' => 'Yod PHP Framework',
			'created' => time(),
		);
		$db->insert($data, 'test');

		$data = array(
			'title' => 'Demo',
			'content' => base64_encode('Yod PHP Framework'),
			'updated' => time(),
		);
		$result = $db->update($data, 'test', 'id = :id', array(':id' => 1));

		$this->assign('result', $result);

		$this->display('update', array('db' => $db));

		$db->execute('DROP TABLE `yod_test`');
	}

	public function deleteAction()
	{
		$this->assign('title', 'Yod_Database::delete()');

		$db = Yod_Database::db('db_mysql');

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
		$db->create($fields, 'test', $extend);

		$data = array(
			'title' => 'Yodphp',
			'content' => 'Yod PHP Framework',
			'created' => time(),
		);
		$db->insert($data, 'test');

		$result = $db->delete('test', 'id = :id', array(':id' => 1));

		$this->assign('result', $result);

		$this->display('delete', array('db' => $db));

		$db->execute('DROP TABLE `yod_test`');
	}

	public function selectAction()
	{
		$this->assign('title', 'Yod_Database::select()');

		$db = Yod_Database::db('db_mysql');

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
		$db->create($fields, 'test', $extend);

		$data = array(
			'title' => 'Yodphp',
			'content' => 'Yod PHP Framework',
			'created' => time(),
		);
		$db->insert($data, 'test');

		$result = $db->select('*', 'test', 'id = :id', array(':id' => 1));

		$this->assign('result', $result);

		$this->display('select', array('db' => $db));

		$db->execute('DROP TABLE `yod_test`');
	}

	public function lastqueryAction()
	{
		$this->assign('title', 'Yod_Database::lastQuery()');

		$db = Yod_Database::db('db_mysql');

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
		$db->create($fields, 'test', $extend);

		$data = array(
			'title' => 'Yodphp',
			'content' => 'Yod PHP Framework',
			'created' => time(),
		);
		$db->insert($data, 'test');

		$db->select('*', 'test');

		$this->assign('lastquery', $db->lastQuery());

		$this->display('lastquery', array('db' => $db));

		$db->execute('DROP TABLE `yod_test`');
	}

}
