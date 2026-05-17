#include <ESP8266WiFi.h>

const char* ssid = "ESP8266_WIFI";
const char* password = "12345678";

WiFiServer server(80);

void setup() {

  Serial.begin(115200);

  WiFi.softAP(ssid, password);

  server.begin();

  Serial.println("Servidor iniciado");
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {

  WiFiClient client = server.available();

  if (!client) {
    return;
  }

  while (!client.available()) {
    delay(1);
  }

  String request = client.readStringUntil('\r');

  Serial.println(request);

  client.flush();

  // Liga LED
  if (request.indexOf("/LED=ON") != -1) {

    Serial.write('1');
  }

  // Desliga LED
  if (request.indexOf("/LED=OFF") != -1) {

    Serial.write('0');
  }

  // Página HTML
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");

  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.println("<head>");
  client.println("<meta charset='UTF-8'>");
  client.println("<meta name='viewport' content='width=device-width, initial-scale=1'>");
  client.println("<title>Controle LED</title>");
  client.println("</head>");

  client.println("<body style='text-align:center;font-family:Arial;'>");

  client.println("<h1>LED Arduino Uno</h1>");

  client.println("<button onclick=\"fetch('/LED=ON')\" style='width:200px;height:60px;font-size:20px;background:green;color:white;'>LIGAR</button>");

  client.println("<br><br>");

  client.println("<button onclick=\"fetch('/LED=OFF')\" style='width:200px;height:60px;font-size:20px;background:red;color:white;'>DESLIGAR</button>");

  client.println("</body>");
  client.println("</html>");

  delay(1);

  client.stop();
}