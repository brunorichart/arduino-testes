#include <ESP8266WiFi.h>

const char* ssid = "ESP8266_WIFI";
const char* password = "12345678";

WiFiServer server(80);

bool ledState = false;

void setup() {

  Serial.begin(115200);

  WiFi.softAP(ssid, password);

  server.begin();

  Serial.println();
  Serial.println("Servidor iniciado");
  Serial.print("IP do ESP8266: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {

  WiFiClient client = server.available();

  if (!client) {
    return;
  }

  Serial.println("Novo cliente");

  // Espera dados
  while (!client.available()) {
    delay(1);
  }

  // Lê requisição inteira
  String request = client.readStringUntil('\n');

  Serial.println(request);

  // Liga LED
  if (request.indexOf("GET /LED=ON") >= 0) {

    ledState = true;
    Serial.write('1');
  }

  // Desliga LED
  if (request.indexOf("GET /LED=OFF") >= 0) {

    ledState = false;
    Serial.write('0');
  }

  // Resposta HTTP
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();

  // HTML
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<meta charset='UTF-8'>");
  client.println("<meta name='viewport' content='width=device-width, initial-scale=1'>");
  client.println("<title>Controle LED</title>");
  client.println("</head>");

  client.println("<body style='text-align:center;font-family:Arial;'>");

  client.println("<h1>Controle LED</h1>");

  // Estado
  if (ledState) {
    client.println("<h2 style='color:green;'>LED LIGADO</h2>");
  } else {
    client.println("<h2 style='color:red;'>LED DESLIGADO</h2>");
  }

  client.println("<br>");

  // Botão ligar
  client.println("<a href='/LED=ON'>");
  client.println("<button style='width:200px;height:60px;font-size:20px;background:green;color:white;'>LIGAR</button>");
  client.println("</a>");

  client.println("<br><br>");

  // Botão desligar
  client.println("<a href='/LED=OFF'>");
  client.println("<button style='width:200px;height:60px;font-size:20px;background:red;color:white;'>DESLIGAR</button>");
  client.println("</a>");

  client.println("</body>");
  client.println("</html>");

  delay(1);

  client.stop();

  Serial.println("Cliente desconectado");
}