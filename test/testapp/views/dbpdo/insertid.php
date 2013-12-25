<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li>Yod_DbPdo::insertid()</li>
</ul>
<pre>
<b>$db->insertid():</b> <?php var_dump($insertid); ?>
<br />
<b>$db->fetchAll():</b><br />
<?php print_r($data); ?>
<br />
<b>$db:</b><br>
<?php print_r($db); ?>
</pre>
<br />
<?php $this->widget('public/footer'); ?>
</body>
</html>