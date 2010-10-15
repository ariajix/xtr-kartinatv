<?php
class LoginDialog {
	function showLogin($username, $password, $loginMsg) {
		echo drawLoginTemplate($username, $password, $loginMsg);
	}
}


/*
require_once "rssTools.inc";
require_once "config.inc";
require_once 'tools.inc';
require_once "ktvFunctions.inc";
$ktvFunctions = new KtvFunctions();

if(isset($_GET['username']) && isset($_GET['password'])) {
	
	if($ktvFunctions->login($_GET['username'], $_GET['password'])) {
		
		$iniArr = array('username'=>$_GET['username'],'password'=>$_GET['password'],'confirmed'=>TRUE);	

		write_ini_file($iniArr,'auth.ini',FALSE);
		
		header( 'Location: '.XK_HOME.'categoryList.php') ;
	}
	else {
		LoginManager $mgr = new LoginManager();
		$mgr->showLogin($username, $password, $loginMsg)
	}
}
else {
	$msg = "";
/*	$ini_array = parse_ini_file("auth.ini");
	if($ini_array['username'] >= '140' && $ini_array['username']<='149') {
		$msg = "Демо Аккаунт!";
	}

	global $username;
	global $password;
	global $loginMsg;
	if($ktvFunctions->authorize()){
		if($username >= '140' && $username <='149') {
			$msg = "Вы используете демоаккаунт. Введите собственные данные или подтвердите демоаккаунт кнопкой 1.";
		}	
	}
	else {
		$msg = 'Ошибка входа ('.$ktvFunctions->authorizationErrorText.'). Пожалуйста проверьте правильность абонемента и пароля.';
	}
	$mgr->showLogin($username, $password, $loginMsg)
	echo drawLoginTemplate($username, $password, $loginMsg);
}

*/
?>