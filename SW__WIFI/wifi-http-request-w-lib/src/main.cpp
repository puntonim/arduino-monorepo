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

/**
 * TODO BUG!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * The requests done in this code often fail.
 * They might not reach the server and get a 65534 status code, eg.:
 *    Making GET request...
 *    Status code: 65534
 *    Response:
 * Or:
 *    Making POST request...
 *    CONNECTION_FAILED  <-- also ERROR_API
 *    Status code: 65534
 *    Response:
 * Or they might reach the server, get a 200, but still the status code
 *  seems to be 65534, eg.:
 *    Making POST request...
 *    Status code: 65533
 *    Response:
 *    {
 *      "method": "POST",
 *      "protocol": "http",
 *      "host": "echo.free.beeceptor.com",
 *      "path": "/ardu",
 *      "ip": "151.55.224.90:61294",
 *      "headers": {
 *        "Host": "echo.free.beeceptor.com",
 *        "User-Agent": "Arduino/2.2.0",
 *        "Content-Length": "12",
 *        "Content-Type": "application/json",
 *        "Via": "1.1 Caddy",
 *        "Accept-Encoding": "gzip"
 *      },
 *      "parsedQueryParams": {},
 *      "parsedBody": {
 *        "temp": 33
 *      }
 *    }
 * Or:
 *    Making POST request...
 *    SUCCESS
 *    Status code: 65533
 *    Response:
 *
 * However, the code in
 * https://github.com/puntonim/iot-monorepo/blob/d310120ced88b65a87dafa51288e8970ec6ffea3/projects/smart-ac/src/main.cpp
 *  has been working for days in many occasions and I've never seen failing
 *  HTTP requests. Maybe because I wasn't looking for them, but anyhow I've
 *  never noticed a failed HTTP request.
 *
 * What to do now:
 *  - it's kind of useless to try with a different lib as this is the
 *     official one, but maybe I can try
 *  - it's not about the JSON data POSTed, as the GET requests are failing too
 *  - it's not about the timeouts, as it happens also when I comment them out
 *  - it's not about sharing the single HttpClient var among multiple reqs
 *  - it's not wifi.stop() as they fails even commenting it out
 */

#include <Arduino.h>
#include <ArduinoHttpClient.h>
#include <WiFiS3.h>

// Functions declaration.
void printWifiConnectionInfo();
void doGetRequest();
void doPostRequest();

// See SW__PROJECT_TEMPLATES/1-big-project-template to know how to manage
//  secrets properly.
char ssid[] = "XXX";  // TODO change to the real values.
char pass[] = "XXX";  // TODO change to the real values.

WiFiClient wifi;
WiFiSSLClient wifiSsl;

// The docs suggests that this should be done only once, in case of many
//  requests. However I noticed that HTTP requests often fail either way:
//  when sharing one single module-lever var or instantiating a new one on
//  each request.
HttpClient http = HttpClient(wifi, "echo.free.beeceptor.com", 80);
HttpClient https = HttpClient(wifiSsl, "echo.free.beeceptor.com", 443);

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
  wifi.setConnectionTimeout(3 * 1000);  // msec.
  wifiSsl.setConnectionTimeout(3 * 1000);
}

void loop() {
  Serial.println("\n\nSTART LOOP");

  // Testing multiple requests.
  doGetRequest();
  doGetRequest();
  doPostRequest();
  doPostRequest();
  doPostRequest();
  doPostRequest();
  doGetRequest();

  Serial.println("\n\nEND LOOP");

  // Do nothing.
  while (true);
}

void doGetRequest() {
  // Example:
  //  https://github.com/arduino-libraries/ArduinoHttpClient/blob/master/examples/DweetGet/DweetGet.ino

  // Close any connection before sending a new request to free the socket on
  //  the NINA module.
  wifi.stop();
  // wifiSsl.stop();

  // Set the HTTP response timeout (milliseconds to wait without receiving any
  //  data before returning HTTP_ERROR_TIMED_OUT (during status code and header
  // processing)).
  http.setHttpResponseTimeout(5 * 1000);  // msec.

  Serial.println("\n\nMaking GET request...");
  int x = http.get("/");
  if (x == HTTP_SUCCESS)
    Serial.println("SUCCESS");
  else if (x == HTTP_ERROR_CONNECTION_FAILED)
    Serial.println("CONNECTION_FAILED");
  else if (x == HTTP_ERROR_API)
    Serial.println("ERROR_API");
  else if (x == HTTP_ERROR_TIMED_OUT)
    Serial.println("TIMED_OUT");
  else if (x == HTTP_ERROR_INVALID_RESPONSE)
    Serial.println("INVALID_RESPONSE");

  const unsigned short statusCode = http.responseStatusCode();
  // Note: the statusCode is not really realiable, as sometimes I get a 65533
  // even if
  //  the actual one was 201.
  String response = http.responseBody();
  Serial.println((String) "Status code: " + statusCode);
  Serial.println("Response:\n" + response);
}

void doPostRequest() {
  // Example:
  //  https://github.com/arduino-libraries/ArduinoHttpClient/blob/master/examples/DweetPost/DweetPost.ino

  // Close any connection before sending a new request to free the socket on
  //  the NINA module.
  wifi.stop();

  Serial.println("\n\nMaking POST request...");

  // Set the HTTP response timeout (milliseconds to wait without receiving any
  //  data before returning HTTP_ERROR_TIMED_OUT (during status code and header
  //  processing)).
  http.setHttpResponseTimeout(5 * 1000);  // msec.

  const char body[] = "{\"temp\": 33}";
  // Or: String body = "{\"temp\": 33}";
  int x = http.post("/ardu", "application/json", body);
  if (x == HTTP_SUCCESS)
    Serial.println("SUCCESS");
  else if (x == HTTP_ERROR_CONNECTION_FAILED)
    Serial.println("CONNECTION_FAILED");
  else if (x == HTTP_ERROR_API)
    Serial.println("ERROR_API");
  else if (x == HTTP_ERROR_TIMED_OUT)
    Serial.println("TIMED_OUT");
  else if (x == HTTP_ERROR_INVALID_RESPONSE)
    Serial.println("INVALID_RESPONSE");

  const unsigned short statusCode = http.responseStatusCode();
  // Note: the statusCode is not really realiable, as sometimes I get a 65533
  // even if
  //  the actual one was 201.
  String response = http.responseBody();
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
