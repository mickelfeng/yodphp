<?php
/**
 * RequestController
 */
class RequestController extends Yod_Controller
{
	public function indexAction()
	{
		$this->assign('title', 'Yod_Request');

		$this->display();
	}

	public function routeAction()
	{
		$this->assign('title', 'Yod_Request::route()');

		$this->_request->route('/c/a/p/v');

		$this->display('route', array('request' => $this->_request));
	}

	public function dispatchAction()
	{
		$this->assign('title', 'Yod_Request::dispatch()');

		$this->_request->route('/request/dispatch/dispatched/1');
		$this->_request->dispatch();

		$this->display('dispatch', array('request' => $this->_request));
	}

	public function erroractionAction()
	{
		$this->_request->errorAction();
	}

	public function error404Action()
	{
		$this->_request->error404();
	}
	
}
