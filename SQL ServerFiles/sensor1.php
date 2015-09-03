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
$stdev = ''; $samplesize = ' ';
 foreach($_GET as $key => $value){
  if(str_replace(','','','$value)!=$value){
   list($value,$stdev,$samplesize) = explode(',',$value);
   $sqlt = "insert into readings (sensor, time_stamp, value, stddev, samplesize) values ('".getridofcrap($key)."',unix_timestamp','".getridofcrap($value)."','".getridofcrap($stdev)."','".getridofcrap($samplesize).")";
  }else{
   $sqlt = "insert into readings (sensor, unix_timestamp(), value) values ('".getridofcrap($key)."unix_timestamp()".getridofcrap($value).")";
  }
  
  mysql_query($sqlt);
  
 }
 function getridofcrap($str){
  $str = str_replace("'","",$str);
  $str = str_replace('"',' ',$str);
  return $str;
 }
 
 die;
 