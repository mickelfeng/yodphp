<?php
/**
 * ApplicationController
 */
class ApplicationController extends Yod_Controller
{
	public function indexAction()
	{
		$this->assign('title', 'Yod_Application');

		$this->display();
	}

	public function runAction()
	{
		Yod_Application::app()->run();
	}

	public function configAction()
	{
		$this->assign('title', 'Yod_Application::config()');

		$app = Yod_Application::app();

		$this->display('config', array(
			'config' => $app->config(),
			'db_dsn' => $app->config('db_dsn'),
		));
	}

	public function importAction()
	{
		$this->assign('title', 'Yod_Application::import()');

		Yod_Application::app()->import('Toolkit');

		$this->import('utils.qrcode', '.php');
		$qrcode = new ImageQrcode();

		$this->display('import', array('qrcode' => $qrcode));
	}

	public function appAction()
	{
		$this->assign('title', 'Yod_Application::app()');

		$app = Yod_Application::app()->app();

		$this->display('app', array('app' => $app));
	}

	public function autoloadAction()
	{
		$this->assign('title', 'Yod_Application::autoload()');

		Yod_Application::autoload('Toolkit');

		Yod_Application::autoload('ImageQrcode');

		$this->display('autoload');
	}
}
