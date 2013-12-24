<?php
class HelloAction extends Yod_Action
{
	public function init()
	{
		$this->assign('title', 'Hello World!');
	}

	public function run()
	{
		$this->assign(array('hello' => 'Hello World!'));

		$this->display('hello', array('yodphp' => 'Yod Framework'));
	}

}
