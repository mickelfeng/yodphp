<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li>Yod_Model::config()</li>
</ul>
<pre>
<b>$this->config():</b><br>
<?php print_r($config); ?>

<b>$this->config('db_dsn'):</b><br>
<?php print_r($db_dsn); ?>

<b>$this->config('tpl_data.PUBLIC'):</b> <?php print_r($this->config('tpl_data.PUBLIC')); ?>
</pre>
<br />
<?php $this->widget('public/footer'); ?>
</body>
</html>