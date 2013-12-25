<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li>Application</li>
	<li><a href="<?php echo $WEBROOT; ?>/application/run" target="_blank">run</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/application/config" target="_blank">config</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/application/import" target="_blank">import</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/application/app" target="_blank">app</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/application/autoload" target="_blank">autoload</a></li>
</ul>
<?php $this->widget('public/footer'); ?>
</body>
</html>