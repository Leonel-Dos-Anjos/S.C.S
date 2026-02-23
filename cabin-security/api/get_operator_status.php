<?php
session_start();
header('Content-Type: application/json');
$conn = new mysqli('localhost', 'root', '', 'cabin_security');

if (!isset($_SESSION['user_id']) || $_SESSION['role'] !== 'admin') {
    die(json_encode(['status' => 'error', 'message' => 'Unauthorized']));
}

$timeout = 5 * 60; // 5 minutes in seconds
$current_time = time();

$result = $conn->query("SELECT u.id, u.email, MAX(al.timestamp) AS last_activity 
                        FROM users u 
                        LEFT JOIN access_logs al ON u.id = al.user_id 
                        WHERE u.role = 'operator' 
                        GROUP BY u.id, u.email");
$operators = [];
while ($row = $result->fetch_assoc()) {
    $last_activity = $row['last_activity'] ? strtotime($row['last_activity']) : 0;
    $status = ($last_activity && ($current_time - $last_activity < $timeout)) ? 'online' : 'offline';
    $operators[] = [
        'id' => $row['id'],
        'email' => $row['email'],
        'status' => $status
    ];
}

echo json_encode($operators);

$conn->close();
?>
