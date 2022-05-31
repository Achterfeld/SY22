/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <WiFi.h>
#include <HTTPClient.h>


void connexionWiFi(const char*, const char*);

int httpCode;
String urlL,urlH;
WiFiClient client;

const char* ssid     = "AP_G3_F1";
const char* password = "";

const char* host = "192.168.4.1";
//const char* streamId   = "....................";
//const char* privateKey = "....................";
int value = 0;
    
void setup()
{
    Serial.begin(115200);
    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    ConnexionWifi(ssid,password);

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}



void loop()
{

    delay(5000);
    Serial.println(++value);

    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections

    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
    } else {
        Serial.println("connected http");
    }

    // We now create a URI for the request
    urlL = "/L";


    Serial.print("Requesting URL: ");
    Serial.println(urlL);

    // This will send the request to the server
    client.print(String("GET ") + urlL + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }

    // Read all the lines of the reply from server and print them to Serial
    while(client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
    }

    Serial.println();
    Serial.println("closing connection");



}


//description des fonctions
void ConnexionWifi(const char *ssid, const char *pass){

  Serial.println("essai de connexion de l'ESP32 sur la borne WiFi : ");
  Serial.println(ssid);

  WiFi.begin(ssid,pass);
  delay(10);
  while(WiFi.status() !=WL_CONNECTED)
  {
    Serial.print(".");
    delay(800);
  }
  Serial.println("ESP32 thing board connected to WIFI");

}
