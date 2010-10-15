<?php
#############################################################################
# Library with all functions needed to connect kartina.tv.                  #
# These functions allow to log on to kartina.tv, get information about      #
# available channels and to get the channels URLs themselves.               #
#                                                                           #
# Author: consros 2008                                                      #
#############################################################################

require_once "settings.inc";
require_once "tools.inc";
require_once "lang.inc";
require_once "loginDialog.inc";

		global $username;
		global $password;
		
define("COOKIE_NAME", "KtvCookie");
define("BASE_URL", "http://iptv.kartina.tv/");

class KtvFunctions {
    var $traces;
    var $cookie;
    var $authorizationErrorText;
    

    function KtvFunctions($traces = false) {
        $this->traces = $traces;
        $this->cookie = isset($_SESSION[COOKIE_NAME]) ? 
            $_SESSION[COOKIE_NAME] : "";
    }

    function forgetCookie() {
        $_SESSION[COOKIE_NAME] = "";
        $this->cookie = "";
    }

    function trace($msg) {
        if ($this->traces) {
            print "DEBUG: " . USERNAME . ": $msg\n";
        }
    }
    
	function login($username, $password) {
        $this->trace("Authorization started");
        $params  = "act=login";
        $params .= "&code_login=" . $username;
        $params .= "&code_pass="  . $password;
        $html = getPageContentByPost(BASE_URL, $params);
        $this->cookie = parseOutCookies($html);
        if(!$this->isAuthorized($html)) {
        	return FALSE;
        }
        $this->trace("Authorization returned: $this->cookie");
        $_SESSION[COOKIE_NAME] = $this->cookie;
        return TRUE;
    }
    
    function authorize() {
    	
    	$iniPath = "auth.ini";
    	// Parse without sections
	    if ( is_readable($iniPath )) {
	    	$ini_array = parse_ini_file($iniPath);	
		}
		
		global $username;
		global $password;
		global $loginMsg;
		if(!isset($ini_array['username'])) {
			$username = USERNAME;
		} else {
			$username = $ini_array['username'];
		}
		
		if(!isset($ini_array['password'])) {
			$password = PASSWORD;
		} else {
			$password = $ini_array['password'];
		}
		
		
		if( ($username >= '140' && $username <='149') && 
			(!isset($ini_array['confirmed']) || $ini_array['confirmed'] != '1')) {
			//first start with demo account
			$loginMsg = "Вы пользуетесь пробным доступом. Введите свои данные или подтвердите пробный доступ кнопкой \"Вход\".";
			$dialog = new LoginDialog();
			$dialog->showLogin($username, $password, $loginMsg);
			exit;
		} else {
			if(!$this->login($username, $password)) {
				//error by login
				$loginMsg = 'Ошибка входа ('.$this->authorizationErrorText.'). Пожалуйста проверьте правильность абонемента и пароля.';
				//header( 'Location: '.XK_HOME.'login.php') ;
				$dialog = new LoginDialog();
				$dialog->showLogin($username, $password, $loginMsg);
				exit;
			}
			return TRUE;
		}
    }

    function isAuthorized($reply) {
        $ok = isset($this->cookie) &&
            0 != strlen($this->cookie) &&
            false == strpos($this->cookie, "deleted") &&
            false == strpos($reply, "code_login") && 
            false == strpos($reply, "code_pass") && 
            false == strpos($reply, "msg=access_denied") && 
            false == strpos($reply, "msg=auth_err");
        if (! $ok) {
            $this->trace("Authorization missed or lost");
            $authorizationErrorText = $this->getAuthorizationError($reply);
        }
        else {
        	$authorizationErrorText = "";
        }
        return $ok;
    }
    function getAuthorizationError($reply) {
    	if(strpos($this->cookie, "deleted")) {
    		return "Аккаунт удалён";
    	}
    	if(strpos($this->cookie, "code_login")) {
    		return "Наверный логин";
    	}
		if(strpos($this->cookie, "code_pass")) {
    		return "Неверный пароль";
    	}
    	if(strpos($this->cookie, "msg=access_denied")) {
    		return "Доступ запрещён";
    	}
    	if(strpos($this->cookie, "msg=auth_err")) {
    		return "Ошибка авторизации";
    	}
        return "Неизвестная ошибка";
    }

    function getData($url, $name) {
        $this->trace("Getting $name");
        $reply = getPageContentByGet($url, $this->cookie);
        if (! $this->isAuthorized($reply)) {
            $this->authorize(); 
            $this->trace("Second try to get $name");
            $reply = getPageContentByGet($url, $this->cookie);
            if($this->isAuthorized($reply))
        		return $reply;    
        }
        return null;
    }

    function getChannelsList() {
        $url = BASE_URL . "?m=channels&act=get_list_xml";
        return $this->getData($url, "channels list");
    }

    function getStreamUrl($id, $time = null) {
        $url = BASE_URL . "?m=channels&act=get_stream_url&cid=$id";
        if (isset($time) && null != $time) {
            $url .= "&gmt=$time";
        }
        if (ALLOW_EROTIC) {
            $url .= "&protect_code=" . PASSWORD;
        }
        return $this->getData($url, "URL of stream $id");
    }
    
    function getChannelsInfo($ids) {
        $param = "";
        foreach($ids as $id) {
            $param .= "" == $param ? "" : ",";
            $param .= $id;
        }
        $url  = BASE_URL . "?m=channels&act=get_info_xml&cids=";
        $url .= urlencode($param);
        return $this->getData($url, "info of streams");
    }

    function getEpg($id, $date = null) {
        # date format is yymmdd: 981231
        # NOTE: at 03:00 starts another EPG day
        $date = isset($date) ? date(EPG_DATE_FORMAT, $date) : 
            date(EPG_DATE_FORMAT, NOW_TIME);
        $url  = BASE_URL;
        $url .= "?m=epg&act=show_day_xml&day=$date&cid=$id";
        return $this->getData($url, "EPG for channel $id");
    }

    function getTimeShift() {
        $url = BASE_URL . "?m=clients&act=form_tshift";
        return $this->getData($url, "time shift");
    }

    function setTimeShift($timeShift) {
        $url  = BASE_URL;
        $url .= "?act=x_set_timeshift&m=clients&ts=$timeShift";
        return $this->getData($url, "(setting) time shift $timeShift");
    }    

    function getBroadcastingServer() {
        $url = BASE_URL . "?m=clients&act=form_sserv";
        return $this->getData($url, "broadcasting server");
    }

    function setBroadcastingServer($server) {
        $server = $server == 1 ? 3 : 1;
        $url = BASE_URL;
        $url .= "?act=x_set_sserv&m=clients&ssrv=$server";
        return $this->getData($url, "(selecting) server $server");
    }    
}
?>
