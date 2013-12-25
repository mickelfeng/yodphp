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
}
