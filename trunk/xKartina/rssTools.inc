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

function displayCategory($category) {
	$catItem = "";
	$catItem = '<item>';
	$catItem .= '<title>'. $category->name .'</title>';
	$catItem .=  '<link>'.XK_HOME.'showCathegory.php?id='.$category->id.'</link>';
	$catItem .= '<media:thumbnail url="'.XK_HOME.'img/cat_5.png" />';
//	$catItem .= '<mediaDisplay name=2MenusNColumnsView ></mediaDisplay>';		 
      
	$catItem .=  '<mediaDisplay name=threePartsView
	sideColorLeft="0:0:0"
	sideColorRight="0:0:0"
	backgroundColor="0:0:0"
	itemBackgroundColor="0:0:0"
	popupBorderColor="0:0:0"
	
	sideLeftWidthPC="0"
	sideRightWidthPC="0"
	
	headerXPC="14"
	headerYPC="3"
	headerWidthPC="95"
	
	itemImageXPC="1"
	itemImageYPC="18"
	itemImageWidthPC="10"
	itemImageHeightPC="10"
	
	itemXPC="12"
	itemYPC="18"
	itemWidthPC="75"
	itemHeightPC="10"
	
	menuXPC="5"
	menuWidthPC="15"
	
	capXPC="100"
	capYPC="10"
	capHeightPC="10"
	
	headerCapXPC="90"
	headerCapYPC="10"
	headerCapWidthPC="0"
    
    showDefaultInfo=yes
	fontSize="10"
	
	bottomYPC="10"
	infoYPC="90"
	popupXPC="7"
	popupWidthPC="15"
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
	$chanelItem .= '<title>'. $name .' / '.$channel->program.'('.$time.')</title>';
	$chanelItem .= '<description>'.  $name . '[' . $time . ']</description>';
	$chanelItem .= '<link>'.XK_HOME.$linkUrl.'</link>';
	$chanelItem .= '<pubDate>'. $channel->program.'<br/>('.$time.')</pubDate>';	
	$chanelItem .= '<enclosure url="'.XK_HOME.$linkUrl.'" type="video/mp4" />';
	$chanelItem .= '<media:thumbnail url="'. $logo . '" />';
	$chanelItem .= '</item>';
	
	return $chanelItem;
}

?>