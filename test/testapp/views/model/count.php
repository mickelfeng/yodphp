<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li>Yod_Model::count()</li>
</ul>
<pre>
<b>$model->count():</b> <?php var_dump($count); ?>
<br />
<b>$model:</b><br />
<?php print_r($model); ?>
</pre>
<br />
<?php $this->widget('public/footer'); ?>
</body>
</html>