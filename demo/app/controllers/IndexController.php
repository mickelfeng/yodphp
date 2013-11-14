<?php
class IndexController extends Yod_Controller {

	public function indexAction(){

		$this->display('index');
	}

	public function errorAction(){
		echo '<pre>';
		print_r($this);
	}

}
