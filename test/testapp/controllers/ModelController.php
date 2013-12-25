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
}
