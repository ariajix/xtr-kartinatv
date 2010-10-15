<?php
require_once "rssTools.inc";
require_once "config.inc";
require_once 'tools.inc';
require_once "ktvFunctions.inc";
require_once "loginDialog.inc";

$ktvFunctions = new KtvFunctions();

if(isset($_GET['username']) && isset($_GET['password'])) {
	
	if($ktvFunctions->login($_GET['username'], $_GET['password'])) {
		
		$iniArr = array('username'=>$_GET['username'],'password'=>$_GET['password'],'confirmed'=>TRUE);	

		write_ini_file($iniArr,'auth.ini',FALSE);
		
		header( 'Location: '.XK_HOME.'categoryList.php') ;
	}
	else {
		$dlg = new LoginDialog();
		$dlg->showLogin($_GET['username'], $_GET['password'], 'Ошибка входа ('.$ktvFunctions->authorizationErrorText.'). Пожалуйста проверьте правильность абонемента и пароля.'); 
	}
}
else {

	$ini_array = parse_ini_file("auth.ini");
	echo drawLoginTemplate($ini_array['username'], $ini_array['password'], $loginMsg);
}

?>