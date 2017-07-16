#include "Arduino.h"
#include "ESP8266WiFi.h"
 
#include "ssid.h"
#include "password.h"
#include "index.h"
char* index_html = reinterpret_cast<char*>(&src_index_html[0]);

#define LED_ON LOW
#define LED_OFF HIGH
 
int ledPin = 2; // GPIO2
WiFiServer server(80);
 
void setup() {
    Serial.begin(115200);
    delay(10);
 
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LED_OFF);
    digitalWrite(ledPin, LED_ON);
    delay(1000);
    digitalWrite(ledPin, LED_OFF);
     
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
     
    WiFi.begin(ssid, password);
     
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
     
    server.begin();
    Serial.println("Server started");
    digitalWrite(ledPin, LED_ON);
    delay(500);
    digitalWrite(ledPin, LED_OFF);
    delay(500);
    digitalWrite(ledPin, LED_ON);
    delay(500);
    digitalWrite(ledPin, LED_OFF);
 
    Serial.print("Use this URL to connect: ");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
        
}
 
void loop() {
    WiFiClient client = server.available();
    if (!client) {
        return;
    }
     
    while(!client.available()){
        delay(1);
    }
     
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();
     
    if (request.indexOf("/favicon.ico") != -1) {
        client.println("HTTP/1.1 204 No Content");
        return;
    }

    int value = LED_OFF;
    if (request.indexOf("/led") != -1) {
        value = LED_ON;
    } 
    digitalWrite(ledPin, value);
 
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.print("Led pin is now: ");
    if(value == LED_ON) {
        client.print("On");    
    } else {
        client.print("Off");
    }
    client.println("<br><br>");
    client.println("Click <a href=\"/led\">here</a> turn the LED on.<br>");
    client.println("Click <a href=\"/\">here</a> turn the LED off.<br>");
    client.println("</html>");
}
