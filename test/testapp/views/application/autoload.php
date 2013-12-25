<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li>Yod_Application::autoload()</li>
</ul>
<pre>
<b>Yod_Application::autoload('Toolkit'):</b> <?php Toolkit::hello(); ?>
</pre>
<br />
<?php $this->widget('public/footer'); ?>
</body>
</html>