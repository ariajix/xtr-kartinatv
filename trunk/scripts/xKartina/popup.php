<?php
require_once "config.inc";
require_once "rssTools.inc";
require_once "tools.inc";

if($_GET["msg"] == "wrong_pwd") {
	$msg = "Неверный пароль для защищённых каналов!";
} else {
	$msg = $_GET["msg"]; 
}

# display collected items
echo drawPopupTemplate($msg);

?>