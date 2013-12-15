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
file_put_contents(TESTS_PATH . '/views/tests/widget.php', <<<PHP
<?php echo \$yodphp; ?>

<?php \$this->widget('public/header'); ?>

<?php echo \$hello; ?>

<?php \$this->widget('public/footer'); ?>

PHP
);

is_dir(TESTS_PATH . '/widgets') or mkdir(TESTS_PATH . '/widgets');
is_dir(TESTS_PATH . '/widgets/public') or mkdir(TESTS_PATH . '/widgets/public');
file_put_contents(TESTS_PATH . '/widgets/PublicWidget.php', <<<PHP
<?php
class PublicWidget extends Yod_Widget
{
    public function headerAction()
    {
        \$this->display('header', array(
            'yodphp' => '<sup>Beta</sup>'
        ));
    }

    public function footerAction()
    {
        \$this->display('footer', array(
            'footer' => 'Copyright'
        ));
    }
}

PHP
);

file_put_contents(TESTS_PATH . '/widgets/public/header.php', 'Header');
file_put_contents(TESTS_PATH . '/widgets/public/footer.php', "Footer\r\n");

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
HTTP/1.0 404 Not Found
