<?php
	require_once "auth.inc";
	require_once "time.inc";

	define("XK_HOME", "http://192.168.178.22/media/sda1/xKartina/");
	# now time 
#	define("USE_NETWORK_TIME", true);
#	define("NTP_SERVER", "time.nist.gov");
#	define("TIME_SERVER", "time.ien.it");
#	define("TIME_ZONE", 1);
	# date format accepted by EPG
#	define("EPG_DATE_FORMAT", 'dmy');
#	if(USE_NETWORK_TIME) {
//		define("NOW_TIME", getNtpTime(NTP_SERVER,37) + (TIME_ZONE+1)*60*60);
#		define("NOW_TIME", getTime868("time.ien.it",37));
#	}
#	else {
#		define("NOW_TIME", time());		
#	}
	
	define("TEMPLATE", "rss-default");
	define("CATEGORY_TEMPLATE", "channel_list.xml");
	define("CHANNEL_ITEM_TEMPLATE", "channel_item.xml");
	
	define("CATEGORY_LIST_TEMPLATE", "category_list.xml");
	define("CATEGORY_ITEM_TEMPLATE", "category_item.xml");
	
	define("EPG_LIST_TEMPLATE", "epg_list.xml");
	define("EPG_ITEM_TEMPLATE", "epg_item.xml");
	
?>