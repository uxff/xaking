<?php
$br = (php_sapi_name() == "cli")? "":"<br>";

if(!extension_loaded('xaking')) {
	dl('./module/xaking.' . PHP_SHLIB_SUFFIX);
}
$module = 'xaking';
$functions = get_extension_funcs($module);
echo "Functions available in the test extension:$br\n";
foreach($functions as $func) {
    echo $func."$br\n";
}
echo "$br\n";
$function = 'confirm_' . $module . '_compiled';
if (extension_loaded($module)) {
	$str = $function($module);
} else {
	$str = "Module $module is not compiled into PHP";
}
echo "called in script: $str\n";

//$ret = xaking_gotask($function, 100, 2.2);
$func2 = function($arg1, ...) {
    return sprintf("make a str:".$arg1, ...);
}
$ret = xaking_gotask('printf', 'IN_PRINTF:%s:%d\n', __FILE__, __LINE__);
echo "the ret var_dump=";
var_dump($ret);

?>
