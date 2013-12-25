<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li>Yod_Controller::model()</li>
</ul>
<pre>
<b>$this->model():</b><br />
<?php print_r($model); ?>
</pre>
<br />
<?php $this->widget('public/footer'); ?>
</body>
</html>