
Saturday, August 15th at 2:48 am
foreach($_GET as $key => $value){
  if(str_replace(',','',$value)!=$value){
   list($value,$stdev) = explode(',',$value);
   $sqlt = "insert into readings (sensor, time_stamp, value,stddev) values ('".getridofcrap($key)."',unix_timestamp(),'".getridofcrap($value)."','".getridofcrap($stdev)."')";
  }else{
   $sqlt = "insert into readings (sensor, time_stamp, value) values ('".getridofcrap($key)."',unix_timestamp(),".getridofcrap($value).")";
  }
  
  mysql_query($sqlt);
  
 }

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
mysql_select_db("tsmartpad");
 // Store data
 foreach($_GET as $key => $value){
  if(str_replace(',','',$value)!=$value){
   list($value,$stddev) = explode(',',$value);
   $sqlt = "insert into readings (sensor, time_stamp, value) values ('".getridofcrap($key)."',unix_timestamp(),'".getridofcrap($value)."','".getridofcrap($stdev)."')";
  }else{
   $sqlt = "insert into readings (sensor, time_stamp, value) values ('".getridofcrap($key)."',time_stamp(),".getridofcrap($value).")";
  }
  
  mysql_query($sqlt);
  

$stdev = '';
 foreach($_GET as $key => $value){
  if(str_replace(',','',$value)!=$value){
   list($value,$stdev) = explode(',',$value);
   $sqlt = "insert into readings (sensor, time_stamp, value,stddev) values ('".getridofcrap($key)."',unix_timestamp(),'".getridofcrap($value)."','".getridofcrap($stdev)."')";
  }else{
   $sqlt = "insert into readings (sensor, time_stamp, value) values ('".getridofcrap($key)."',unix_timestamp(),".getridofcrap($value).")";
  }
  
  mysql_query($sqlt);
  
 }


db_connect();
mysql_select_db("energy_project");
$stdev = ''; $samplesize = ' ';
 foreach($_GET as $key => $value){
  if(str_replace(',','',$value)!=$value){
   list($value,$stdev) = explode(',',$value);
   $sqlt = "insert into readings (sensor, time_stamp, value,stddev, samplesize) values ('".getridofcrap($key)."',datetime()','".getridofcrap($value)."','".getridofcrap($stdev)."','".getridofcrap($samplesize).")";
  }else{
   $sqlt = "insert into readings (sensor, time_stamp, value) values ('".getridofcrap($key)."',timestamp(),".getridofcrap($value).")";
  }
  
  mysql_query($sqlt);
  
 }
 function getridofcrap($str){
  $str = str_replace("'","",$str);
  $str = str_replace('"',"",$str);
  return $str;
 }
 
 die;
 
