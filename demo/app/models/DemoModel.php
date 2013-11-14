<?php
class DemoModel extends Yod_Model
{
	//protected $_table = 'demo';

	public function init()
	{
		//$this->_dsn = $this->config('db_slave.2');
	}

	public function demo_list()
	{
		$sql = 'SELECT * FROM `yod_demo` WHERE 1 LIMIT 10';
		echo $this->_db->errorCode();
		$this->_db->query($sql);
		echo $this->_db->errorCode();
		return $this->_db->fetchAll();
	}
}