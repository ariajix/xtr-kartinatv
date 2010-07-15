<?php
	require_once("playlistParser.inc");
	
	$playlistDir = "playlists";
	if ($handle = opendir($playlistDir)) {

	    while (false !== ($file = readdir($handle))) {
	    	if ("." != $file && ".." != $file) {

	    		$parser = new M3uParser();
				$parser->parseFile($playlistDir."/".$file);
			    foreach ($parser->items as $item) {
					echo "Item:". $item->name . " uri:" . $item->uri. " lenght:".$item->length."\r\n";
				}
	    	}
    	}
	    closedir($handle);
	}
?>