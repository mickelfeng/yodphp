<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li>Yod_Application::import()</li>
</ul>
<pre>
<b>Yod_Application::import('Toolkit'):</b> <?php Toolkit::hello(); ?>
<br />
<b>$this->import('utils.qrcode', '.php'):</b><br />
<?php print_r($qrcode); ?>
</pre>
<br />
<?php $this->widget('public/footer'); ?>
</body>
</html>