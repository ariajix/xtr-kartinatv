<?php
require_once "config.inc";
require_once "rssTools.inc";
require_once "tools.inc";

require_once "channelsParser.inc";
require_once "ktvFunctions.inc";



# at 03:00 starts another EPG day
define("EPG_START_OFFSET", 3 * 60 * 60);

$id = $_GET['id'];
$nowTime = NOW_TIME;

$arcTime = isset($_GET['archiveTime']) ? $_GET['archiveTime'] : $nowTime;


function getTime($program, $ignoreTimeShift = false) {
	if($ignoreTimeShift) {
		return $program->beginTime;
	}

	return $program->beginTime;
	
/*
	$dateTimeZoneLocal = new DateTimeZone(date_default_timezone_get());
	$dateTimeZoneUTC = new DateTimeZone("UTC");


	$dateTimeLocal = new DateTime("now", $dateTimeZoneLocal);
	$timeOffset = $dateTimeLocal->getOffset();
	
    return $program->beginTime + $timeOffset;
*/
}

function getEpg($id, $date) {
    # at 03:00 starts another EPG day
    # all below 03:00 belongs to previous day
    $date -= EPG_START_OFFSET;

    # renew the list using existing cookie
    $ktvFunctions = new KtvFunctions();
    $program = $ktvFunctions->getEpg($id, $date);

    $parser = new ProgramsParser();
    $parser->parse($program);

    return $parser;
}
?>

<?php

    $parser = getEpg($id, $arcTime);
       
    $programs = array(); # programs on current page
    $page     = 0;       # page index iterating over all possible
    $lines    = 0;       # lines amount on current page
    $dstPage  = null;    # index of active page

    $lastProgram = null; # previous loop element, to compare begin times
    $prevPage    = null; # time stamp of first begin time before current page
    $nextPage    = null; # time stamp of first begin time after current page

    $currentProgram = null; # program currently running

    foreach ($parser->programs as $program) {
        # check whether it's our destination page
        if ((! isset($lastProgram) || getTime($lastProgram) <= $arcTime) && $arcTime < getTime($program)) {
            $dstPage = $page;
        }

        # remember current program if it fits to bounds
        if (isset($lastProgram) && getTime($lastProgram) <= $nowTime && $nowTime < getTime($program)) {
            $currentProgram = $lastProgram;
        }

        # add program to list if there is no destination page yet
        # or if current page is the destination one
        if (! isset($dstPage) || $dstPage == $page) {
            $programs[] = $program;
        }

        # if current is beyond destination remember first program beginning
        if (isset($dstPage) && $page > $dstPage && ! isset($nextPage)) {
            $nextPage = getTime($program);
        }

        # remember last program for next iteration
        $lastProgram = $program;
    }

    # generate title
    $totalPages = $page + 1;
    //$title  = $_GET['title'] . "   (" . LANG_EPG_FROM . " ";
    $title = formatDate('d.m', $arcTime - EPG_START_OFFSET);

    # show pages only if there are more than one
    if ($totalPages > 1) {
        $title .= ", " . LANG_EPG_PAGE . " ";
        $title .= isset($dstPage) ? $dstPage + 1 : $totalPages;
        $title .= "/" . $totalPages;
    }
    $title .= ")";

    $titleTime = formatDate('d.m H:i', $nowTime);
?>


<?php
    if (count($programs) == 0) {
        print '<tr><td class="no-data" colspan="4" align="center">';
        print '<table><tr><td>';
        print '<img src="img/errors/empty-list.png" /></td><td>';
        print LANG_ERR_NO_EPG;
        print '</td></tr></table>';
        print "</td></tr>\n";
       
    } else {
        # generate link to open program URL
        $openRef  = "playChannel.php?id=$id";
        $openRef .= ! isset($_GET['number']) ? "" : "&number=" . $_GET['number'];
        $openRef .= ! isset($_GET['title'])  ? "" : "&title="  . $_GET['title'];
        $openRef .= ! isset($_GET['vid'])    ? "" : "&vid="    . $_GET['vid'];
    }

	echo drawEpgTemplate($programs, $arcTime, $parser->hasArchive,
	    $openRef,$id, $currentProgram);

?>
