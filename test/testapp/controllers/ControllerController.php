<?php
/**
 * ControllerController
 */
class ControllerController extends Yod_Controller
{
	public function indexAction()
	{
		$this->assign('title', 'Yod_Controller');

		$this->display();
	}

	public function configAction()
	{
		$this->assign('title', 'Yod_Controller::config()');

		$this->display('config', array(
			'config' => $this->config(),
			'db_dsn' => $this->config('db_dsn'),
		));
	}

	public function importAction()
	{
		$this->assign('title', 'Yod_Controller::import()');

		$this->import('Toolkit');

		$this->import('Utils.qrcode', '.php');
		$qrcode = new ImageQrcode();

		$this->display('import', array('qrcode' => $qrcode));
	}

	public function modelAction()
	{
		$this->assign('title', 'Yod_Controller::model()');

		$model = $this->model();

		$this->display('model', array('model' => $model));
	}

	public function assignAction()
	{
		$this->assign('title', 'Yod_Controller::assign()');

		$this->assign('this', $this);

		$this->display('assign');
	}

	public function renderAction()
	{
		$this->assign('title', 'Yod_Controller::render()');

		header('Content-type: text/html; charset=utf-8');

		echo $this->render('render');
	}
	
	public function displayAction()
	{
		$this->assign('title', 'Yod_Controller::display()');

		$this->display();
	}
	
	public function widgetAction()
	{
		$this->assign('title', 'Yod_Controller::widget()');

		$this->display();
	}

	public function forwardAction()
	{
		$this->assign('title', 'Yod_Controller::forward()');

		$this->forward('forward', 1);

		$this->display();
	}

	public function redirectAction()
	{
		$this->assign('title', 'Yod_Controller::redirect()');

		if (empty($_GET['r'])) {
			$this->redirect(APP_WEBROOT . '/controller/redirect/r/1');
		}

		$this->display();
	}

	public function error404Action()
	{
		$this->assign('title', 'Yod_Controller::error404()');

		$this->error404($this->render('error404'));
	}
	
}
