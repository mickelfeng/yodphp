<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
"http://www.w3.org/TR/html4/loose.dtd">
<html>
<head><meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<title><?php echo $title; ?></title></head>
<body>
<div><?php $this->widget('manual/header', $this->_link); ?><hr></div>
<div><h2 class="title"><?php echo $title; ?></h2></div>
<div><?php echo $intro; ?></div>
<div><?php echo $content; ?></div>
<div><hr><?php $this->widget('manual/footer', $this->_link); ?></div>
</body>
</html>