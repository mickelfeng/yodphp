<?php
class IndexController extends Yod_Controller
{
	public function indexAction()
	{
		$this->import('Tests');
		Tests::yodphp();
		$this->assign(array(
			'yodphp' => 'Yod PHP Framework',
			'hello' => 'Hello World!',
		));
		$this->display('/tests/widget');
	}
}
