<?php
//this file collects the data from the energy_project database and puts it into 3 specially formated 
//data arrays that can be read by highcharts
//The original tutorial used for this code is from:
//http://blueflame-software.com/blog/how-to-load-mysql-results-to-highcharts-using-json/
//by Tim Kang
$con = mysql_connect("localhost","root", "tttt");

if (!$con) {
  die('Could not connect: ' . mysql_error());
}mysql_select_db("tsmartpad", $con);

$sth = mysql_query("SELECT value FROM readings where sensor='rm_temp'");
$rows = array();
$rows['name'] = 'Temperature';
while ($r = mysql_fetch_array($sth));
    $rows['data'][] = $r['value'];

$sth = mysql_query("SELECT stddev FROM readings where sensor='rm_temp'");
$rows2 = array();
$rows2['name'] = 'Temp STD DEV';
while ($rr = mysql_fetch_array($sth));
    $rows2['data'][] = $rr['stddev'];

$sth = mysql_query("SELECT samplesize FROM readings where sensor='rm_temp'");
$rows3 = array();
$rows3['name'] = 'Temp READINGS';
while($rrr = mysql_fetch_array($sth)) {
    $rows3['data'][] = $rrr['samplesize'];
}

/*
$sth = mysql_query("SELECT value FROM readings where sensor='rm_light'");
$rows1 = array();
$rows1['name'] = 'Light';
while($rr = mysql_fetch_assoc($sth)) {
    $rows1['data'][] = $rr['value'];
}
*/
$sth = mysql_query("SELECT timestamp as timestamp FROM readings where sensor='rm_temp'");
$rows4 = array();
$rows4['name'] = 'time';
while($rrrr = mysql_fetch_assoc($sth)) {
    $rows4['data'][] = $rrrr['timestamp'];
}


$result = array();
array_push($result,$rows2);
array_push($result,$rows);
///array_push($result,$rows1);
array_push($result,$rows4);

print json_encode($result, JSON_NUMERIC_CHECK);

mysql_close($con);
?>
