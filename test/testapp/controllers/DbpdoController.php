<?php
/**
 * DbPdoController
 */
class DbPdoController extends Yod_Controller
{
	public function indexAction()
	{
		$this->assign('title', 'Yod_DbPdo');

		$this->display();
	}

	public function connectAction()
	{
		$this->assign('title', 'Yod_DbPdo::connect()');

		$db = Yod_Database::db();

		$this->assign('result', $db->connect());

		$this->display('connect', array('db' => $db));
	}
	
	public function fieldsAction()
	{
		$this->assign('title', 'Yod_DbPdo::fields()');

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
		$db->create($fields, 'test', $extend);

		$this->assign('fields', $db->fields('test'));

		$this->display('fields', array('db' => $db));

		$db->execute('DROP TABLE `yod_test`');
	}

	public function executeAction()
	{
		$this->assign('title', 'Yod_DbPdo::execute()');

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
		$db->create($fields, 'test', $extend);

		$data = array(
			'title' => 'Yodphp',
			'content' => 'Yod PHP Framework',
			'created' => time(),
		);
		$db->insert($data, 'test');

		$query = 'UPDATE yod_test SET updated = :updated WHERE id = :id';
		$params = array(
			':updated' => time(),
			':id' => 1,
		);
		$this->assign('result', $db->execute($query, $params));

		$this->display('execute', array('db' => $db));

		$db->execute('DROP TABLE `yod_test`');
	}

	public function queryAction()
	{
		$this->assign('title', 'Yod_DbPdo::query()');

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
		$db->create($fields, 'test', $extend);

		$data = array(
			'title' => 'Yodphp',
			'content' => 'Yod PHP Framework',
			'created' => time(),
		);
		$db->insert($data, 'test');

		$query = 'SELECT * FROM yod_test WHERE id = :id';
		$params = array(
			':id' => 1,
		);
		$this->assign('result', $db->query($query, $params));

		$this->display('query', array('db' => $db));

		$db->execute('DROP TABLE `yod_test`');
	}

	public function fetchAction()
	{
		$this->assign('title', 'Yod_DbPdo::fetch()');

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
		$db->create($fields, 'test', $extend);

		$data = array(
			'title' => 'Yodphp',
			'content' => 'Yod PHP Framework',
			'created' => time(),
		);
		$db->insert($data, 'test');

		$query = 'SELECT * FROM yod_test WHERE id = :id';
		$params = array(
			':id' => 1,
		);
		$db->query($query, $params);

		$this->assign('data', $db->fetch());

		$this->display('fetch', array('db' => $db));

		$db->execute('DROP TABLE `yod_test`');
	}

	public function fetchallAction()
	{
		$this->assign('title', 'Yod_DbPdo::fetchAll()');

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
		$db->create($fields, 'test', $extend);

		$data = array(
			'title' => 'Yodphp',
			'content' => 'Yod PHP Framework',
			'created' => time(),
		);
		$db->insert($data, 'test');

		$query = 'SELECT * FROM yod_test';
		$db->query($query);

		$this->assign('data', $db->fetchAll());

		$this->display('fetchall', array('db' => $db));

		$db->execute('DROP TABLE `yod_test`');
	}

	public function transactionAction()
	{
		$this->assign('title', 'Yod_DbPdo::transaction()');

		$db = Yod_Database::db();

		$this->assign('result', $db->transaction());

		$this->display('transaction', array('db' => $db));
	}

	public function commitAction()
	{
		$this->assign('title', 'Yod_DbPdo::commit()');

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
		$db->create($fields, 'test', $extend);

		$db->transaction();

		$data = array(
			'title' => 'Yodphp',
			'content' => 'Yod PHP Framework',
			'created' => time(),
		);
		$db->insert($data, 'test');

		$this->assign('result', $db->commit());

		$query = 'SELECT * FROM yod_test';
		$db->query($query);

		$this->assign('data', $db->fetchAll());

		$this->display('commit', array('db' => $db));

		$db->execute('DROP TABLE `yod_test`');
	}

	public function rollbackAction()
	{
		$this->assign('title', 'Yod_DbPdo::rollback()');

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
		$db->create($fields, 'test', $extend);

		$db->transaction();

		$data = array(
			'title' => 'Yodphp',
			'content' => 'Yod PHP Framework',
			'created' => time(),
		);
		$db->insert($data, 'test');

		$this->assign('result', $db->rollback());

		$query = 'SELECT * FROM yod_test';
		$db->query($query);

		$this->assign('data', $db->fetchAll());

		$this->display('rollback', array('db' => $db));

		$db->execute('DROP TABLE `yod_test`');
	}

	public function insertidAction()
	{
		$this->assign('title', 'Yod_DbPdo::insertid()');

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
		$db->create($fields, 'test', $extend);

		$db->transaction();

		$data = array(
			'title' => 'Yodphp',
			'content' => 'Yod PHP Framework',
			'created' => time(),
		);
		$db->insert($data, 'test');

		$this->assign('insertid', $db->insertid());

		$query = 'SELECT * FROM yod_test';
		$db->query($query);

		$this->assign('data', $db->fetchAll());

		$this->display('insertid', array('db' => $db));

		$db->execute('DROP TABLE `yod_test`');
	}

	public function quoteAction()
	{
		$this->assign('title', 'Yod_DbPdo::quote()');

		$db = Yod_Database::db();

		$db->connect();

		$this->assign('quote', $db->quote('"quote"'));

		$this->display('quote', array('db' => $db));
	}

	public function freeAction()
	{
		$this->assign('title', 'Yod_DbPdo::free()');

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
		$db->create($fields, 'test', $extend);

		$data = array(
			'title' => 'Yodphp',
			'content' => 'Yod PHP Framework',
			'created' => time(),
		);
		$db->insert($data, 'test');

		$query = 'SELECT * FROM yod_test WHERE id = :id';
		$params = array(
			':id' => 1,
		);
		$this->assign('result', $db->query($query, $params));

		$db->free();

		$this->display('free', array('db' => $db));

		$db->execute('DROP TABLE `yod_test`');
	}

	public function closeAction()
	{
		$this->assign('title', 'Yod_DbPdo::close()');

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
		$db->create($fields, 'test', $extend);

		$data = array(
			'title' => 'Yodphp',
			'content' => 'Yod PHP Framework',
			'created' => time(),
		);
		$db->insert($data, 'test');

		$query = 'SELECT * FROM yod_test WHERE id = :id';
		$params = array(
			':id' => 1,
		);
		$db->query($query, $params);

		$db->close();

		$this->display('close', array('db' => $db));

		$db->execute('DROP TABLE `yod_test`');
	}

	public function errnoAction()
	{
		$this->forward('error');
	}

	public function errorAction()
	{
		error_reporting(0);

		$this->assign('title', 'Yod_DbPdo::error()');

		if ($this->_request->action == 'errno') {
			$this->assign('title', 'Yod_DbPdo::errno()');
		}

		$db = Yod_Database::db();
		
		$query = 'SELECT * FROM yod_test1 WHERE id = :id';
		$params = array(
			':id' => 1,
		);
		$db->query($query, $params);

		$this->assign('errno', $db->errno());

		$this->assign('error', $db->error());

		$this->display('error', array('db' => $db));
	}
}
