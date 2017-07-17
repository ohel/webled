#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
 
#include "wificonfig.h"
#include "index.h"
char* index_html = reinterpret_cast<char*>(&src_index_html[0]);

int ledPin = 2; // GPIO2 - NB: the integrated LED is inverted (on when pin is LOW).
int altPin = 4; // GPIO4 - alternative signal pin so as not to signal on setup.

ESP8266WebServer server(80);
 
void setup() {
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);
    pinMode(altPin, OUTPUT);

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
        digitalWrite(ledPin, HIGH);
        digitalWrite(altPin, HIGH);
        delay(250);
        digitalWrite(ledPin, LOW);
        digitalWrite(altPin, LOW);
    });

    server.begin();
    Serial.println("Server is running.");
    digitalWrite(ledPin, HIGH); delay(250);
    digitalWrite(ledPin, LOW); delay(250);
    digitalWrite(ledPin, HIGH); delay(250);
    digitalWrite(ledPin, LOW);
}
 
void loop() {
    server.handleClient();
}
