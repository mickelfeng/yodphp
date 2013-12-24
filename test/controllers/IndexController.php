<?php
/**
 * IndexController
 */
class IndexController extends Yod_Controller
{
	public function indexAction()
	{
		//$model = Yod_Model::getInstance(null);

		//print_r($model);
		//$demoUser = $this->model('DemoUser');

		$this->display();
	}

	public function applicationAction()
	{
		$this->display('app');
	}

	public function widgetAction()
	{
		$this->widget('demo/test');
	}

	public function modelAction()
	{
		echo '<pre>';
		$model = $this->model();
		print_r($model);
		echo '<br>find:'; print_r($model->find());
		//echo '<br>findAll:'; print_r($model->findAll());
		//echo '<br>count:'; print_r($model->count());
		$data = array(
			'title' => 'Demo',
			'content' => base64_encode(json_encode(var_export($this, true))),
			'created' => time(),
		);
		//echo '<br>save:'; print_r($model->save($data));
		//echo '<br>save:'; print_r($model->save($data, 'id = :id', array(':id' => 1)));
		//echo '<br>remove:'; print_r($model->remove('id = :id', array(':id' => 1)));
		//echo '<br>lastQuery:'; print_r($model->lastQuery());
	}

	public function dbmAction()
	{
		/*
		echo '<pre>';
		$dbm = $this->model(true);
		$dbm->table('demo');
		//echo '<br>find:'; print_r($dbm->find());
		//echo '<br>findAll:'; print_r($dbm->findAll());
		//echo '<br>count:'; print_r($dbm->count());
		$data = array(
			'title' => 'Demo',
			'content' => base64_encode(json_encode(var_export($this, true))),
			'created' => time(),
		);
		//echo '<br>save:'; print_r($dbm->save($data));
		//echo '<br>save:'; print_r($dbm->save($data, 'id = :id', array(':id' => 1)));
		//echo '<br>remove:'; print_r($dbm->remove('id = :id', array(':id' => 1)));
		//echo '<br>lastQuery:'; print_r($dbm->lastQuery());

		$duser = $this->model('DemoUser');
		$select = array('id', 'title', 'content', 'pubtime' => 'created');
		$where = 'id = :id';
		$param1 = array(':id' => 2);
		$param2 = array(':mid' => 1);
		$table = 'demo';
		$duser->select($select)
			  ->from($table)
			//  ->join('user', '', '')
			  ->where($where, $param1)
			//  ->group('type')
			//  ->having('id > :mid', $param2)
			  ->order('id DESC')
			  ->limit('5, 3')
			//  ->union(array('SELECT' => '*'))
			  ->comment('comment');

		echo '<br>find:'; print_r($duser->find());

		echo '<br>findAll:'; print_r($duser->select($select)->from($table)->findAll());

		echo '<br>count:'; print_r($duser->select($select)->from($table)->count());

		$data = array(
			'title' => 'Demo',
			'content' => base64_encode(json_encode(var_export($this, true))),
			'created' => time(),
		);

		$duser->table($table);

		//echo '<br>save:'; print_r($duser->save($data));

		echo '<br>save:'; print_r($duser->save($data, $where, $param1));

		echo '<br>remove:'; print_r($duser->remove($where, $param1));

		//$du01 = new DemoUserModel();

		echo '<br>'; print_r($duser);
		*/
	}

	public function pdoAction()
	{
		echo '<pre>';
		//print_r(Yod_Application::app());
		$db = Yod_Database::db();
		$db->connect();
		echo '<br>db:'; print_r($db);

		//echo '<br>transaction:'; echo $db->transaction();

		$query = 'UPDATE yod_demo SET updated = '.time().' WHERE id = 2';
		//echo '<br>execute:'; echo $db->execute($query);

		$query = 'UPDATE yod_demo SET updated = 1 WHERE id = :id';
		$params = array(':id' => 2);
		//echo '<br>execute:'; echo $db->execute($query, $params);

		//echo '<br>commit:'; echo $db->commit();

		//echo '<br>transaction:'; echo $db->transaction();

		$query = 'UPDATE yod_demo SET updated = :updated WHERE id = :id';
		$params = array(':updated' => time(), ':id' => 2);
		//echo '<br>execute:'; echo $db->execute($query, $params);

		//echo '<br>rollback:'; echo $db->rollback();

		$query = 'SELECT * FROM yod_demo WHERE id = :id';
		$params = array(':id' => 2);
		//$db->query($query, $params);
		//echo '<br>fetch:'; print_r($db->fetch());
/*
		$query = 'SELECT * FROM yod_demo';
		$result = $db->query($query);
		$data = $db->fetchAll($result);
		//echo '<br>fetchAll:'; print_r($data);

		$fields = $db->fields('demo');
		//echo '<br>fields:'; print_r($fields);
*/
		//echo '<br>errno:'; echo $db->errno();
		//echo '<br>error:'; echo $db->error();
		//echo '<br>quote:'; $db->quote("'quote'");
		//echo '<br>close:'; echo $db->close();
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

		echo $db->insert($data, 'demo');
		$db1->insert($data, 'demo');

		$data = array(
			'title' => 'Demo',
			'content' => base64_encode(time()),
			'updated' => time(),
		);

		$db1->update($data, 'demo', 'id = :id', array(':id' => 1));

		$db1->delete('demo', 'id = :id', array(':id' => 1));

		$db1->select('*', 'demo', 'id = :id', array(':id' => 2));

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
		print_r($this);
	}
}
