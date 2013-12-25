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
}
