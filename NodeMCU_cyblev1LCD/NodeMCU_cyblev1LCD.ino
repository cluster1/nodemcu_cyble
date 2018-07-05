/*
Reading Itron Cyble V1 2Wire Version 

Deskripsi : 
Aplikasi ini membaca hasil bacaan dari sensor Cyble Itron v2 2 wire version
data yang terbaca ditampilkan diserial & LCD 16x2 dengan modul I2C
Device menggunakan NodeMCU ESP8266 

Setting : 
Cara memasang kabel sesor cyble ke board : 
  White/Putih  : D3 : digital input
  Brown / Coklat  : Ground 

LCD 16x2 I2C : 
    SCL : D1
    SDA : D2 
    VCC : VCC out
    GND ; Ground 
I2C Address : 0x27 
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
 
/* Config variable */
const int ItronCyble = D3;          // Kabel input data dari ITron warna Putih hubungkan ke pin D3 di MCU 
LiquidCrystal_I2C lcd(0x27,16,2);   // set the LCD address to 0x27 , 16 : kolom , 2 : Baris

/* LED pin */
const int LED1 = 16  ;       // Indikator system berjalan   
const int LED2 = 2  ;        // Indikator ketika data terbaca

/* Initial Variable */ 
int CybleCounter = 0 ;      // initial counter / bisa disamakan dengan angka pada stand meter 
int SensorState = 0;        // var state read
int lastSensorState = 0;    // var. last read 

/* Apps Begin */
int bar_count = 0 ; 

void setup() {
  Serial.begin(9600) ;
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0); lcd.print("Standby");
    
  /* Setup Cyble Sensor & LED indicator */
  pinMode(ItronCyble, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  
  /* Set indikator system : on */
  digitalWrite(LED1, LOW);
  
  /* Set indikator read data : off */
  digitalWrite(LED2, HIGH);
  // Print a message to the LCD.
  delay(250) ;
  lcd.clear();
  lcd.setCursor(0,0); lcd.print("CTRead");
  lcd.setCursor(0,1); lcd.print("Counter:");
  lcd.setCursor(8,1); lcd.print(CybleCounter);  
  Serial.println("System Ready");
}

void loop() {

  /* Read sensor */
  SensorState = digitalRead(ItronCyble);   
  if (SensorState != lastSensorState) {
    /* if sensor state indicate LOW */
    if (SensorState == LOW) {
      Serial.print("counter:");
      CybleCounter++;
      Serial.println(CybleCounter);
      digitalWrite(LED2, LOW);      
      lcd.setCursor(0,1); lcd.print("Counter:");
      lcd.setCursor(8,1); lcd.print(CybleCounter);       
    } 
    delay(50);
    digitalWrite(LED2, HIGH);
  }
  delay(250);
  bar_display() ;
  lastSensorState = SensorState;
}
 
void bar_display(){
  if (bar_count >= 4 ){
    bar_count = 0 ;     
    lcd.setCursor(6,0); lcd.print("     ");
  }
  int col_bar = bar_count + 7 ;
  lcd.setCursor(col_bar,0); lcd.print(".");
  bar_count = bar_count + 1 ;   
}

