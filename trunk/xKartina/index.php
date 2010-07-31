<?php
require_once "config.inc";
require_once "rssTools.inc";
require_once "tools.inc";

require_once "channelsParser.inc";
require_once "ktvFunctions.inc";


$d = time();

//echo "NOW_TIME : " . NOW_TIME  . ", time: " . $d. ", dif: ". NOW_TIME  -  $d." \n"; 
echo "NOW_TIME : " . NOW_TIME ;  
echo  ", time: " . $d ;
//echo  " df: ". Integer(NOW_TIME)  -  $d." \n";
	    
/*    $ktvFunctions = new KtvFunctions();
//    $xmlData =  $ktvFunctions->getData("http://myxtreamer/media/sda1/scripts/epg.xml", "test epg");
$xmlData =  getPageContentByGet("http://myxtreamer/media/sda1/scripts/epg.xml", "");
    $parser = new EregProgramsParser();
    $parser->parse($xmlData);
  */  
	 
	//echo date("Y", strtotime("now"));


	$content  = '<?xml version="1.0" encoding="UTF-8"?><url url="http/ts://217.19.223.4:28006/?ticket=Im8LyDysSO4Oo5RUduRIDEkeoWlJgQfBWtjRb0TJpcI%2FNYWszO%2BTCnHNoTnkSUwyJ8HO5uptqk6dj5oqo9m33TKEauLg%2FtRwJoVMdSVM8NeBQJN9sLZn5p6uVwzJ0tzT2X4xT4yCRRgR%2FJVvljqzqAA%2FnuQ18%2BJjchFpIfN4WxM%3D :http-caching=3000 :no-http-reconnect" />';
	ereg('://([^" ]*)', $content, $regs);

	#hack - remove url parts after space
	#todo: chaneg regexp
	$parts = explode(' ',$regs[1]);  
	$url = $parts[0];
	
?>
<html>
<body>
<a href = "categoryList.php">categoryList.php</a>
<a href = "channelsList.php">channelsList.php</a>
<a href = "showCategory.php?id=1">Category 1</a>
<a href = "showCategory.php?id=21">Category 31</a>

<a href = "playChannel.php?d=0&id=7&number=1&title=%D0%A0%D0%B5%D0%BD%D0%A2%D0%92&vid=1">RenTv</a>
<a href = "getEpg.php?d=0&id=7&number=1&title=%D0%A0%D0%B5%D0%BD%D0%A2%D0%92&vid=1">RenTv EPG</a>

<a href = "playlist.php">playlist</a>

</body></html>