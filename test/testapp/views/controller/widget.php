<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li>Yod_Controller::widget()</li>
</ul>
<pre>
<b>$this->widget('public/widget'):</b><br />
<?php $this->widget('public/widget'); ?>
</pre>
<br />
<?php $this->widget('public/footer'); ?>
</body>
</html>