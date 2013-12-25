<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li>Yod_DbPdo::fields()</li>
</ul>
<pre>
<b>$db->fields('test'):</b><br>
<?php print_r($fields); ?>

<b>$db:</b><br>
<?php print_r($db); ?>
</pre>
<br />
<?php $this->widget('public/footer'); ?>
</body>
</html>