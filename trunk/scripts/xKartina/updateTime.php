<?php 
echo "<?xml version='1.0' encoding='utf-8' ?>\n";
	$ntpServ = "ntps1-0.uni-erlangen.de";
	#update system Time
	system("rdate -s ".$ntpServ);
	#update hw clock
	system("hwclock -w");
	date_default_timezone_set('Europe/Berlin'); 
	
?>

<rss version="2.0" xmlns:media="http://purl.org/dc/elements/1.1/" xmlns:dc="http://purl.org/dc/elements/1.1/">


<mediaDisplay name="onePartView"
				itemXPC="18"  
				itemWidthPC="80" 
/>

<channel>
	<title>Время актуализировано: <?php echo date(DATE_RFC822, time()); ?></title>

<item>
	<title>Вернуться.</title>
	<link>http://127.0.0.1/media/sda1/scripts/xKartina/categoryList.php</link>
</item>


</channel>
</rss>
