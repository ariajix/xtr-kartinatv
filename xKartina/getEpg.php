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
    # time shift will be taken directly from channel name
    $timeShift = $ignoreTimeShift || 1 != preg_match('/^.* -(\d+)$/', $_GET['title'], $m) ? 
        0 : $m[1];
    return $program->beginTime + $timeShift * 60 * 60 + TIME_ZONE_SECONDS;
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

function displayProgram_orig($program, $nowTime, $hasArchive, $openRef, $currentProgram = null) {
    $class = "future";
    $name = $program->name;
    if ($program === $currentProgram) {
        $class="current";
        if ($hasArchive) {
            $openRef .= "&gmt=" . $program->beginTime;
        }
        $name = EMBEDDED_BROWSER ?
            "<marquee behavior=\"focus\">$name</marquee>" : $name;
        $name = "<a href=\"$openRef\">$name</a>";
    } else if (getTime($program, true) <= $nowTime) {
        // no time shift offset for links, since it doesn't affect record
        if ($hasArchive) {
            $openRef .= "&gmt=" . $program->beginTime;
            $name = EMBEDDED_BROWSER ?
                "<marquee behavior=\"focus\">$name</marquee>" : $name;
            $name = "<a href=\"$openRef\">$name</a>";
        }
        // but there is time shift for displayed style
        if (getTime($program) <= $nowTime) {
            $class="past";
        }
    }

    $t = getTime($program);
    $hour = date('H', $t) * 60 * 60 + date('i', $t) * 60 + date('s', $t);
    if ($hour < EPG_START_OFFSET) {
        $timeClass = "timeNight";
        $beginTime = formatDate('H:i', $t);
    } else {
        $timeClass = "time";
        $beginTime = date('H:i', $t);
    }


    $details = ! isset($program->details) || "" == $program->details ? "" :
        "</tr><tr><td class=\"${class}-details\">$program->details</td>\n";

    print "<tr>\n";
    print "<td class=\"$timeClass\" align=\"center\">$beginTime</td>\n";
    print "<td class=\"$class\" colspan=\"3\">\n";
    print "<table width=\"100%\"><tr>\n";
    print '<td>' . $name . "</td>\n";
    print $details;
    print "</tr></table>\n";
    print "</td></tr>\n";
}

function displayProgram($program, $nowTime, $hasArchive, $openRef, $currentProgram = null) {
    $class = "future";
    $name = $program->name;
    if ($program === $currentProgram) {
        $class="current";
        if ($hasArchive) {
            $openRef .= "&gmt=" . $program->beginTime;
        }
    } else if (getTime($program, true) <= $nowTime) {
    	// no time shift offset for links, since it doesn't affect record
        if ($hasArchive) {
            $openRef .= "&gmt=" . $program->beginTime;
        }
        // but there is time shift for displayed style
        if (getTime($program) <= $nowTime) {
            $class="past";
        }
    }

    $t = getTime($program);
    $hour = date('H', $t) * 60 * 60 + date('i', $t) * 60 + date('s', $t);
    if ($hour < EPG_START_OFFSET) {
        $timeClass = "timeNight";
        $beginTime = formatDate('H:i', $t);
    } else {
        $timeClass = "time";
        $beginTime = date('H:i', $t);
    }

/*    $chanelItem = '<item>';
	$title = "";

	$title = '['.$beginTime.']   '. $name;
	
	$chanelItem .= '<title>'.$program->name.'</title>';
	
	$chanelItem .= '<description>'.  $program->details . '</description>';
	$chanelItem .= '<link>'.XK_HOME.$openRef.'</link>';
	$chanelItem .= '<beginTime>'. $program->beginTime.'</beginTime>';	
	$chanelItem .= '<humanTime>'. $beginTime.'</humanTime>';
	$chanelItem .= '<enclosure url="'.XK_HOME.$openRef.'" type="video/mp4" />';
	$chanelItem .= '<media:thumbnail url="'. $logo . '" />';
	$chanelItem .=  '<mediaDisplay name=threePartsView
					backgroundColor="' . formatColor($category->color) . '"
					/>';  
	$chanelItem .= '</item>';
	
	return $chanelItem;    
	*/
    return "";
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
    $title  = $_GET['title'] . "   (" . LANG_EPG_FROM . " ";
    $title .= formatDate('d.m', $arcTime - EPG_START_OFFSET);

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
//    echo $parser->hasArchive ? "green" : "gray"
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
	/*
 	foreach ($programs as $program) {
		echo displayProgram($program, $nowTime, $parser->hasArchive,
	    $openRef, $currentProgram);
	}
	*/
    echo "hhehehe";
		echo drawEpgTemplate($programs, $nowTime, $parser->hasArchive,
	    $openRef, $currentProgram);
/*    
    # define previous page link if it wasn't already
    if (! isset($prevPage)) {
        $time = $arcTime -  EPG_START_OFFSET;
        $time -= 24 * 60 * 60;
        $time = mktime(23, 59, 59,
            date("n", $time), date("j", $time), date("Y", $time));
        $prevPage = $time + EPG_START_OFFSET;
    }

    # define next page link if it wasn't already
    if (! isset($nextPage)) {
        $time = $arcTime - EPG_START_OFFSET;
        $time += 24 * 60 * 60;
        $time = mktime(0, 0, 1,
            date("n", $time), date("j", $time), date("Y", $time));
        $nextPage = $time + EPG_START_OFFSET;
    }
*/
  
#  displayRssFooter();
?>
