<?php
class PublicWidget extends Yod_Widget
{
	public function headerAction()
	{
		$this->display('header', array(
			'yodphp' => '<sup>Beta</sup>'
		));
	}

	public function footerAction()
	{
		$this->display('footer', array(
			'footer' => 'Copyright'
		));
	}
}