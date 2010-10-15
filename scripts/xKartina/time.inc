<?php
function getNtpTime($timeserver, $socket) {
  $fp = fsockopen($timeserver,$socket,$err,$errstr,5) or die("FSOCKOPEN FAILED");
  if ($fp) {
    fputs($fp,"\n");
//  $tim = 0;  
//    fscanf ($fp, "%d",$tim);
    $timevalue = fread($fp,49);
    fclose($fp); // close the connection
	$tm = substr($timevalue,6,17);
  	return strtotime($tm);    
  } else {
    return -1;
  }
}

function query_time_server ($timeserver, $socket) {
/* Query a time server
   (C) 1999-09-29, Ralf D. Kloth (QRQ.software) <ralf at qrq.de> */

  $fp = fsockopen($timeserver,$socket,$err,$errstr,5);
        # parameters: server, socket, error code, error text, timeout
  if ($fp) {
   // fputs($fp,"\n");
    $timevalue = fread($fp,49);
    fclose($fp); # close the connection
  }
  else {
    $timevalue = " ";
  }

  $ret = array();
  $ret[] = $timevalue;
  $ret[] = $err;     # error code
  $ret[] = $errstr;  # error text
  return($ret);

} # function query_time_server 


function getTime868($timeserver, $port) {
//	$timeserver = "ntp1.sf-bay.org";
//	$timercvd = query_time_server($timeserver,37);
	$fp = fsockopen($timeserver,$port,$err,$errstr,5);
	$tmestamp = 0;
	if ($fp) {
    	$timevalue = fread($fp,8);

    	fclose($fp); # close the connection
    	$timevalue = bin2hex ($timevalue);
	  	$timevalue = abs (HexDec('7fffffff') - HexDec($timevalue) - HexDec('7fffffff')) ;
	  	$tmestamp = (int)($timevalue - 2208988800.0); # convert to UNIX epoch time stamp
  	}
	return $tmestamp;
}

?>