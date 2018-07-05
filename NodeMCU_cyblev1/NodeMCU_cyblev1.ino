/*
Reading Itron Cyble V1 2Wire Version 

Deskripsi : 
Aplikasi ini membaca hasil bacaan dari sensor Cyble Itron v2 2 wire version
data yang terbaca ditampilkan diserial 
Device menggunakan NodeMCU ESP8266 

Setting : 
Cara memasang kabel sesor cyble ke board : 

  White/Putih  : D3 : digital input
  Brown / Coklat  : Ground 

*/

#include <Wire.h>
 
/* Config variable */
const int ItronCyble = D3;   // input port white cable 

/* LED pin */
const int LED1 = 16  ;       // indicator system running  
const int LED2 = 2  ;        // indicator read data 

/* Initial Variable */ 
int CybleCounter = 0 ;       // initial counter / bisa disamakan dengan angka pada stand meter 
int SensorState = 0;        // var state read
int lastSensorState = 0;    // var. last read 

/* Apps Begin */
void setup() {
  Serial.begin(9600) ;
  /* Setup Cyble Sensor & LED indicator */
  pinMode(ItronCyble, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  
  /* Set indikator system : on */
  digitalWrite(LED1, LOW);
  
  /* Set indikator read data : off */
  digitalWrite(LED2, HIGH);
  
  Serial.println("System Ready");
}

void loop() {

  /* Read sensor */
  SensorState = digitalRead(ItronCyble);
  if (SensorState != lastSensorState) {
    if (SensorState == LOW) {
      Serial.print("counter:");
      CybleCounter++;
      Serial.println(CybleCounter);
      digitalWrite(LED2, LOW);      
    } 
    delay(50);
  }
  lastSensorState = SensorState;
  digitalWrite(LED2, HIGH);
}
 
 

