Search a student<br />
<?php

/* PointCarré Student Fetch - by Hassan Daoudi - June 2009 */

//Include curl library and initialize curl connection to PointCarré
include_once('pointcarre.functions.inc');
//include_once('modules/pointcarre/pointcarre.functions.inc');
$pc	= new curl;
$pc->curl_init();
global $user;


if(!isset($_POST["searchstud"])){
	echo "<form method='post'>
		Enter student's firstname	<input type='text' name='firstname' /><br />
		and/or enter student's lastname	<input type='text' name='lastname' /><br />
		<input name='searchstud' type='submit' />
		</form>   </div>";
} else {

	$name		=	$_POST["firstname"];
	$lastname	=	$_POST["lastname"];
	
	//$pc->curl_login($user->name, $user->basic_webmail_passsword);
	$pc->curl_login();
	$url	=	"http://pointcarre.vub.ac.be/main/curriculum/search.php?action=info&organisation=vub&student=&type=student&firstname=".$name."&lastname=".$lastname;
	$page	=	$pc->curl_link($url);
	
	//Delete all irrelevant html code
	$firstPos 	=	strpos($page,'<h3>Search curriculum information</h3>');
	$lastPos	=	strpos($page,'<div id="footer">');
	$filter 	= 	substr($page,$firstPos,$lastPos-$firstPos);
	
	//Remove the name links referring back to PointCarré - e.g. <a href="?action=info&organisation=vub&student=80749">Daoudi</a>
	$pattern	=	'/(<([a])(.*)(\?action=info&organisation=vub&student=)([0-9]*)(">))/';
	$result		=	preg_replace($pattern, '', $filter);
	$students 	=	str_replace('mailto:', '/VUB Services/?q=basic_webmail/sendmail/', $result);
	
	echo $students;

}

$pc->curl_exit();
?>







