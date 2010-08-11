<?php
	require_once "config.inc";
	require_once "playlistParser.inc";
	require_once "rssTools.inc";
	require_once "tools.inc";
	
	$playlistDir = "playlists";
	$playlists = array();
	if(isset($_GET['playlist'])) {
		$parser = new M3uParser();
		$parser->parseFile($playlistDir."/".$_GET['playlist'], $_GET['playlist']);
		$playlists[] =$parser;
	}
	else { 
		if ($handle = opendir($playlistDir)) {
			
		    while (false !== ($file = readdir($handle))) {
		    	if ("." != $file && ".." != $file) {
					if(substr($file,strlen($file)-4) == ".m3u") {
			    		$parser = new M3uParser();
						$parser->parseFile($playlistDir."/".$file, $file);
						$playlists[] =$parser; 
					    /*foreach ($parser->items as $item) {
							echo "Item:". $item->name . " uri:" . $item->uri. " lenght:".$item->length."\r\n";
						}*/
					}
		    	}
	    	}
		    closedir($handle);
		}
	}
	echo drawPlaylistsTemplate($playlists);
?>