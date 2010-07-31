<?php
header("Content-type: text/xml");

require_once "../xKartina/ktvFunctions.inc";

$user = isset($HTTP_GET_VARS['user']) ? $HTTP_GET_VARS['user'] : "148";
$pass = isset($HTTP_GET_VARS['pass']) ? $HTTP_GET_VARS['pass'] : "841";
$id   = isset($HTTP_GET_VARS['id'])   ? $HTTP_GET_VARS['id']   : 7;

$ktvFunctions = new KtvFunctions($user, $pass);

print $ktvFunctions->getStreamUrl($id);

?>
