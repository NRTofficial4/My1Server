#include<ESP8266WebServer.h>
#include<ESP8266WiFi.h>
#include "LittleFS.h"

//Your wifi Name 
const char* SSID = "MyTeritoryMyRules";

//your password - at least 8 characters!
const char* password = "12341234";

IPAddress local_IP(192,168,10,1);
IPAddress subnet(255,255,255,0);

#define LED_PIN D10 

ESP8266WebServer server(80);

void HandleRoot(){

  File file = LittleFS.open("index.html","r");

  if(!file){
     server.send(404,"text/plain","ERROR, NO HTML detected!");
    return ;
  }
  server.streamFile(file,"text/html");
  file.close();
}

void TurnOnLed(){
  digitalWrite(LED_PIN,HIGH);
}

void TurnOffLed(){
  digitalWrite(LED_PIN,LOW);
}

void setup() {


  Serial.begin(115200);
  
  //optional - manually setting up the IP adress of our WiFI;
  WiFi.softAPConfig(local_IP,local_IP ,subnet);

  WiFi.softAP(SSID,password);

  Serial.println(WiFi.softAPIP());

  while(!LittleFS.begin()){
    Serial.print("SPIFFS not init!");
    return;
  }
  server.on("/",HTTP_GET,HandleRoot);

  server.on("/turn_on_led",HTTP_GET,TurnOnLed);

  server.on("/turn_off_led",HTTP_GET,TurnOffLed);


  pinMode(LED_PIN,OUTPUT);
  server.begin();
}

void loop() {

  server.handleClient();
}
