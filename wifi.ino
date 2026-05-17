#include <ESP8266WiFi.h>

const char* ssid = "ESP8266_WIFI";
const char* password = "12345678";

WiFiServer server(80);

void setup() {

  Serial.begin(115200);

  WiFi.softAP(ssid, password);

  server.begin();

  Serial.println(WiFi.softAPIP());
}

void loop() {

  WiFiClient client = server.available();
  if (!client) return;

  String request = client.readStringUntil('\r');
  client.flush();

  Serial.println(request);

  // 🔥 ENVIA COMANDO LIMPO E FORTE
  if (request.indexOf("/LED=ON") != -1) {
    Serial.println("#ON");
  }

  if (request.indexOf("/LED=OFF") != -1) {
    Serial.println("#OFF");
  }

  // resposta HTTP
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html; charset=UTF-8");
  client.println("Connection: close");
  client.println();

  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head><meta charset='UTF-8'></head>");
  client.println("<body style='text-align:center;'>");

  client.println("<h1>LED Arduino Uno</h1>");

  client.println("<button onclick=\"fetch('/LED=ON')\" style='width:200px;height:60px;background:green;color:white;'>LIGAR</button>");
  client.println("<br><br>");
  client.println("<button onclick=\"fetch('/LED=OFF')\" style='width:200px;height:60px;background:red;color:white;'>DESLIGAR</button>");

  client.println("</body></html>");

  client.stop();
}