<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li>DbModel</li>
	<li><a href="<?php echo $WEBROOT; ?>/dbmodel/getinstance" target="_blank">getInstance</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/dbmodel/table" target="_blank">table</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/dbmodel/find" target="_blank">find</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/dbmodel/select" target="_blank">select</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/dbmodel/count" target="_blank">count</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/dbmodel/save" target="_blank">save</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/dbmodel/remove" target="_blank">remove</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/dbmodel/field" target="_blank">field</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/dbmodel/from" target="_blank">from</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/dbmodel/join" target="_blank">join</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/dbmodel/where" target="_blank">where</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/dbmodel/group" target="_blank">group</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/dbmodel/having" target="_blank">having</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/dbmodel/order" target="_blank">order</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/dbmodel/limit" target="_blank">limit</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/dbmodel/union" target="_blank">union</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/dbmodel/comment" target="_blank">comment</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/dbmodel/params" target="_blank">params</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/dbmodel/parsequery" target="_blank">parseQuery</a></li>
</ul>
<?php $this->widget('public/footer'); ?>
</body>
</html>