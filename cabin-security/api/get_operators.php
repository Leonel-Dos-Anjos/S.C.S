<?php
session_start();
header('Content-Type: application/json');
$conn = new mysqli('localhost', 'root', '', 'cabin_security');

if (!isset($_SESSION['user_id']) || $_SESSION['role'] !== 'admin') {
    die(json_encode(['status' => 'error', 'message' => 'Unauthorized']));
}

$result = $conn->query("SELECT u.id, u.email, f.fingerprint_id FROM users u LEFT JOIN fingerprints f ON u.id = f.user_id WHERE u.role = 'operator'");
$operators = [];
while ($row = $result->fetch_assoc()) {
    $operators[] = $row;
}
echo json_encode(['status' => 'success', 'operators' => $operators]);

$conn->close();
?>
