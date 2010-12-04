<?php
require_once "tools.inc";

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

function drawLoginTemplate($username,$password, $msgString = '') {
    $templateFileName = "templates/".TEMPLATE."/".LOGIN_TEMPLATE;
    // file should exist
    if (! is_readable($templateFileName)) {
        return;
    }

    $template = readLocalFile($templateFileName);
    
    $template = tmplReplace("XK_HOME",XK_HOME,$template);
    $template = tmplReplace("username",$username,$template);
    $template = tmplReplace("password",$password,$template);
    $template = tmplReplace("msgString",$msgString ,$template);
    
    return $template; 
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
    
    $template = tmplReplace("XK_HOME",XK_HOME,$template);
    $tmp = tmplReplace("items",$items,$template);
    return $tmp; 
}

function drawCategoryItemTemplate($category) {
    $templateFileName = "templates/".TEMPLATE."/".CATEGORY_ITEM_TEMPLATE;
    // file should exist
    if (! is_readable($templateFileName )) {
        return;
    }
    if(!$category->isVideo) {
        return;
    }
    $template = readLocalFile($templateFileName );
    $template = tmplReplace("XK_HOME",XK_HOME,$template);
    $template = tmplReplace("name",$category->name,$template);
    $template = tmplReplace("id",$category->id,$template);
    $template = tmplReplace("color",$category->color,$template);
    $template = tmplReplace("isVideo",$category->isVideo,$template);
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
    $categoryName = "";
    if(count($categories) > 1 ) {
        $categoryName = "Все категории";
    } else { 
        if(count($categories) > 0 ) {
            $categoryName = $categories[0]->name;
        }
        else {
            $categoryName = "---"; 
        }
    }
    
    $template = tmplReplace("categoryName",$categoryName,$template);
    $selfUrl = "http://".$_SERVER["HTTP_HOST"] . $_SERVER["PHP_SELF"] . "?" . $_SERVER["QUERY_STRING"];
    
    $template = tmplReplace("selfUrl",$selfUrl,$template);
    
    foreach ($categories as $category) {
        foreach ($category->channels as $channel) {
            if(null != $category->color) {
                $channel->color = $category->color;
            }
            $cnt=1;
            for($i=0;$i<$cnt;$i++) {
                $items = $items.drawChannelItemTemplate($category, $channel);
                $menuItems = $menuItems.drawChannelItemTemplate($category, $channel,CHANNEL_MENU_ITEM_TEMPLATE);
            }
        }
    }
    $template = tmplReplace("XK_HOME",XK_HOME,$template);
    $template = tmplReplace("menuItems",$menuItems,$template);
    return tmplReplace("items",$items,$template);
}

function drawChannelItemTemplate($category, $channel, $temp = CHANNEL_ITEM_TEMPLATE) {

    $templateFileName = "templates/".TEMPLATE."/".$temp;
    // file should exist
    if (! is_readable($templateFileName )) {
        return;
    }
    //ignore radio
    if(!$channel->isVideo) {
        return;
    }
    $name = preg_replace('/ -\d+$/', '', $channel->name);
    
    $template = readLocalFile($templateFileName );

    $template = tmplReplace("category",$category->name,$template);
    $template = tmplReplace("XK_HOME",XK_HOME,$template);
    $template = tmplReplace("name",$name,$template);
    $template = tmplReplace("number",$channel->number,$template);
    $template = tmplReplace("id",$channel->id,$template);
    $template = tmplReplace("color",$channel->color,$template);
    $template = tmplReplace("program",$channel->program,$template);
    
    if (isset($channel->beginTime) && isset($channel->endTime)) {
        $template = tmplReplace("beginTime",$channel->beginTime,$template);
        $template = tmplReplace("endTime",$channel->endTime,$template);    
        $template = tmplReplace("humanTimeFromTo",date('H:i', $channel->beginTime) . " - ".date('H:i', $channel->endTime),$template);

        $full = ($channel->endTime - $channel->beginTime);
        $curr = time() - $channel->beginTime;
        $percent = 0 == $full ? 0 : (int) ($curr * 100 / $full);
        $template = tmplReplace("progress", $percent, $template);
    }
    else {
        $template = tmplReplace("humanTimeFromTo","",$template);
        $template = tmplReplace("progress", "", $template);
        $template = tmplReplace("beginTime","",$template);
        $template = tmplReplace("endTime","",$template);    
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
    
    $titleTime = formatDate('d.m.Y',  $nowTime -  EPG_START_OFFSET);
    
    $template = tmplReplace("XK_HOME",XK_HOME,$template);
    $template = tmplReplace("epgDate",$titleTime ,$template);
    
    
    #define previous page link
    
    $time = $nowTime -  EPG_START_OFFSET;
    $time -= 24 * 60 * 60;
    $time = mktime(23, 59, 59, date("n", $time), date("j", $time), date("Y", $time));
    $prevPage = $time + EPG_START_OFFSET;
    $template = tmplReplace("prevDayTime",$prevPage ,$template);
    
    #define next page link
    $time = $nowTime - EPG_START_OFFSET;
    $time += 24 * 60 * 60;
    $time = mktime(0, 0, 1,
    date("n", $time), date("j", $time), date("Y", $time));
    if($time < time()) {
        $nextPage = $time + EPG_START_OFFSET;
    }
    else {
        $nextPage = $nowTime;
    }
            
    $template = tmplReplace("nextDayTime",$nextPage ,$template);
    $template = tmplReplace("id",$channelId ,$template);
    $template = tmplReplace("archive",$hasArchive ? "A" : "",$template);
    
    $items = "";
    $index = 0;
    $selectedIndex = 0;
    foreach ($programs as $program) {
        $items = $items.drawEpgItemTemplate($program, $nowTime, $hasArchive,
        $openRef, $channelId,  $currentProgram);
        
        if($currentProgram == $program) {
            $selectedIndex = $index;
        }
        $index = $index +1;
    }
    $template = tmplReplace("selectedIndex",$selectedIndex ,$template);
    return tmplReplace("items",$items,$template);
}

function drawEpgItemTemplate($program, $nowTime, $hasArchive, $openRef,$channelId, $currentProgram = null) {
    $templateFileName = "templates/".TEMPLATE."/".EPG_ITEM_TEMPLATE;
    // file should exist
    if (! is_readable($templateFileName )) {
        return;
    }
    $template = readLocalFile($templateFileName );

    $class = "future";
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
    $template = tmplReplace("class",$class,$template);
    $template = tmplReplace("archive",$hasArchive ? "A" : "",$template);
    
    return $template;
}

function drawPlaylistsTemplate($playlists) {
    
    if(count($playlists) == 1) {
        return drawPlayListItemsTemplate($playlists[0]);
    }
    
    //display list of lists
    $templateFileName = "templates/".TEMPLATE."/".PLAYLISTS_TEMPLATE;
    
    // file should exist
    if (! is_readable($templateFileName )) {
        return;
    }
    $template = readLocalFile($templateFileName );
    
    $template = tmplReplace("XK_HOME",XK_HOME,$template);
    if(count($playlists) > 1) {
        $items = "";
        foreach ($playlists as $playlist) {
            $items = $items.drawPlayListFileTemplate($playlist);
        }
        $template = tmplReplace("playLists",$items,$template);
    }
    else {
    
    }
    
    return $template;
}
function drawPlayListFileTemplate($playlist) {
    $templateFileName = "templates/".TEMPLATE."/".PLAYLIST_TEMPLATE;
    
    // file should exist
    if (! is_readable($templateFileName )) {
        return;
    }
    $template = readLocalFile($templateFileName );

    $template = tmplReplace("XK_HOME",XK_HOME,$template);
    $template = tmplReplace("name",$playlist->name,$template);
    $template = tmplReplace("uri",$playlist->uri,$template);
    return $template;
}

function drawPlayListItemsTemplate($playlist) {
    $templateFileName = "templates/".TEMPLATE."/".PLAYLIST_ITEMS_TEMPLATE;
    // file should exist
    if (! is_readable($templateFileName )) {
        return;
    }
    $template = readLocalFile($templateFileName );
    
    $template = tmplReplace("XK_HOME",XK_HOME,$template);
    
    $items = "";
    foreach ($playlist->items as $item) {
        $items = $items.drawPlayListItemTemplate($item);
    }
    $template = tmplReplace("items",$items,$template);
        
    
    return $template; 
}

function drawPlayListItemTemplate($item) {
    $templateFileName = "templates/".TEMPLATE."/".PLAYLIST_ITEM_TEMPLATE;
    // file should exist
    if (! is_readable($templateFileName )) {
        return;
    }
    $template = readLocalFile($templateFileName );
    
    $template = tmplReplace("XK_HOME",XK_HOME,$template);
    
    $template = tmplReplace("name",$item->name,$template);
    $template = tmplReplace("number",$item->number,$template);
    $template = tmplReplace("length",$item->length,$template);
    $template = tmplReplace("uri",$item->uri,$template);
    
    return $template; 
}
?>
