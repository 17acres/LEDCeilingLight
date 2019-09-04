#include "webServer.hpp"
ESP8266WebServer WebServer::server(80);
bool WebServer::isWakeupSoon;
time_t WebServer::wakeupStartTime;