<?php
/**
 * IndexController
 */
class IndexController extends Yod_Controller
{
	public function indexAction()
	{

		$this->display();
	}

	public function errorAction()
	{
		echo '<pre>';
		print_r($this);
	}
}
