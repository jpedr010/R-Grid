/*  Accesspoint - station communication without router
 *  see: https://github.com/esp8266/Arduino/blob/master/doc/esp8266wifi/soft-access-point-class.rst
 *       https://github.com/esp8266/Arduino/blob/master/doc/esp8266wifi/soft-access-point-examples.rst
 *       https://github.com/esp8266/Arduino/issues/504
 *  Works with: station_bare_01.ino
 */ 


#include <ESP8266WiFi.h>

WiFiServer server(80);
IPAddress IP(192,168,4,15);
IPAddress mask = (255, 255, 255, 0);

int opcode = 0;
#define data_buffer_size 1
byte buff[data_buffer_size];
int data = 254;


void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("Wemos_AP", "Wemos_comm");
  WiFi.softAPConfig(IP, IP, mask);
  server.begin();
  Serial.println();
  Serial.println("accesspoint_bare_01.ino");
  Serial.println("Server started.");
  Serial.print("IP: ");     Serial.println(WiFi.softAPIP());
  Serial.print("MAC:");     Serial.println(WiFi.softAPmacAddress());
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D5, OUTPUT);
}



void loop() {

  WiFiClient client = server.available();
  if (!client) {return;}

  String request = client.readStringUntil('\r');
  if (request != "NONE")
  {
    Serial.println("********************************");
    Serial.println("From the station: " + request);
    client.flush();
    digitalWrite(D2, LOW);
    digitalWrite(D0, LOW);
    digitalWrite(D1, LOW);
    digitalWrite(D5, LOW);

  }
  if (request == "ADDC1")
  {
    for (int i = 0; i < 5; i++)
    {
      Serial.println("Add Cell One");
      digitalWrite(D0, LOW);
      digitalWrite(D1, HIGH);
      digitalWrite(D2, HIGH);
      digitalWrite(D5, HIGH);
    }
  }
  else if (request == "ADDC2")
  {
    for (int i = 0; i < 5; i++)
    {
      Serial.println("Add Cell Two");
      digitalWrite(D0, HIGH);
      digitalWrite(D1, LOW);
      digitalWrite(D2, HIGH);
      digitalWrite(D5, HIGH);
    }
  }
  else if (request == "ADDC3")
  {
    for (int i = 0; i < 5; i++)
    {
      Serial.println("Add Cell Three");
      digitalWrite(D0, HIGH);
      digitalWrite(D1, HIGH);
      digitalWrite(D2, LOW);
      digitalWrite(D5, HIGH);
    }
  }
  else if (request == "ADDC4")
  {
    for (int i = 0; i < 5; i++)
    {
      Serial.println("Add Cell Four");
      digitalWrite(D0, HIGH);
      digitalWrite(D1, HIGH);
      digitalWrite(D2, HIGH);
      digitalWrite(D5, LOW);
    }
  }
  else if (request == "REMC1")
  {
    for (int i = 0; i < 5; i++)
    {
      Serial.println("Remove Cell One");
      digitalWrite(D0, LOW);
      digitalWrite(D1, HIGH);
      digitalWrite(D2, HIGH);
      digitalWrite(D5, LOW);
    }
  }

  else if (request == "REMC2")
  {
    for (int i = 0; i < 5; i++)
    {
      Serial.println("Remove Cell Two");
      digitalWrite(D0, HIGH);
      digitalWrite(D1, LOW);
      digitalWrite(D2, HIGH);
      digitalWrite(D5, LOW);
    }
  }
  else if (request == "REMC3")
  {
    for (int i = 0; i < 5; i++)
    {
      Serial.println("Remove Cell Three");
      digitalWrite(D0, LOW);
      digitalWrite(D1, LOW);
      digitalWrite(D2, HIGH);
      digitalWrite(D5, LOW);
    }
  }
  else if (request == "REMC4")
  {
    for (int i = 0; i < 5; i++)
    {
      Serial.println("Remove Cell FOUR");
      digitalWrite(D0, HIGH);
      digitalWrite(D1, HIGH);
      digitalWrite(D2, LOW);
      digitalWrite(D5, LOW);
    }
      
  }


  
  /*WiFiClient client = server.available();
  if (!client) {return;}
  byte request = client.read(buff, data_buffer_size);
  if (request != 255)
  {
    Serial.println("********************************");
    Serial.print(request, BIN);
    Serial.println();
  }*/
  /*if (opcode != 0)
  {
      Serial.println("Sending to client");
      Serial.print(opcode, BIN);
      client.write(opcode);
      Serial.println();
      client.flush();
  }
  client.stop();*/
}
