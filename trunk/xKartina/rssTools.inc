<?php
function displayRssHeader($title = "xTreamer Kartina.TV plugin", 
    $refreshTime = 0, $fontSize = FONT_SIZE) 
{
header("Content-type: text/xml");
echo"<?xml version='1.0' ?>";
echo '<rss version="2.0" xmlns:dc="http://purl.org/dc/elements/1.1/">';
echo '<channel>';
echo '<menu></menu>';
}

function displayRssFooter()
{
echo '</channel>';
echo '</rss>';
}
function formatColor($color) {
	//return "255:0:0";
	return hexdec(substr($color, 1,2)) .':'.hexdec(substr($color, 3,2)).':'.hexdec(substr($color, 5,2)); 
}
function displayCategory($category) {
	$catItem = "";
	$catItem = '<item>';
	$catItem .= '<title>'. $category->name .'</title>';
	$catItem .=  '<link>'.XK_HOME.'showCategory.php?id='.$category->id.'</link>';
	$catItem .= '<media:thumbnail url="'.XK_HOME.'img/cat.png" />';
      
	$catItem .=  '	<mediaDisplay name="threePartsView" 
					showHeader="yes" 
					headerXPC="20"
					headerYPC="3"
					showDefaultInfo="yes"
				
					imageLeftSide="image/MVIX_LIVE_MAIN_BG_2.jpg" 
					sideLeftWidthPC="10" 
					sideRightWidthPC="0" 
					sideColorRight="0:0:0" 
					sideColorLeft="0:0:0" 
					
					itemImageXPC="12" 
					itemImageYPC="14" 
					itemImageHeightPC="8" 
					itemImageWidthPC="8" 
				
					drawItemText="yes"
					slidingItemText="yes"

					itemGap="0"
					itemXPC="20" 
					itemYPC="14" 
					itemHeightPC="8" 
					itemWidthPC="70" 
					itemPerPage="10" 
					fontSize="12"
					idleImageXPC="45" 
					idleImageYPC="42" 
					idleImageWidthPC="10" 
					idleImageHeightPC="15" 
					viewAreaWidthPC="100"
					infoYPC="95"
					>	  
        <idleImage>image/POPUP_LOADING_01.jpg</idleImage>
        <idleImage>image/POPUP_LOADING_02.jpg</idleImage>
        <idleImage>image/POPUP_LOADING_03.jpg</idleImage>
        <idleImage>image/POPUP_LOADING_04.jpg</idleImage>
        <idleImage>image/POPUP_LOADING_05.jpg</idleImage>
        <idleImage>image/POPUP_LOADING_06.jpg</idleImage>
  </mediaDisplay>';
	$catItem .=  '</item>';
	
	return $catItem;
}


function displayChannel($channel)
{
    $number  = ($channel->number < 10 ? "0" : "") . $channel->number;
    $logo    = "http://iptv.kartina.tv/img/ico/$channel->id.gif";
    $linkUrl = "playChannel.php";
    $linkExt = "";

    //$linkUrl .= "?d=0"; // dummy option to include in REF argument
    $linkUrl .= "?id=$channel->id";
//    $linkUrl .= "&number=$channel->number";
//    $linkUrl .= "&title=" . urlencode($channel->name);
//    $linkUrl .= "&vid=" . ($channel->isVideo ? 1 : 0);


    # add time stamps only if they are provided
    if (isset($channel->beginTime) && isset($channel->endTime)) {
        $start = date('H:i', $channel->beginTime);
        $stop  = date('H:i', $channel->endTime);
       	$time  = "$start - $stop";
    }

    # decide how to display the program
#    $name = preg_replace('/ -\d+$/', '', $channel->name);
    $name = $channel->name;
/*    if ("" == $channel->program) {
        $name = EMBEDDED_BROWSER ?
            '<marquee behavior="focus">'.$name.'</marquee>':
            utf8_cutByPixel($name, CL_WIDTH_LONGNAME, true);
        $name = "<a href=\"$linkUrl\" $linkExt>$name</a>";
        $program = "";
    } else {
        $name = utf8_cutByPixel($name, CL_WIDTH_NAME, true);
        $program = EMBEDDED_BROWSER ?
            '<marquee behavior="focus">'.$channel->program.'</marquee>':
            utf8_cutByPixel($channel->program, CL_WIDTH_PROGRAM, false);
        $program = "<a href=\"$linkUrl\" $linkExt>$program</a>";
    }
*/
    # now the entry itself
	$chanelItem = "";
	$chanelItem = '<item>';
	$title = "";
	if(isset($channel->program)) {
		$title = '['.$time.']   '. $channel->program;
	}
	$chanelItem .= '<title>'.$title.'</title>';
	
	$chanelItem .= '<description>'.  $name . ' [' . $time . ']<b>bold</b><br/>line</description>';
	$chanelItem .= '<link>'.XK_HOME.$linkUrl.'</link>';
	$chanelItem .= '<pubDate>'. $name .' [' . $time . ']: '. $channel->program.'<br/>('.$time.')</pubDate>';	
	$chanelItem .= '<enclosure url="'.XK_HOME.$linkUrl.'" type="video/mp4" />';
	$chanelItem .= '<media:thumbnail url="'. $logo . '" />';
	$chanelItem .=  '<mediaDisplay name=threePartsView
					backgroundColor="' . formatColor($category->color) . '"
					/>';  
	$chanelItem .= '</item>';
	
	return $chanelItem;
}

?>