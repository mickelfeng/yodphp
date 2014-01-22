<?php
return array(
	'index' => array(
		'node' => 'index',
		'prev' => '',
		'next' => 'about',
		'title' => 'Yod Framework 用户手册 ',
		'intro' => '<p>冰星</p>',
		'content' => '',
	),

	'about' => array(
		'node' => 'about',
		'prev' => 'index',
		'next' => 'setup',
		'title' => '1.1 关于Yod',
		'intro' => '<p>Yod是一个C语言编写的PHP框架，同时也提供了php的源码</p>',
		'content' => '<p>Yod同时支持单入口和多入口模式，应用灵活，形式自由</p>',
	),

	'setup' => array(
		'node' => 'setup',
		'prev' => 'about',
		'next' => 'const',
		'title' => '1.2 Yod安装/配置',
		'intro' => '<p>Yod只支持PHP5.2及以上的版本</p>',
		'content' => '<p><b>1.2.1 编译Yod</b><pre>$PHP_BIN/phpize
./configure --with-php-config=$PHP_BIN/php-config
make
make install</pre></p>
<p><b>1.2.2 以debug模式编译Yod</b><pre>$PHP_BIN/phpize
./configure --enable-yod-debug --with-php-config=$PHP_BIN/php-config
make
make install</pre></p>',
	),

	'const' => array(
		'node' => 'const',
		'prev' => 'setup',
		'next' => 'hello',
		'title' => '1.3 Yod定义的常量',
		'intro' => '',
		'content' => '<p><pre>YOD_VERSION	Yod框架的三位版本信息
YOD_RUNTIME	Yod框架的开始运行时间
YOD_FORWARD	forward最大嵌套深度
YOD_RUNMODE	Yod框架的运行模式（按位标记：1 自动运行，2 记录错误日志，4 显示Debug信息（仅在debug模式下有效），8 记录Debug信息（仅在debug模式下有效））
YOD_CHARSET	输出的字符编码方式
YOD_VIEWEXT	视图文件的扩展名（默认：.php）
YOD_PATHVAR	获取uri路由信息的变量名
YOD_EXTPATH	Yod框架的扩展文件（数据库驱动等）的路径
YOD_LOGPATH	Yod框架的日志存储目录
YOD_RUNPATH	APP的运行目录（定义了此常量后自动运行模式才会生效）</pre></p>',
	),

	'hello' => array(
		'node' => 'hello',
		'prev' => 'const',
		'next' => 'layout',
		'title' => '1.4 Hello World!',
		'intro' => '这是一个简单的模式，只有两个文件',
		'content' => '<p><b>hello.php</b></p><p>'. highlight_string('<?php
//set_time_limit(0);
error_reporting(E_ALL);
date_default_timezone_set(\'Asia/Shanghai\');

defined(\'YOD_VIEWEXT\') or define(\'YOD_VIEWEXT\', \'.tpl\');
defined(\'YOD_RUNPATH\') or define(\'YOD_RUNPATH\', dirname(__FILE__));

class_exists(\'Yod_Application\', false) or require YOD_RUNPATH . \'/yodphp.php\';

class HelloController extends Yod_Controller
{
	public function init()
	{
		$this->_name = \'\';
		$this->_view[\'tpl_path\'] = YOD_RUNPATH;
	}

    public function indexAction()
    {
        $this->assign($content, \'Hello World!\');
        $this->display(\'hello\');
    }

    public function errorAction()
    {
        echo \'<pre>\';
        print_r($this);
    }
}

', true) .'</p>
<p><b>hello.tpl</b></p><p>'. highlight_string('<html>
  <head>
    <title>Hello World!</title>
  </head>
  <body>
    <?php echo $content; ?>
  </body>
</html>', true) .'</p>',
	),

	'layout' => array(
		'node' => 'layout',
		'prev' => 'hello',
		'next' => 'config',
		'title' => '1.5 Yod目录结构',
		'intro' => 'Yod的目录结构比较灵活',
		'content' => '<p><b>1.5.1 标准目录结构</b></p><p><pre>
- index.php // APP 入口
+ public
+ yodphp
    + drivers   // 数据库驱动
    - yodphp.php    // Yodphp（php源码）
+ app
    + actions   // Other actions
    + configs
        - config.php    // 配置文件
    + controllers
        - IndexController.php   // 控制器
    + extends   // APP扩展
    + models    // 模型
        - DemoModel.php
    + views
        + index // 视图
            - index.php
</pre></p>
<p><b>1.5.2 多入口目录结构</b></p><p><pre>
- index.php // index 入口（一般情况控制器会写在入口文件里，参考Hello World!）
- hello.php // hello 入口
+ public
+ app
</pre></p>
<p><b>1.5.3 精简目录结构</b></p><p><pre>
- index.php // index 入口（require configs.php, controllers.php, models.php）
+ public
+ app
    - configs.php
    - controllers.php
    - models.php
    + views
        + index // 视图
            - index.php
</pre></p>',
	),

	'config' => array(
		'node' => 'config',
		'prev' => 'layout',
		'next' => 'application',
		'title' => '1.6 Yod应用的配置结构',
		'intro' => 'Yod应用配置的结构比较灵活',
		'content' => '<p><b>1.6.1 标准配置结构</b></p><p><pre>
- index.php // APP 入口
+ app
    + configs
        - config.php    // 配置文件
</pre></p>
<p><b>1.6.2 多文件配置结构</b></p><p><pre>
- index.php // APP 入口
+ app
    + configs
        - base.config.php    // 配置文件
        - db_dsn.config.php  // 数据库配置
        ...
</pre></p>
<p><b>1.6.3 使用$config变量配置</b></p><p>全局变量$config会覆盖其他方式的配置值</p>',
	),

	'application' => array(
			'node' => 'application',
			'prev' => 'config',
			'next' => 'request',
			'title' => '2.1 Yod_Application',
			'intro' => 'Yod_Application 代表一个产品/项目, 是Yod运行的主导者, 真正执行的主题. 它负责接收请求, 协调路由, 分发, 执行, 输出.',
			'content' => '<p>'. highlight_string('<?php
final class Yod_Application
{
	protected static $_app;
	protected static $_config = array();
	protected static $_imports = array();

	protected $_request = null;
	protected $_running = false;

	/**
	 * __construct
	 * @access public
	 * @param mixed $config
	 * @return void
	 */
	public function __construct($config = null)
	{
		
	}

	/**
	 * run
	 * @access public
	 * @param void
	 * @return void
	 */
	public function run()
	{
		
	}

	/**
	 * app
	 * @access public
	 * @param mixed $config
	 * @return Yod_Application
	 */
	public static function app($config = null)
	{
		
	}

	/**
	 * config
	 * @access public
	 * @param string $name
	 * @return mixed
	 */
	public static function config($name = null)
	{
		
	}

	/**
	 * import
	 * @access public
	 * @param string $alias
	 * @param string $classext
	 * @return boolean
	 */
	public static function import($alias, $classext = \'.class.php\')
	{
		
	}

	/**
	 * autorun
	 * @access public
	 * @param void
	 * @return void
	 */
	public static function autorun()
	{
		
	}

	/**
	 * autoload
	 * @access public
	 * @param string $classname
	 * @return boolean
	 */
	public static function autoload($classname)
	{
		
	}

	/**
	 * errorlog
	 * @access public
	 * @param int $errno
	 * @param string $errstr
	 * @param string $errfile
	 * @param int $errline
	 * @param array $errcontext
	 * @return boolean
	 */
	public static function errorlog($errno, $errstr, $errfile = \'\', $errline = 0, $errcontext = array())
	{
		
	}

	/**
	 * __destruct
	 * @access public
	 * @param void
	 */
	public function __destruct()
	{
		
	}

}

', true) .'</p>',
	),

	'request' => array(
		'node' => 'request',
		'prev' => 'application',
		'next' => 'controller',
		'title' => '2.2 Yod_Request',
		'intro' => 'Yod_Request 代表了一个实际请求, 一般的不用自己实例化它, Yod_Application 会自动根据当前请求实例它.',
		'content' => '<p>'. highlight_string('<?php
final class Yod_Request
{
	protected $_routed = false;
	protected $_dispatched = false;

	public $controller;
	public $action;
	public $params;
	public $method;

	/**
	 * __construct
	 * @access public
	 * @param string $route
	 * @return void
	 */
	public function __construct($route = null)
	{

	}

	/**
	 * route
	 * @access public
	 * @param string $route
	 * @return Yod_Request
	 */
	public function route($route = null)
	{

	}

	/**
	 * dispatch
	 * @access public
	 * @param void
	 * @return void
	 */
	public function dispatch()
	{

	}

	/**
	 * errorAction
	 * @access public
	 * @return void
	 */
	public function errorAction()
	{

	}

	/**
	 * error404
	 * @access public
	 * @param string $html
	 * @return void
	 */
	public function error404($html = null)
	{

	}

	/**
	* isGet
	* @access public
	* @param void
	* @return boolean
	*/
	public function isGet()
	{

	}

	/**
	* isPost
	* @access public
	* @param void
	* @return boolean
	*/
	public function isPost()
	{

	}

	/**
	* isPut
	* @access public
	* @param void
	* @return boolean
	*/
	public function isPut()
	{

	}

	/**
	* isHead
	* @access public
	* @param void
	* @return boolean
	*/
	public function isHead()
	{

	}

	/**
	* isOptions
	* @access public
	* @param void
	* @return boolean
	*/
	public function isOptions()
	{

	}

	/**
	* isCli
	* @access public
	* @param void
	* @return boolean
	*/
	public function isCli()
	{

	}

}

', true) .'</p>',
	),

	'controller' => array(
		'node' => 'controller',
		'prev' => 'request',
		'next' => 'action',
		'title' => '2.3 Yod_Controller',
		'intro' => 'Yod_Controller 是Yod的MVC体系的核心部分, 可以通过继承已有的类, 来实现这个抽象类, 从而添加应用自己的应用逻辑.',
		'content' => '<p>'. highlight_string('<?php
abstract class Yod_Controller
{
	protected static $_forward = 0;

	protected $_name;
	protected $_action;
	protected $_request;

	protected $_view = array(
		\'tpl_data\' => array(),
		\'tpl_path\' => \'\',
		\'tpl_file\' => \'\',
	);

	/**
	 * __construct
	 * @access public
	 * @param Yod_Request $request
	 * @param string $action
	 * @return void
	 */
	public function __construct($request, $action = null)
	{

	}

	/**
	 * init
	 * @access protected
	 * @return void
	 */
	protected function init()
	{

	}

	/**
	 * run
	 * @access protected
	 * @return void
	 */
	protected function run()
	{

	}

	/**
	 * config
	 * @access protected
	 * @param string $name
	 * @return array
	 */
	protected function config($name = null)
	{

	}

	/**
	 * import
	 * @access protected
	 * @param string $alias
	 * @param string $classext
	 * @return boolean
	 */
	protected function import($alias, $classext = \'.class.php\')
	{

	}

	/**
	 * model
	 * @access protected
	 * @param string $name
	 * @param mixed $config
	 * @return Yod_Model
	 */
	protected function model($name = \'\', $config = \'\')
	{

	}

	/**
	 * dbmodel
	 * @access protected
	 * @param string $name
	 * @param mixed $config
	 * @return Yod_DbModel
	 */
	protected function dbmodel($name = \'\', $config = \'\')
	{

	}

	/**
	 * assign
	 * @access protected
	 * @param string $name
	 * @param mixed $value
	 * @return Yod_Controller
	 */
	protected function assign($name, $value = null)
	{

	}

	/**
	 * render
	 * @access protected
	 * @param string $view
	 * @param array $data
	 * @return string
	 */
	protected function render($view = null, $data = array())
	{

	}

	/**
	 * display
	 * @access protected
	 * @param string $view
	 * @param array $data
	 * @return void
	 */
	protected function display($view = null, $data = array())
	{

	}

	/**
	 * widget
	 * @access protected
	 * @param string $route
	 * @param array $params
	 * @return void
	 */
	protected function widget($route, $params = array())
	{

	}

	/**
	 * forward
	 * @access protected
	 * @param string $route
	 * @return void
	 */
	protected function forward($route, $exited = false)
	{

	}

	/**
	 * redirect
	 * @access protected
	 * @param string $url
	 * @param integer $code
	 * @return void
	 */
	protected function redirect($url, $code = 302)
	{

	}

	/**
	 * error404
	 * @access protected
	 * @param mixed $html
	 * @param boolean $exit
	 * @return void
	 */
	protected function error404($html = null)
	{

	}

	/**
	 * __destruct
	 * 
	 * @return void
	 */
	public function __destruct() {

	}
}

', true) .'</p>',
	),

	'action' => array(
		'node' => 'action',
		'prev' => 'controller',
		'next' => 'widget',
		'title' => '2.4 Yod_Action',
		'intro' => 'Yod_Action 是MVC中C的动作, 一般而言动作都是定义在Yod_Controller的派生类中的, 但是有的时候, 为了使得代码清晰, 分离一些大的控制器, 则可以采用单独定义Yod_Action来实现.',
		'content' => '<p>'. highlight_string('<?php
abstract class Yod_Action extends Yod_Controller
{
	/**
	 * run
	 * @access protected
	 * @return void
	 */
	protected function run()
	{

	}
}

', true) .'</p>',
	),

	'widget' => array(
		'node' => 'widget',
		'prev' => 'action',
		'next' => 'model',
		'title' => '2.5 Yod_Widget',
		'intro' => 'Yod_Widget 在MVC中V的一些公共的部分可以采用单独定义Yod_Widget来实现.',
		'content' => '<p>'. highlight_string('<?php
abstract class Yod_Widget extends Yod_Controller
{
	/**
	 * __construct
	 * @access public
	 * @param Yod_Request $request
	 * @param string $action
	 * @param array $params
	 * @return void
	 */
	public function __construct($request, $action = null, $params = null)
	{

	}

	/*
	 * run
	 * @access protected
	 * @return void
	 */
	protected function run()
	{

	}
}

', true) .'</p>',
	),

	'model' => array(
		'node' => 'model',
		'prev' => 'widget',
		'next' => 'dbmodel',
		'title' => '2.6 Yod_Model',
		'intro' => 'Yod_Model 是Yod的MVC体系的核心部分, 可以通过继承已有的类, 来实现这个抽象类, 从而添加应用自己的应用模型.',
		'content' => '<p>'. highlight_string('<?php
class Yod_Model
{
	protected static $_model = array();

	protected $_db;
	protected $_dsn = \'db_dsn\';

	protected $_name;
	protected $_table;
	protected $_prefix;


	/**
	 * __construct
	 * @access public
	 * @return void
	 */
	public function __construct($name = \'\', $config = null)
	{

	}

	/**
	 * init
	 * @access protected
	 * @param void
	 * @return void
	 */
	protected function init()
	{

	}

	/**
	 * getInstance
	 * @access public
	 * @param string	$name
	 * @param mixed		$config
	 * @return Yod_Model
	 */
	public static function getInstance($name = \'\', $config = null)
	{

	}

	/**
	 * find
	 * @access public
	 * @param mixed		$where
	 * @param array		$params
	 * @param mixed		$select
	 * @return mixed
	 */
	public function find($where = null, $params = array(), $select = \'*\')
	{

	}

	/**
	 * select
	 * @access public
	 * @param mixed		$where
	 * @param array		$params
	 * @param mixed		$select
	 * @return mixed
	 */
	public function select($where = null, $params = array(), $select = \'*\')
	{

	}

	/**
	 * count
	 * @access public
	 * @param string	$where
	 * @param array		$params
	 * @return integer
	 */
	public function count($where = \'\', $params = array())
	{

	}

	/**
	 * save
	 * @access public
	 * @return boolean
	 */
	public function save($data, $where = \'\', $params = array())
	{

	}

	/**
	 * remove
	 * @access public
	 * @return boolean
	 */
	public function remove($where, $params = array())
	{

	}

	/**
	 * lastQuery
	 * @access public
	 * @return string
	 */
	public function lastQuery()
	{

	}

	/**
	 * config
	 * @access protected
	 * @param void
	 * @return array
	 */
	protected function config($name = null)
	{

	}
	
	/**
	 * import
	 * @access protected
	 * @param string $alias
	 * @param string $classext
	 * @return boolean
	 */
	protected function import($alias, $classext = \'.class.php\')
	{

	}

	/**
	 * model
	 * @access protected
	 * @param string $name
	 * @param mixed $config
	 * @return Yod_Model
	 */
	protected function model($name = \'\', $config = null)
	{

	}

	/**
	 * dbmodel
	 * @access protected
	 * @param string $name
	 * @param mixed $config
	 * @return Yod_Model
	 */
	protected function dbmodel($name = \'\', $config = null)
	{

	}
}

', true) .'</p>',
	),

	'dbmodel' => array(
		'node' => 'dbmodel',
		'prev' => 'model',
		'next' => 'database',
		'title' => '2.7 Yod_DbModel',
		'intro' => 'Yod_DbModel 是Yod的MVC中M的扩展, 可以通过继承已有的类, 来实现这个抽象类, 从而添加应用自己的应用模型.',
		'content' => '<p>'. highlight_string('<?php
class Yod_DbModel extends Yod_Model
{
	protected static $_model = array();

	protected $_query = array();
	protected $_params = array();

	/**
	 * __construct
	 * @access public
	 * @return void
	 */
	public function __construct($name = \'\', $config = null)
	{

	}

	/**
	 * getInstance
	 * @access public
	 * @param string	$name
	 * @param mixed		$config
	 * @return Yod_DbModel
	 */
	public static function getInstance($name = \'\', $config = null)
	{

	}

	/**
	 * table
	 * @access public
	 * @return Yod_DbModel
	 */
	public function table($table)
	{

	}

	/**
	 * find
	 * @access public
	 * @param mixed		$where
	 * @param array		$params
	 * @param mixed		$select
	 * @return mixed
	 */
	public function find($where = null, $params = array(), $select = \'*\')
	{

	}

	/**
	 * select
	 * @access public
	 * @param mixed		$where
	 * @param array		$params
	 * @param mixed		$select
	 * @return mixed
	 */
	public function select($where = null, $params = array(), $select = \'*\')
	{

	}

	/**
	 * count
	 * @access public
	 * @param string	$where
	 * @param array		$params
	 * @return integer
	 */
	public function count($where = \'\', $params = array())
	{

	}

	/**
	 * save
	 * @access public
	 * @param array		$data
	 * @param string	$where
	 * @param array		$params
	 * @return boolean
	 */
	public function save($data, $where = \'\', $params = array())
	{

	}

	/**
	 * remove
	 * @access public
	 * @param string	$where
	 * @param array		$params
	 * @return boolean
	 */
	public function remove($where, $params = array())
	{

	}

	/**
	 * field
	 * @access public
	 * @param mixed		$select
	 * @return Yod_DbModel
	 */
	public function field($select)
	{

	}

	/**
	 * from
	 * @access public
	 * @param string	$table
	 * @return Yod_DbModel
	 */
	public function from($table)
	{

	}

	/**
	 * join
	 * @access public
	 * @param string	$table
	 * @param string	$where
	 * @param string	$mode
	 * @return Yod_DbModel
	 */
	public function join($table, $where = \'\', $mode = \'LEFT\')
	{

	}

	/**
	 * where
	 * @access public
	 * @param string	$where
	 * @param array		$params
	 * @param string	$mode
	 * @return Yod_DbModel
	 */
	public function where($where, $params = array(), $mode = \'AND\')
	{

	}

	/**
	 * group
	 * @access public
	 * @param string	$group
	 * @return Yod_DbModel
	 */
	public function group($group)
	{

	}

	/**
	 * having
	 * @access public
	 * @param string	$having
	 * @param array		$params
	 * @return Yod_DbModel
	 */
	public function having($having, $params = array())
	{

	}

	/**
	 * order
	 * @access public
	 * @param string	$order
	 * @return Yod_DbModel
	 */
	public function order($order)
	{

	}

	/**
	 * limit
	 * @access public
	 * @param mixed		$limit
	 * @return Yod_DbModel
	 */
	public function limit($limit)
	{

	}

	/**
	 * union
	 * @access public
	 * @param mixed		$union
	 * @param array		$params
	 * @param string	$mode
	 * @return Yod_DbModel
	 */
	public function union($union, $params = array(), $mode = \'\')
	{

	}

	/**
	 * comment
	 * @access public
	 * @param string	$comment
	 * @return Yod_DbModel
	 */
	public function comment($comment)
	{

	}

	/**
	 * params
	 * @access public
	 * @param array		$params
	 * @return Yod_DbModel
	 */
	public function params($params)
	{

	}

	/**
	 * parseQuery
	 * @access public
	 * @param array		$query
	 * @return mixed
	 */
	public function parseQuery($query = null)
	{

	}

	/**
	 * initQuery
	 * @access protected
	 * @return string
	 */
	protected function initQuery()
	{

	}

}

', true) .'</p>',
	),

	'database' => array(
		'node' => 'database',
		'prev' => 'dbmodel',
		'next' => 'dbpdo',
		'title' => '2.8 Yod_Database',
		'intro' => 'Yod_Database 是Yod的数据库驱动抽象类, 可以通过继承已有的类, 来实现这个抽象类, 从而添加应用自己的数据库驱动.',
		'content' => '<p>'. highlight_string('<?php
abstract class Yod_Database
{
	protected static $_db = array();

	protected $_config;
	protected $_driver;
	protected $_prefix;
	protected $_result;
	protected $_linkid;
	protected $_linkids = array();
	protected $_locked = false;
	protected $_lastquery = \'\';


	/**
	 * __construct
	 * @access public
	 * @param array $config
	 * @return void
	 */
	public function __construct($config)
	{

	}

	/**
	 * __construct
	 * @access public
	 * @param void
	 * @return void
	 */
	public function __destruct()
	{

	}

	/**
	 * db
	 * @access public
	 * @param mixed $config
	 * @return Yod_Database
	 */
	public static function db($config = \'db_dsn\')
	{

	}

	/**
	 * getInstance
	 * @access public
	 * @param mixed $config
	 * @return Yod_Database
	 */
	public static function getInstance($config = \'db_dsn\')
	{

	}

	/**
	 * config
	 * @access public
	 * @param string $name
	 * @param mixed $value
	 * @return array
	 */
	public function config($name = \'\', $value = null)
	{

	}

	/**
	 * create
	 * @access public
	 * @param mixed $fields
	 * @param string $table
	 * @param string $extend
	 * @return mixed
	 */
	public function create($fields, $table, $extend = \'\')
	{

	}

	/**
	 * insert
	 * @access public
	 * @param array $data
	 * @param string $table
	 * @param boolean $replace
	 * @return mixed
	 */
	public function insert($data, $table, $replace=false)
	{

	}

	/**
	 * update
	 * @access public
	 * @param array $data
	 * @param string $table
	 * @param string $where
	 * @param array $params
	 * @return integer
	 */
	public function update($data, $table, $where = null, $params = array())
	{

	}

	/**
	 * delete
	 * @access public
	 * @param string $table
	 * @param string $where
	 * @param array $params
	 * @return integer
	 */
	public function delete($table, $where = null, $params = array())
	{

	}

	/**
	 * select
	 * @access public
	 * @param string $select
	 * @param string $table
	 * @param string $where
	 * @param array $params
	 * @param string $extend
	 * @return mixed
	 */
	public function select($select, $table, $where = null, $params = array(), $extend = null)
	{

	}

	/**
	 * lastQuery
	 * @access public
	 * @param void
	 * @return string
	 */
	public function lastQuery()
	{

	}

	/**
	 * dbconfig
	 * @param array $config
	 * @param integer $linknum
	 * @return array
	 */
	protected function dbconfig($config, $linknum = 0)
	{

	}

	/**
	 * connect
	 * @access public
	 * @param array $config
	 * @param integer $linknum
	 * @return mixed
	 */
	abstract public function connect($config = null, $linknum = 0);

	/**
	* fields
	* @access public
	* @param string $table
	* @return array
	*/
	abstract public function fields($table);

	/**
	 * execute
	 * @access public
	 * @param string $query
	 * @param array $params
	 * @param boolean $affected
	 * @return mixed
	 */
	abstract public function execute($query, $params = array(), $affected = false);

	/**
	 * query
	 * @access public
	 * @param string $query
	 * @param array $params
	 * @return mixed
	 */
	abstract public function query($query, $params = array());

	/**
	 * fetch
	 * @access public
	 * @param mixed $result
	 * @return mixed
	 */
	abstract public function fetch($result = null);

	/**
	 * fetchAll
	 * @access public
	 * @param mixed $result
	 * @return mixed
	 */
	abstract public function fetchAll($result = null);

	/**
	 * transaction
	 * @access public
	 * @param void
	 * @return boolean 
	 */
	abstract public function transaction();

	/**
	 * commit
	 * @access public
	 * @param void
	 * @return boolean 
	 */
	abstract public function commit();

	/**
	 * rollback
	 * @access public
	 * @param void
	 * @return boolean 
	 */
	abstract public function rollback();

	/**
	 * insertid
	 * @access public
	 * @param void
	 * @return integer
	 */
	abstract public function insertid();

	/**
	 * quote
	 * @access public
	* @param string $string
	 * @return string
	 */
	abstract public function quote($string);

	/**
	 * free
	 * @access public
	 * @param mixed $result
	 * @return void
	 */
	abstract public function free($result = null);

	/**
	 * close
	 * @access public
	 * @return void
	 */
	abstract public function close();

}

', true) .'</p>',
	),

	'dbpdo' => array(
		'node' => 'dbpdo',
		'prev' => 'database',
		'next' => '',
		'title' => '2.9 Yod_DbPdo',
		'intro' => 'Yod_DbPdo 是Yod的数据库PDO驱动类，实现了对PDO的简单封装.',
		'content' => '<p>'. highlight_string('<?php
class Yod_DbPdo extends Yod_Database
{
	/**
	 * __construct
	 * @access public
	 * @param array $config
	 * @return void
	 */
	public function __construct($config)
	{
	
	}

	/**
	 * connect
	 * @access public
	 * @param array $config
	 * @param integer $linknum
	 * @return mixed
	 */
	public function connect($config = null, $linknum = 0)
	{

	}

	/**
	 * fields
	 * @access public
	 * @param string $table
	 * @return array
	 */
	public function fields($table)
	{

	}

	/**
	 * execute
	 * @access public
	 * @param string $query
	 * @param array $params
	 * @param boolean $affected
	 * @return boolean
	 */
	public function execute($query, $params = array(), $affected = false)
	{

	}

	/**
	 * query
	 * @access public
	 * @param string $query
	 * @param array $params
	 * @return mixed
	 */
	public function query($query, $params = array())
	{

	}

	/**
	 * fetch
	 * @access public
	 * @param mixed $result
	 * @return mixed
	 */
	public function fetch($result = null)
	{

	}

	/**
	 * fetchAll
	 * @access public
	 * @param mixed $result
	 * @return mixed
	 */
	public function fetchAll($result = null)
	{

	}

	/**
	 * transaction
	 * @access public
	 * @param void
	 * @return boolean
	 */
	public function transaction()
	{

	}

	/**
	 * commit
	 * @access public
	 * @param void
	 * @return boolean
	 */
	public function commit()
	{

	}

	/**
	 * rollback
	 * @access public
	 * @param void
	 * @return boolean
	 */
	public function rollback()
	{

	}

	/**
	 * insertid
	 * @access public
	 * @param void
	 * @return integer
	 */
	public function insertid()
	{

	}

	/**
	 * quote
	 * @access public
	 * @param string $result
	 * @return string
	 */
	public function quote($string)
	{

	}

	/**
	 * free
	 * @access public
	 * @param mixed $result
	 * @return void
	 */
	public function free($result = null)
	{

	}

	/**
	 * close
	 * @access public
	 * @param void
	 * @return void
	 */
	public function close()
	{

	}

	/**
	 * errno
	 * @access public
	 * @param void
	 * @return mixed
	 */
	public function errno()
	{

	}

	/**
	 * error
	 * @access public
	 * @param void
	 * @return mixed
	 */
	public function error()
	{

	}
}

', true) .'</p>',
	),

);
