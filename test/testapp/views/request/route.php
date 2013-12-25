<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li>Yod_Request::route()</li>
</ul>
<pre>
<b>$this->_request->route('/c/a/p/v'):</b><br />
<?php print_r($request); ?>
</pre>
<br />
<?php $this->widget('public/footer'); ?>
</body>
</html>