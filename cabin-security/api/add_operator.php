<?php
session_start();
header('Content-Type: application/json');

if (!isset($_SESSION['user_id']) || $_SESSION['role'] !== 'admin') {
    die(json_encode(['status' => 'error', 'message' => 'Unauthorized']));
}

$conn = new mysqli('localhost', 'root', '', 'cabin_security');
if ($conn->connect_error) {
    die(json_encode(['status' => 'error', 'message' => 'Connection failed']));
}

$email = $_POST['email'] ?? '';
$password = $_POST['password'] ?? '';
$fingerprint_id = $_POST['fingerprint_id'] ?? '';

if (empty($email) || empty($password) || empty($fingerprint_id)) {
    die(json_encode(['status' => 'error', 'message' => 'Email, password, and fingerprint ID are required']));
}
if (strpos($email, '@gmail.com') === false) {
    die(json_encode(['status' => 'error', 'message' => 'Email must be a Gmail address']));
}
if (strlen($password) < 6) {
    die(json_encode(['status' => 'error', 'message' => 'Password must be at least 6 characters']));
}

// 1) Verifica se já existe
$stmt = $conn->prepare("SELECT COUNT(*) FROM users WHERE email = ?");
$stmt->bind_param("s", $email);
$stmt->execute();
$stmt->bind_result($count);
$stmt->fetch();
$stmt->close();

if ($count > 0) {
    echo json_encode(['status' => 'exists', 'message' => 'Email already registered']);
    $conn->close();
    exit;
}

// 2) Insere operador
$hash = password_hash($password, PASSWORD_DEFAULT);
$stmt = $conn->prepare("INSERT INTO users (email, password, role) VALUES (?, ?, 'operator')");
$stmt->bind_param("ss", $email, $hash);

if ($stmt->execute()) {
    echo json_encode(['status' => 'success', 'message' => 'Operator added']);
} else {
    echo json_encode(['status' => 'error', 'message' => $stmt->error]);
}

$stmt->close();
$conn->close();
?>
