#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

const byte DNS_PORT = 53;
DNSServer dnsServer;
IPAddress apIP(192, 168, 1, 1);

String apSSID = "WiFi config";
String apPassword = "wificonfig";


ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", "<!DOCTYPE html><html><head> <meta charset='UTF-8'>"
  "<title>ESP NETWORK CONFIG</title>"
"</head>"

"<form method='post' action='/setup'>"
    "<label for='ssid'>SSID:</label><br>"
    "<input type='text' id='ssid' name='ssid'><br>"
    "<label for='password'>Password:</label><br>"
    "<input type='text' id='password' name='password'><br><br>"
    "<input type='submit' value='Submit'>"
"</form>"

"</html>"
);
}


void handleSetup() {
  
  server.send(200, "text/html", "<!DOCTYPE html><html><head> <meta charset='UTF-8'>"
  "<title>ESP NETWORK CONFIG</title>"
"</head>"

"<h1>CONFIG ACCEPTED</h1>"

"</html>"
);
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(apSSID, apPassword);
  
  IPAddress myIP = WiFi.softAPIP();

  dnsServer.setTTL(300);
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);

  dnsServer.start(DNS_PORT, "config.me", apIP);

  
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
 server.on("/setup", HTTP_POST, handleConfig);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}

void handleConfig() {                         // If a POST request is made to URI /login
  if( ! server.hasArg("ssid") || ! server.hasArg("password") 
      || server.arg("ssid") == NULL || server.arg("password") == NULL) { // If the POST request doesn't have username and password data
    server.send(400, "text/plain", "400: Invalid Request");         // The request is invalid, so send HTTP status 400
    return;
  }

  Serial.print("SSID: "); Serial.println(server.arg("ssid"));
  Serial.print("PASSWORD: "); Serial.println(server.arg("password"));
  server.send(200, "text/html", "<h1>Welcome, to " + server.arg("ssid") + "!</h1><p>Login successful</p>");
}
