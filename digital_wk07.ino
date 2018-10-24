#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "IOT_LAB";
const char* password = "password";

WebServer server(80);
String website;
const int led = 13;
void H(){
  digitalWrite(led, HIGH);
}
void L(){
  digitalWrite(led, LOW);
}

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/html", website);
  digitalWrite(led, 0);
}
void buildWeb(){
  website += "<html>";
  website += "<head>\n";
  website += "<meta charset='UTF-8'/>\n";
  website += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
  website += "<title>Remote Control LED</title>\n";
  website += "<style>";
  website += "html{background-color:#CCCCFF;}";
  website += ".title{text-align:left;color:#77BB34;text-shadow:0px 3px 5px #000; margin-bottom: 50px;}";
  website += "a{display:block;text-decoration:none;background-color:#5555FF;radius:3px; ";
  website += "width:150px;text-align:left;margin:0 auto;margin-bottom:30px;font-size:2em;box-shadow: 0px 3px 6px #000;}";
  website += ".btn1{color:#8C0044;}";
  website += ".btn2{color:#880000;}";
  website += "</style>";
  website += "</head>\n";
  website += "<body>";
  website += "<h1 class=\"title\">Remote LED Control</h1>";
  website += "<a class=\"btn1\" href=\"/H\">ON</a>";
  website += "<a class=\"btn2\" href=\"/L\">OFF</a>";
  website += "</body></html>";
 }
void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  buildWeb();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);


  server.on("/H",H);
  server.on("/L",L);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
