<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li>Yod_Database::update()</li>
</ul>
<pre>
<b>$db->update($data, 'test', 'id = :id', array(':id' => 1)):</b><?php var_dump($result); ?>

<b>$db:</b><br>
<?php print_r($db); ?>
</pre>
<br />
<?php $this->widget('public/footer'); ?>
</body>
</html>