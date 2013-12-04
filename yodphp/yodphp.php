<?php
// +----------------------------------------------------------------------
// | yodphp [ Yod PHP Framework ]
// +----------------------------------------------------------------------
// | Copyright (c) 2013 http://yodphp.duapp.com All rights reserved.
// +----------------------------------------------------------------------
// | Licensed ( http://www.apache.org/licenses/LICENSE-2.0 )
// +----------------------------------------------------------------------
// | Author: Baoqiang Su <zmrnet@qq.com>
// +----------------------------------------------------------------------

// yodphp constant
defined('YOD_RUNTIME') or define('YOD_RUNTIME', microtime(true));
defined('YOD_VERSION') or define('YOD_VERSION', '1.1.0');
defined('YOD_FORWARD') or define('YOD_FORWARD', 5);
defined('YOD_CHARSET') or define('YOD_CHARSET', 'utf-8');
defined('YOD_PATHVAR') or define('YOD_PATHVAR', '');
defined('YOD_EXTPATH') or define('YOD_EXTPATH', dirname(__FILE__));

// yodphp autorun
if (defined('YOD_RUNPATH')) {
	Yod_Application::app();
} else {
	define('YOD_RUNPATH', dirname(__FILE__));
}

/**
 * yod_autoload
 * 
 * @param string $classname
 */
function yod_autoload($classname)
{
	$classfile = $classname;
	// class name with namespace in PHP 5.3
	if (strpos($classname, '\\') !== false) {
		$classfile = str_replace('\\', '_', $classname);
	}

	if (substr($classfile, 0, 4) == 'Yod_') { // yodphp extends class
		if (strncmp($classfile, 'Yod_Db', 6) == 0) {
			$directory = '/drivers/';
		} else {
			$directory = '/extends/';
		}
		$classpath = YOD_EXTPATH . $directory . substr($classfile, 4) . '.class.php';
	} else {
		if (strncmp(substr($classname, -10), 'Controller', 10) == 0) {
			$directory = '/controllers/';
		} elseif (strncmp(substr($classname, -5), 'Model', 5) == 0) {
			$directory = '/models/';
		} else {
			$directory = '/extends/';
			$classfile = $classfile . '.class';
		}
		$classpath = YOD_RUNPATH . $directory . $classfile . '.php';
	}

	if (is_file($classpath)) include $classpath;

	return class_exists($classname, false) || interface_exists($classname, false);
}
spl_autoload_register('yod_autoload');

/**
 * Yod_Application
 * 
 */
final class Yod_Application
{
	protected static $_app;

	protected $_config = array();
	protected $_request = null;
	protected $_imports = array();
	protected $_running = false;

	/**
	 * __construct
	 * @access public
	 * @param mixed $config
	 * @return void
	 */
	public function __construct($config = null)
	{
		if (is_object(self::$_app)) {
			trigger_error('Only one application can be initialized', E_USER_ERROR);
			return;
		}

		// config
		if (is_array($config)) {
			$this->_config = $config;
		} else {
			if (!is_string($config)) {
				$config = YOD_RUNPATH . '/configs/config.php';
			}
			if (is_file($config)) {
				$this->_config = include($config);
			} else {
				$this->_config = array();
				$scandir = dirname($config);
				if (is_dir($scandir) && ($handle = opendir($scandir))) {
					while (($file = readdir($handle)) != false) {
						if (substr($file, -11) == '.config.php') {
							$key = substr($file, 0, -11);
							$value = include($scandir .'/'. $file);
							if (is_array($value)) {
								if ($key == 'base') {
									$this->_config = array_merge($this->_config, $value);
								}elseif (isset($this->_config[$key])){
									if (is_array($this->_config[$key])) {
										$value = array_merge($this->_config[$key], $value);
									}
									$this->_config[$key] = $value;
								}
							}
						}
					}
					closedir($handle);
				}
			}
		}
		if (isset($GLOBALS['config']) && is_array($GLOBALS['config'])) {
			$this->_config = array_merge($this->_config, $GLOBALS['config']);
		}

		// request
		$this->_request = new Yod_Request();

		self::$_app = $this;
	}

	/**
	 * run
	 * @access public
	 * @param void
	 * @return void
	 */
	public function run()
	{
		if ($this->_running) {
			trigger_error('An application instance already running', E_USER_WARNING);
			return;
		}
		$this->_running = true;

		// dispatch
		$this->_request->dispatch();
	}

	/**
	 * config
	 * @access public
	 * @param mixed
	 * @return array
	 */
	public function config($name = null)
	{
		if (is_null($name)) {
			return $this->_config;
		} elseif(isset($this->_config[$name])) {
			return $this->_config[$name];
		} elseif(strstr($name, '.')) {
			$name = explode('.', $name);
			$value = $this->_config;
			foreach ($name as $skey) {
				if (isset($value[$skey])) {
					$value = $value[$skey];
				} else {
					return null;
				}
			}
			return $value;
		} else {
			return null;
		}
	}

	/**
	 * import
	 * @access public
	 * @param string $alias
	 * @return boolean
	 */
	public function import($alias)
	{
		$classfile = trim(str_replace('\\', '/', str_replace('.', '/', $alias)), '/');
		$classname = basename($classfile);

		if (empty($this->_imports[$alias])) {
			$classpath = YOD_RUNPATH . '/extends/' . $classfile . '.class.php';
			if (is_file($classpath)) include $classpath;
			$this->_imports[$alias] = $classpath;
		}

		return class_exists($classname, false) || interface_exists($classname, false);
	}

	/**
	 * app
	 * @access public
	 * @param mixed $config
	 * @return Yod_Application
	 */
	public static function app($config = null)
	{
		if (self::$_app) {
			return self::$_app;
		}
		return new self($config);
	}

	/**
	 * __destruct
	 * @access public
	 * @param void
	 */
	public function __destruct()
	{
		if (!$this->_running) {
			$this->run();
		}
	}

}

/**
 * Yod_Request
 * 
 */
final class Yod_Request
{
	protected $_routed = false;

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
		if (isset($_SERVER['REQUEST_METHOD'])) {
			$this->method = $_SERVER['REQUEST_METHOD'];
		} else {
			if (!strncasecmp(PHP_SAPI, 'cli', 3)) {
				$this->method = 'Cli';
			} else {
				$this->method = 'Unknown';
			}
		}

		empty($route) or $this->route($route);
	}

	/**
	 * route
	 * @access public
	 * @param void
	 * @return void
	 */
	public function route($route = null)
	{
		if (empty($route)) {
			if (strtoupper($this->method) == 'CLI') {
				$route = isset($_SERVER['argv'][1]) ? $_SERVER['argv'][1] : '';
			} elseif (empty($_GET[YOD_PATHVAR])) {
				$route = empty($_SERVER['PATH_INFO']) ? '' : $_SERVER['PATH_INFO'];
			} else {
				$route = $_GET[YOD_PATHVAR];
			}
		}
		$route = str_replace('\\', '/', $route);
		$route = str_replace('//', '/', $route);
		$route = explode('/', trim($route, '/'));

		$controller = basename($_SERVER['SCRIPT_FILENAME'], '.php');
		$classname = $controller . 'Controller';
		if (class_exists($classname, false)) {
			array_unshift($route, $controller);
		}

		$this->controller = empty($route[0]) ? 'Index' : ucfirst(strtolower($route[0]));
		$this->action = empty($route[1]) ? 'index' : strtolower($route[1]);
		$this->params = array();
		$count = count($route);
		for ($i=2; $i<$count; $i+=2) {
			$_GET[$route[$i]] = empty($route[$i+1]) ? '' : $route[$i+1];
			$this->params[$route[$i]] = $_GET[$route[$i]];
		}

		$this->_routed = true;
	}

	/**
	 * dispatch
	 * @access public
	 * @param void
	 * @return void
	 */
	public function dispatch()
	{
		$this->_routed or $this->route();

		$controller = empty($this->controller) ? 'Index' : $this->controller;
		$classname = $controller . 'Controller';
		if (class_exists($classname, false)) {
			new $classname($this);
		} else {
			$classpath = YOD_RUNPATH . '/controllers/' . $controller . 'Controller.php';
			if (is_file($classpath)) {
				require $classpath;
				new $classname($this);
			} else {
				$action = empty($this->action) ? 'Index' : ucfirst($this->action);
				$classpath = YOD_RUNPATH . '/actions/' . strtolower($controller) . '/' . $action . 'Action.php';
				if (is_file($classpath)) {
					require $classpath;
					$classname = $action . 'Action';
					new $classname($this);
				} else {
					$classpath = YOD_RUNPATH . '/controllers/ErrorController.php';
					if (is_file($classpath)) {
						require $classpath;
						new ErrorController($this, 'error');
					} else {
						$this->errorAction();
					}
				}
			}
		}
	}

	/**
	 * errorAction
	 * @access public
	 * @return void
	 */
	public function errorAction()
	{
		$controller = empty($this->controller) ? 'index' : strtolower($this->controller);
		$classpath = YOD_RUNPATH . '/actions/' . $controller . '/ErrorAction.php';
		if (is_file($classpath)) {
			require $classpath;
			new ErrorAction($this);
		} else {
			$this->controller = 'Error';
			$classpath = YOD_RUNPATH . '/actions/ErrorAction.php';
			if (is_file($classpath)) {
				require $classpath;
				new ErrorAction($this);
			} else {
				$this->error404();
			}
		}
	}

	/**
	 * error404
	 * @access public
	 * @param mixed $html
	 * @return void
	 */
	public function error404($html = null)
	{
		header('HTTP/1.0 404 Not Found');
		if (is_null($html)) {
			if (strtoupper($this->method) == 'CLI') {
				$html = 'HTTP/1.0 404 Not Found';
			} else {
				$html = array(
					'<!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML 2.0//EN">',
					'<html><head>',
					'<title>404 Not Found</title>',
					'</head><body>',
					'<h1>Not Found</h1>',
					'<p>The requested URL ' . $_SERVER['PHP_SELF'] . ' was not found on this server.</p>',
					'</body></html>',
				);
				$html = implode(PHP_EOL, $html);
			}
		}
		exit($html);
	}
}

/**
 * Yod_Controller
 * 
 */
abstract class Yod_Controller
{
	protected static $_forward = 0;

	protected $_name;
	protected $_action;
	protected $_request;

	protected $_view = array(
		'tpl_data' => array(),
		'tpl_path' => '',
		'tpl_file' => '',
	);

	/**
	 * __construct
	 * @access public
	 * @param Yod_Request $request
	 * @param mixed $action
	 * @param mixed $params
	 * @return void
	 */
	public function __construct($request, $action = null)
	{
		$this->_name = strtolower($request->controller);
		$this->_action = empty($action) ? $request->action : strtolower($action);
		$this->_request = $request;
		$this->_view['tpl_path'] = YOD_RUNPATH . '/views';

		$this->init();
		$this->run();
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
		$this->_action = empty($this->_action) ? 'index' : strtolower($this->_action);
		$method = $this->_action . 'Action';
		if (method_exists($this, $method)) {
			call_user_func(array($this, $method), $this->_request->params);
		} else {
			$this->_name = empty($this->_name) ? 'index' : strtolower($this->_name);
			$classpath = YOD_RUNPATH . '/actions/' . $this->_name . '/' . $method . '.php';
			if (is_file($classpath)) {
				require $classpath;
				$action = new $method($this->_request);
				$action->run($this->_request->params);
			} elseif (method_exists($this, 'errorAction')) {
				$this->errorAction($this->_request->params);
			} else {
				$this->_request->errorAction();
			}
		}
	}

	/**
	 * config
	 * @access protected
	 * @param string $name
	 * @return array
	 */
	protected function config($name = null)
	{
		return Yod_Application::app()->config($name);
	}

	/**
	 * import
	 * @access protected
	 * @param string $alias
	 * @return boolean
	 */
	protected function import($alias)
	{
		return Yod_Application::app()->import($alias);
	}

	/**
	 * model
	 * @access protected
	 * @param string $name
	 * @return array
	 */
	protected function model($name = '', $config = '')
	{
		if (empty($name)) {
			$name = ucfirst($this->_name);
		}
		return Yod_Model::getInstance($name, $config);
	}

	/**
	 * dbmodel
	 * @access protected
	 * @param string $name
	 * @return array
	 */
	protected function dbmodel($name = '', $config = '')
	{
		if (empty($name)) {
			$name = ucfirst($this->_name);
		}
		return Yod_DbModel::getInstance($name, $config);
	}

	/**
	 * assign
	 * @access protected
	 * @param mixed $name
	 * @param mixed $value
	 * @return Yod_Controller
	 */
	protected function assign($name, $value = null)
	{
		if (is_array($name)) {
			foreach ($name as $key => $value) {
				$this->_view['tpl_data'][$key] = $value;
			}
		} elseif(is_string($name)) {
			$this->_view['tpl_data'][$name] = $value;
		}
		return $this;
	}

	/**
	 * render
	 * @access protected
	 * @param mixed $view
	 * @param array $data
	 * @return string
	 */
	protected function render($view = null, $data = array())
	{
		// tpl_file
		$view = empty($view) ? $this->_action : $view;
		if ($view) {
			$view = str_replace('..', '', $view);
			$view = str_replace('\\', '/', $view);
			$view = str_replace('//', '/', $view);
		} else {
			$view = 'index';
		}

		if (substr($view, 0, 1) != '/') {
			$view = '/' . $this->_name . '/' . $view;
		}
		if (empty($this->_view['tpl_path']) || !is_string($this->_view['tpl_path'])) {
			trigger_error('Unavailable property ' . __CLASS__ . '::$_view', E_USER_WARNING);
			return null;
		}
		$this->_view['tpl_file'] = $this->_view['tpl_path'] . strtolower($view) . '.php';
		// tpl_data
		$tpl_path = $this->_view['tpl_path'];
		unset($this->_view['tpl_data']['this'], $data['this']);
		extract($this->_view['tpl_data']);
		if (is_array($data)) extract($data);
		// response
		ob_start();
		include $this->_view['tpl_file'];
		$response = ob_get_contents();
		ob_end_clean();

		return $response;
	}

	/**
	 * display
	 * @access protected
	 * @param mixed $view
	 * @param array $data
	 * @return void
	 */
	protected function display($view = null, $data = array())
	{
		headers_sent() or header('Content-type: text/html; charset=' . YOD_CHARSET);
		echo $this->render($view, $data);
	}

	/**
	 * widget
	 * @access protected
	 * @param string $route
	 * @return void
	 */
	protected function widget($route)
	{
		$route = str_replace('\\', '/', $route);
		$route = str_replace('//', '/', $route);
		$route = explode('/', trim($route, '/'));

		$widget = empty($route[0]) ? 'Index' : ucfirst(strtolower($route[0]));
		$action = empty($route[1]) ? 'index' : strtolower($route[1]);
		$params = array();
		$count = count($route);
		for ($i=2; $i<$count; $i+=2) {
			$params[$route[$i]] = empty($route[$i+1]) ? '' : $route[$i+1];
		}

		$classname = $widget . 'Widget';
		if (class_exists($classname, false)) {
			new $classname($this->_request, $action, $params);
		} else {
			$classpath = YOD_RUNPATH . '/widgets/' . $widget . 'Widget.php';
			if (is_file($classpath)) {
				require $classpath;
				new $classname($this->_request, $action, $params);
			} else {
				trigger_error("Widget '{$widget}Widget' not found", E_USER_WARNING);
			}
		}

	}

	/**
	 * forward
	 * @access protected
	 * @param string $route
	 * @return void
	 */
	protected function forward($route, $exited = false)
	{
		if (self::$_forward++ > YOD_FORWARD) return;

		if (strpos($route,'/') === false) {
			$this->_action = strtolower($route);
			$this->run();
		} else {
			$request = new Yod_Request($route);
			$request->dispatch();
		}

		if ($exited) exit;
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
		headers_sent() or header('Location:' . $url, true, $code);
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
		$this->_request->error404($html);
	}

	/**
	 * __destruct
	 * 
	 * @return void
	 */
	public function __destruct() {

	}
}

/**
 * Yod_Action
 * 
 */
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

/**
 * Yod_Widget
 * 
 */
abstract class Yod_Widget extends Yod_Controller
{
	/**
	 * __construct
	 * @access public
	 * @param Yod_Request $request
	 * @param mixed $action
	 * @param mixed $params
	 * @return void
	 */
	public function __construct($request, $action = null, $params = null)
	{
		$name = get_class($this);
		$this->_name = strtolower(substr($name, 0, -6));
		$this->_action = empty($action) ? 'index' : strtolower($action);
		$this->_request = $request;
		$this->_view['tpl_path'] = YOD_RUNPATH . '/widgets';

		$this->init();

		$this->_action = empty($this->_action) ? 'index' : strtolower($this->_action);
		$method = $this->_action . 'Action';
		if (method_exists($this, $method)) {
			call_user_func(array($this, $method), $params);
		} else {
			trigger_error('Unavailable action ' . $name . '::' . $method . '()', E_USER_WARNING);
		}
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

/**
 * Yod_Model
 * 
 */
class Yod_Model
{
	protected static $_model = array();

	protected $_db;
	protected $_dsn = 'db_dsn';

	protected $_name;
	protected $_table;
	protected $_prefix;


	/**
	 * __construct
	 * @access public
	 * @return void
	 */
	public function __construct($name = '', $config = '')
	{
		$this->init();

		if (empty($name)) {
			if (empty($this->_name)) {
				if (get_class($this) == get_called_class()) {
					$this->_name = '';
				} else {
					$this->_name = substr(get_class($this), 0, -5);
				}
			}
		} else {
			$this->_name = $name;
		}
		if (empty($this->_table)) {
			$this->_table = strtolower(trim(preg_replace('/[A-Z]/', '_\\0', $this->_name), '_'));
		}

		if (empty($config)) {
			$config = $this->config($this->_dsn);
		}
		if ($this->_db = Yod_Database::getInstance($config)) {
			$this->_prefix = $this->_db->config('prefix');
		}
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
	public static function getInstance($name, $config = '')
	{
		$modelname = get_called_class();
		$classname = ucfirst(strtolower($name)) . (($modelname == __CLASS__) ? 'Model' : 'DbModel');
		if (empty(self::$_model[$classname])) {
			$classpath = YOD_EXTPATH . '/models/' . $classname . '.class.php';
			if (is_file($classpath)) {
				include $classpath;
				self::$_model[$classname] = new $classname($name, $config);
			} else {
				if (class_exists($classname, false)) {
					self::$_model[$classname] = new $classname($name, $config);
				} else {
					self::$_model[$classname] = new $modelname($name, $config);
				}
			}
		}

		return self::$_model[$classname];
	}

	/**
	 * find
	 * @access public
	 * @param string	$where
	 * @param array		$params
	 * @param mixed		$select
	 * @return mixed
	 */
	public function find($where = '', $params = array(), $select = '*')
	{
		if (is_array($select)) {
			foreach ($select as $key => $value) {
				if (is_string($key)) {
					$select[$key] = "{$key} AS {$value}"; 
				}
			}
			$select = implode(', ', $select);
		}

		$query = 'SELECT ' . (empty($select) ? '*' : $select) . ' FROM ' . $this->_prefix . $this->_table . (empty($where) ? '' : ' WHERE ' . $where);
		if ($result = $this->_db->query($query, $params)) {
			$data = $this->_db->fetch($result);
			$this->_db->free($result);
			return $data;
		}

		return false;
	}

	/**
	 * findAll
	 * @access public
	 * @param string	$where
	 * @param array		$params
	 * @param mixed		$select
	 * @return mixed
	 */
	public function findAll($where = '', $params = array(), $select = '*')
	{

		if (is_array($select)) {
			foreach ($select as $key => $value) {
				if (is_string($key)) {
					$select[$key] = "{$key} AS {$value}"; 
				}
			}
			$select = implode(', ', $select);
		}

		$query = 'SELECT ' . (empty($select) ? '*' : $select) . ' FROM ' . $this->_prefix . $this->_table . (empty($where) ? '' : ' WHERE ' . $where);
		if ($result = $this->_db->query($query, $params)) {
			$data = $this->_db->fetchAll($result);
			$this->_db->free($result);
			return $data;
		}

		return false;
	}

	/**
	 * count
	 * @access public
	 * @param string	$where
	 * @param array		$params
	 * @return integer
	 */
	public function count($where = '', $params = array())
	{
		$query = 'SELECT COUNT(*) FROM ' . $this->_prefix . $this->_table . (empty($where) ? '' : ' WHERE ' . $where);
		if ($result = $this->_db->query($query, $params)) {
			$count = 0;
			if ($data = $this->_db->fetch($result)) {
				$count = current($data);
			}
			$this->_db->free($result);
			return $count;
		}

		return 0;
	}

	/**
	 * save
	 * @access public
	 * @return boolean
	 */
	public function save($data, $where = '', $params = array())
	{
		if (empty($data)) return false;

		if (empty($where)) {
			return $this->_db->insert($data, $this->_table);
		} else {
			return $this->_db->update($data, $this->_table, $where, $params);
		}
	}

	/**
	 * remove
	 * @access public
	 * @return boolean
	 */
	public function remove($where, $params = array())
	{
		return $this->_db->delete($this->_table, $where, $params);
	}

	/**
	 * lastQuery
	 * @access public
	 * @return string
	 */
	public function lastQuery()
	{
		return $this->_db->lastQuery();
	}

	/**
	 * config
	 * @access protected
	 * @param void
	 * @return array
	 */
	protected function config($name = null)
	{
		return Yod_Application::app()->config($name);
	}
	
	/**
	 * import
	 * @access protected
	 * @param string $alias
	 * @return boolean
	 */
	protected function import($alias)
	{
		return Yod_Application::app()->import($alias);
	}

	/**
	 * model
	 * @access protected
	 * @param string $name
	 * @return array
	 */
	protected function model($name = '', $config = '')
	{
		if (empty($name)) return $this;
		return self::getInstance($name, $config);
	}
}

/**
 * Yod_DbModel
 * 
 */
class Yod_DbModel extends Yod_Model
{

	protected $_query = array();
	protected $_params = array();

	/**
	 * __construct
	 * @access public
	 * @return void
	 */
	public function __construct($name = '', $config = '')
	{
		parent::__construct($name, $config);

		$this->initQuery();
	}

	/**
	 * find
	 * @access public
	 * @param string	$where
	 * @param array		$params
	 * @param mixed		$select
	 * @return mixed
	 */
	public function find($where = '', $params = array(), $select = '*')
	{
		$query = $this->select($select)->where($where, $params)->parseQuery();
		if ($result = $this->_db->query($query, $this->_params)) {
			$data = $this->_db->fetch($result);
			$this->_db->free($result);
			$this->initQuery();
			return $data;
		}

		return false;
	}

	/**
	 * findAll
	 * @access public
	 * @param string	$where
	 * @param array		$params
	 * @param mixed		$select
	 * @return mixed
	 */
	public function findAll($where = '', $params = array(), $select = '*')
	{
		$query = $this->select($select)->where($where, $params)->parseQuery();
		if ($result = $this->_db->query($query, $this->_params)) {
			$data = $this->_db->fetchAll($result);
			$this->_db->free($result);
			$this->initQuery();
			return $data;
		}

		return false;
	}

	/**
	 * count
	 * @access public
	 * @param string	$where
	 * @param array		$params
	 * @return integer
	 */
	public function count($where = '', $params = array())
	{
		$query = $this->select('COUNT(*)')->where($where, $params)->parseQuery();
		if ($result = $this->_db->query($query, $this->_params)) {
			$count = 0;
			if ($data = $this->_db->fetch($result)) {
				$count = current($data);
			}
			$this->_db->free($result);
			$this->initQuery();
			return $count;
		}

		return 0;
	}

	/**
	 * save
	 * @access public
	 * @return boolean
	 */
	public function save($data, $where = '', $params = array())
	{
		if (empty($data)) return false;
		$this->where($where, $params);
		if (empty($this->_query['WHERE'])) {
			$result = $this->_db->insert($data, $this->_table);
		} else {
			$result = $this->_db->update($data, $this->_table, $this->_query['WHERE'], $this->_params);
		}
		$this->initQuery();
		return $result;
	}

	/**
	 * remove
	 * @access public
	 * @return boolean
	 */
	public function remove($where, $params = array())
	{
		$this->where($where, $params);
		$result = $this->_db->delete($this->_table, $this->_query['WHERE'], $this->_params);
		$this->initQuery();
		return $result;
	}

	/**
	 * select
	 * @access public
	 * @return Yod_DbModel
	 */
	public function select($select)
	{
		if (is_array($select)) {
			foreach ($select as $key => $value) {
				if (is_string($key)) {
					$select[$key] = "{$key} AS {$value}"; 
				}
			}
			$select = implode(', ', $select);
		}
		if ($select) {
			$this->_query['SELECT'] = $select;
		}
		return $this;
	}

	/**
	 * from
	 * @access public
	 * @return Yod_DbModel
	 */
	public function from($table = null)
	{
		if ($table) {
			$this->_query['FROM'] = "{$this->_prefix}{$table} AS t1";
		}
		return $this;
	}

	/**
	 * join
	 * @access public
	 * @return Yod_Database
	 */
	public function join($table, $where, $mode = 'LEFT')
	{
		$alias = 't' . (count($this->_query['JOIN']) + 2);
		$this->_query['JOIN'][] = "{$mode} JOIN {$this->_prefix}{$table} AS {$alias} ON {$where}";
		return $this;
	}

	/**
	 * where
	 * @access public
	 * @return Yod_Database
	 */
	public function where($where, $params = array(), $mode = 'AND')
	{
		if ($where) {
			if (is_string($params)) {
				$mode = $params;
			} else {
				$this->params($params);
			}
			if ($this->_query['WHERE']) {
				$where = "({$this->_query['WHERE']}) {$mode} ({$where})";
			}
			$this->_query['WHERE'] = $where;
		}
		return $this;
	}

	/**
	 * group
	 * @access public
	 * @return Yod_Database
	 */
	public function group($group)
	{
		$this->_query['GROUP BY'] = $group;
		return $this;
	}

	/**
	 * having
	 * @access public
	 * @return Yod_Database
	 */
	public function having($having)
	{
		$this->_query['HAVING'] = $having;
		return $this;
	}

	/**
	 * order
	 * @access public
	 * @return Yod_Database
	 */
	public function order($order)
	{
		$this->_query['ORDER BY'] = $order;
		return $this;
	}

	/**
	 * limit
	 * @access public
	 * @return Yod_Database
	 */
	public function limit($limit)
	{
		$this->_query['LIMIT'] = $limit;
		return $this;
	}

	/**
	 * union
	 * @access public
	 * @return Yod_Database
	 */
	public function union($union)
	{
		$this->_query['UNION'] = $union;
		return $this;
	}

	/**
	 * comment
	 * @access public
	 * @return Yod_Database
	 */
	public function comment($comment)
	{
		$this->_query['COMMENT'] = $comment;
		return $this;
	}

	/**
	 * params
	 * @access public
	 * @return Yod_Database
	 */
	public function params($params)
	{
		if (is_array($params)) {
			if (is_array($this->_params)) {
				$params = array_merge($this->_params, $params);
			}
			$this->_params = $params;
		}
		return $this;
	}

	/**
	 * parseQuery
	 * @access protected
	 * @return string
	 */
	protected function parseQuery($query = null)
	{
		$parse = array();
		$query = empty($query) ? $this->_query : $query;
		if (empty($query['FROM'])) {
			$query['FROM'] = "{$this->_prefix}{$this->_table} AS t1";
		}
		foreach ($query as $key => $value) {
			if (empty($value)) {
				continue;
			}
			if (is_array($value)) {
				if ($key == 'UNION') {
					$value = $this->parseQuery($value);
				} elseif ($key == 'JOIN') {
					$value = implode(' ', $value);
					$key = '';
				}
			}
			$parse[] = $key . ' ' . $value;
		}

		return implode(' ', $parse);
	}

	/**
	 * initQuery
	 * @access protected
	 * @return string
	 */
	protected function initQuery()
	{
		$this->_query = array(
			'SELECT' => '*',
			'FROM' => '',
			'JOIN' => array(),
			'WHERE' => '',
			'GROUP BY' => '',
			'HAVING' => '',
			'ORDER BY' => '',
			'LIMIT' => '',
			'UNION' => '',
			'COMMENT' => '',
		);
		$this->_params = array();

		return $this;
	}

}

/**
 * Yod_Database
 * 
 */
abstract class Yod_Database
{
	protected static $_db = array();

	protected $_config;
	protected $_driver;
	protected $_prefix;
	protected $_result;
	protected $_linkid;
	protected $_linkids = array();
	protected $_islocked = false;
	protected $_lastquery = '';


	/**
	 * __construct
	 * @access public
	 * @return void
	 */
	public function __construct($config)
	{
		$this->_config = $config;
		$this->_prefix = empty($config['prefix']) ? '' : $config['prefix']; 
	}

	/**
	 * __construct
	 * @access public
	 * @return void
	 */
	public function __destruct()
	{
		$this->close();
	}

	/**
	 * db
	 * @access public
	 * @param mixed $config
	 * @return Yod_Database
	 */
	public static function db($config = 'db_dsn')
	{
		return self::getInstance($config);
	}

	/**
	 * getInstance
	 * @access public
	 * @param mixed $config
	 * @return Yod_Database
	 */
	public static function getInstance($config = 'db_dsn')
	{
		if (is_string($config)) {
			$config = Yod_Application::app()->config($config);
		}

		if (empty($config['type'])) {
			return false;
		} elseif ($config['type'] == 'pdo') {
			$classname = 'Yod_DbPdo';
		} else {
			$classname = 'Yod_Db'.ucwords($config['type']);
		}

		$md5hash = md5(serialize($config));
		if (empty(self::$_db[$md5hash])) {
			if (!class_exists($classname, false)) {
				include YOD_EXTPATH . '/drivers/' . substr($classname, 4) . '.class.php';
			}
			self::$_db[$md5hash] = new $classname($config);
		}
		return self::$_db[$md5hash];
	}

	/**
	 * config
	 * @access public
	 * @param void
	 * @return array
	 */
	public function config($name = '', $value = null)
	{
		$argc = func_num_args();
		switch ($argc) {
			case 0:
				return $this->_config;
				break;
			case 1:
				return isset($this->_config[$name]) ? $this->_config[$name] : null;
				break;
			case 2:
				if (is_null($value)) {
					unset($this->_config[$name]);
				} else {
					$this->_config[$name] = $value;
				}
				break;
			default :
				if ($name) {
					$argv = func_get_args();
					array_shift($argv);
					$this->_config[$name] = $argv;
				}
		}
	}

	/**
	 * create
	 * @access public
	 * @return mixed
	 */
	public function create($fields, $table, $extend = '')
	{
		if (empty($fields) || empty($table)) return false;

		foreach ($fields as $key => $value) {
			$fields[$key] = $key . ' ' . $value;
		}
		$query = 'CREATE TABLE ' . $this->_prefix . $table . ' (' . implode(', ', $fields) . ')' . $extend;

		return $this->execute($query);
	}

	/**
	 * insert
	 * @access public
	 * @return mixed
	 */
	public function insert($data, $table, $replace=false)
	{
		if (empty($data) || empty($table)) return false;

		$values = $fields = $params = array();
		foreach ($data as $key => $value) {
			if(is_scalar($value) || is_null($value)) {
				$name = ':'. md5($key);
				$fields[] =  $key;
				$values[] = $name;
				$params[$name] = $value;
			}
		}
		$query = ($replace ? 'REPLACE' : 'INSERT') . ' INTO ' . $this->_prefix . $table . ' (' . implode(', ', $fields) . ') VALUES (' . implode(',', $values) . ')';
		return $this->execute($query, $params);
	}

	/**
	 * update
	 * @access public
	 * @return integer
	 */
	public function update($data, $table, $where = null, $params = array())
	{
		if (empty($data) || empty($table)) return false;

		$update = array();
		foreach ($data as $key => $value) {
			if(is_scalar($value) || is_null(($value))) {
				$name = ':'. md5($key);
				$update[] = $key.'='.$name;
				$params[$name] = $value;
			}
		}
		$query = 'UPDATE ' . $this->_prefix . $table . ' SET ' .implode(', ', $update) . (empty($where) ? '' : ' WHERE ' . $where);
		return $this->execute($query, $params);
	}

	/**
	 * delete
	 * @access public
	 * @return integer
	 */
	public function delete($table, $where = null, $params = array())
	{
		if (empty($table)) return false;

		$query = 'DELETE FROM ' . $this->_prefix . $table . (empty($where) ? '' : ' WHERE ' . $where);
		return $this->execute($query, $params);
	}

	/**
	 * select
	 * @access public
	 * @return mixed
	 */
	public function select($select = '*', $table, $where = null, $params = array())
	{
		if (empty($table)) return false;

		if (is_array($select)) {
			foreach ($select as $key => $value) {
				if (is_string($key)) {
					$select[$key] = "{$key} AS {$value}"; 
				}
			}
			$select = implode(', ', $select);
		}
		$query = 'SELECT ' . (empty($select) ? '*' : $select) . ' FROM ' . $this->_prefix . $table . (empty($where) ? '' : ' WHERE ' . $where);
		return $this->query($query, $params);
	}

	/**
	 * lastQuery
	 * @access public
	 * @return string
	 */
	public function lastQuery()
	{
		return $this->_lastquery;
	}

	/**
	 * dbconfig
	 * @access protected
	 * @return void
	 */
	protected function dbconfig($config, $linknum = 0)
	{

		if (empty($config)) {
			$config = $this->_config;
		}
		if ($this->_islocked) {
			$linknum = 0;
		}
		if ($linknum == 1) {
			if (empty($this->_config['slaves'])) {
				$linknum = 0;
			} elseif(is_array($this->_config['slaves'])) {
				if (isset($this->_config['slaves']['dsn'])) {
					$slaves = $this->_config['slaves'];
				} else {
					$k_rand = array_rand($this->_config['slaves'], 1);
					$slaves = $this->_config['slaves'][$k_rand];
				}
				$config = array_merge($config, $slaves);
			}
		}
		if (is_array($config)) {
			$config['linknum'] = $linknum;
		}
		return $config;
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
	 * execute
	 * @access public
	 * @return mixed
	 */
	abstract public function execute($query, $params = array());

	/**
	 * query
	 * @access public
	 * @return mixed
	 */
	abstract public function query($query, $params = array());

	/**
	 * fetch
	 * @access public
	 * @return mixed
	 */
	abstract public function fetch($result = null);

	/**
	 * fetchAll
	 * @access public
	 * @return mixed
	 */
	abstract public function fetchAll($result = null);

	/**
	 * transaction
	 * @access public
	 * @return boolean 
	 */
	abstract public function transaction();

	/**
	 * commit
	 * @access public
	 * @return boolean 
	 */
	abstract public function commit();

	/**
	 * rollback
	 * @access public
	 * @return boolean 
	 */
	abstract public function rollback();

	/**
	 * insertid
	 * @access public
	 * @return integer
	 */
	abstract public function insertid();

	/**
	 * quote
	 * @access public
	 * @return string
	 */
	abstract public function quote($string);

	/**
	 * free
	 * @access public
	 * @return mixed
	 */
	abstract public function free($result = null);

	/**
	 * close
	 * @access public
	 * @return mixed
	 */
	abstract public function close();

}


class Yod_DbPdo extends Yod_Database
{
	/**
	 * __construct
	 * @access public
	 * @param mixed $config
	 * @return void
	 */
	public function __construct($config)
	{
		parent::__construct($config);

		$this->_driver = __CLASS__;
		
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
		$config = $this->dbconfig($config, $linknum);
		$linknum = isset($config['linknum']) ? $config['linknum'] : 0;
		if (isset($this->_linkids[$linknum])) {
			return $this->_linkid = $this->_linkids[$linknum];
		}
		if (empty($config['dsn'])) {
			trigger_error('Database DSN configure error', E_USER_ERROR);
			return false;
		}
		$config['user'] = empty($config['user']) ? null : $config['user'];
		$config['pass'] = empty($config['pass']) ? null : $config['pass'];
		$config['charset'] = empty($config['charset']) ? 'utf8' : $config['charset'];
		$config['options'] = empty($config['options']) ? array() : $config['options'];
		if (isset($config['pconnect']) && $config['pconnect'] == true) {
			$config['options'][PDO::ATTR_PERSISTENT] = true;
		}
		try {
			$this->_linkids[$linknum] = new PDO($config['dsn'], $config['user'], $config['pass'], $config['options']);
		} catch (PDOException $e) {
			trigger_error($e->getMessage(), E_USER_ERROR);
			return false;
		}

		$this->_linkids[$linknum]->exec('SET NAMES '. $config['charset']);
		if (error_reporting()) {
			$this->_linkids[$linknum]->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_WARNING);
		}
		return $this->_linkid = $this->_linkids[$linknum];
	}

	/**
	* fields
	* @access public
	* @return array
	*/
	public function fields($table) {
		$fields = array();
		if ($result = $this->query('SHOW COLUMNS FROM '.$table)) {
			foreach ($result as $key => $value) {
				$fields[$value['Field']] = array(
					'name'    => $val['Field'],
					'type'    => $val['Type'],
					'notnull' => (bool) ($val['Null'] === ''), // not null is empty, null is yes
					'default' => $val['Default'],
					'primary' => (strtolower($val['Key']) == 'pri'),
					'autoinc' => (strtolower($val['Extra']) == 'auto_increment'),
				);
			}
		}
		return $fields;
	}

	/**
	 * query
	 * @access public
	 * @return mixed
	 */
	public function query($query, $params = array())
	{
		$this->connect($this->_config, 1);

		$this->_lastquery = $query;
		if (empty($params)) {
			return $this->_result = $this->_linkid->query($query);
		} else {
			$bind_params = array();
			foreach ($params as $key => $value) {
				if (strstr($query, $key)) {
					$bind_params[$key] = $value;
				}
			}
			if ($this->_result = $this->_linkid->prepare($query)) {
				$this->_result->execute($bind_params);
				return $this->_result;
			}
		}

		return false;
	}

	/**
	 * execute
	 * @access public
	 * @return boolean
	 */
	public function execute($query, $params = array())
	{
		$this->connect($this->_config, 0);

		$this->_lastquery = $query;
		if (empty($params)) {
			return $this->_linkid->exec($query);
		}
		$bind_params = array();
		foreach ($params as $key => $value) {
			if (strstr($query, $key)) {
				$bind_params[$key] = $value;
			}
		}
		if ($this->_result = $this->_linkid->prepare($query)) {
			$this->_result->execute($bind_params);
			return $this->_result->rowCount();
		}
		return false;
	}

	/**
	 * fetch
	 * @access public
	 * @return mixed
	 */
	public function fetch($result = null)
	{
		if (is_null($result)) {
			$result = $this->_result;
		}
		if (is_object($result)) {
			return $result->fetch(PDO::FETCH_ASSOC);
		}
		return false;
	}

	/**
	 * fetchAll
	 * @access public
	 * @return mixed
	 */
	public function fetchAll($result = null)
	{
		if (is_null($result)) {
			$result = $this->_result;
		}
		if (is_object($result)) {
			return $result->fetchAll(PDO::FETCH_ASSOC);
		}
		return array();
	}

	/**
	 * free
	 * @access public
	 * @return mixed
	 */
	public function free($result = null)
	{
		if (is_null($result)) {
			$this->_result = null;
		} else {
			$result = null;
		}
	}

	/**
	 * close
	 * @access public
	 * @return mixed
	 */
	public function close()
	{
		foreach ($this->_linkids as $key => $linkid) {
			if ($linkid) {
				unset($this->_linkids[$key]);
			}
		}
		if ($this->_linkid) {
			$this->_linkid = null;
		}
	}

	/**
	 * transaction
	 * @access public
	 * @return boolean 
	 */
	public function transaction()
	{
		$this->_islocked = true;

		$this->connect($this->_config, 0);

		return $this->_linkid->beginTransaction();
	}

	/**
	 * commit
	 * @access public
	 * @return boolean 
	 */
	public function commit()
	{
		$this->_islocked = false;
		$this->connect($this->_config, 0);
		return $this->_linkid->commit();
	}

	/**
	 * rollback
	 * @access public
	 * @return boolean 
	 */
	public function rollback()
	{
		$this->_islocked = false;
		$this->connect($this->_config, 0);
		return $this->_linkid->rollBack();
	}

	/**
	 * insertid
	 * @access public
	 * @return integer
	 */
	public function insertid()
	{
		return $this->_linkid->lastInsertId();
	}

	/**
	 * quote
	 * @access public
	 * @return string
	 */
	public function quote($string)
	{
		return $this->_linkid->quote($string);
	}

	/**
	 * errno
	 * @access public
	 * @return mixed
	 */
	public function errno()
	{
		if ($this->_result) {
			return $this->_result->errorCode();
		} elseif ($this->_linkid) {
			return $this->_linkid->errorCode();
		} else {
			return false;
		}
	}

	/**
	 * errer
	 * @access public
	 * @return mixed
	 */
	public function error()
	{
		if ($this->_result) {
			if ($error = $this->_result->errorInfo()) {
				return $error[2];
			}
		} elseif ($this->_linkid) {
			if ($error = $this->_linkid->errorInfo()) {
				return $error[2];
			}
		} else {
			return false;
		}
	}
}