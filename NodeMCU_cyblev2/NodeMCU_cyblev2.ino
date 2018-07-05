/*
Reading Itron Cyble V2 2Wire Version 

deskripsi : 
Aplikasi ini membaca hasil bacaan dari sensor Cyble Itron v2 2 wire version
data yang terbaca dikirimka ke server via http 
Aplikasi dapat di akses via ip /data/ untuk mengetahui nilai terakhir yang dibaca


Setting : 
Cara memasang kabel sesor cyble ke board : 

  White : D3 : digital input
  Brown : Ground 


*/

#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include "FS.h"

const int ItronCyble = D3;  // input port white cable , brown cable to Ground 
const int LED1 = 16  ;       // indicator system running  
const int LED2 = 2  ;        // indicator read data 
 
int CybleCounter = 0;       // counter 
int SensorState = 0;        // var state read
int lastSensorState = 0;    // var. last read 

ESP8266WebServer server(80); // webserver PORT

/* Config variable */
/* file config.json*/

struct Config {
    char  ssid[32] ;        // SSID 
    char  password[12] ;    // WPA Passkey
    char  host[16] ;        // API Host server 
    int   httpPort ;        // PORT server
    char  api_path[64] ;    // url path 
    int   delay_send ;      // delay sending data 
    int   init_value ;      // inital value for counter 
    char  device_id[64] ;   // ID perangkat 
};


/* Apps Begin */
Config config; 

void setup() {
  Serial.begin(9600) ;
  /* Read config file */
  Serial.println("[01]Read Config");
  if (!SPIFFS.begin()) {
    Serial.println("[0101]Failed to mount file system");
    return;
  }
  if (!loadConfig()) {
    Serial.println("[0102]Failed to load config");
  } else {
    Serial.println("[0100]Config loaded");
  }  
  /* Setup Wireless */
  Serial.print("SSID: ");
  Serial.println(config.ssid);
  WiFi.begin(config.ssid, config.password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  long rssi = WiFi.RSSI();
  Serial.print("RSSI:");
  Serial.println(rssi);  
  String hostnya =   String (config.host)+":"+String(config.httpPort);
  Serial.print("connecting to ");
  Serial.println(hostnya);

  /* Setup Web Server */
  server.on("/", handleRoot);
  server.on("/data", [](){
      String PostData ="{\"DEVICE_ID\": \""+String(config.device_id) +"\", \"DEVICE_VALUE\": \""+String(CybleCounter)+"\" }" ;
      server.send(200, "text/plain", PostData  );
   });
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
  
  /* Setup Cyble Sensor & LED indicator */
  pinMode(ItronCyble, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);

  Serial.print("init val : ");
  Serial.println(config.init_value);
  Serial.println("Cyble Ready");
  CybleCounter = config.init_value ;
}

void loop() {

  /* Read sensor */
  server.handleClient();
  SensorState = digitalRead(ItronCyble);
  if (SensorState != lastSensorState) {
    if (SensorState == LOW) {
      Serial.print("counter:");
      CybleCounter++;
      Serial.println(CybleCounter);
      send_data(); // send data to server 
      digitalWrite(LED2, LOW);      
    } 
    delay(50);
  }
  lastSensorState = SensorState;
  digitalWrite(LED2, HIGH);
 
}

/* Function Send Data to server */
bool send_data(){
              String device_id     = String(config.device_id) ;
              String device_value  = String(CybleCounter) ;
              String device_name   = "Cyble V02 2 wire" ;
              
             // String PostData = "{\"DEVICE_ID\": \""+device_id+"\",\"DEVICE_VALUE\":\""+device_value+"\", \"book\": \""+device_value+"\" }";

              String PostData ="{\"DEVICE_ID\": \""+device_id+"\", \"DEVICE_VALUE\": \""+device_value+"\", \"DEVICE_NAME\": \""+device_name+"\" }";
              HTTPClient http;
              Serial.println("[HTTP] begin...");
              Serial.println(PostData);                       
              http.begin("http://"+String (config.host)+":"+String(config.httpPort)+String(config.api_path)); //HTTP   
              http.addHeader("Content-Type", "text/json", false, true);
              int httpCode = http.POST(PostData);   //Send the request        
              if(httpCode == HTTP_CODE_OK) {
                 String payload = http.getString();                  //Get the response payload
                 payload.trim();
                 Serial.print("data:");   
                 Serial.println(httpCode);   //Print HTTP return code
                 Serial.print("respon:");
                 Serial.println(payload);    //Print request response payload
                 /* Urusan Per JSON an */
                 DynamicJsonBuffer jsonBuffer;
                 JsonObject& root = jsonBuffer.parseObject(payload);
                 String statusnya = root["respond"] ;
                 Serial.println("status:"+statusnya) ;                       
               } else {
                  Serial.printf("[HTTP]POST ... failed, error: %s\n", http.errorToString(httpCode).c_str());            
                  Serial.println(httpCode) ;
                  String payload = http.getString();
                  Serial.println(payload);    //Print request response payload                        
              }
              http.end();
              Serial.println("[HTTP] End");   
              return true ; 
}

/* Waiting service */

/* Function load configuration file : config.json */
bool loadConfig() {
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }
  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());
  if (!json.success()) {
    Serial.println("Failed to parse config file");
    return false;
  } 
  strlcpy(config.ssid,json["ssid"] ,sizeof(config.ssid)); 
  strlcpy(config.password,json["paswd"] ,sizeof(config.password)); 
  strlcpy(config.host,json["host"] ,sizeof(config.host)); 
  config.httpPort = json["port"] ;
  strlcpy(config.api_path,json["path"] ,sizeof(config.api_path)); 
  config.delay_send = json["delay"];
  config.init_value = json["initialvalue"];
  strlcpy(config.device_id,json["device_id"] ,sizeof(config.device_id)); 

  return true;
}


void handleRoot() {
   server.send(200, "text/plain", "JPS: Itron Cyble Reader v.0.1");
 }

void handleNotFound(){
   String message = "undefine \n\n";
  server.send(404, "text/plain", message);
 }


