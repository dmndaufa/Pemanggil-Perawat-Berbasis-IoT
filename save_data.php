<?php
// Konfigurasi Database
$host = "localhost";
$user = "root";
$password = ""; 
$dbname = "rumahsakit"; 

// Koneksi ke database
$conn = new mysqli($host, $user, $password, $dbname);

// Periksa koneksi
if ($conn->connect_error) {
    die("Koneksi gagal: " . $conn->connect_error);
}

// Ambil data dari POST
$nama = $_POST['Nama'];
$tempat = $_POST['Tempat'];

// Query untuk menyimpan data
$sql = "INSERT INTO requests (nama, tempat) VALUES ('$nama', '$tempat')";

if ($conn->query($sql) === TRUE) {
    echo "Data berhasil disimpan!";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}

// Tutup koneksi
$conn->close();
?>
