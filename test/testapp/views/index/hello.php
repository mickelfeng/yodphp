<html>
<head>
	<title><?php echo $title; ?></title>
</head>
<body>
<?php $this->widget('public/header'); ?>
<ul>
	<li>Hello</li>
</ul>
<pre>
	<?php echo $yodphp ?>, <?php echo $hello; ?>

<br />-------------------------------------------------------------
<?php print_r($this); ?>
</pre>
<br />
<?php $this->widget('public/footer'); ?>
</body>
</html>