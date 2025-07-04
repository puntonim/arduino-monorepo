#include "devices/wifi-device.h"

#include <ArduinoHttpClient.h>
#include <TaskManagerIO.h>
#include <WiFiS3.h>

namespace bigpjtemplate {

// "Soft" *singleton* global objects defined here,
//  but defined as extern and initialized in wifi-device.h.
WifiDevice wifiDevice;
WiFiClient wifiClient;
// WiFiSSLClient wifiHttpsClient;

void WifiDevice::setup() {
  // Ensure there is a WiFi module on the board.
  if (WiFi.status() == WL_NO_MODULE) {
#if IS_DEBUG == true
    Serial.println("ERROR WiFi module not detected!");
    Serial.println("Exiting...");
#endif
    while (true);
  }

  // Check the WiFi firmware version.
  String fv = WiFi.firmwareVersion();
#if IS_DEBUG == true
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println(
        "Please upgrade the firmware: "
        "https://support.arduino.cc/hc/en-us/articles/"
        "9670986058780-Update-the-connectivity-module-firmware-on-UNO-R4-WiFi");
  }
#endif

  // Connect to WiFi.
  unsigned short wifiConnectionStatus = WL_IDLE_STATUS;
  while (wifiConnectionStatus != WL_CONNECTED) {
#if IS_DEBUG == true
    Serial.println((String) "Connecting to SSID " + _ssid + "...");
#endif
    wifiConnectionStatus = WiFi.begin(_ssid, _ppp);

#if IS_DEBUG == true
    Serial.println("Waiting 10 secs...");
#endif
    // Sleep 10 secs: this is how to sleep using TaskManagerIo. It's blocking.
    taskManager.yieldForMicros(10 * 1000 * 1000);
  }
#if IS_DEBUG == true
  _printWifiConnectionInfo();
#endif
  // Set the connection timeout (msec).
  wifiClient.setConnectionTimeout(3 * 1000);  // msec.
}

void WifiDevice::_printWifiConnectionInfo() {
#if IS_DEBUG == true
  Serial.print("Connected to SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("Assigned IP address: ");
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
#endif
}

}  // namespace bigpjtemplate