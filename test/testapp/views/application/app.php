<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li>Yod_Application::app()</li>
</ul>
<pre>
<b>Yod_Application::app():</b><br>
<?php print_r($app); ?>
</pre>
<br />
<?php $this->widget('public/footer'); ?>
</body>
</html>