/**
 * WIFI: HTTP REQUEST WITH LIB
 * ==========================
 * I recommend making HTTP requests using the lib:
 *  https://github.com/arduino-libraries/ArduinoHttpClient
 *
 * Note: Arduino Uno R4 WiFi has an embedded ESP32-S3-MINI-1-N8 module that
 * handles Wifi and Bluetooth.
 *
 * Docs:
 *  -
 *  - https://docs.arduino.cc/tutorials/uno-r4-wifi/wifi-examples/
 *  -
 * https://github.com/arduino/ArduinoCore-renesas/blob/main/libraries/WiFiS3/docs/api.md
 *
 * Libs: none
 */

#include <Arduino.h>
#include <ArduinoHttpClient.h>
#include <WiFiS3.h>

// Functions declaration.
void printWifiConnectionInfo();
void doGetRequest(bool isHttps = false);
void doPostRequest();

// See SW__PROJECT_TEMPLATES/big-project-template to know how to manage
//  secrets properly.
char ssid[] = "XXX";  // TODO change to the real values.
char pass[] = "XXX";  // TODO change to the real values.
WiFiClient wifiClient;
WiFiSSLClient wifiHttpsClient;

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
  wifiHttpsClient.setConnectionTimeout(3 * 1000);
}

void loop() {
  Serial.println("\n\nSTART LOOP");

  // Testing multiple requests.
  doGetRequest(true);
  doGetRequest(true);
  doPostRequest();
  doPostRequest();
  doPostRequest();
  doPostRequest();

  Serial.println("\n\nEND LOOP");

  // Do nothing.
  while (true);
}

void doGetRequest(bool isHttps /* = false */) {
  // Example:
  //  https://github.com/arduino-libraries/ArduinoHttpClient/blob/master/examples/DweetGet/DweetGet.ino

  // Close any connection before sending a new request to free the socket on
  //  the NINA module.
  wifiClient.stop();
  if (isHttps) wifiHttpsClient.stop();

  // The docs suggests that this should be done only once, in case of many requests.
  // However I noticed that in case of multiple POST reqs in a sequence, some can fail
  //  (the request is not actually performed and the client.responseStatusCode is 65533).
  //  Using many HttpClient instances, instead of sharing the same one, leads to
  //  less errors.
  HttpClient client = HttpClient(wifiClient, "echo.free.beeceptor.com", 80);
  if (isHttps)
    client = HttpClient(wifiHttpsClient, "echo.free.beeceptor.com", 443);

  // Set the HTTP response timeout (milliseconds to wait without receiving any
  //  data before returning HTTP_ERROR_TIMED_OUT (during status code and header
  // processing)).
  client.setHttpResponseTimeout(5 * 1000);  // msec.

  Serial.println("\n\nMaking GET request...");
  client.get("/");

  // HttpClient client = HttpClient(wifiClient, IPAddress(192, 168, 1, 251),
  // 7777);
  // client.get("/iot/temps/");

  const unsigned short statusCode = client.responseStatusCode();
  // Note: the statusCode is not really realiable, as sometimes I get a 65533 even if
  //  the actual one was 201.
  String response = client.responseBody();
  Serial.println((String) "Status code: " + statusCode);
  Serial.println("Response:\n" + response);
}

void doPostRequest() {
  // Example:
  //  https://github.com/arduino-libraries/ArduinoHttpClient/blob/master/examples/DweetPost/DweetPost.ino

  // Close any connection before sending a new request to free the socket on
  //  the NINA module.
  wifiClient.stop();

  // The docs suggests that this should be done only once, in case of many requests.
  // However I noticed that in case of multiple POST reqs in a sequence, some can fail
  //  (the request is not actually performed and the client.responseStatusCode is 65533).
  //  Using many HttpClient instances, instead of sharing the same one, leads to
  //  less errors.
  HttpClient client = HttpClient(wifiClient, "echo.free.beeceptor.com", 80);
  Serial.println("\n\nMaking POST request...");

  // Set the HTTP response timeout (milliseconds to wait without receiving any
  //  data before returning HTTP_ERROR_TIMED_OUT (during status code and header
  //  processing)).
  client.setHttpResponseTimeout(5 * 1000);  // msec.

  const char body[] = "{\"temp\": 33}";
  // Or: String body = "{\"temp\": 33}";
  client.post("/ardu", "application/json", body);

  const unsigned short statusCode = client.responseStatusCode();
  // Note: the statusCode is not really realiable, as sometimes I get a 65533 even if
  //  the actual one was 201.
  String response = client.responseBody();
  Serial.println((String) "Status code: " + statusCode);
  Serial.println("Response:\n" + response);
}

void printWifiConnectionInfo() {
  Serial.print("Connected to SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("Assigned IP address: ");
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI): ");
  Serial.print(rssi);
  Serial.println(" dBm");
}
