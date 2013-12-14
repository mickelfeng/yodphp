<?php
class IndexAction extends Yod_Action
{
	public function run()
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
