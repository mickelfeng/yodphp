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
		if (empty($config)) {
			$config = $this->_config;
		}
		if ($this->_pconnect) {
			$config['params'][PDO::ATTR_PERSISTENT] = true;
		}
		if (empty($config['pdodsn'])) {
			$config['pdodsn'] = $this->_create_dsn($config);
		}
		try{
			$this->_linkids[$linknum] = new PDO($config['pdodsn'], $config['user'], $config['pass'], $config['params']);
		}catch (PDOException $e) {
			trigger_error($e->getMessage(), E_USER_WARNING);
			return false;
		}
		$this->_linkids[$linknum]->exec('SET NAMES '. $config['charset']);
		if (error_reporting()) {
			$this->_linkids[$linknum]->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_WARNING);
		}
		return $this->_linkids[$linknum];
	}

	/**
	 * query
	 * @access public
	 * @return mixed
	 */
	public function query($sql, $params = array())
	{
		if (empty($params)) {
			return $this->exec($sql);
		}
		return $this->execute($sql, $params);
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
		foreach ($params as $key => $value) {
			if (strstr($sql, $key)) {
				$bind_params[$key] = $value;
			}
		}
		$this->_queryid = $this->_linkid->prepare($sql);
		return $this->_queryid->execute($bind_params);
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
		if ($this->_queryid = $this->_linkid->query($sql)) {
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
		return $this->_linkid->beginTransaction();
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
	 * fetch
	 * @access public
	 * @return mixed
	 */
	public function fetch()
	{
		if ($this->_queryid) {
			return $this->_queryid->fetch(PDO::FETCH_ASSOC);
		}
		return false;
	}

	/**
	 * fetchAll
	 * @access public
	 * @return mixed
	 */
	public function fetchAll()
	{
		if ($this->_queryid) {
			return $this->_queryid->fetchAll(PDO::FETCH_ASSOC);
		}
		return array();
	}

	/**
	 * free
	 * @access public
	 * @return mixed
	 */
	public function free()
	{
		if ($this->_queryid) {
			$this->_queryid = null;
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
	 * errno
	 * @access public
	 * @return mixed
	 */
	public function errno()
	{
		if ($this->_queryid) {
			return $this->_queryid->errorCode();
		} elseif($this->_linkid) {
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
		if ($this->_queryid) {
			if ($error = $this->_queryid->errorInfo()) {
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

	private function _create_dsn($config){
		$pdodsn = array();
		$dbtype = substr($config['type'], 4);
		switch ($dbtype) {
			case 'mysql':
				$pdodsn[] = 'host=' . $config['host'];
				if ($config['port']) {
					$pdodsn[] = 'port=' . $config['port'];
				}
				$pdodsn[] = 'dbname=' . $config['dbname'];
				break;
			case 'sqlite':
				$pdodsn[] = $config['dbname'];
				break;
		}
		return $dbtype . ':' .implode(';', $pdodsn);
	}
}