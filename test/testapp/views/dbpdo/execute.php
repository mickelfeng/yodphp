<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li>Yod_DbPdo::execute()</li>
</ul>
<pre>
<b>$db->execute($query, $params):</b><?php var_dump($result); ?>

<b>$db:</b><br>
<?php print_r($db); ?>
</pre>
<br />
<?php $this->widget('public/footer'); ?>
</body>
</html>