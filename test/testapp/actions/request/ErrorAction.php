<?php
class ErrorAction extends Yod_Action
{
	public function init()
	{
		$this->assign('title', 'Yod_Request::errorAction()');
	}

	public function run()
	{
		$this->display('erroraction');
	}

}
