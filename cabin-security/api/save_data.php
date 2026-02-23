<?php
header('Content-Type: application/json');
$conn = new mysqli('localhost', 'root', '', 'cabin_security');

if ($conn->connect_error) {
    die(json_encode(['status' => 'error', 'message' => 'Connection failed']));
}

$temp = $_POST['temperature'] ?? 0;
$humid = $_POST['humidity'] ?? 0;
$flame = $_POST['flame'] ?? 0;

$stmt = $conn->prepare("INSERT INTO sensor_logs (temperature, humidity, flame) VALUES (?, ?, ?)");
$stmt->bind_param("ddi", $temp, $humid, $flame);

if ($stmt->execute()) {
    echo json_encode(['status' => 'success']);
} else {
    echo json_encode(['status' => 'error', 'message' => $stmt->error]);
}

$stmt->close();
$conn->close();
?>
