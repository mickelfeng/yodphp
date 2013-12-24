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
Yod_Application::app()->import('Toolkit'):
<?php Toolkit::hello(); ?>

$this->import('Utils.qrcode', '.php'):
<?php print_r($qrcode); ?>

</pre>
<br />
<?php $this->widget('public/footer'); ?>
</body>
</html>