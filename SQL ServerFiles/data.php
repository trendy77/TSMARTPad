<?php
//this file collects the data from the energy_project database and puts it into 3 specially formated 
//data arrays that can be read by highcharts
//The original tutorial used for this code is from:
//http://blueflame-software.com/blog/how-to-load-mysql-results-to-highcharts-using-json/
//by Tim Kang
$con = mysql_connect("localhost","energy_user", "your_password");

if (!$con) {
  die('Could not connect: ' . mysql_error());
}

mysql_select_db("energy_project", $con);

$sth = mysql_query("SELECT value FROM readings where sensor='pool_temp'");
$rows = array();
$rows['name'] = 'Temperature';
while($r = mysql_fetch_array($sth)) {
    $rows['data'][] = $r['value'];
}

$sth = mysql_query("SELECT value FROM readings where sensor='pool_hum'");
$rows1 = array();
$rows1['name'] = 'Humidity';
while($rr = mysql_fetch_assoc($sth)) {
    $rows1['data'][] = $rr['value'];
}

$sth = mysql_query("SELECT time_stamp*1000 as time_stamp FROM readings where sensor='pool_hum'");
$rows2 = array();
$rows2['name'] = 'time';
while($rrr = mysql_fetch_assoc($sth)) {
    $rows2['data'][] = $rrr['time_stamp'];
}


$result = array();
array_push($result,$rows2);
array_push($result,$rows);
array_push($result,$rows1);


print json_encode($result, JSON_NUMERIC_CHECK);

mysql_close($con);
?>
