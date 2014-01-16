<?php
set_time_limit(0);
error_reporting(E_ALL);
date_default_timezone_set('Asia/Shanghai');

//defined('YOD_RUNMODE') or define('YOD_RUNMODE', 3);
defined('YOD_PATHVAR') or define('YOD_PATHVAR', 'r');
defined('YOD_RUNPATH') or define('YOD_RUNPATH', dirname(__FILE__));

// config
$config = array(
	// db
	'db_dsn' => array(
		'type'   => 'pdo',
		'pdsn'   => 'sqlite:' . YOD_RUNPATH . '/manual.sdb',
		'prefix' => 'yod_',
		'slaves' => array(
			
		),
	),

	// tpl
	'tpl_data' => array(
		'PUBLIC' => 'Public',

	),
);

// yodphp
class_exists('Yod_Application', false) or require YOD_RUNPATH . '/../yodphp/yodphp.php';

/**
 * DevelController
 */
class DevelController extends Yod_Controller
{

	public function init()
	{

	}

	public function indexAction()
	{
		echo '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">';
		echo '<html xmlns="http://www.w3.org/1999/xhtml"><head></head><body>';

		echo '<br>'. (memory_get_usage(true) / 1024 / 1024);

		$str = '';
		for($i=0;$i<1000000;$i++){
			$str .= sprintf('12345%06d,', $i);
		}
		//echo '<br>'. $str;

		$runtime = microtime(true);

		$len = strlen($str);

		for($i=1200000; $i<$len;$i+=1200000){
			$this->sql_execute(substr($str, $i, 1200000));
		}

		echo '<br>'. (microtime(true) - $runtime);

		echo '<br>'. $str;

		echo '<br>'. ($len / 1024 / 1024);

		echo '<br>'. base64_encode('13716119967');
		echo '<br>'. base64_encode('12345678901');
		echo '<br>'. base64_encode('12345678902');
		echo '</body></html>';
	}
	protected function sql_execute($str)
	{
		$len = strlen($str);

		for($i=12000; $i<$len;$i+=12000){
			$sql = $this->sql_encode(substr($str, $i, 12000));
			//echo '<br>'. $sql;
		}
	}

	protected function sql_encode($str)
	{
		$str = base64_encode($str);

		$fag = '=\'),(\'1\',\'';
		$num = strlen($fag) - 1;

		$len = strlen($str);
		for($i=15; $i<$len;$i+=16){
			$str = substr_replace($str, $fag, $i, 1);
			$len += $num;
			$i += $num;
		}
		$str = 'INSERT INTO test (\'1\',\''. substr($str, 0, 2-$num);
		return $str;
	}

	public function errorAction()
	{

	}
}


/**
 * DevelModel
 */
class DevelModel extends Yod_DbModel
{

}
