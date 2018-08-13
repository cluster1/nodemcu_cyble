# NodeMCU Itron Cyble 
Aplikasi ini membaca hasil bacaan dari sensor Itron Cyble Sensor v2 2 wire version  [https://www.itron.com/eu/technology/product-services-catalog/products/2/4/4/cyble-sensor]. 
Data yang terbaca ditampilkan diserial. Device menggunakan NodeMCU ESP8266

Scrip ditulis menggunakan Arduino IDE. Untuk menggunakan NodeMCU pada Arduino IDE, tutorial & library bisa dilihat di : 
https://github.com/esp8266/Arduino 

# Modul Note 

- NodeMCU_cyblev1	: versi lite/simple, aplikasi hanya mengambil data dari Cyble dan ditampilkan ke serial (serial monitor)
- NodeMCU_cyblev1LCD	: aplikasi menampilkan data pada LCD dan serial . LCD library yang digunakan : https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library/
- NodeMCU_cyblev2	: versi yg dilengkapi dengan file config, data dikirimkan ke serial, dan dikirim ke server dalam format JSON 
