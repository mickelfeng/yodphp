--TEST--
Check for yod request
--SKIPIF--
<?php if (!extension_loaded("yod")) print "skip"; ?>
--FILE--
<?php
error_reporting(E_ALL);
date_default_timezone_set('Asia/Shanghai');

$config = array(
	//db
	'db_dsn' => array(
		'type' => 'pdo',
		'dsn' => 'mysql:host=localhost;port=3306;dbname=test',
		'host' => 'localhost',
		'user' => 'root',
		'pass' => '123456',
		'dbname' => 'test',
		'prefix' => 'yod_',
		'slaves' => array(

		),
	),
	//tpl
	'tpl_data' => array(
		'_PUBLIC_' => '/Public/',
	),
);

$yodapp = Yod_Application::app();
$request = new Yod_Request('tests/request');
$request->dispatch();
print_r($yodapp);

class TestsController extends Yod_Controller
{
	public function indexAction()
	{
		
	}

    public function requestAction()
    {
        $this->_view['tpl_path'] = './tests/views';
        $this->forward('forward');
    }

    public function forwardAction()
    {
        print_r($this);
        $this->assign('yodphp', 'Yod PHP Framework');
        $this->display('widget', array('hello' => 'Hello World!'));
    }
}
?>
--EXPECTF--
TestsController Object
(
    [_name:protected] => tests
    [_action:protected] => forward
    [_request:protected] => Yod_Request Object
        (
            [_routed:protected] => 1
            [controller] => Tests
            [action] => request
            [params] => Array
                (
                )

            [method] => Cli
        )

    [_view:protected] => Array
        (
            [tpl_data] => Array
                (
                    [_PUBLIC_] => /Public/
                )

            [tpl_path] => ./tests/views
            [tpl_file] => 
        )

)
Yod PHP Framework
Header
Hello World!
Footer
Yod_Application Object
(
    [_config:protected] => Array
        (
            [db_dsn] => Array
                (
                    [type] => pdo
                    [dsn] => mysql:host=localhost;port=3306;dbname=test
                    [host] => localhost
                    [user] => root
                    [pass] => 123456
                    [dbname] => test
                    [prefix] => yod_
                    [slaves] => Array
                        (
                        )

                )

            [tpl_data] => Array
                (
                    [_PUBLIC_] => /Public/
                )

        )

    [_request:protected] => Yod_Request Object
        (
            [_routed:protected] => 
            [controller] => 
            [action] => 
            [params] => 
            [method] => Cli
        )

    [_imports:protected] => Array
        (
        )

    [_running:protected] => 
)
