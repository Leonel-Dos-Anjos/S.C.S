<?php
session_start();
header('Content-Type: application/json');
$conn = new mysqli('localhost', 'root', '', 'cabin_security');

if (!isset($_SESSION['user_id'])) {
    die(json_encode(['status' => 'error', 'message' => 'Not logged in']));
}

$result = $conn->query("SELECT * FROM sensor_logs ORDER BY timestamp DESC LIMIT 1");
$data = $result->fetch_assoc();
echo json_encode($data);

$conn->close();
?>
