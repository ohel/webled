#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "string.h"
 
#include "wificonfig.h"
#include "apconfig.h"
#include "index.h"
#include "favicon.h"
char* index_html = reinterpret_cast<char*>(&src_index_html[0]);
char* favicon = reinterpret_cast<char*>(&src_favicon_ico[0]);

// On D1 boards the integrated LED is inverted (on when pin is LOW).
int ledPin = LED_BUILTIN;
// Alternative signal pin so as not to signal on setup.
// On D1 mini LED_BUILTIN == D4.
#ifdef ESP8266_WEMOS_D1MINI
static const int altPin = D2;
#else
static const int altPin = D4;
#endif

ESP8266WebServer server(80);
 
void setup() {
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);
    pinMode(altPin, OUTPUT);

    IPAddress subnet(255, 255, 255, 0);

    Serial.println("");
    if (strlen(soft_ap_ssid) > 0) {
        Serial.println("Setting up AP:");
        Serial.print("    SSID: ");
        Serial.println(soft_ap_ssid);
        WiFi.mode(WIFI_AP_STA);
        Serial.print("    Password: ");
        if (strlen(soft_ap_password) < 8) {
            Serial.println("N/A");
            WiFi.softAP(soft_ap_ssid);
        } else {
            Serial.println(soft_ap_password);
            WiFi.softAP(soft_ap_ssid, soft_ap_password);
        }
        Serial.print("    IP/Gateway: ");
        Serial.println(soft_ap_ip);
        WiFi.softAPConfig(soft_ap_ip, soft_ap_ip, subnet);
    } else {
        Serial.println("Not setting up AP.");
        WiFi.mode(WIFI_STA);
    }

    Serial.println("");
    if (ip == INADDR_NONE) {
        Serial.println("Using dynamic IP address.");
    } else {
        Serial.print("Using static IP address: ");
        Serial.println(ip.toString());
        WiFi.config(ip, gateway, subnet);
    }

    WiFi.begin(ssid, password);
    int timeout = 0;
    Serial.print("Connecting to: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED && timeout < 10) {
        delay(1000);
        Serial.print(".");
        timeout++;
    }
    Serial.println("");
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Unable to connect. Operating as access point only.");
        WiFi.mode(WIFI_AP);
    } else {
        Serial.print("WiFi connected using IP address: ");
        Serial.println(WiFi.localIP());
    }

    if (strlen(soft_ap_ssid) > 0) {
        Serial.print("WiFi AP IP address: ");
        Serial.println(WiFi.softAPIP());
    }
     
    server.on("/", HTTP_GET, [](){
        Serial.println("GET: /");
        server.send(200, "text/html", index_html);
    });
    server.on("/favicon.ico", HTTP_GET, [](){
        Serial.println("GET: /favicon.ico");
        server.send_P(200, "image/x-icon", favicon, src_favicon_ico_len);
    });
    server.on("/", HTTP_POST, [](){
        Serial.println("POST: /");
        server.send(200, "text/plain", "");
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
