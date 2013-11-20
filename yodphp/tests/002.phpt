--TEST--
Check for yod application
--SKIPIF--
<?php if (!extension_loaded("yod")) print "skip"; ?>
--FILE--
<?php 
$yodapp = Yod_Application::app();
print_r($yodapp);
?>
--EXPECTF--
Yod_Application Object
(
    [_config:protected] => Array
        (
        )

    [_request:protected] => Yod_Request Object
        (
            [_routed:protected] => 
            [controller] => 
            [action] => 
            [params] => 
            [method] => 
        )

    [_imports:protected] => 
    [_running:protected] => 
)
