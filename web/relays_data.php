<?php

ini_set("display_errors","1");              // Display all
error_reporting(E_ALL);                     // Error if exist

$servername = "localhost";
$username = "reusnguyen1199";
$password = "1199";
$dbname = "IoT";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}
else
{
    $relays_data = array();

    $result = mysqli_query($conn, "select Light, Pump FROM Relays ORDER BY id desc LIMIT 20");

    while($row = mysqli_fetch_array($result))
    {  
        $temp= array("light" => $row['Light'] , "pump" => $row['Pump']);
        array_push($relays_data, $temp);
    }
    echo json_encode($relays_data, JSON_NUMERIC_CHECK);  
}
mysqli_close($conn);
?> 
