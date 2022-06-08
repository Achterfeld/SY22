/*
  WiFiAccessPoint.ino creates a WiFi access point and provides a web server on it.

  Steps:
  1. Connect to the access point "yourAp"
  2. Point your web browser to http://192.168.4.1/H to turn the LED on or http://192.168.4.1/L to turn it off
     OR
     Run raw TCP "GET /H" and "GET /L" on PuTTY terminal with 192.168.4.1 as IP address and 80 as port

  Created for arduino-esp32 on 04 July, 2018
  by Elochukwu Ifediora (fedy0)
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <XBee.h>


XBee xbee = XBee();
uint8_t payload[] = {0};
XBeeAddress64 addr64 = XBeeAddress64(0xfffffffff,0xfffffffff);


const int ledPin =  4;

//#define LED_BUILTIN 2   // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED

// Set these to your desired credentials.
const char *ssid = "AP_G3_F1";
const char *password = "";

WiFiServer server(80);


void setup() {
  //pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  xbee.setSerial(Serial);
  pinMode(ledPin, OUTPUT);
  
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
}

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients
  
  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/H\">here</a> to turn ON the LED.<br>");
            client.print("Click <a href=\"/L\">here</a> to turn OFF the LED.<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          client.println("Client message HIGH");
          digitalWrite(ledPin, HIGH);               // GET /H turns the LED on

          //Send message to zigbee
          payload[0]='h';
          ZBTxRequest zigMsg = ZBTxRequest(addr64,payload,sizeof(payload));
          xbee.send(zigMsg);
          Serial.println("Message Low sent to zigbbe network");
        }
        if (currentLine.endsWith("GET /L")) {
          client.println("Client message LOW");
          digitalWrite(ledPin, LOW);                // GET /L turns the LED off

          //Send message to zigbee
          payload[0]='l';
          ZBTxRequest zigMsg = ZBTxRequest(addr64,payload,sizeof(payload));
          xbee.send(zigMsg);
          Serial.println("Message High sent to zigbbe network");
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
