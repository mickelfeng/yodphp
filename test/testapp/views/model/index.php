<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li>Model</li>
	<li><a href="<?php echo $WEBROOT; ?>/model/getinstance" target="_blank">getInstance</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/model/find" target="_blank">find</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/model/findAll" target="_blank">findAll</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/model/select" target="_blank">select</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/model/count" target="_blank">count</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/model/save" target="_blank">save</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/model/remove" target="_blank">remove</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/model/lastquery" target="_blank">lastQuery</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/model/config" target="_blank">config</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/model/import" target="_blank">import</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/model/model" target="_blank">model</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/model/dbmodel" target="_blank">dbmodel</a></li>
</ul>
<?php $this->widget('public/footer'); ?>
</body>
</html>