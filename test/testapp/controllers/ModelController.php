<?php
/**
 * ModelController
 */
class ModelController extends Yod_Controller
{
	public function indexAction()
	{
		$this->assign('title', 'Yod_Model');

		$this->display();
	}

	public function getinstanceAction()
	{
		$this->assign('title', 'Yod_Model::getInstance()');

		$model = Yod_Model::getInstance('Test');

		$this->display('getinstance', array('model' => $model));
	}
	
	public function findAction()
	{
		$this->assign('title', 'Yod_Model::find()');

		$model = $this->model('Test');

		$this->assign('data', $model->find());

		$this->display('find', array('model' => $model));
	}

	public function findallAction()
	{
		$this->assign('title', 'Yod_Model::findAll()');

		$model = $this->model('Test');

		$this->assign('data', $model->findAll());

		$this->display('findall', array('model' => $model));
	}

	public function countAction()
	{
		$this->assign('title', 'Yod_Model::count()');

		$model = $this->model('Test');

		$this->assign('count', $model->count());

		$this->display('count', array('model' => $model));
	}

	public function saveAction()
	{
		$this->assign('title', 'Yod_Model::save()');

		$model = $this->model('Test');

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
		$this->assign('title', 'Yod_Model::remove()');

		$model = $this->model('Test');

		$this->assign('resilt', $model->remove('id = :id', array(':id' => 1)));

		$this->assign('data', $model->findAll());

		$this->display('remove', array('model' => $model));
	}

	public function lastqueryAction()
	{
		$this->assign('title', 'Yod_Model::lastQuery()');

		$model = $this->model('Test');

		$this->assign('lastquery', $model->lastQuery());

		$this->display('lastquery', array('model' => $model));
	}

	public function configAction()
	{
		$this->assign('title', 'Yod_Model::config()');

		$this->display('config', array(
			'config' => $this->config(),
			'db_dsn' => $this->config('db_dsn'),
		));
	}

	public function importAction()
	{
		$this->assign('title', 'Yod_Model::import()');

		$this->import('Toolkit');

		$this->import('utils.qrcode', '.php');
		$qrcode = new ImageQrcode();

		$this->display('import', array('qrcode' => $qrcode));
	}

	public function modelAction()
	{
		$this->assign('title', 'Yod_Model::model()');

		$model = $this->model('Test');

		$this->assign('data', $model->test());

		$this->display('model', array('model' => $model));
	}

	public function dbmodelAction()
	{
		$this->assign('title', 'Yod_Model::dbmodel()');

		$dbmodel = $this->dbmodel('db_mysql');

		$this->display('dbmodel', array('dbmodel' => $dbmodel));
	}

}
