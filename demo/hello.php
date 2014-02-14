<?php
//set_time_limit(0);
error_reporting(E_ALL);
date_default_timezone_set('Asia/Shanghai');

defined('YOD_VIEWEXT') or define('YOD_VIEWEXT', '.tpl');
defined('YOD_RUNPATH') or define('YOD_RUNPATH', dirname(__FILE__));
defined('YOD_EXTPATH') or define('YOD_EXTPATH', dirname(__FILE__) . '/../yodphp');

class_exists('Yod_Application', false) or require YOD_EXTPATH . '/yodphp.php';

class HelloController extends Yod_Controller
{
	public function init()
	{
		$this->_name = '';
		$this->_view['tpl_path'] = YOD_RUNPATH;
	}

    public function indexAction()
    {
        $this->assign('content', 'Hello World!');
        $this->display('hello');
    }

    public function errorAction()
    {
        echo '<pre>';
        print_r($this);
    }
}
