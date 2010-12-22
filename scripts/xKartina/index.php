<?php 
require_once "config.inc";
require_once "tools.inc";
require_once "rssTools.inc";

$templateFileName = "templates/".TEMPLATE."/".START_TEMPLATE;
$template = readLocalFile($templateFileName);
$template = tmplReplace("XK_HOME",XK_HOME,$template);

$itemsFileName = "../items.tpl";
if(file_exists($itemsFileName)) {
	$items= readLocalFile($itemsFileName);
} else {
	$items= ""; 
}
$template = tmplReplace("items",$items,$template);

print $template;   

?>
