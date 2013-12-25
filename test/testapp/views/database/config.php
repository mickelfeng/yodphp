<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li>Yod_Database::config()</li>
</ul>
<pre>
<b>$db->config():</b><br>
<?php print_r($db->config()); ?>

<b>$db->config('type'):</b> <?php echo $db->config('type'); ?>
</pre>
<br />
<?php $this->widget('public/footer'); ?>
</body>
</html>