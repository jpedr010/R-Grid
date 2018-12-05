/*  Accesspoint - station communication without router
 *  see: https://github.com/esp8266/Arduino/blob/master/doc/esp8266wifi/station-class.rst
 *  Works with: accesspoint_bare_01.ino
 */

#include <ESP8266WiFi.h>
char ssid[] = "Wemos_AP";           // SSID of your AP
char pass[] = "Wemos_comm";         // password of your AP
int data = 50;
String Send = "changeme\r";

IPAddress server(192,168,4,15);     // IP address of the AP
WiFiClient client;

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);           // connects to the WiFi AP
  Serial.println();
  Serial.println("Connection to the AP");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Connected");
  Serial.println("station_bare_01.ino");
  Serial.print("LocalIP:"); Serial.println(WiFi.localIP());
  Serial.println("MAC:" + WiFi.macAddress());
  Serial.print("Gateway:"); Serial.println(WiFi.gatewayIP());
  Serial.print("AP MAC:"); Serial.println(WiFi.BSSIDstr());
  pinMode(D0, INPUT);
  pinMode(D1, INPUT);
  pinMode(D2, INPUT);
  pinMode(D5, INPUT);
}

int opval()
{
  int totalval = 0;
  if (digitalRead(D0) == HIGH){
    totalval = totalval + 1;
  }
  if (digitalRead(D1) == HIGH){
    totalval = totalval + 2;
  }
  if (digitalRead(D2) == HIGH){
    totalval = totalval + 4;
  }
  if (digitalRead(D5) == HIGH){
    totalval = totalval + 8;
  }
  return totalval; 
}

void loop() {
  client.connect(server, 80);

  data = opval();
  if (data > 0)
  {
    if (data == 8)
    {
      Send = "ADDC1\r";
      Serial.println("********************************");
      Serial.print("Byte sent to the AP: ");
      Serial.println(client.print(Send));
      client.flush();
      client.stop();
      delay(1000);
    }
    else if (data == 9)
    {
      Send = "ADDC2\r";
      Serial.println("********************************");
      Serial.print("Byte sent to the AP: ");
      Serial.println(client.print(Send));
      client.flush();
      client.stop();
      delay(1000);
    }
    else if (data == 10)
    {
      Send = "ADDC3\r";
      Serial.println("********************************");
      Serial.print("Byte sent to the AP: ");
      Serial.println(client.print(Send));
      client.flush();
      client.stop();
      delay(1000);
    }
    else if (data == 11)
    {
      Send = "ADDC4\r";
      Serial.println("********************************");
      Serial.print("Byte sent to the AP: ");
      Serial.println(client.print(Send));
      client.flush();
      client.stop();
      delay(1000);
    }
    else if (data == 4)
    {
      Send = "REMC1\r";
      Serial.println("********************************");
      Serial.print("Byte sent to the AP: ");
      Serial.println(client.print(Send));
      client.flush();
      client.stop();
      delay(1000);
    }
    else if (data == 5)
    {
      Send = "REMC2\r";
      Serial.println("********************************");
      Serial.print("Byte sent to the AP: ");
      Serial.println(client.print(Send));
      client.flush();
      client.stop();
      delay(1000);
    }
    else if (data == 6)
    {
      Send = "REMC3\r";
      Serial.println("********************************");
      Serial.print("Byte sent to the AP: ");
      Serial.println(client.print(Send));
      client.flush();
      client.stop();
      delay(1000);
    }
    else if (data == 7)
    {
      Send = "REMC4\r";
      Serial.println("********************************");
      Serial.print("Byte sent to the AP: ");
      Serial.println(client.print(Send));
      client.flush();
      client.stop();
      delay(1000);
    }
  }
  else
  {
      Send = "NONE\r";
      Serial.println("********************************");
      Serial.print("Byte sent to the AP: ");
      Serial.println(client.print(Send));
      client.flush();
      client.stop();
      delay(1000);
  }
}
