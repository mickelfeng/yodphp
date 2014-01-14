<?php
defined('YOD_RUNMODE') or define('YOD_RUNMODE', 2|4|8);

$yodapp = new Yod_Application();

class YodappController extends Yod_Controller
{
	public function indexAction()
	{
		$yodapp = Yod_Application::app();
		var_dump($GLOBALS['yodapp'] === $yodapp);
	}
}
