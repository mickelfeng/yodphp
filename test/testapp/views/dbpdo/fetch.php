<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li>Yod_DbPdo::fetch()</li>
</ul>
<pre>
<b>$db->fetch():</b><br>
<?php print_r($data); ?>
<br>
<b>$db:</b><br>
<?php print_r($db); ?>
</pre>
<br />
<?php $this->widget('public/footer'); ?>
</body>
</html>