#include "domain/http-clients/remote-log-domain.h"

#include "devices/wifi-device.h"

namespace bigpjtemplate {

// "Soft" *singleton* global objects defined here,
//  but defined as extern and initialized in remote-log-domain.h.
RemoteLogDomain remoteLogDomain;

void RemoteLogDomain::setup() {}

void RemoteLogDomain::postRemoteLog(const char* message) {
#if IS_DEBUG == true
  Serial.print("POSTing remote log: " + (String)message);
#endif

  // Close any connection before sending a new request to free the socket on
  //  the NINA module.
  wifiClient.stop();

  // clang-format off
  // Allocate a char[] that is long enough (longer than needed is ok).
  char body[strlen(message)+100];
  sprintf(
    body, 
    "{"
      "\"message\": \"%s\","
      "\"app_id\": \"BIG_PROJECT_TEMPLATE\","
      "\"device_id\": \"ARDU_LION\""
    "}",
    message
  );
  // clang-format on
  _client.post("/iot/remote-logs/", "application/json", body);

  const unsigned short statusCode = _client.responseStatusCode();
#if IS_DEBUG == true
  Serial.println((String) " [" + statusCode + "]");
  if ((statusCode != 200) && (statusCode != 201)) {
    Serial.println("ERROR received an HTTP error response");
    Serial.println((String) "Status code: " + statusCode);
    String response = _client.responseBody();
    Serial.println("Response:\n" + response);
  }
#endif
}

}  // namespace bigpjtemplate
