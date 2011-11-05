<?php

include_once('pointcarre.functions.inc');

$pc	= new curl;
$pc->pointcarre_init();
$pc->pointcarre_login();
//$page = $pc->pointcarre_login();

echo "<form method='post'>
		Click here to update your courses	<input name='updateCourses' type='submit' />
		</form>   </div>";
		
if(isset($_POST["updateCourses"])) {

	$pc->get_courses();
	print_r($pc->courses);

}

echo "<form method='post'>
		Click here to update your documents	<input name='updateDocs' type='submit' />
		</form>   </div>";

if(isset($_POST["updateDocs"])) {

	//	Get all document information and put them in an array for all the courses! 
	$pc->get_courses();
	$pc->get_docs();
	$docs=$pc->documents;
	print_r($docs);

	//	List all the links to the files!
	foreach($docs as $code => $file){
		foreach($file as $doc){
			if(is_array($doc)){
				foreach($doc as $folderfile){
					$url = "http://pointcarre.vub.ac.be/courses/".$code."/document/".$folderfile."?cidReq=".$code."<br />";
					echo $url."<br />";
				}
			} else {
				$url = "http://pointcarre.vub.ac.be/courses/".$code."/document/".$doc."?cidReq=".$code."<br />";
				echo $url."<br />";
			}			
		}
	}
}


echo "<form method='post'>
		Click here to get file Class_Diagrams.xls	<input name='getdoc' type='submit' />
		</form>   </div>";

if(isset($_POST["getdoc"])) {

//http://pointcarre.vub.ac.be/courses/VUB12996/document/UML_/Class_Diagrams.xls?cidReq=VUB12996
$url	=	"http://pointcarre.vub.ac.be/courses/VUB12996/document/Part_I/sessions.PartI.html?cidReq=VUB12996&rand=7766";
$page	=	$pc->pointcarre_file($url, "Session.html");
return $page;
}

$pc->pointcarre_exit();
?>
