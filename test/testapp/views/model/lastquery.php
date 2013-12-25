<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li>Yod_Model::lastQuery()</li>
</ul>
<pre>
<b>$model->lastQuery():</b> <?php echo $lastquery; ?>
<br>
<b>$model:</b><br>
<?php print_r($model); ?>
</pre>
<br />
<?php $this->widget('public/footer'); ?>
</body>
</html>