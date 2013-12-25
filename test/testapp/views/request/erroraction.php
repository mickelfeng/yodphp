<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li>Yod_Request::errorAction()</li>
</ul>
<pre>
<b>$this:</b><br />
<?php print_r($this); ?>
</pre>
<br />
<?php $this->widget('public/footer'); ?>
</body>
</html>