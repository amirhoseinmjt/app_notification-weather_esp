#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <base64.h>


#define OLED_ADDR   0x3C
Adafruit_SSD1306 display(128, 64, &Wire, -1);

//gmail
const char* gmailToken = "your_gmail_token";// Gmail API credentials
//AIzaSyA9K5LIfaEbsjePPc97PHq89LpHAroKZiw
//client_id=836420961256-r0fusrs0adg32686mc9shjetri6cgi26.apps.googleusercontent.com
//client_secret=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

//telegram api token = 6780863786:AAFiY2-CDOCrumd7oV6W3_B1x6chhqFZFWw
//telegram bot username : @Esp32_notificationprogram_Bot
//const char* gmailEndpoint = "www.googleapis.com";// Gmail API endpoint

//mqtt
const char *mqtt_username = "amir"; // username for authentication
const char *mqtt_password = "1234";// password for authentication
const char *topic = "esp32/project"; // my topic 
const char *mqtt_broker = "w9a8ccf9.ala.us-east-1.emqxsl.com";// broker address
const int mqtt_port = 8883;// port of MQTT over TCP

const char* ca_cert= \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n" \
"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n" \
"d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n" \
"QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n" \
"MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n" \
"b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n" \
"9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n" \
"CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n" \
"nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n" \
"43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n" \
"T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n" \
"gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n" \
"BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n" \
"TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n" \
"DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n" \
"hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n" \
"06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n" \
"PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n" \
"YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n" \
"CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\n" \
"-----END CERTIFICATE-----\n" ;


// WiFi
const char *ssid = "Amir.MJT"; //WiFi name
const char *password = "am8896626";  //WiFi password



WiFiClientSecure espClient;
// use wifi client to init mqtt client
PubSubClient client(espClient); 

//weather
const char* weatherApiUrl = "http://api.openweathermap.org/data/2.5/weather?q=Tehran&appid=c4db12923b089f57a9d1f0cb0c003ec5";


void setup() {

  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(20, 20);
  //display.println("MJT");
  //display.display();
  //display.println("MJT");
  // connecting to a WiFi network
  connectWIFI();
  // set root ca cert
  espClient.setCACert(ca_cert);
  // setup mqtt broker
  //client.setServer(mqtt_broker, mqtt_port);
  //client.setCallback(callback);
  
  connectMQTT();
}

void loop() {
  client.loop();
}


void connectWIFI(){
  //display.println("MJT");
  //display.display();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      //display.println("Connecting to WiFi..");
      //display.display();
  }
  display.println("Connected to WiFi");
  display.display();
}

void connectMQTT() {
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
    Serial.println("connecting to MQTT broker...");
    display.println("--------");
    display.display();
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
        display.println("connected to MQTT broker.");
        display.display();
        delay(1000);
        display.clearDisplay();
        display.println("hi");
        display.display();
        //Serial.println("hi");
    } else {
        display.println("Failed to connect MQTT ");
        display.display();
        display.println(client.state());
        Serial.println("Retrying in 3 seconds.");
        display.display();
        delay(3000);
        display.clearDisplay();
    }
  }
  client.publish(topic, "Hi EMQX I'm ESP32 ^^");
  client.subscribe(topic);
}

void callback(char *topic, byte *payload, unsigned int length) {
 display.clearDisplay();
 display.display();
 printweather();
 display.setCursor(0, 17);
 
 //display.print("Message arrived in topic: ");
 //display.println(topic);
 //display.display();
 //display.print("* ");
 for (int i = 0; i < length; i++) {
     display.print((char) payload[i]);
 }
 //display.println("-------------------");
 display.display();
}

void printweather(){
 //display.setCursor(0, 0);
 Serial.print("Fetching weather data...");
 if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;


    http.begin(weatherApiUrl);  // Specify the URL of the weather API
    int httpResponseCode = http.GET();

    if (httpResponseCode == HTTP_CODE_OK) {
      String payload = http.getString();

      DynamicJsonDocument jsonDoc(1024);
      deserializeJson(jsonDoc, payload);

      const char* weatherDescription = jsonDoc["weather"][0]["description"];
      //float temperature = jsonDoc["main"]["temp"];
      int humidity = jsonDoc["main"]["humidity"];
      //float windSpeed = jsonDoc["wind"]["speed"];
      //serial
      Serial.println("\nWeather Information:");
      //Serial.print("Description: ");
      //Serial.println(weatherDescription);
      //Serial.print("Temperature: ");
      //Serial.print(temperature);
      //Serial.println(" °C");
      //Serial.print("Humidity: ");
      //Serial.print(humidity);
      //Serial.println(" %");
      //Serial.print("Wind Speed: ");
      //Serial.print(windSpeed);
      //Serial.println(" m/s");
      //display
      display.setCursor(0, 0);
      display.print("W: ");
      display.print(weatherDescription);
      display.print(" | H:");
      display.print(humidity);
      display.print("%");
      display.display();
    } else {
      Serial.print("Error: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  }
}

