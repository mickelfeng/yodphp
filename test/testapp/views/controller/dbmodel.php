<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li>Yod_Controller::dbmodel()</li>
</ul>
<pre>
<b>$this->dbmodel():</b><br />
<?php print_r($dbmodel); ?>
</pre>
<br />
<?php $this->widget('public/footer'); ?>
</body>
</html>