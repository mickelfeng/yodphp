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

class Yod_DbMysql extends Yod_Database
{
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
		$server = $config['host'] . (empty($config['port']) ? '' : ":{$config['port']}");
		if ($this->_pconnect) {
			$this->_linkids[$linknum] = mysql_pconnect($server, $config['user'], $config['pass']);
		} else {
			$this->_linkids[$linknum] = mysql_connect($server, $config['user'], $config['pass']);
		}
		if (!$this->_linkids[$linknum] || (!empty($config['dbname']) && !mysql_select_db($config['dbname'], $this->_linkids[$linknum])) ) {
			trigger_error(mysql_error(), E_USER_WARNING);
			return false;
		}
		$dbversion = mysql_get_server_info($this->_linkids[$linknum]);
		mysql_query("SET NAMES '{$config['charset']}'", $this->_linkids[$linknum]);
		if($dbversion >'5.0.1'){
			mysql_query("SET sql_mode=''", $this->_linkids[$linknum]);
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
	public function query($query, $params = array())
	{
		if (empty($params)) {
			return $this->exec($query);
		}
		return $this->execute($query, $params);
	}

	/**
	 * execute
	 * @access public
	 * @return boolean
	 */
	public function execute($query, $params = array())
	{
		if (empty($params)) {
			return $this->exec($query);
		}
		if (empty($this->_linkid)) {
			$this->_linkid = $this->connect();
		}
		$this->_lastquery = $query;
		foreach ($params as $key => $value) {
			if (strstr($query, $key)) {
				$value = mysql_real_escape_string($value);
				$query = str_replace($key, "'{$value}'", $query);
			}
		}
		if ($this->_result = mysql_query($query, $this->_linkid)) {
			return $this->_result;
		}
		if (error_reporting()) {
			if ($error = mysql_error($this->_linkid)) {
				trigger_error($error, E_USER_WARNING);
			}
		}
		return false;
	}

	/**
	 * exec
	 * @access public
	 * @return boolean
	 */
	public function exec($query)
	{
		if (empty($this->_linkid)) {
			$this->_linkid = $this->connect();
		}
		$this->_lastquery = $query;
		if ($this->_result = mysql_query($query, $this->_linkid)) {
			return $this->_result;
		}
		if (error_reporting()) {
			if ($error = mysql_error($this->_linkid)) {
				trigger_error($error, E_USER_WARNING);
			}
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
		return $result ? mysql_fetch_assoc($result) : false;
	}

	/**
	 * free
	 * @access public
	 * @return mixed
	 */
	public function free($result = null)
	{
		if (is_null($result)) {
			$result = $this->_result;
		}
		return $result ? mysql_free_result($result) : false;
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
				mysql_close($linkid);
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
		if (empty($this->_linkid)) {
			$this->_linkid = $this->connect();
		}
		if (mysql_query('START TRANSACTION', $this->_linkid)) {
			return true;
		}
		return false;
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
		if (mysql_query('COMMIT', $this->_linkid)) {
			return true;
		}
		return false;
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
		if (mysql_query('ROLLBACK', $this->_linkid)) {
			return true;
		}
		return false;
	}

	/**
	 * insertid
	 * @access public
	 * @return integer
	 */
	public function insertid()
	{
		return mysql_insert_id($this->_linkid);
	}

	/**
	 * quote
	 * @access public
	 * @return string
	 */
	public function quote($string)
	{
		return mysql_real_escape_string($string, $this->_linkid);
	}

	/**
	 * errno
	 * @access public
	 * @return mixed
	 */
	public function errno()
	{
		if ($this->_linkid) {
			return mysql_errno($this->_linkid);
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
		if ($this->_linkid) {
			return mysql_error($this->_linkid);
		} else {
			return false;
		}
	}
}