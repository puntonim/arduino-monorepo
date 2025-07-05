/**
 * WIFI: HTTP REQUEST
 * =================
 * Making HTTP requests without any extra lib: not worth, better to use a lib.
 * My recommendation is to always use the lib:
 *  https://github.com/arduino-libraries/ArduinoHttpClient
 *
 * Note: Arduino Uno R4 WiFi has an embedded ESP32-S3-MINI-1-N8 module that
 * handles Wifi and Bluetooth.
 *
 * Docs:
 *  - https://docs.arduino.cc/tutorials/uno-r4-wifi/wifi-examples/
 *  -
 * https://github.com/arduino/ArduinoCore-renesas/blob/main/libraries/WiFiS3/docs/api.md
 *
 * Libs: none
 */

#include <Arduino.h>
#include <WiFiS3.h>

// Functions declaration.
void printWifiConnectionInfo();
void sendHttpGetRequest();
void readHttpResponseIn1CharChunks();
void readHttpResponseIn1BigChunk();
void readHttpResponseInSmallChunks();

// See SW__PROJECT_TEMPLATES/big-project-template to know how to manage
//  secrets properly.
char ssid[] = "XXX";  // TODO change to the real values.
char pass[] = "XXX";  // TODO change to the real values.
WiFiClient wifiClient;
// WiFiSSLClient wifiHttpsClient; // For HTTPS requests.

void setup() {
  Serial.begin(57600);
  unsigned short i = 30;
  while (!Serial && i--) delay(1);
  Serial.println("START " __FILE__ " compiled on " __DATE__);

  // Ensure there is a WiFi module on the board.
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("ERROR WiFi module not detected!");
    Serial.println("Exiting...");
    while (true);
  }

  // Check the WiFi firmware version.
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println(
        "Please upgrade the firmware: "
        "https://support.arduino.cc/hc/en-us/articles/"
        "9670986058780-Update-the-connectivity-module-firmware-on-UNO-R4-WiFi");
  }

  // Connect to WiFi.
  unsigned short wifiConnectionStatus = WL_IDLE_STATUS;
  while (wifiConnectionStatus != WL_CONNECTED) {
    Serial.println((String) "Connecting to SSID " + ssid + "...");
    wifiConnectionStatus = WiFi.begin(ssid, pass);
    Serial.println("Waiting 10 secs...");
    delay(10 * 1000);
  }
  printWifiConnectionInfo();

  // Set the connection timeout (msec).
  wifiClient.setConnectionTimeout(3 * 1000);  // msec.
}

void loop() {
  Serial.println("\n");
  sendHttpGetRequest();

  readHttpResponseIn1CharChunks();
  // readHttpResponseIn1BigChunk();
  // readHttpResponseInSmallChunks();

  // Do nothing.
  while (true);
}

void printWifiConnectionInfo() {
  Serial.print("Connected to SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("Assigned IP address: ");
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void sendHttpGetRequest() {
  Serial.println("Sending HTTP GET request...");
  // Close any connection before sending a new request to free the socket on
  //  the NINA module.
  wifiClient.stop();

  // Note: use the class WiFiSSLClient for HTTPS requests.
  if (wifiClient.connect("example.org", 80)) {
    // if (wifiClient.connect(IPAddress(192, 168, 1, 251), 7777)) {
    Serial.println("Successfully connected to the HTTP server");

    wifiClient.println("GET /iot/temps/ HTTP/1.1");
    wifiClient.println("Host: example.org");
    wifiClient.println("User-Agent: ArduinoWiFi/1.1");
    wifiClient.println("Connection: close");
    wifiClient.println();

    // A POST request would llok like this, instead:
    // POST /update-sensor HTTP/1.1
    // Host: example.com
    // {sensor_name: "name", temperature: 10}
    // Content-Type: application/json
  } else {
    Serial.println("The connection to the HTTP server failed");
  }
}

void readHttpResponseIn1CharChunks() {
  Serial.println("Reading response in many 1-char chunks...");

  // TODO see comments in readHttpResponseInSmallChunks().

  String body = "";
  while (wifiClient.connected()) {
    if (wifiClient.available()) {
      Serial.println(" > Response received");
      while (wifiClient.available()) {
        char c = wifiClient.read();
        // Serial.print(c);
        body += c;
      }
      Serial.println(" > Response complete");
      Serial.println(body);
      // Get the status code.
      // You can cnvert it to int, but in weird cases this conversion might
      //  fail, so maybe it is better to keep it as string.
      // const unsigned short statusCode = body.substring(9, 12).toInt();
      String statusCode = body.substring(9, 12);
      Serial.println("\nResponse status code: X" + statusCode + "X");
    }
  }

  Serial.println(
      "The client is not connected, closing the connection to the HTTP "
      "server...");
  wifiClient.stop();
}

void readHttpResponseIn1BigChunk() {
  Serial.println("Reading response in 1 big chunk...");

  // TODO see comments in readHttpResponseInSmallChunks().

  while (wifiClient.connected()) {
    // Determine the size of the response data.
    const unsigned int size = wifiClient.available();
    uint8_t body[size];
    if (size > 0) {
      Serial.println(" > Response received");
      wifiClient.read(body, size);
      Serial.println(" > Response complete");
    }
    Serial.println((char *)&body);
    // Get the status code.
    String statusCode;
    for (int i = 9; i <= 11; i++) statusCode += (char)body[i];
    Serial.println("\nResponse status code: X" + statusCode + "X");
  }

  Serial.println(
      "The client is not connected, closing the connection to the HTTP "
      "server...");
  wifiClient.stop();
}

void readHttpResponseInSmallChunks() {
  Serial.println("Reading response in small chunks...");
  // TODO use a timeout here? To make sure we exit the while loop even if
  //  connected when the timeout fires. Or is
  //  `wifiClient.setConnectionTimeout()` already doing this?
  String body = "";
  while (wifiClient.connected()) {
    if (wifiClient.available()) {
      // TODO use a timeout since the first received data, in order to
      //  prevent the attack where the senders sends packets very slowly and
      //  forever in order to keep the client busy?
      // TODO what happens when the response is large (1 Mbyte)?
      Serial.println(" > Response received");
      while (wifiClient.available() > 0) {
        // Read max 100 bytes per loop. If you use 100 Kbyte (100*1000) it will
        //  not work (silent crash).
        const unsigned short size = min(100, wifiClient.available());
        uint8_t buffer[size];
        wifiClient.read(buffer, size);
        // Strings are null terminated. And the last char in the buffer is
        //  garbage (I don't know why). So overwrite the last char with null.
        buffer[size] = NULL;
        // Serial.print((char *)&buffer);
        body += (char *)&buffer;
      }
      Serial.println("\n > Response complete");
      Serial.println(body);
      // Get the status code.
      // You can cnvert it to int, but in weird cases this conversion might
      //  fail, so maybe it is better to keep it as string.
      // const unsigned short statusCode = body.substring(9, 12).toInt();
      String statusCode = body.substring(9, 12);
      Serial.println("\nResponse status code: X" + statusCode + "X");
    }
  }

  Serial.println(
      "The client is not connected, closing the connection to the HTTP "
      "server...");
  wifiClient.stop();
}
