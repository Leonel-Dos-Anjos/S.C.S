<?php
session_start();
header('Content-Type: application/json');
$conn = new mysqli('localhost', 'root', '', 'cabin_security');

if ($conn->connect_error) {
    die(json_encode(['status' => 'error', 'message' => 'Connection failed']));
}

$email = $_POST['email'] ?? '';
$password = $_POST['password'] ?? '';

$stmt = $conn->prepare("SELECT id, password, role FROM users WHERE email = ?");
$stmt->bind_param("s", $email);
$stmt->execute();
$stmt->store_result();

if ($stmt->num_rows > 0) {
    $stmt->bind_result($id, $hashed_pass, $role);
    $stmt->fetch();
    if (password_verify($password, $hashed_pass) && strpos($email, '@gmail.com') !== false) {
        $_SESSION['user_id'] = $id;
        $_SESSION['role'] = $role;
        echo json_encode(['status' => 'success', 'role' => $role]);
    } else {
        echo json_encode(['status' => 'error', 'message' => 'Invalid credentials']);
    }
} else {
    echo json_encode(['status' => 'error', 'message' => 'User not found']);
}

$stmt->close();
$conn->close();
?>
