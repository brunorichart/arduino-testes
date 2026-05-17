#include <ESP8266WiFi.h>

const char* ssid = "ESP8266_WIFI";
const char* password = "12345678";
WiFiServer server(80);

unsigned long ultimoEnvioStatus = 0;
bool ultimoEstadoLED = false;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);
  
  WiFi.softAP(ssid, password);
  server.begin();
  
  // Aguarda Arduino Uno iniciar
  delay(2000);
  Serial.println("ESP8266 iniciado");
}

void loop() {
  // Verifica estado atual do LED no Arduino Uno
  if (millis() - ultimoEnvioStatus > 5000) {  // A cada 5 segundos
    Serial.println("STATUS");
    ultimoEnvioStatus = millis();
  }
  
  // Lê resposta do Arduino Uno
  while (Serial.available()) {
    String resposta = Serial.readStringUntil('\n');
    if (resposta.startsWith("STATUS:")) {
      String estado = resposta.substring(7);
      estado.trim();
      // Opcional: Mostra no Serial Monitor
      Serial.print("Estado do LED: ");
      Serial.println(estado);
    }
  }
  
  WiFiClient client = server.available();
  if (!client) return;
  
  String request = client.readStringUntil('\r');
  client.flush();
  
  // Envia comandos para o Arduino Uno
  if (request.indexOf("/LED=ON") != -1) {
    Serial.println("ON");
    Serial.println("<ON>");
  }
  
  if (request.indexOf("/LED=OFF") != -1) {
    Serial.println("OFF");
    Serial.println("<OFF>");
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