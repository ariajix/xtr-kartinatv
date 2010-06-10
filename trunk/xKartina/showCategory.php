<?php
require_once "config.inc";
require_once "rssTools.inc";
require_once "tools.inc";
#require_once "uft8_tools.inc";
require_once "channelsParser.inc";
require_once "ktvFunctions.inc";



displayRssHeader("xTreamer chanel list for Kartina.TV");

# decide whether chanels list update is needed
if (! isset($_SESSION['channelsList']) || ! isset($_SESSION['lastUpdate']) ||
    NOW_TIME - $_SESSION['lastUpdate'] > CL_UPDATE_INTERVAL) 
{
    # renew the list using existing cookie
    $ktvFunctions = new KtvFunctions();
    $rawList = $ktvFunctions->getChannelsList();

    # remember new state
    $_SESSION['channelsList'] = $rawList;
    $_SESSION['lastUpdate'] = NOW_TIME;
} else {
    # use remembered list
    $rawList = $_SESSION['channelsList'];
}

# parse raw list into prepared class hierarchy
$channelsParser = new EregChannelsParser();
$channelsParser->parse($rawList);
#$channelsParser->selectedChannel = $_SESSION['selectedChannel'];



$itemsToDisplay     = array(); # itmes to be displayed on page
//$firstChannelNumber = 0;       # first displayed channel on page
//$lastChannelNumber  = 0;       # last displayed channel on page
//$currentEntry       = 0;
//$foundSelected      = false;
//$limit = CL_ITEMS_PER_PAGE;
$categoryId = $_GET['id'];
foreach ($channelsParser->categories as $category) {
//    $itemsToDisplay[] = $category;
//    $currentEntry++;
    if($category->id == $categoryId || $categoryId == null) {
	    foreach ($category->channels as $channel) {
	    	if(null != $category->color) {
	    		$channel->color = $category->color;
	    	}
	        $itemsToDisplay[] = $channel;
//	        $currentEntry++;
	    }
    }
}
# display collected items
$cnt=1;
for($i=0;$i<$cnt;$i++) {
foreach ($itemsToDisplay as $item) {
    echo   displayChannel($item);
}
}
displayRssFooter();
?>