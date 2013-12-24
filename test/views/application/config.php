<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li>Yod_Application::config()</li>
</ul>
<pre>
$app->config():
<?php print_r($config); ?>

$app->config('db_dsn'):
<?php print_r($db_dsn); ?>

$this->config('tpl_data.PUBLIC'): <?php print_r($this->config('tpl_data.PUBLIC')); ?>
</pre>
<br />
<?php $this->widget('public/footer'); ?>
</body>
</html>