<?php
class ErrorAction extends Yod_Action
{
	protected function run()
	{
		header('HTTP/1.0 404 Not Found');

		$this->display('error404');
	}
}
