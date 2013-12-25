--TEST--
Check for yod request
--SKIPIF--
<?php if (!extension_loaded("yod") || defined('YOD_RUNMODE')) print "skip"; ?>
--FILE--
<?php
error_reporting(E_ALL ^ E_WARNING);
date_default_timezone_set('Asia/Shanghai');

define('TESTS_PATH', dirname(__FILE__));
include TESTS_PATH . '/clean.php';

is_dir(TESTS_PATH . '/views') or mkdir(TESTS_PATH . '/views');
is_dir(TESTS_PATH . '/views/tests') or mkdir(TESTS_PATH . '/views/tests');
file_put_contents(TESTS_PATH . '/views/tests/hello.php', <<<PHP
<?php echo \$yodphp; ?>

<?php echo \$hello; ?>


PHP
);

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

class TestsController extends Yod_Controller
{
	public function indexAction()
	{
		
	}

    public function requestAction()
    {
        $this->_view['tpl_path'] = './tests/views';
        $this->forward('hello');
    }

    public function helloAction()
    {
        print_r($this);
        $this->assign('yodphp', 'Yod PHP Framework');
        $this->assign(array('hello' => 'Hello World!'));
        $this->display();
    }
}
?>
--EXPECTF--
TestsController Object
(
    [_name:protected] => tests
    [_action:protected] => hello
    [_request:protected] => Yod_Request Object
        (
            [_routed:protected] => 1
            [_dispatched:protected] => 1
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
Hello World!
HTTP/1.0 404 Not Found
