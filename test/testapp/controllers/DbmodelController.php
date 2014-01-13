<?php
/**
 * DbModelController
 */
class DbModelController extends Yod_Controller
{
	public function indexAction()
	{
		$this->assign('title', 'Yod_DbModel');

		$this->display();
	}

	public function getinstanceAction()
	{
		$this->assign('title', 'Yod_DbModel::getInstance()');

		$model = Yod_DbModel::getInstance();

		$model = Yod_DbModel::getInstance('DbTest');

		$this->display('getinstance', array('model' => $model));
	}

	public function tableAction()
	{
		$this->assign('title', 'Yod_DbModel::table()');

		$model = $this->dbmodel('test');

		$model->table('test1');

		$this->display('table', array('model' => $model));
	}

	public function findAction()
	{
		$this->assign('title', 'Yod_DbModel::find()');

		$this->model('Test');

		$model = $this->dbmodel('Test');

		$this->assign('data', $model->find());

		$this->display('find', array('model' => $model));
	}

	public function findallAction()
	{
		$this->assign('title', 'Yod_DbModel::findAll()');

		$this->model('Test');

		$model = $this->dbmodel('Test');

		$this->assign('data', $model->findAll());

		$this->display('findall', array('model' => $model));
	}

	public function selectAction()
	{
		$this->assign('title', 'Yod_DbModel::select()');

		$this->model('Test');

		$model = $this->dbmodel('Test');

		$this->assign('data', $model->select());

		$this->display('select', array('model' => $model));
	}
	
	public function countAction()
	{
		$this->assign('title', 'Yod_DbModel::count()');

		$this->model('Test');

		$model = $this->dbmodel('Test');

		$this->assign('count', $model->count());

		$this->display('count', array('model' => $model));
	}

	public function saveAction()
	{
		$this->assign('title', 'Yod_DbModel::save()');

		$this->model('Test');

		$model = $this->dbmodel('Test');

		$data = array(
			'title' => 'Yodtest',
			'content' => 'Yod PHP Framework!',
			'updated' => time(),
		);
		$this->assign('resilt', $model->save($data, 'id = :id', array(':id' => 1)));

		$this->assign('data', $model->findAll());

		$this->display('save', array('model' => $model));
	}

	public function removeAction()
	{
		$this->assign('title', 'Yod_DbModel::remove()');

		$this->model('Test');
		
		$model = $this->dbmodel('Test');

		$this->assign('resilt', $model->remove('id = :id', array(':id' => 1)));

		$this->assign('data', $model->findAll());

		$this->display('remove', array('model' => $model));
	}

	public function fieldAction()
	{
		$this->assign('title', 'Yod_DbModel::field()');

		$model = $this->dbmodel('Test');

		$model->field(array('id', 'title', 'content', 'updated', 'created'));

		$this->display('field', array('model' => $model));
	}
	
	public function fromAction()
	{
		$this->assign('title', 'Yod_DbModel::from()');

		$model = $this->dbmodel('Test');

		$model->from('test1');

		$this->display('from', array('model' => $model));
	}

	public function joinAction()
	{
		$this->assign('title', 'Yod_DbModel::join()');

		$model = $this->dbmodel('Test');

		$model->join('test1', 't1.id = t2.tid');

		$this->display('join', array('model' => $model));
	}

	public function whereAction()
	{
		$this->assign('title', 'Yod_DbModel::where()');

		$model = $this->dbmodel('Test');

		$model->where('t1.id = :id', array(':id' => 1));

		$this->display('where', array('model' => $model));
	}

	public function groupAction()
	{
		$this->assign('title', 'Yod_DbModel::where()');

		$model = $this->dbmodel('Test');

		$model->group('t1.type');

		$this->display('group', array('model' => $model));
	}

	public function havingAction()
	{
		$this->assign('title', 'Yod_DbModel::having()');

		$model = $this->dbmodel('Test');

		$model->having('t1.id > :hid', array(':hid' => 1));

		$this->display('having', array('model' => $model));
	}

	public function orderAction()
	{
		$this->assign('title', 'Yod_DbModel::order()');

		$model = $this->dbmodel('Test');

		$model->order('t1.id DESC');

		$this->display('order', array('model' => $model));
	}

	public function limitAction()
	{
		$this->assign('title', 'Yod_DbModel::limit()');

		$model = $this->dbmodel('Test');

		$model->limit('0, 10');

		$this->display('limit', array('model' => $model));
	}

	public function unionAction()
	{
		$this->assign('title', 'Yod_DbModel::union()');

		$model = $this->dbmodel('Test');

		$query = array(
			'SELECT' => '*',
			'FROM' => 'yod_test1',
			'WHERE' => 'type = :type',
			'LIMIT' => 10,
		);
		$model->union($query, array(':type' => 1));

		$this->display('union', array('model' => $model));
	}

	public function commentAction()
	{
		$this->assign('title', 'Yod_DbModel::comment()');

		$model = $this->dbmodel('Test');

		$model->comment('It\'s just a test!');

		$this->display('comment', array('model' => $model));
	}

	public function paramsAction()
	{
		$this->assign('title', 'Yod_DbModel::params()');

		$model = $this->dbmodel('Test');

		$model->params(array(':id' => 1));

		$this->display('params', array('model' => $model));
	}

	public function parsequeryAction()
	{
		$this->assign('title', 'Yod_DbModel::parseQuery()');

		$model = $this->dbmodel('Test');

		$model->select(array('id', 'title', 'content', 'updated', 'created'));

		$model->where('t1.id = :id', array(':id' => 1));

		$model->order('t1.id DESC');

		$model->limit(1);

		$query = array(
			'SELECT' => '*',
			'FROM' => 'yod_test1',
			'WHERE' => 'type = :type',
			'LIMIT' => 10,
		);
		$model->union($query, array(':type' => 1));

		$this->assign('query', $model->parseQuery());

		$this->display('parsequery', array('model' => $model));
	}

}
