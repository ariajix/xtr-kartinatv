<?php
require_once "config.inc";
require_once "tools.inc";
require_once "ktvFunctions.inc";
#require_once "backgrounds.inc";

define("FILE_REF", "http://localhost:8088/stream/file=");
define("TMP_CHANNEL", "/tmp/channel.jsp");
define("TMP_BACKGROUND", "/tmp/bg.jsp");


    $ktvFunctions = new KtvFunctions();

    $id     = $_GET['id'];
    $number = $_GET['number'];
    $name   = urldecode($_GET['title']);
    $vid    = $_GET['vid'];
    $gmt    = $_GET['gmt'];
    $pkey    = $_GET['pkey'];
    $ref    = isset($_GET['ref']) ? urldecode($_GET['ref']) : "index.php";

    $content = $ktvFunctions->getStreamUrl($id, $gmt,$pkey);
    #$url = preg_replace('/.*url="(rtsp|http)(\/ts|)([^ "]*).*/s', '$1$3', $content);
 
	ereg('://([^" ]*)', $content, $regs);

	#hack - remove url parts after space
	#todo: chaneg regexp
	#$parts = explode(' ',$regs[1]);  
	$url = $regs[1];


   header( 'Location: http://'.$url ) ;

?>
