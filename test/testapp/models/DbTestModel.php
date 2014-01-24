<?php
/**
 * DbTestModel
 */
class DbTestModel extends Yod_DbModel
{
	public function init()
	{
		$fields = array(
			'id' => 'int(11) NOT NULL AUTO_INCREMENT COMMENT \'ID\'',
			'title' => 'varchar(255) NOT NULL COMMENT \'标题\'',
			'content' => 'text DEFAULT NULL COMMENT \'内容\'',
			'updated' => 'int(11) NOT NULL DEFAULT \'0\' COMMENT \'更新时间\'',
			'created' => 'int(11) NOT NULL DEFAULT \'0\' COMMENT \'创建时间\'',
			'status' => 'tinyint(2) NOT NULL DEFAULT \'0\' COMMENT \'状态\'',
			'PRIMARY' => 'KEY (`id`)',
		);
		$extend = 'ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT=\'Demo\' AUTO_INCREMENT=1';
		$this->_db->create($fields, 'test', $extend);

		$data = array(
			'title' => 'Yodphp',
			'content' => 'Yod PHP Framework',
			'created' => time(),
		);
		$this->save($data);
	}

	public function test()
	{
		return $this->model()->select();
	}

	public function __destruct()
	{
		$this->_db->execute('DROP TABLE `yod_test`');
	}
}