# NodeMCU Itron Cyble 
Sistem ini membaca hasil bacaan dari sensor [Itron Cyble Sensor v2 2 wire version](https://www.itron.com/eu/technology/product-services-catalog/products/2/4/4/cyble-sensor). 
Dengan menggunakan device [NodeMCU ESP8266](http://nodemcu.com/index_en.html), data yang terbaca ditampilkan pada serial output (v1). 

![Block Diagram](http://apps.noobwork.com/content/NodeMCU_CybleL1LCD.png)

Scrip ditulis menggunakan Arduino IDE. Untuk menggunakan NodeMCU pada Arduino IDE.
Tutorial & library untuk menggunakan [NodeMCU ESP8266](http://nodemcu.com/index_en.html) pada Arduino IDE bisa dilihat di https://github.com/esp8266/Arduino .

# Modul Note 

- NodeMCU_cyblev1	: versi lite/simple, aplikasi hanya mengambil data dari Cyble dan ditampilkan ke serial (serial monitor)
- NodeMCU_cyblev1LCD	: aplikasi menampilkan data pada LCD dan serial . LCD library yang digunakan : https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library/
- NodeMCU_cyblev2	: versi yg dilengkapi dengan file config, data dikirimkan ke serial, dan dikirim ke server dalam format JSON 
