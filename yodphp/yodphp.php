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
defined('YOD_DSNCONF') or define('YOD_DSNCONF', 'db_dsn');
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
	protected function model($name = '', $prefix = '', $config = '')
	{
		if (empty($name)) {
			$name = ucfirst($this->_name);
		}
		return Yod_Model::getInstance($name, $prefix, $config);
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
	protected $_dsn;

	protected $_name;
	protected $_table;
	protected $_prefix;

	protected $_query = array();
	protected $_params = array();

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
				if (get_class($this) == __CLASS__) {
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
			$this->_dsn = empty($this->_dsn) ? YOD_DSNCONF : strtolower($this->_dsn);
			$config = $this->config($this->_dsn);
		}
		if ($this->_db = Yod_Database::getInstance($config)) {
			$this->_prefix = $this->_db->config('prefix');
		}

		$this->initQuery();

		self::$_model[$this->_name] = $this;
	}

	/**
	 * getInstance
	 * @access public
	 * @param mixed $config
	 * @return Yod_Model
	 */
	public static function getInstance($name, $config = '')
	{
		$classname = $name .'Model';
		if (empty(self::$_model[$name])) {
			if (class_exists($classname)) {
				self::$_model[$name] = new $classname($name, $config);
			} else {
				self::$_model[$name] = new Yod_Model($name, $config);
			}
		}
		return self::$_model[$name];
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
	 * find
	 * @access public
	 * @return mixed
	 */
	public function find($where = null, $params = array())
	{
		if (is_array($where)) {
			$params = $where;
			$where = null;
		}
		empty($where) or $this->where($where);
		$query = $this->parseQuery();
		$params = array_merge($this->_params, $params);
		$this->initQuery();
		if ($result = $this->_db->query($query, $params)) {
			$data = $this->_db->fetch($result);
			$this->_db->free($result);
			return $data;
		}
		return false;
	}

	/**
	 * query
	 * @access public
	 * @return mixed
	 */
	public function query($where = null, $params = array())
	{
		if (is_array($where)) {
			$params = $where;
			$where = null;
		}
		empty($where) or $this->where($where);
		$query = $this->parseQuery();
		$params = array_merge($this->_params, $params);
		$this->initQuery();
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
	 * @return mixed
	 */
	public function count($where = null, $params = array())
	{
		if (is_array($where)) {
			$params = $where;
			$where = null;
		}
		$this->_query['SELECT'] = 'COUNT(*)';
		empty($where) or $this->where($where);
		$this->_query['LIMIT'] = 1;
		$query = $this->parseQuery();
		$params = array_merge($this->_params, $params);
		$this->initQuery();
		if ($result = $this->_db->query($query, $params)) {
			if ($data = $this->_db->fetch($result)) {
				$this->_db->free($result);
				return current($data);
			}
		}
		return 0;
	}

	/**
	 * save
	 * @access public
	 * @return boolean
	 */
	public function save($data, $where = null, $params = array())
	{
		if (empty($data)) return false;
		empty($where) or $this->where($where);
		if (empty($this->_query['WHERE'])) {
			return $this->_db->insert($data, $this->_table);
		} else {
			return $this->_db->update($data, $this->_table, $this->_query['WHERE'], $params);
		}
	}

	/**
	 * remove
	 * @access public
	 * @return boolean
	 */
	public function remove($where, $params = array())
	{
		empty($where) or $this->where($where);
		return $this->_db->delete($this->_table, $this->_query['WHERE'], $params);
	}

	/**
	 * select
	 * @access public
	 * @return Yod_Model
	 */
	public function select($select)
	{
		if (is_array($select)) {
			$select = implode(', ', $select);
		}
		$this->_query['SELECT'] = $select;
		return $this;
	}

	/**
	 * from
	 * @access public
	 * @return Yod_Model
	 */
	public function from($table)
	{
		$this->_query['FROM'] = "`{$this->_prefix}{$table}` AS `t1`";
		return $this;
	}

	/**
	 * join
	 * @access public
	 * @return Yod_Model
	 */
	public function join($table, $where, $mode = 'LEFT')
	{
		$alias = 't' . (count($this->_query['JOIN']) + 2);
		$this->_query['JOIN'][] = "{$mode} JOIN `{$this->_prefix}{$table}` AS `{$alias}` ON {$where}";
		return $this;
	}

	/**
	 * where
	 * @access public
	 * @return Yod_Model
	 */
	public function where($where)
	{
		if ($this->_query['WHERE']) {
			$where = "({$this->_query['WHERE']}) AND ({$where})";
		}
		$this->_query['WHERE'] = $where;
		return $this;
	}

	/**
	 * group
	 * @access public
	 * @return Yod_Model
	 */
	public function group($group)
	{
		$this->_query['GROUP BY'] = $group;
		return $this;
	}

	/**
	 * having
	 * @access public
	 * @return Yod_Model
	 */
	public function having($having)
	{
		$this->_query['HAVING'] = $having;
		return $this;
	}

	/**
	 * order
	 * @access public
	 * @return Yod_Model
	 */
	public function order($order)
	{
		$this->_query['ORDER BY'] = $order;
		return $this;
	}

	/**
	 * limit
	 * @access public
	 * @return Yod_Model
	 */
	public function limit($limit)
	{
		$this->_query['LIMIT'] = $limit;
		return $this;
	}

	/**
	 * union
	 * @access public
	 * @return Yod_Model
	 */
	public function union($union)
	{
		$this->_query['UNION'] = $union;
		return $this;
	}

	/**
	 * comment
	 * @access public
	 * @return Yod_Model
	 */
	public function comment($comment)
	{
		$this->_query['COMMENT'] = $comment;
		return $this;
	}

	/**
	 * params
	 * @access public
	 * @return Yod_Model
	 */
	public function params($params)
	{
		$this->_params = empty($params) ? array() : $params;
		return $this;
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
			$query['FROM'] = "`{$this->_prefix}{$this->_table}` AS `t1`";
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
	protected function model($name = '', $prefix = '', $config = '')
	{
		if (empty($name)) return $this;
		return self::getInstance($name, $prefix, $config);
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
	protected $_activeid = 0;
	protected $_lastquery = '';
	
	protected $_errno = 0;
	protected $_error = '';

	/**
	 * __construct
	 * @access public
	 * @return void
	 */
	public function __construct($config)
	{
		$this->_config = $config;
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
	 * getInstance
	 * @access public
	 * @param mixed $config
	 * @return Yod_Database
	 */
	public static function getInstance($config = null, $pconnect = false)
	{
		if (empty($config)) {
			$config = Yod_Application::app()->config(YOD_DSNCONF);
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
			self::$_db[$md5hash] = new $classname($config, $pconnect);
		}
		return self::$_db[$md5hash];
	}

	/**
	 * db
	 * @access public
	 * @param mixed $config
	 * @return Yod_Database
	 */
	public static function db($config = null)
	{
		return self::getInstance($config);
	}

	/**
	 * config
	 * @access public
	 * @param void
	 * @return array
	 */
	public function config($name = null, $value = null)
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
				$argv = func_get_args();
				array_shift($argv);
				$this->_config[$name] = $argv;
		}
	}

	/**
	 * active
	 * @access public
	 * @return mixed
	 */
	public function active($linknum)
	{
		if ($linknum == $this->_activeid) {
			return $this;
		}
		if (isset($this->_linkids[$linknum])) {
			$this->_linkid = $this->_linkids[$linknum];
			$this->_activeid = $linknum;
		} elseif($this->_activeid != 0) {
			$this->_linkid = $this->_linkids[0];
		}
		
		return $this;
	}

	/**
	 * create
	 * @access public
	 * @return void
	 */
	public function create($query, $params)
	{
		return $this->active(0)->execute($query, $params);
	}

	/**
	 * select
	 * @access public
	 * @return void
	 */
	public function select($query, $params)
	{
		return $this->active(1)->query($query, $params);
	}

	/**
	 * insert
	 * @access public
	 * @return void
	 */
	public function insert($data, $table, $replace=false) {
		if (empty($data) || empty($table)) return false;
		$values = $fields = $params = array();
		foreach ($data as $key => $value){
			if(is_scalar($value) || is_null($value)) {
				$name = ':'. md5($key);
				$fields[] =  $key;
				$values[] = $name;
				$params[$name] = $value;
			}
		}
		$query = ($replace ? 'REPLACE' : 'INSERT') . ' INTO `' . $this->_prefix . $table . '` (' . implode(',', $fields) . ') VALUES (' . implode(',', $values) . ')';

		return $this->active(0)->execute($query, $params);
	}

	/**
	 * update
	 * @access public
	 * @return void
	 */
	public function update($data, $table, $where = null, $params = array())
	{
		if (empty($data) || empty($table)) return false;
		$update = $params1 = array();
		foreach ($data as $key => $value){
			if(is_scalar($value) || is_null(($value))) {
				$name = ':'. md5($key);
				$update[] = $key.'='.$name;
				$params1[$name] = $value;
			}
		}
		$params = array_merge($params1, $params);
		$query = 'UPDATE `' . $this->_prefix . $table . '` SET ' .implode(',', $update) . (empty($where) ? '' : ' WHERE ' . $where);
		return $this->active(0)->execute($query, $params);
	}

	/**
	 * delete
	 * @access public
	 * @return void
	 */
	public function delete($table, $where = null, $params = array())
	{
		if (empty($table)) return false;
		$query = 'DELETE FROM `' . $this->_prefix . $table . '`' . (empty($where) ? '' : ' WHERE ' . $where);
		return $this->active(0)->execute($query, $params);
	}

	/**
	 * lastQuery
	 * @access public
	 * @return void
	 */
	public function lastQuery()
	{
		return $this->_lastquery;
	}

	/**
	 * connect
	 * @access public
	 * @param array $config
	 * @param integer $linknum
	 * @return mixed
	 */
	abstract public function connect($config = null, $linknum = 0)
	{

	}

	/**
	 * query
	 * @access public
	 * @return mixed
	 */
	abstract public function query($query, $params = array())
	{

	}

	/**
	 * fetch
	 * @access public
	 * @return mixed
	 */
	abstract public function fetch($result = null)
	{

	}

	/**
	 * fetchAll
	 * @access public
	 * @return mixed
	 */
	abstract public function fetchAll($result = null)
	{

	}

	/**
	 * transaction
	 * @access public
	 * @return boolean 
	 */
	abstract public function transaction()
	{

	}

	/**
	 * commit
	 * @access public
	 * @return boolean 
	 */
	abstract public function commit()
	{

	}

	/**
	 * rollback
	 * @access public
	 * @return boolean 
	 */
	abstract public function rollback()
	{

	}

	/**
	 * insertid
	 * @access public
	 * @return integer
	 */
	abstract public function insertid()
	{

	}

	/**
	 * quote
	 * @access public
	 * @return string
	 */
	abstract public function quote($string)
	{

	}

	/**
	 * free
	 * @access public
	 * @return mixed
	 */
	abstract public function free($result = null)
	{

	}

	/**
	 * close
	 * @access public
	 * @return mixed
	 */
	abstract public function close()
	{

	}

}
