#include <ESP8266WiFi.h>

const char* ssid = "ESP8266_WIFI";
const char* password = "12345678";
WiFiServer server(80);

void setup() {
  Serial.begin(9600);  // IMPORTANTE: mesma velocidade do Arduino Uno
  Serial.setTimeout(10);
  
  WiFi.softAP(ssid, password);
  server.begin();
  
  Serial.println("ESP8266 pronto");
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;
  
  String request = client.readStringUntil('\r');
  client.flush();
  
  // Envia comandos para o Arduino Uno com quebra de linha
  if (request.indexOf("/LED=ON") != -1) {
    Serial.println("ON");  // println adiciona \n automaticamente
    Serial.println("<ON>");  // Para debug
  }
  
  if (request.indexOf("/LED=OFF") != -1) {
    Serial.println("OFF");  // println adiciona \n automaticamente
    Serial.println("<OFF>");  // Para debug
  }
  
  // HTML
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html; charset=UTF-8");
  client.println("Connection: close");
  client.println();
  client.println("<!DOCTYPE HTML><html>");
  client.println("<body style='text-align:center;'>");
  client.println("<h1>LED Arduino Uno</h1>");
  client.println("<button onclick=\"fetch('/LED=ON')\" style='width:200px;height:60px;background:green;color:white;'>LIGAR</button>");
  client.println("<br><br>");
  client.println("<button onclick=\"fetch('/LED=OFF')\" style='width:200px;height:60px;background:red;color:white;'>DESLIGAR</button>");
  client.println("</body></html>");
  client.stop();
}