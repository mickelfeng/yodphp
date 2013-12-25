<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li>Yod_Request::dispatch()</li>
</ul>
<pre>
<b>$this->_request->dispatch():</b><br />
<?php print_r($request); ?>
</pre>
<br />
<?php $this->widget('public/footer'); ?>
</body>
</html>