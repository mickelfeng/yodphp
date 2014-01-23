<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li>Yod_Model::remove()</li>
</ul>
<pre>
<b>$model->remove('id = :id', array(':id' => 1)):</b> <?php var_dump($resilt); ?>
<br />
<b>$model->select():</b><br />
<?php print_r($data); ?>
<br />
<b>$model:</b><br />
<?php print_r($model); ?>
</pre>
<br />
<?php $this->widget('public/footer'); ?>
</body>
</html>