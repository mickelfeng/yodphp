<?php
//set_time_limit(0);
error_reporting(E_ALL);
date_default_timezone_set('Asia/Shanghai');

define('YOD_PATHVAR', 'r');
define('YOD_RUNPATH', dirname(__FILE__));
define('YOD_EXTPATH', dirname(__FILE__) . '/../yodphp');
define('YOD_LOGFILE', dirname(__FILE__) . '/debugs/' . date('YmdHis') . '.log');

define('APP_WEBROOT', '/yodphp/test/index.php');

class_exists('Yod_Application', false) or require dirname(__FILE__) . '/../yodphp/yodphp.php';


class DemoUserModel extends Yod_Model
{
	protected function init()
	{
		echo '<pre>';
		//$m_info = $this->model('DemoInfo');
		//print_r($m_info);
	}

	public function parse(){
		echo $this->parseQuery();
	}
}

class DemoInfoModel extends Yod_Model
{
	protected function init()
	{
		echo '<pre>';
		$m_text = $this->model('DemoText');
		echo $m_text->lastQuery();
		print_r($m_text);
	}
}