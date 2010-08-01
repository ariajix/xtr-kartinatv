<?php

function formatColor($color) {
	//return "255:0:0";
	return hexdec(substr($color, 1,2)) .':'.hexdec(substr($color, 3,2)).':'.hexdec(substr($color, 5,2)); 
}

function tmplReplace($placeholder,$value,$str) {
	if(null == $value) {
		$value = "";
	}
 	return  str_replace("%%".$placeholder."%%",$value,$str);
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
    $tmp = tmplReplace("items",$items,$template);
    return $tmp; 
}

function drawCategoryItemTemplate($category) {
	$templateFileName = "templates/".TEMPLATE."/".CATEGORY_ITEM_TEMPLATE;
	// file should exist
	if (! is_readable($templateFileName )) {
    	return;
	}
    $template = readLocalFile($templateFileName );
    
    $template = tmplReplace("XK_HOME",XK_HOME,$template);
    $template = tmplReplace("name",$category->name,$template);
    $template = tmplReplace("id",$category->id,$template);
    $template = tmplReplace("color",$category->color,$template);
    $template = tmplReplace("isVideo%",$category->isVideo,$template);

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
    $menuItems = "";
    foreach ($categories as $category) {
	  	foreach ($category->channels as $channel) {
			if(null != $category->color) {
				$channel->color = $category->color;
		    }
	  		$cnt=1;
			for($i=0;$i<$cnt;$i++) {
				$items = $items.drawChannelItemTemplate($channel);
				$menuItems = $menuItems.drawChannelItemTemplate($channel,CHANNEL_MENU_ITEM_TEMPLATE);
			}
	  	}
    }
    $template = tmplReplace("menuItems",$menuItems,$template);
    return tmplReplace("items",$items,$template);
}

function drawChannelItemTemplate($channel, $temp = CHANNEL_ITEM_TEMPLATE) {

	$templateFileName = "templates/".TEMPLATE."/".$temp;
	// file should exist
	if (! is_readable($templateFileName )) {
    	return;
	}
	$name = preg_replace('/ -\d+$/', '', $channel->name);
	
    $template = readLocalFile($templateFileName );

	$template = tmplReplace("XK_HOME",XK_HOME,$template);
    $template = tmplReplace("name",$name,$template);
    $template = tmplReplace("number",$channel->number,$template);
    $template = tmplReplace("id",$channel->id,$template);
    $template = tmplReplace("color",$channel->color,$template);
    $template = tmplReplace("program",$channel->program,$template);
	if (isset($channel->beginTime)) {
		$template = tmplReplace("beginTime",date('H:i', $channel->beginTime),$template);
    }

	if (isset($channel->endTime)) {
    	$template = tmplReplace("endTime",date('H:i', $channel->endTime),$template);    
    }

    $linkUrl = XK_HOME."playChannel.php";
    $linkUrl .= "?id=$channel->id";
    $linkUrl .= "&amp;vid=" . ($channel->isVideo ? 1 : 0);
    $template = tmplReplace("linkUrl",$linkUrl,$template);
    $template = tmplReplace("isVideo",$channel->isVideo,$template);
    
    
    

    return $template;
}

function drawEpgTemplate($programs, $nowTime, $hasArchive, $openRef, $channelId, $currentProgram = null) {

	$templateFileName = "templates/".TEMPLATE."/".EPG_LIST_TEMPLATE;
	// file should exist
	if (! is_readable($templateFileName)) {
    	return;
	}

    $template = readLocalFile($templateFileName);
    
    $titleTime = formatDate('d.m H:i', $nowTime);
	
    $template = tmplReplace("nowTime",$titleTime ,$template);
    
    $items = "";
  	foreach ($programs as $program) {
		$items = $items.drawEpgItemTemplate($program, $nowTime, $hasArchive,
	    $openRef, $channelId,  $currentProgram);
	}
    return tmplReplace("items",$items,$template);
}

function drawEpgItemTemplate($program, $nowTime, $hasArchive, $openRef,$channelId, $currentProgram = null) {
	$templateFileName = "templates/".TEMPLATE."/".EPG_ITEM_TEMPLATE;
	// file should exist
	if (! is_readable($templateFileName )) {
    	return;
	}
    $template = readLocalFile($templateFileName );
    
 	if ($program === $currentProgram) {
        $class="current";
        if ($hasArchive) {
            $openRef .= "&amp;gmt=" . $program->beginTime;
        }
    } else if (getTime($program, true) <= $nowTime) {
        // no time shift offset for links, since it doesn't affect record
        if ($hasArchive) {
            $openRef .= "&amp;gmt=" . $program->beginTime;
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

	$template = tmplReplace("XK_HOME",XK_HOME,$template);

	$template = tmplReplace("description",$program->details,$template);

	$template = tmplReplace("openRef",$openRef,$template);

	$template = tmplReplace("humanTime",$beginTimeHuman,$template);
		
    $template = tmplReplace("name",$program->name,$template);

    $template = tmplReplace("id",$channelId,$template);
    
    $template = tmplReplace("pageTitle",$channelId,$template);
    
    
    if ($hasArchive) {
    	$template = tmplReplace("archive","A",$template);
    }
    else {
    	$template = tmplReplace("archive","",$template);
    }
    
    return $template;
}


?>