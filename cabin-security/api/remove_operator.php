<?php
session_start();
header('Content-Type: application/json');
$conn = new mysqli('localhost', 'root', '', 'cabin_security');

if (!isset($_SESSION['user_id']) || $_SESSION['role'] !== 'admin') {
    die(json_encode(['status' => 'error', 'message' => 'Unauthorized']));
}

if ($conn->connect_error) {
    die(json_encode(['status' => 'error', 'message' => 'Connection failed']));
}

$user_id = isset($_POST['user_id']) ? (int)$_POST['user_id'] : 0;

if ($user_id <= 0) {
    die(json_encode(['status' => 'error', 'message' => 'Invalid user ID']));
}

$stmt = $conn->prepare("DELETE FROM users WHERE id = ? AND role = 'operator'");
$stmt->bind_param("i", $user_id);

if ($stmt->execute()) {
    if ($stmt->affected_rows > 0) {
        echo json_encode(['status' => 'success', 'message' => 'Operator removed']);
    } else {
        echo json_encode(['status' => 'error', 'message' => 'Operator not found']);
    }
} else {
    echo json_encode(['status' => 'error', 'message' => $conn->error]);
}

$stmt->close();
$conn->close();
?>
