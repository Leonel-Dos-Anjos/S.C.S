CREATE DATABASE cabin_security;
USE cabin_security;

CREATE TABLE users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    email VARCHAR(255) NOT NULL UNIQUE,
    password VARCHAR(255) NOT NULL,
    role ENUM('admin', 'operator') NOT NULL
);

CREATE TABLE fingerprints (
    id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT,
    fingerprint_id INT NOT NULL UNIQUE,
    FOREIGN KEY (user_id) REFERENCES users(id)
);

CREATE TABLE sensor_logs (
    id INT AUTO_INCREMENT PRIMARY KEY,
    temperature FLOAT NOT NULL,
    humidity FLOAT NOT NULL,
    flame INT NOT NULL,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE access_logs (
    id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT,
    event_type ENUM('unlock', 'lock', 'login', 'logout') NOT NULL,
    fingerprint_id INT,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(id)
);

INSERT INTO users (email, password, role) VALUES 
('admin@gmail.com', '$2y$10$YOUR_HASHED_PASSWORD', 'admin'),
('operator@gmail.com', '$2y$10$YOUR_HASHED_PASSWORD', 'operator');
