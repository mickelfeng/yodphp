<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li>Yod_Model::dbmodel()</li>
</ul>
<pre>
<b>$this->dbmodel():</b><br />
<?php print_r($dbmodel); ?>
</pre>
<br />
<?php $this->widget('public/footer'); ?>
</body>
</html>