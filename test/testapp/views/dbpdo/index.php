<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li>DbPdo</li>
	<li><a href="<?php echo $WEBROOT; ?>/dbpdo/connect" target="_blank">connect</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/dbpdo/fields" target="_blank">fields</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/dbpdo/execute" target="_blank">execute</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/dbpdo/query" target="_blank">query</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/dbpdo/fetch" target="_blank">fetch</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/dbpdo/fetchall" target="_blank">fetchAll</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/dbpdo/transaction" target="_blank">transaction</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/dbpdo/commit" target="_blank">commit</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/dbpdo/rollback" target="_blank">rollback</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/dbpdo/insertid" target="_blank">insertid</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/dbpdo/quote" target="_blank">quote</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/dbpdo/free" target="_blank">free</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/dbpdo/close" target="_blank">close</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/dbpdo/errno" target="_blank">errno</a></li>
	<li><a href="<?php echo $WEBROOT; ?>/dbpdo/error" target="_blank">error</a></li>
</ul>
<?php $this->widget('public/footer'); ?>
</body>
</html>