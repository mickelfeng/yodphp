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
}
