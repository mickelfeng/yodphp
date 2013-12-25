<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li>Yod_Model::getInstance()</li>
</ul>
<pre>
<b>Yod_Model::getInstance():</b><br />
<?php print_r($model); ?>
</pre>
<br />
<?php $this->widget('public/footer'); ?>
</body>
</html>