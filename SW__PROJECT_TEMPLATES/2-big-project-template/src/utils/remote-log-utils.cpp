#include "utils/remote-log-utils.h"

#include "devices/wifi-device.h"

namespace myproject {
namespace remote_log_utils {

// TODO BUG !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// The RemoteLog makes HTTP requests to iot-be /iot/remote-logs/
//  but often they fail (not the 1st but the next ones), getting a 65533 status
//  code: sometimes the request did not reach the webserver, sometimes the
//  request was done correctly and the actual response status code was 201.
//  So I don't know what is going on, however this code here has been working
//  correctly for many days:
//  https://github.com/puntonim/iot-monorepo/blob/486bdbc87e40d0480e7091afd95982bd78d02fb4/projects/smart-ac/src/main.cpp
// *****************************************************************************

// "Soft" *singleton* global objects defined here,
//  but defined as extern and initialized in remote-log-domain.h.
RemoteLog remoteLog;

void RemoteLog::setup() {}

/**
 * Post the given text as remote log (or error) to iot-be /iot/remote-logs/
 *  endpoint.
 * Note: the message should always be text, so if you want to POST JSON then
 *  you have to convert it to text and escape quotes (as in
 *  ErrorDomain::_handleErrors).
 */
void RemoteLog::postRemoteLog(const char* message, bool isError /* = false */) {
  // Close any connection before sending a new request to free the socket on
  //  the NINA module.
  wifiClient.stop();

  // clang-format off
  // Allocate a char[] that is long enough (longer than needed is ok).
  char body[strlen(message)+100];
  sprintf(
    body,
    "{"
      "\"%s\": \"%s\","
      "\"app_id\": \"%s\","
      "\"device_id\": \"%s\""
    "}",
    isError ? "error" : "message",
    message,
    settings::IOT_BE_REMOTE_LOG_APP_ID,
    settings::IOT_BE_REMOTE_LOG_DEVICE_ID
  );
  // clang-format on

#if IS_DEBUG == true
  Serial.print("POSTing remote " + (String)(isError ? "ERROR " : "") +
               "log: " + body);
#endif

  // The docs suggests that this should be done only once, in case of many
  // requests. However I noticed that in case of multiple POST reqs in a
  // sequence, some can fail (the request is not actually performed and the
  //  client.responseStatusCode is 65533). Using many HttpClient instances,
  //  instead of sharing the same one, leads to less errors.
  HttpClient _client =
      HttpClient(wifiClient, settings::IOT_BE_IP, settings::IOT_BE_PORT);
  // Set the HTTP response timeout (milliseconds to wait without receiving any
  //  data before returning HTTP_ERROR_TIMED_OUT (during status code and header
  // processing)).
  _client.setHttpResponseTimeout(5 * 1000);  // 5 msec.

  _client.post("/iot/remote-logs/", "application/json", body);

#if IS_DEBUG == true
  const unsigned short statusCode = _client.responseStatusCode();
  Serial.println((String) " [" + statusCode + "]");
  // Note: the statusCode is not really realiable, as sometimes I get a 65533
  //  even if the actual one was 201.
  if ((statusCode != 200) && (statusCode != 201)) {
    Serial.println(
        "ERROR received an HTTP error response, but often this is not reliable "
        "(false error)");
    Serial.println((String) "Status code: " + statusCode);
    String response = _client.responseBody();
    Serial.println("Response:\n" + response);
  }
#endif
}

}  // namespace remote_log_utils
}  // namespace myproject
