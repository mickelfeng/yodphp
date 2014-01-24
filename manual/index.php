<?php
//set_time_limit(0);
error_reporting(E_ALL);
date_default_timezone_set('Asia/Shanghai');

//defined('YOD_RUNMODE') or define('YOD_RUNMODE', 3);
defined('YOD_PATHVAR') or define('YOD_PATHVAR', 'r');
defined('YOD_VIEWEXT') or define('YOD_VIEWEXT', '.tpl');
defined('YOD_RUNPATH') or define('YOD_RUNPATH', dirname(__FILE__));
defined('YOD_LOGPATH') or define('YOD_LOGPATH', dirname(__FILE__));

defined('APP_WEBROOT') or define('APP_WEBROOT', '/index.php');

//defined('APP_MANDATA') or define('APP_MANDATA', dirname(__FILE__) . '/manual.php');

// yodphp
class_exists('Yod_Application', false) or require YOD_RUNPATH . '/../yodphp/yodphp.php';

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

/**
 * IndexController
 */
class IndexController extends Yod_Controller
{
	protected $_data;
	protected $_link = array();

	public function init()
	{
		$this->_name = '';
		$this->_view['tpl_path'] = YOD_RUNPATH;

		$model = $this->model('Manual');
		$this->_data = $model->findByNode($this->_action) or $this->error404();

		$this->_link['title'] = $this->_data['title'];
		$this->_link['prev'] = $model->findByNode($this->_data['prev'], 'node, title');
		$this->_link['next'] = $model->findByNode($this->_data['next'], 'node, title');
	}

	public function indexAction()
	{
		$model = $this->model('Manual');
		$rows = $model->select();
		$this->_data['content'] = '<ul>';
		foreach ($rows as $data) {
			$this->_data['content'] .= '<li><p><a href="'. APP_WEBROOT .'/'. $data['node'] .'">'. $data['title'] .'</a></p></li>';
		}
		$this->_data['content'] .= '</ul>';
		
		$this->display('manual', $this->_data);
	}

	public function errorAction()
	{
		$this->_name = '';
		
		$this->display('manual', $this->_data);
	}
}

/**
 * ManualWidget
 */
class ManualWidget extends Yod_Widget
{
	public function init()
	{
		$this->_name = '';
		$this->_view['tpl_path'] = YOD_RUNPATH;
	}

	public function headerAction($link)
	{
		echo '<table width="100%"><tr>';
		echo '<th align="center" colspan="3">'. $link['title'] .'</th>';
		echo '</tr><tr>';
		echo '<td align="left" width="20%">'. (empty($link['prev']) ? '' : '<a accesskey="p" href="'. APP_WEBROOT .'/'. $link['prev']['node'] .'">上一页</a>') .'&nbsp;</td>';
		echo '<th align="center" width="60%">&nbsp;</th>';
		echo '<td align="right" width="20%">&nbsp;'. (empty($link['next']) ? '' : '<a accesskey="n" href="'. APP_WEBROOT .'/'. $link['next']['node'] .'">下一页</a>') .'</td>';
		echo '</tr></table>';
	}

	public function footerAction($link)
	{
		echo '<table width="100%"><tr>';
		echo '<td align="left" width="40%">'. (empty($link['prev']) ? '' : '<a accesskey="p" href="'. APP_WEBROOT .'/'. $link['prev']['node'] .'">上一页</a>') .'&nbsp;</td>';
		echo '<td align="center" width="20%">&nbsp;</td>';
		echo '<td align="right" width="40%">&nbsp;'. (empty($link['next']) ? '' : '<a accesskey="n" href="'. APP_WEBROOT .'/'. $link['next']['node'] .'">下一页</a>') .'</td>';
		echo '</tr><tr>';
		echo '<td valign="top" align="left" width="40%">'. (empty($link['prev']) ? '' : $link['prev']['title']) .'&nbsp;</td>';
		echo '<td align="center" width="20%"><a accesskey="h" href="'. APP_WEBROOT .'">Home</a></td>';
		echo '<td valign="top" align="right" width="40%">&nbsp;'. (empty($link['next']) ? '' : $link['next']['title']) .'</td>';
		echo '</tr></table>';
	}
}


/**
 * ManualModel
 */
class ManualModel extends Yod_DbModel
{
	public function select($where = '', $params = array(), $select = '*')
	{
		if (defined('APP_MANDATA') && is_file(APP_MANDATA)) {
			$data = include APP_MANDATA;
			return $data;
		}

		return parent::select($where, $params, $select);
	}

	public function findByNode($node, $fields = null)
	{
		if (defined('APP_MANDATA') && is_file(APP_MANDATA)) {
			$data = include APP_MANDATA;
			return isset($data[$node]) ? $data[$node] : false;
		}

		return $this->find('node = :node', array(':node' => $node), $fields);
	}
}
