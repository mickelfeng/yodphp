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

class Yod_DbMysqli extends Yod_Database
{
	protected $_result;
	protected $_fields;

	/**
	 * __construct
	 * @access public
	 * @return void
	 */
	public function __construct($dbconfig)
	{
		parent::__construct($dbconfig);

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
		if (empty($config)) {
			$config = $this->_config;
		}
		$config['port'] = empty($config['port']) ? 3306 : intval($config['port']);
		$this->_linkids[$linknum] = new mysqli($config['host'], $config['user'], $config['pass'], $config['dbname'], $config['port']);
		if (mysqli_connect_errno()) {
			trigger_error(mysqli_connect_error(), E_USER_WARNING);
			return false;
		}
		$dbversion = $this->_linkids[$linknum]->server_version;
		$this->_linkids[$linknum]->query("SET NAMES '{$config['charset']}'");
		if($dbversion >'5.0.1'){
			$this->_linkids[$linknum]->query("SET sql_mode=''");
		}
		if (error_reporting()) {
			mysqli_report(MYSQLI_REPORT_ERROR);
		}
		return $this->_linkids[$linknum];
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
	public function query($sql, $params = array())
	{
		if (empty($params)){
			return $this->exec($sql);
		}
		if ($query = $this->execute($sql, $params)) {
			if (method_exists($this->_queryid, 'get_result')){
				$this->_result = $this->_queryid->get_result();
			} else {
				$result = $this->_queryid->result_metadata();
				$fields = $result->fetch_fields();
				$this->_fields = array();
				foreach ($fields as $field) {
					$this->_fields[$field->name] = null;
				}
			}
		}
		return $query;
	}

	/**
	 * execute
	 * @access public
	 * @return boolean
	 */
	public function execute($sql, $params = array())
	{
		if (empty($params)) {
			return $this->exec($sql);
		}
		if (empty($this->_linkid)) {
			$this->_linkid = $this->connect();
		}
		$this->_lastsql = $sql;
		$bind_params = array();
		$bind_params[0] = '';
		foreach ($params as $key => $value) {
			if (strstr($sql, $key)) {
				$bind_params[0] .= 's';
				$bind_params[strpos($sql, $key)] = &$params[$key];
				$sql = str_replace($key, '?', $sql);
			}
		}
		ksort($bind_params);
		if ($this->_queryid) {
			$this->_queryid->close();
		}
		if ($this->_queryid = $this->_linkid->prepare($sql)) {
			if (count($bind_params) > 1) {
				call_user_func_array(array($this->_queryid, 'bind_param'), $bind_params);
			}
			return $this->_queryid->execute();
		}
		return false;
	}

	/**
	 * exec
	 * @access public
	 * @return mixed
	 */
	public function exec($sql)
	{
		if (empty($this->_linkid)) {
			$this->_linkid = $this->connect();
		}
		$this->_lastsql = $sql;
		if ($this->_queryid) {
			$this->_queryid->close();
		}
		if ($this->_result = $this->_linkid->query($sql)) {
			return true;
		}
		return false;
	}

	/**
	 * transaction
	 * @access public
	 * @return boolean 
	 */
	public function transaction()
	{
		if (empty($this->_linkid)) {
			$this->_linkid = $this->connect();
		}
		return $this->_linkid->begin_transaction();
	}

	/**
	 * commit
	 * @access public
	 * @return boolean 
	 */
	public function commit()
	{
		if (empty($this->_linkid)) {
			$this->_linkid = $this->connect();
		}
		return $this->_linkid->commit();
	}

	/**
	 * rollback
	 * @access public
	 * @return boolean 
	 */
	public function rollback()
	{
		if (empty($this->_linkid)) {
			$this->_linkid = $this->connect();
		}
		return $this->_linkid->rollback();
	}

	/**
	 * insertid
	 * @access public
	 * @return integer
	 */
	public function insertid()
	{
		return $this->_linkid->insert_id;
	}

	/**
	 * quote
	 * @access public
	 * @return string
	 */
	public function quote($string)
	{
		return $this->_linkid->real_escape_string($string);
	}

	/**
	 * fetch
	 * @access public
	 * @return mixed
	 */
	public function fetch($free = false)
	{
		if ($this->_result) {
			if ($fetch = $this->_result->fetch_assoc()) {
				return $fetch;
			} else {
				$this->free();
			}
		} elseif ($this->_fields) {
			$fetch = array();
			foreach ($this->_fields as $key => $value) {
				$fetch[$key] = null;
			}
			foreach ($this->_fields as $key => $value) {
				$this->_fields[$key] = &$fetch[$key];
			}
			call_user_func_array(array($this->_queryid, 'bind_result'), $this->_fields);
			if ($this->_queryid->fetch()) {
				return $this->_fields;
			} else {
				$this->free();
			}
		}
		if ($free) $this->free();
		return false;
	}

	/**
	 * free
	 * @access public
	 * @return mixed
	 */
	public function free()
	{
		if ($this->_result) {
			$this->_result->free();
		}
		$this->_result = null;
		$this->_fields = null;
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
				$linkid->close();
				unset($this->_linkids[$key]);
			}
		}
		if ($this->_linkid) {
			$this->_linkid = null;
		}
	}

	/**
	 * errno
	 * @access public
	 * @return mixed
	 */
	public function errno()
	{
		if ($this->_queryid) {
			return $this->_queryid->errno;
		} elseif ($this->_linkid) {
			return $this->_linkid->errno;
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
		if ($this->_queryid) {
			return $this->_queryid->error;
		} elseif ($this->_linkid) {
			return $this->_linkid->error;
		} else {
			return false;
		}
	}

}