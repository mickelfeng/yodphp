<?php
class IndexAction extends Yod_Action
{
	public function init()
	{
		$this->assign('title', 'Yod_Action');
	}

	public function run()
	{

		$this->display();
	}

}
