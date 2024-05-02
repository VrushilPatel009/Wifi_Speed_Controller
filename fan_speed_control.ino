
#include <ESP8266WiFi.h>
#include "DHT.h"

#define DHTPIN D1

const char* ssid     = "IIT(ISM)_aryanbansal";
const char* password = "ESP@8266";
int sp=0;
int pin=2;
#define pin2 D5
#define DHTTYPE DHT11 
float temp=0.0;
WiFiServer server(80);
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
    Serial.begin(115200);
    pinMode(pin, OUTPUT);      // set the LED pin mode
    pinMode(pin2, OUTPUT);      // set the LED pin mode
    analogWrite(pin, 0);
    analogWrite(pin2, 0);
    dht.begin();
    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.begin();

}

int value = 0;

void loop(){
 WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";   
    String value= "";// make a String to hold incoming data from the client
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
            temp = dht.readTemperature();
            client.println(temp);
//            client.println(sp);
            
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
        if(currentLine.endsWith("GET /temp"))
        {
            Serial.println(temp);          
        }
        else if(currentLine.endsWith("num"))
        {
          value=currentLine.substring(currentLine.indexOf('/')+1,currentLine.indexOf('n'));
          sp=value.toInt();
          if(sp>240)
          {
            sp=255;
          }
          else if(sp<15)
          {
            sp=0;
          }
          Serial.println(sp);
          analogWrite(pin, sp);
          analogWrite(pin2, sp);
        }
        
       
        
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
