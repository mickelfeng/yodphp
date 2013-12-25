<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li>Yod_Database::db()</li>
</ul>
<pre>
<b><?php echo $title; ?>:</b><br>
<?php print_r($db); ?>
</pre>
<br />
<?php $this->widget('public/footer'); ?>
</body>
</html>