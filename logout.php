<?php
require_once 'auth.php';

// Enforce admin-only access
authorise(['admin', 'student', 'staff', 'teach']);

session_start();
session_unset();
session_destroy();
header("Location: login.php");
exit;
