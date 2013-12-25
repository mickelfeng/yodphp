<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li>Yod_Controller::assign()</li>
</ul>
<pre>
<b>$this->assign('this', $this):</b><br />
<?php print_r($this->_view); ?>
</pre>
<br />
<?php $this->widget('public/footer'); ?>
</body>
</html>