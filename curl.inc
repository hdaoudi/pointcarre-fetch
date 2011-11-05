<?php

class curl{
	
	var $ch;
	var	$LOGINURL;
	var $courses					=	array();
	var $documents					=	array();
	var $folders					=	array();
	var $pagelogin;
	public $headers;
	
	/* 1- Get First Login Page This page will set some cookies.*/
	function curl_init($server = '', $ssl = '', $refer=''){
		$this->ch			= curl_init();
		if($server == '') {$this->LOGINURL = "http://pointcarre.vub.ac.be";} else {$this->LOGINURL = $server;}
		if(!($refer =='')){curl_setopt($this->ch, CURLOPT_REFERER, $refer);}
    	curl_setopt($this->ch, CURLOPT_URL,$this->LOGINURL);
    	curl_setopt($this->ch, CURLOPT_RETURNTRANSFER, 1); 
		curl_setopt($this->ch, CURLOPT_FOLLOWLOCATION, 1);
		if($ssl=='ssl'){curl_setopt($this->ch, CURLOPT_SSL_VERIFYHOST, 2);  curl_setopt($this->ch, CURLOPT_SSL_VERIFYPEER, FALSE);}
    	$result 	= curl_exec($this->ch);
    	return $result;
	}

	/* 2- Post Login Data to Page */
	function curl_login($user_id, $user_password, $postlogin='', $postpass=''){
		if($postlogin == '' && $postpass =='') {
			$POSTFIELDS = 'login='. $user_id .'&password='. $user_password;
		} else { 
			$POSTFIELDS = $postlogin.'='.$user_id . '&'.$postpass.'='.$user_password;
		}
   		curl_setopt($this->ch, CURLOPT_URL,$this->LOGINURL);
    	curl_setopt($this->ch, CURLOPT_POST, 1); 
    	curl_setopt($this->ch, CURLOPT_POSTFIELDS,$POSTFIELDS); 
		curl_setopt($this->ch, CURLOPT_COOKIEJAR, "/");
    	$result 	= curl_exec($this->ch);
		$this->pagelogin	=	$result;
		return $result;
	}
	
	/* 3- Function to visit a given link //
	// An example is /courses/VUB12996/document/Part_I/sessions.PartI.html?cidReq=VUB12996&rand=7766*/
	function curl_link($link){
		curl_setopt ($this->ch, CURLOPT_SSL_VERIFYHOST, 0);
		curl_setopt ($this->ch, CURLOPT_SSL_VERIFYPEER, 0);
		curl_setopt($this->ch, CURLOPT_COOKIEFILE, "/");
   		curl_setopt($this->ch, CURLOPT_URL,$link);
		$result		= curl_exec($this->ch);
		curl_setopt($this->ch, CURLOPT_BINARYTRANSFER, 0);
		return $result;
	}

	function curl_file($link){
		set_time_limit(0);
		//$fp		=	fopen($doc, "w+");
		curl_setopt($this->ch, CURLOPT_URL,$link);
		curl_setopt($this->ch, CURLOPT_TIMEOUT, 50);
		//curl_setopt($this->ch, CURLOPT_BINARYTRANSFER, 1);
		//curl_setopt($this->ch, CURLOPT_FILE, $fp);
		$result = curl_exec($this->ch);
		//fclose($fp);

		/*$this->set_headers($result);
		curl_setopt($this->ch, CURLOPT_HTTPHEADER, $this->headers);
		
		$result = curl_exec($this->ch);
		$this->set_headers($result);*/
		return $result;
	}

	/* Get all course information on PointCarré i.e. links refering to http://pointcarre.vub.ac.be/courses/VUB*	*/
	function get_courses(){
		$content			=	$this->pagelogin;
		//$html 			= 	htmlspecialchars($content);
		$pattern			=	"/(<([a])(.*)(http:\/\/pointcarre.vub.ac.be\/courses\/)(?<code>VUB[a-z0-9]*)(\/)[^>]*>)(?<titel>.*)(<\/a><br \/>)/";
		preg_match_all($pattern, $content, $matches);
		$filter				=	array_combine($matches['code'],$matches['titel']); //Filter the array to the core info.
		$this->courses		= 	$filter;
	}	
	
	function get_folders(){
		foreach($this->courses as $code => $titel){
			$docpage		=	"http://pointcarre.vub.ac.be/main/document/document.php?cidReq=".$code;
			$content		=	$this->curl_link($docpage);
			//main/document/document.php?cidReq=VUB12996&amp;curdirpath=%2FReading
			$pattern		=	'/((VUB[0-9]*)(\&amp;curdirpath=%2F))(?<folder>.*)(")/U';
			preg_match_all($pattern,$content,$matches);
			$maps			=	$matches['folder'];
			$this->folders[$code] = $maps;
		}		
	}
	
	/* Fetch all documents from PointCarré*/
	function get_docs(){
		foreach($this->courses as $code => $titel){
			$files 			=	array();
			
			$docpage		=	"http://pointcarre.vub.ac.be/main/document/document.php?cidReq=".$code;
			$content		=	$this->curl_link($docpage);
			//http://pointcarre.vub.ac.be/courses/VUB8318/document/Task_Operation_management-pc.doc?cidReq=VUB8318
			$pattern		=	"/((VUB[0-9]*)(\/document\/))(?<file>.*)(\?cidReq=VUB[0-9]*)/U";
			preg_match_all($pattern,$content,$matches);
			$files			=	$matches['file'];
			
			//Get the documents that are in the folders
			$this->get_folders();
			foreach($this->folders[$code] as $key => $folder){
				$docpage		=	"http://pointcarre.vub.ac.be/main/document/document.php?cidReq=".$code."&curdirpath=%2F".$folder;
				$content		=	$this->curl_link($docpage);
				$pattern		=	"/(VUB[0-9]*)(\/document\/)(?<file>(".$folder."\/)(.*))(\?cidReq=VUB[0-9]*)/U";
				preg_match_all($pattern,$content,$matches);
				
				$files[$folder]	=	$matches['file'];
				//array_push($files, $matches['file']);
				
			}
			//print_r($files); echo "<br /><br />_______________1_______________<br />";
			$this->documents[$code] = $files;
		}
	}
	
	function curl_exit(){
		curl_close($this->ch);
	}		
}

?>