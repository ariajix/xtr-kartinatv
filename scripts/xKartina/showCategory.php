<?php
require_once "config.inc";
require_once "rssTools.inc";
require_once "tools.inc";
require_once "uft8_tools.inc";
require_once "channelsParser.inc";
require_once "ktvFunctions.inc";

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
$channelsParser = new ChannelsParser();
$channelsParser->parse($rawList);
#$channelsParser->selectedChannel = $_SESSION['selectedChannel'];

$itemsToDisplay     = array(); # itmes to be displayed on page

$categoryId = $_GET['id'];
foreach ($channelsParser->categories as $category) {
    if($category->id == $categoryId || $categoryId == null) {
    	$itemsToDisplay[] = $category;
    }
}

# display collected items
echo drawChannelListTemplate($itemsToDisplay);

?>