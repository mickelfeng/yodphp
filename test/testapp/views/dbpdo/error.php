<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li><?php echo $title; ?></li>
</ul>
<pre>
<b>$db->errno():</b> <?php echo $errno; ?>
<br>
<b>$db->error():</b> <?php echo $error; ?>
<br>
<b>$db:</b><br>
<?php print_r($db); ?>
</pre>
<br />
<?php $this->widget('public/footer'); ?>
</body>
</html>