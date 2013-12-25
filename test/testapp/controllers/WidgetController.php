<?php
/**
 * WidgetController
 */
class WidgetController extends Yod_Controller
{
	public function indexAction()
	{
		$this->assign('title', 'Yod_Widget');

		$this->display();
	}
}
