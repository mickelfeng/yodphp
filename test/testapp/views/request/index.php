<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li>Request</li>
	<li><a href="<?php echo $WEBROOT; ?>/request/route" target="_blank">route</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/request/dispatch" target="_blank">dispatch</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/request/erroraction" target="_blank">errorAction</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/request/error404" target="_blank">error404</a></li>
</ul>
<?php $this->widget('public/footer'); ?>
</body>
</html>