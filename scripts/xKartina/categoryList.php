<?php
require_once "rssTools.inc";
require_once "config.inc";
require_once "channelsParser.inc";
require_once "ktvFunctions.inc";


# decide whether chanels list update is needed
if (! isset($_SESSION['channelsList']) || ! isset($_SESSION['lastUpdate']) ||
    time() - $_SESSION['lastUpdate'] > CL_UPDATE_INTERVAL) 
{
	
	
    # renew the list using existing cookie
    $ktvFunctions = new KtvFunctions();

    $rawList = $ktvFunctions->getChannelsList();

    # remember new state
    $_SESSION['channelsList'] = $rawList;
    $_SESSION['lastUpdate'] = time();
} else {
    # use remembered list
    $rawList = $_SESSION['channelsList'];
}

# parse raw list into prepared class hierarchy
$channelsParser = new ChannelsParser();
$channelsParser->parse($rawList);
$channelsParser->selectedChannel = $_SESSION['selectedChannel'];



$itemsToDisplay     = array(); # itmes to be displayed on page
$firstChannelNumber = 0;       # first displayed channel on page
$lastChannelNumber  = 0;       # last displayed channel on page
$currentEntry       = 0;
$foundSelected      = false;
$limit = CL_ITEMS_PER_PAGE;

echo drawCategoryListTemplate($channelsParser->categories);

?>