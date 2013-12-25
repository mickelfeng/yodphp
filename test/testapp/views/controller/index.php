<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li>Controller</li>
	<li><a href="<?php echo $WEBROOT; ?>/controller/config" target="_blank">config</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/controller/import" target="_blank">import</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/controller/model" target="_blank">model</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/controller/assign" target="_blank">assign</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/controller/render" target="_blank">render</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/controller/display" target="_blank">display</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/controller/widget" target="_blank">widget</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/controller/forward" target="_blank">forward</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/controller/redirect" target="_blank">redirect</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/controller/error404" target="_blank">error404</a></li>
</ul>
<?php $this->widget('public/footer'); ?>
</body>
</html>