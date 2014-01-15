<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>yodphp - [ Yod PHP Framework ]</title>
<meta name="keywords" content="yodphp,Yod PHP Framework" />
<meta name="description" content="yodphp - [ Yod PHP Framework ]" />
</head>
<body>
<h1>yodphp <sup>[ Yod Framework for PHP ]</sup></h1>
<hr>
<p>It is a simple PHP framework</p>
<p><h2>Requirement</h2></p>
<p><ul><li>PHP 5.2 +</li></ul></p>
<p><h2>Download</h2></p>
<p><ul><li><a href="https://github.com/zmrnet/yodphp" target="_blank">https://github.com/zmrnet/yodphp</a></li></ul></p>
<p><h2>Tutorial</h2></p>
<p><h3>layout</h3></p>
<p><ul><li>A classic Application directory layout:</li></ul></p>
<p><pre>-------------------------------------------------------------------------
 - index.php	// Application entry
 + public
 + yodphp
	+ drivers	// Database Drivers
	- yodphp.php	// Yodphp entry
 + app
	+ actions	// Other actions
	+ configs
		- config.php	// Configure 
	+ controllers
		- IndexController.php	// Default controller
	+ extends	// Extends
	+ models	// Models
		- DemoModel.php
	+ views
		+ index	// View templates for default controller
			- index.php

-------------------------------------------------------------------------</pre></p>
<p><ul><li>Multiple entry Application directory layout:</li></ul></p>
<p><pre>-------------------------------------------------------------------------
 - index.php	// Application index entry
 - hello.php	// Application hello entry
 + public
 + app

-------------------------------------------------------------------------</pre></p>
<p><h3>index.php</h3></p>
<p>index.php is the application entry</p>
<p><pre>-------------------------------------------------------------------------</pre>
<?php highlight_string(
"<?php
//set_time_limit(0);
error_reporting(E_ALL);
date_default_timezone_set('Asia/Shanghai');

define('YOD_RUNPATH', dirname(__FILE__) . '/app');
class_exists('Yod_Application', false) or require YOD_RUNPATH . '/../yodphp/yodphp.php';

"
); ?><pre>-------------------------------------------------------------------------</pre></p>
<p><h3>hello.php</h3></p>
<p>hello.php is the application hello entry</p>
<p><pre>-------------------------------------------------------------------------</pre>
<?php highlight_string(
"<?php
//set_time_limit(0);
error_reporting(E_ALL);
date_default_timezone_set('Asia/Shanghai');

define('YOD_RUNPATH', dirname(__FILE__) . '/app');
class_exists('Yod_Application', false) or require YOD_RUNPATH . '/yodphp.php';

class HelloController extends Yod_Controller
{
	public function indexAction()
	{
		\$this->assign(\$content, 'Hello World');
		\$this->display('/index/index');
	}

	public function errorAction()
	{
		echo '<pre>';
		print_r(\$this);
	}
}

"
); ?><pre>-------------------------------------------------------------------------</pre></p>
<p><h3>config.php</h3></p>
<p>config.php is the application config file</p>
<p><pre>-------------------------------------------------------------------------</pre>
<?php highlight_string(
"<?php
return array(
	// db_dsn
	'db_dsn' => array(
		'type'   => 'pdo',
		'dsn'    => 'mysql:host=localhost;port=3306;dbname=test',
		'host'   => 'localhost',
		'port'   => '3306',
		'user'   => 'root',
		'pass'   => '123456',
		'dbname' => 'test',
		'prefix' => 'yod_',
		'charset' => 'utf8',
		// slaves
		'slaves' => array(
			array(
				'dsn'    => 'mysql:host=localhost;port=3306;dbname=test',
				'user'   => 'root',
				'pass'   => '123456',
			),
		),
	),
	// tpl_data
	'tpl_data' => array(
		'_PUBLIC_' => '/public/'
	),
);

"
); ?><pre>-------------------------------------------------------------------------</pre></p>
<p><h3>default controller</h3></p>
<p>In Yodphp, the default controller is named IndexController</p>
<p><pre>-------------------------------------------------------------------------</pre>
<?php highlight_string(
"<?php
class IndexController extends Yod_Controller {
	// default action name
	public function indexAction() {
		\$this->assign(\$content, 'Hello World');
		\$this->display();
	}
}

"
); ?><pre>-------------------------------------------------------------------------</pre></p>
<p><h3>view script</h3></p>
<p>The view script for default controller and default action is in the app/views/index/index.php</p>
<p><pre>-------------------------------------------------------------------------</pre>
<?php highlight_string(
"<html>
  <head>
    <title>Hello World</title>
  </head>
  <body>
    <?php echo \$content; ?>
  </body>
</html>
"
); ?><pre>-------------------------------------------------------------------------</pre></p>
<p>&nbsp;</p>
<p><script language="javascript" type="text/javascript" src="http://js.users.51.la/3906523.js"></script></p>
</body>
</html>