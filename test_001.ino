/**
    It is testing task for Havrilenko Olexiy
    Created on: 13.10.2021
    
*/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Wire.h>

ESP8266WiFiMulti WiFiMulti;
String mainURL = "";
String mainTwoURL = "";
String postURL = "";
String content = "";
String req = "";
char character;

void setup() {

  Wire.begin();
  pinMode (2, OUTPUT);
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  // use your name of SID and password
  WiFiMulti.addAP("Besedka", "wifibysamaelsystem");

  Serial.println(" ");
  Serial.println("------------------------------------------------------------------------");
  Serial.println("!      Please input request: 'http <REQ> <URL> <BODY> ;'               !");
  Serial.println("!              use ';' to ending input data.                           !");
  Serial.println("!   REQ - GET or POST request.                                         !");
  Serial.println("!     URL - remote server address.                                     !");
  Serial.println("!       BODY - request body in case of POST                            !");
  Serial.println("!                                                                      !");
  Serial.println("!       For example:                                                   !");
  Serial.println("!       http GET http://httpbin.org                                    !");
  Serial.println("!       http POST http://httpbin.org/post post_body                    !");
  Serial.println("!                                                                      !");
  Serial.println("------------------------------------------------------------------------");
  Serial.println('\n');
  digitalWrite(2, 1);
  WiFi.mode(WIFI_STA);


}

void loop() {

  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;
    HTTPClient httpclient;

    if (Serial.available() > 0) {
      character = Serial.read();
      content.concat(character);
    }

    if (content != "") {
      req = content;
      if (req.indexOf(';') > 0) {
        if (content.substring(0, 4) == "http" || content.substring(1, 5) == "http") {
          digitalWrite(2, 0);
          delay(100);
          digitalWrite(2, 1);
          if (content.lastIndexOf(" GET ") != -1) {
            int firstClosingBracket = content.indexOf(' ');
            int secondClosingBracket = content.indexOf(' ', firstClosingBracket + 1 );
            int therdClosingBracket = content.indexOf(';', secondClosingBracket + 1 );
            mainURL = content.substring(therdClosingBracket, secondClosingBracket + 1);
            //httpclient.begin(client, mainURL+user+"&temp="+temperature+"&type="+type))
            //Serial.println(mainURL);
            httpclient.begin(client, mainURL);
            int httpHtml = httpclient.GET();
            if (httpHtml > 0)
            {
              String payload = httpclient.getString();
              Serial.println(httpHtml);
              Serial.println(payload);
            }

            httpclient.end();

          } else {

            if (content.lastIndexOf(" POST ") != -1) {
              int firstClosingBracket = content.indexOf(' ');
              int secondClosingBracket = content.indexOf(' ', firstClosingBracket + 1 );
              int therdClosingBracket = content.indexOf(' ', secondClosingBracket + 1 );
              int fourthClosingBracket = content.indexOf(';', therdClosingBracket + 1 );
              mainURL = content.substring(therdClosingBracket, secondClosingBracket + 1);
              postURL =  content.substring(fourthClosingBracket, therdClosingBracket + 1);
              httpclient.begin(client, mainURL);
              httpclient.addHeader("Content-Type", "application/json");
              int httpHtml = httpclient.POST(postURL);

              Serial.println(mainURL);
              Serial.println(postURL);

              if (httpHtml > 0) {
                Serial.printf("POST: %d\n", httpHtml);
                if (httpHtml == HTTP_CODE_OK) {
                  const String& payload = httpclient.getString();
                  Serial.println("We receive payload:\n<<--------------");
                  Serial.println(payload);
                  Serial.println("-------------->>");
                }
              } else {
                Serial.printf("failed, error: %s\n", httpclient.errorToString(httpHtml).c_str());
              }

              httpclient.end();
            } else {
              Serial.println("Can not recognize method GET/POST..." );
            }
          }
          content = "";
          req = "";
        } else {
          Serial.println("Please, use syntax 'http <REQ> <URL> <BODY>;'...");
          content = "";
          req = "";
        }

      }

    }

  }
}
