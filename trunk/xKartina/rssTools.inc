<?php

function formatColor($color) {
	//return "255:0:0";
	return hexdec(substr($color, 1,2)) .':'.hexdec(substr($color, 3,2)).':'.hexdec(substr($color, 5,2)); 
}



function drawCategoryListTemplate($categories ) {
	$templateFileName = "templates/".TEMPLATE."/".CATEGORY_LIST_TEMPLATE;
	// file should exist
	if (! is_readable($templateFileName)) {
    	return;
	}

    $template = readLocalFile($templateFileName);
    
    $items = "";
	foreach ($categories as $category) {
		
		$cnt=1;
		for($i=0;$i<$cnt;$i++) {
			$items = $items.drawCategoryItemTemplate($category);
		}
		
	    
	}
    $tmp = str_replace("%%items%%",$items,$template);
    return $tmp; 
}

function drawCategoryItemTemplate($category) {
	$templateFileName = "templates/".TEMPLATE."/".CATEGORY_ITEM_TEMPLATE;
	// file should exist
	if (! is_readable($templateFileName )) {
    	return;
	}
    $template = readLocalFile($templateFileName );
    
    $template = str_replace("%%XK_HOME%%",XK_HOME,$template);
    $template = str_replace("%%name%%",$category->name,$template);
    $template = str_replace("%%id%%",$category->id,$template);
    $template = str_replace("%%color%%",$category->color,$template);
    $template = str_replace("%%isVideo%",$category->isVideo,$template);

    return $template;	
}

function drawChannelListTemplate($categories) {

	$templateFileName = "templates/".TEMPLATE."/".CATEGORY_TEMPLATE;
	// file should exist
	if (! is_readable($templateFileName)) {
    	return;
	}

    $template = readLocalFile($templateFileName);
    
    $items = "";
    foreach ($categories as $category) {
	  	foreach ($category->channels as $channel) {
			if(null != $category->color) {
				$channel->color = $category->color;
		    }
	  		$cnt=1;
			for($i=0;$i<$cnt;$i++) {
				$items = $items.drawChannelItemTemplate($channel);
			}
			
	  	}
    }
    return str_replace("%%items%%",$items,$template);
}

function drawChannelItemTemplate($channel) {

	$templateFileName = "templates/".TEMPLATE."/".CHANNEL_ITEM_TEMPLATE;
	// file should exist
	if (! is_readable($templateFileName )) {
    	return;
	}
    $template = readLocalFile($templateFileName );

	$template = str_replace("%%XK_HOME%%",XK_HOME,$template);
    $template = str_replace("%%name%%",$channel->name,$template);
    $template = str_replace("%%number%%",$channel->number,$template);
    $template = str_replace("%%id%%",$channel->id,$template);
    $template = str_replace("%%color%%",$channel->color,$template);
    $template = str_replace("%%program%%",$channel->program,$template);
	if (isset($channel->beginTime)) {
		$template = str_replace("%%beginTime%%",date('H:i', $channel->beginTime),$template);
    }

	if (isset($channel->endTime)) {
    	$template = str_replace("%%endTime%%",date('H:i', $channel->endTime),$template);    
    }

    $linkUrl = XK_HOME."playChannel.php";
    $linkUrl .= "?id=$channel->id";
    $linkUrl .= "&vid=" . ($channel->isVideo ? 1 : 0);
    $template = str_replace("%%linkUrl%%",$linkUrl,$template);
    $template = str_replace("%%isVideo%%",$channel->isVideo,$template);

    return $template;
}

function drawEpgTemplate($programs, $nowTime, $hasArchive, $openRef, $currentProgram = null) {

	$templateFileName = "templates/".TEMPLATE."/".EPG_LIST_TEMPLATE;
	// file should exist
	if (! is_readable($templateFileName)) {
    	return;
	}

    $template = readLocalFile($templateFileName);
    
    $titleTime = formatDate('d.m H:i', $nowTime);
	
    $template = str_replace("%%nowTime%%",$titleTime ,$template);
    
    $items = "";
  	foreach ($programs as $program) {
		$items = $items.drawEpgItemTemplate($program, $nowTime, $hasArchive,
	    $openRef, $currentProgram);
	}
    return str_replace("%%items%%",$items,$template);
}

function drawEpgItemTemplate($program, $nowTime, $hasArchive, $openRef, $currentProgram = null) {
	$templateFileName = "templates/".TEMPLATE."/".EPG_ITEM_TEMPLATE;
	// file should exist
	if (! is_readable($templateFileName )) {
    	return;
	}
    $template = readLocalFile($templateFileName );
    
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
        $beginTimeHuman = formatDate('H:i', $t);
    } else {
        $timeClass = "time";
        $beginTimeHuman = date('H:i', $t);
    }

	$template = str_replace("%%XK_HOME%%",XK_HOME,$template);

	$template = str_replace("%%description%%",$program->details,$template);

	$template = str_replace("%%openRef%%",$openRef,$template);

	$template = str_replace("%%humanTime%%",$beginTimeHuman,$template);
		
    $template = str_replace("%%name%%",$program->name,$template);

    return $template;
}


?>