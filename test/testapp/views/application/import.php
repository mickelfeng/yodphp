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
<b>Yod_Application::app()->import('Toolkit'):</b> <?php Toolkit::hello(); ?>
<br />
<b>$this->import('Utils.qrcode', '.php'):</b><br />
<?php print_r($qrcode); ?>
</pre>
<br />
<?php $this->widget('public/footer'); ?>
</body>
</html>