<?php
header("Content-type: text/xml");

require_once "../xKartina/ktvFunctions.inc";

$user = isset($HTTP_GET_VARS['user']) ? $HTTP_GET_VARS['user'] : "148";
$pass = isset($HTTP_GET_VARS['pass']) ? $HTTP_GET_VARS['pass'] : "841";

$ktvFunctions = new KtvFunctions($user, $pass);
print $ktvFunctions->getChannelsList();

?>
