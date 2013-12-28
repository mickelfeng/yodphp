<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li>Yod_DbModel::parseQuery()</li>
</ul>
<pre>
<b>$model->parseQuery():</b><br />
<?php print_r($query); ?>
<br />
<b>$model:</b><br />
<?php print_r($model); ?>
</pre>
<br />
<?php $this->widget('public/footer'); ?>
</body>
</html>