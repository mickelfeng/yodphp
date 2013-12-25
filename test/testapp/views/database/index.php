<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li>Database</li>
	<li><a href="<?php echo $WEBROOT; ?>/database/db" target="_blank">db</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/database/getinstance" target="_blank">getInstance</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/database/config" target="_blank">config</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/database/create" target="_blank">create</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/database/insert" target="_blank">insert</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/database/update" target="_blank">update</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/database/delete" target="_blank">delete</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/database/select" target="_blank">select</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/database/lastquery" target="_blank">lastQuery</a></li>
</ul>
<?php $this->widget('public/footer'); ?>
</body>
</html>