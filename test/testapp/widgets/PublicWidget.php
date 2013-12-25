<?php
class PublicWidget extends Yod_Widget
{
	public function headerAction()
	{
		$this->display();
	}

	public function footerAction()
	{
		$this->display();
	}

	public function widgetAction()
	{
		print_r($this);
	}

}
