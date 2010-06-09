<?php
#header("Content-Type: audio/mpegurl");
#header("Content-Disposition: attachment; filename=playlist.m3u");

require_once "ktvFunctions.inc";

$user = isset($HTTP_GET_VARS['user']) ? $HTTP_GET_VARS['user'] : "148";
$pass = isset($HTTP_GET_VARS['pass']) ? $HTTP_GET_VARS['pass'] : "841";
$timeshift = isset($HTTP_GET_VARS['timeshift']) ? $HTTP_GET_VARS['timeshift'] : "0";

$ktvFunctions = new KtvFunctions($user, $pass);

$timeshift = $ktvFunctions->setTimeShift($timeshift);
$content = $ktvFunctions->getChannelsList();

# compact all to a single line
$content = str_replace("\n","",$content);

# no spaces between tags
$content = str_replace('>[[:space:]]+', '>', $content);
$content = str_replace('[[:space:]]+<', '<', $content);

# parse out channels
$channels = explode('><',$content);

$path  = getFullUrlPath();

print '#EXTM3U' . "\n";
//print '#EXTVLCOPT:http-caching=1500' . "\n";
//print '#EXTVLCOPT:http-reconnect=0' . "\n";

$i=0;

foreach($channels as $k => $v) {
  $i=$i+1;
	if(strstr ($v,"channel ")) {
		//ereg('channel .*id="([^"]*)".*title="([^"]*)".*idx="([^"]*)".*', $v, $matches);
		ereg('id="([^"]*)"', $v, $matches_id);
		//$ids[]   = $matches_id[1];
		ereg('title="([^"]*)"', $v, $matches_name);
		//$names[] = $matches_name[1];
		ereg('idx="([^"]*)"', $v, $matches_idx);
		//$nums[]  = $matches_idx[1];
		  print '#EXTINF:0,';
      print $matches_idx[1] . ". " . $matches_name[1] . " (" .$matches_id[1]. ")\n";
	    print $path . "stream.php?id=".$matches_id[1]."&user=$user&pass=$pass&timeshift=0\n";
	}
}


# returns full URL of current script
function getFullUrlPath() { 
    $protocol = strtolower($_SERVER['SERVER_PROTOCOL']);
    $protocol = substr($protocol, 0, strpos($protocol, '/'));
    $protocol .= "on" === $_SERVER['HTTPS'] ? "s" : "";
    $port = "80" === $_SERVER['SERVER_PORT'] ? "" : (":" . $_SERVER['SERVER_PORT']);
    $url = $protocol . "://" . $_SERVER['HTTP_HOST'] . "$port";
    $path = $_SERVER['REQUEST_URI'];
    $path = substr($path, 0, strrpos($path, '/') + 1);
    return $url . $path;
}

?>
