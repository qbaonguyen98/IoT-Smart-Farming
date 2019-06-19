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
    $data_points1 = array();
    $data_points2 = array();
    $data_points3 = array();
    $data_points4 = array();

    $result = 
    mysqli_query($conn, "select Time, Moisture, Brightness, Temperature, Humidity FROM Sensors ORDER BY id desc LIMIT 20");

    while($row = mysqli_fetch_array($result))
    {        
        $moisture = array("label" => $row['Time'] , "y" => $row['Moisture']);
        array_push($data_points1, $moisture);    
        $brightness = array("label" => $row['Time'] , "y" => $row['Brightness']);
        array_push($data_points2, $brightness);  
        $temperature = array("label" => $row['Time'] , "y" => $row['Temperature']);
        array_push($data_points3, $temperature); 
        $humidity = array("label" => $row['Time'] , "y" => $row['Humidity']);
        array_push($data_points4, $humidity);  
    }
    $data_points = array_merge($data_points1, $data_points2, $data_points3, $data_points4);
    echo json_encode($data_points, JSON_NUMERIC_CHECK);          
}
mysqli_close($conn);
?> 
