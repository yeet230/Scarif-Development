<?php
require_once 'auth.php';

// Enforce admin-only access
authorise(['admin', 'student', 'staff', 'teach']);

if (session_status() === PHP_SESSION_NONE) {
    session_start();
}
session_unset();
session_destroy();
header("Location: login.php");

exit;
