#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>

// Konfigurasi Pin
#define SS_PIN 21
#define RST_PIN 22

MFRC522 mfrc522(SS_PIN, RST_PIN);

// Koneksi WiFi
const char* ssid = "Samsung";
const char* password = "permata45";

// Definisi pin
const int buzzer = 5;
const int buttonPin = 4;
const int ledPin = 2;

// Variabel global
String tempat;
String dataAlarm;
String perawat;
int buttonState = 0;

// Fungsi untuk menginisialisasi koneksi WiFi
void initWiFi() 
{
    WiFi.begin(ssid, password);
    Serial.println("Menghubungkan ke WiFi...");
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("Terhubung ke jaringan WiFi");
    Serial.print("Alamat IP: ");
    Serial.println(WiFi.localIP());
}

void setup()
{
    Serial.begin(9600);

    // Inisialisasi WiFi
    initWiFi();

    // Konfigurasi pin
    pinMode(ledPin, OUTPUT);
    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(buzzer, OUTPUT);

    // Inisialisasi RFID
    SPI.begin();
    mfrc522.PCD_Init();
    Serial.println("Tekan Tombol untuk Memanggil Perawat");
}

// Fungsi untuk mengirim permintaan HTTP POST
void httppost() 
{
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;
        
        // Menyiapkan data untuk permintaan HTTP POST
        String postData = "Nama=" + perawat + "&Tempat=" + tempat;

        http.begin("http://192.168.45.44/Nursecall/save_data.php"); // URL Webserver saya
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        int httpCode = http.POST(postData);
        String payload = http.getString();

        Serial.print("Kode HTTP: ");
        Serial.println(httpCode);
        Serial.print("Respon: ");
        Serial.println(payload);

        http.end();
    }
    else
    {
        Serial.println("WiFi tidak terhubung");
    }
}

void loop()
{
    // Periksa status tombol
    buttonState = digitalRead(buttonPin);

    if (buttonState == LOW)
    {
        digitalWrite(ledPin, HIGH);
        digitalWrite(buzzer, HIGH);
        dataAlarm = "ALARM";
        tempat = "Lantai 1, Ruangan A, Ranjang 1A";

        Serial.println(dataAlarm);
        Serial.println(tempat);

        httppost();
        delay(1000);
    }

    // Periksa kartu RFID baru
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
    {
        String content = "";

        for (byte i = 0; i < mfrc522.uid.size; i++)
        {
            content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
            content.concat(String(mfrc522.uid.uidByte[i], HEX));
        }

        content.toUpperCase();
        Serial.println("UID tag: " + content);

        // Perbandingan UID yang benar
        if (content == " 21 6B 37 00")
        {
            digitalWrite(ledPin, LOW);
            digitalWrite(buzzer, LOW);  // Mematikan buzzer
            perawat = "PERAWAT A";

            Serial.println(perawat);
            httppost();

            delay(5000);
            ESP.restart();
        }
        else if (content == " 73 4D B3 F7")
        {
            digitalWrite(ledPin, LOW);
            digitalWrite(buzzer, LOW);  // Mematikan buzzer
            perawat = "PERAWAT B";

            Serial.println(perawat);
            httppost();

            delay(5000);
            ESP.restart();
        }
    }
}
