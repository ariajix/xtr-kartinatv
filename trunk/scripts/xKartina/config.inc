<?php
    require_once "auth.inc";


    session_start();
    
    # home url since only absolute urls are allowed
    # define("XK_HOME", "http://127.0.0.1/media/sda1/scripts/xKartina/");
    define("XK_HOME", "http://" . $_SERVER['SERVER_ADDR'] . 
        dirname($_SERVER['SCRIPT_NAME']) . "/");

   	/*password for protected channels expires in 10 min*/
    define("EROTIC_PASS_LIFETIME", 10*60);

    define("TEMPLATE", "rss-default");

    define("CATEGORY_TEMPLATE", "channel_list.xml");
    define("CHANNEL_ITEM_TEMPLATE", "channel_item.xml");
    define("CHANNEL_MENU_ITEM_TEMPLATE", "channel_menu_item.xml");

    define("CATEGORY_LIST_TEMPLATE", "category_list.xml");
    define("CATEGORY_ITEM_TEMPLATE", "category_item.xml");

    define("EPG_LIST_TEMPLATE", "epg_list.xml");
    define("EPG_ITEM_TEMPLATE", "epg_item.xml");

    define("PLAYLISTS_TEMPLATE", "playlists.xml");
    define("PLAYLIST_TEMPLATE", "playlist.xml");
    define("PLAYLIST_ITEMS_TEMPLATE", "playlist_items.xml");
    define("PLAYLIST_ITEM_TEMPLATE", "playlist_item.xml");

    define("LOGIN_TEMPLATE", "login.xml");
    define("START_TEMPLATE", "start.xml");
    define("POPUP_TEMPLATE", "popup.xml");
?>
