#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include<Wire.h>
 const int MPU_addr=0x68;
int16_t axis_X,axis_Y,axis_Z;
int minVal=265;
int maxVal=402;
double x;
double y;
double z;
double push;
 
//SSID and Password of your WiFi router
const char* ssid = "OnePlus 5";
const char* password = "starflux";
 
ESP8266WebServer server(80); //Server on port 80
void handleRoot() {
  digitalWrite(5,HIGH);
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);
  axis_X=Wire.read()<<8|Wire.read();
  axis_Y=Wire.read()<<8|Wire.read();
  axis_Z=Wire.read()<<8|Wire.read();
  int xAng = map(axis_X,minVal,maxVal,-90,90);
  int yAng = map(axis_Y,minVal,maxVal,-90,90);
  int zAng = map(axis_Z,minVal,maxVal,-90,90);
     x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
     y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
     z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);
  String s = String(y);
  server.send(200, "text/plain", s);
}

void setup(void){
  Serial.begin(9600);
  Serial.println("AYUSH CHUTIYA HAI");
  Wire.begin(13,12);//d76
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  
  pinMode(13,INPUT);
  pinMode(15,OUTPUT);
  pinMode(5,OUTPUT);
  Serial.begin(9600);
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
 
  server.on("/", handleRoot);      //Which routine to handle at root location
 
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();          //Handle client requests
}