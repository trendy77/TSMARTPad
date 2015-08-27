 <?php
 //This file stores the data posted from the CC3000 in your MySQL database
function db_connect()
{
   $result = mysql_connect("localhost", "root", ""); 
   if (!$result)
      return false;
   return $result;
}
db_connect();
mysql_select_db("energy_project");
	// Store data
	$value = ' ';
 foreach($_GET as $key => $value ){
  if(str_replace(',','',$value)!=$value){
list($value,$stddev,$samplesize) = explode(',',$value);
   
   $sqlt = "INSERT INTO readings (sensor, time_stamp, value, stddev, samplesize) values 
   ('".getridofcrap($key)."','".datetime()."','".getridofcrap($value)."','".getridofcrap($stddev)."','".getridofcrap($samplesize)."')";
  }else{
   $sqlt = "insert into readings (sensor, time_stamp, value, stddev, samplesize) values ('".getridofcrap($key)."',datetime(),'".getridofcrap($value)."')";
  }
  
 mysql_query($sqlt);
  $a = mysql_error();
  if(strlen($a)>0) echo $a;
 }
 function getridofcrap($str){
  $str = str_replace("'","",$str);
  $str = str_replace('"',' ',$str);
return $str;  
 }

 header('index.php');
 
?>
  