<?php
session_start();
header('Content-Type: application/json');
$conn = new mysqli('localhost', 'root', '', 'cabin_security');

if (isset($_SESSION['user_id'])) {
    $stmt = $conn->prepare("INSERT INTO access_logs (user_id, event_type) VALUES (?, 'logout')");
    $stmt->bind_param("i", $_SESSION['user_id']);
    $stmt->execute();
    $stmt->close();
}

session_destroy();
echo json_encode(['status' => 'success']);
$conn->close();
?>
