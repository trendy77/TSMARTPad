 <?php
 //This file stores the data posted from the CC3000 in your MySQL database
function db_connect()
{
   $result = mysql_connect("localhost", "energy_user", "your_password"); 
   if (!$result)
      return false;
   return $result;
}
db_connect();
mysql_select_db("energy_project");
	// Store data
    if ($_GET["temp"] && $_GET["hum"]) {
    $temp = $_GET["temp"];
    $hum  = $_GET["hum"];
$sqlt = "insert into readings (sensor, time_stamp, value) values ('pool_temp',unix_timestamp(),$temp)";
$sqlh = "insert into readings (sensor, time_stamp, value) values ('pool_hum',unix_timestamp(),$hum)";
mysql_query($sqlt);
mysql_query($sqlh);
    }
?>