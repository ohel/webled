#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
 
#include "wificonfig.h"
#include "index.h"
char* index_html = reinterpret_cast<char*>(&src_index_html[0]);

#define LED_ON LOW
#define LED_OFF HIGH
 
int ledPin = 2; // GPIO2
ESP8266WebServer server(80);
 
void setup() {
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);

    Serial.print("Connecting to: ");
    Serial.println(ssid);
     
    WiFi.config(ip, gateway, subnet);
    WiFi.begin(ssid, password);
     
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("WiFi connected using IP address: ");
    Serial.println(WiFi.localIP());
     
    server.on("/", HTTP_GET, [](){
        Serial.println("GET: /");
        server.send(200, "text/html", index_html);
    });
    server.on("/favicon.ico", HTTP_GET, [](){
        Serial.println("GET: /favicon.ico");
        server.send(404);
    });
    server.on("/", HTTP_POST, [](){
        Serial.println("POST: /");
        server.sendHeader("Location", "/", true);
        server.send(302, "text/plain", "");
        digitalWrite(ledPin, LED_ON);
        delay(250);
        digitalWrite(ledPin, LED_OFF);
    });

    server.begin();
    Serial.println("Server is running.");
    digitalWrite(ledPin, LED_ON); delay(250);
    digitalWrite(ledPin, LED_OFF); delay(250);
    digitalWrite(ledPin, LED_ON); delay(250);
    digitalWrite(ledPin, LED_OFF);
}
 
void loop() {
    server.handleClient();
}
