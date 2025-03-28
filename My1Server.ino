#include<ESP8266WebServer.h>
#include<ESP8266WiFi.h>
#include "LittleFS.h"

//Your wifi Name 
const char* SSID = "MyTeritoryMyRules";

//your password - at least 8 characters!
const char* password = "12341234";

IPAddress local_IP(192,168,10,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);


#define LED_G D13
#define LED_Y D12
#define LED_R D11

class LED{
    public:

    //htpp adresses of each json endpoint to turn on/off led
    const char* led_on;
    const char* led_off;

    int led_pin;

      LED(int led_pin1, const char* led_off1, const char* led_on1){
        led_pin = led_pin1;
        led_on = led_on1;
        led_off = led_off1;
        };
};

LED Led_g(LED_G,"/led_g_off","/led_g_on");
LED Led_r(LED_R,"/led_r_off","/led_r_on");
LED Led_y(LED_Y,"/led_y_off","/led_y_on");

void TurnRoff(){ server.on(Led_r.led_off,HTTP_GET,[](){digitalWrite(LED_R,LOW);}); }
void TurnYoff(){ server.on(Led_y.led_off,HTTP_GET,[](){digitalWrite(LED_Y,LOW);}); }
void TurnGoff(){ server.on(Led_g.led_off,HTTP_GET,[](){digitalWrite(LED_G,LOW);}); }

void TurnRon(){ server.on(Led_r.led_on,HTTP_GET,[](){digitalWrite(LED_R,HIGH);}); }
void TurnYon(){ server.on(Led_y.led_on,HTTP_GET,[](){digitalWrite(LED_Y,HIGH);}); }
void TurnGon(){ server.on(Led_g.led_on,HTTP_GET,[](){digitalWrite(LED_G,HIGH);}); }

void LedControl(){

  TurnRoff();
  TurnGoff();
  TurnYoff();

  TurnRon();
  TurnGon();
  TurnYon();
}

void HandleRoot(){

  File file = LittleFS.open("index.html","r");

  if(!file){
     server.send(404,"text/plain","ERROR, NO HTML detected!");
    return ;
  }
  server.streamFile(file,"text/html");
  file.close();
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

  LedControl();


  pinMode(LED_G,OUTPUT);
  pinMode(LED_R,OUTPUT);
  pinMode(LED_Y,OUTPUT);
  server.begin();
}

void loop() {

  server.handleClient();
}
