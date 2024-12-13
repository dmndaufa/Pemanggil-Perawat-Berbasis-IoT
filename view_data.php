<?php
// Konfigurasi Database
$host = "localhost";
$user = "root";
$password = ""; 
$dbname = "rumahsakit"; 

// Koneksi ke database
$conn = new mysqli($host, $user, $password, $dbname);

// Periksa koneksi
if ($conn->connect_error) 
{
    die("Koneksi gagal: " . $conn->connect_error);
}

// Query untuk mengambil data dari tabel requests
$sql = "SELECT * FROM requests ORDER BY id DESC"; // Mengambil data terbaru
$result = $conn->query($sql);

// Tampilkan data dalam bentuk tabel HTML
if ($result->num_rows > 0) 
{
    echo "<table border='1'>";
    echo "<tr><th>ID</th><th>Nama</th><th>Tempat</th><th>Waktu</th></tr>"; 
    while ($row = $result->fetch_assoc()) 
    {
        echo "<tr>";
        echo "<td>" . $row["id"] . "</td>";
        echo "<td>" . $row["nama"] . "</td>";
        echo "<td>" . $row["tempat"] . "</td>";
        echo "<td>" . $row["waktu"] . "</td>";  
        echo "</tr>";
    }
    echo "</table>";
} else {
    echo "Tidak ada data";
}

// Tutup koneksi
$conn->close();
?>
