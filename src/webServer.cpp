#include "webServer.hpp"
ESP8266WebServer WebServer::server(80);
WiFiServer WebServer::tcpServer(23);
bool WebServer::isWakeupSoon;
time_t WebServer::wakeupStartTime;
AsyncPing WebServer::ping;

void WebServer::runPingTest()
{
    ping.on(false,[](const AsyncPingResponse &response) {
        if (response.total_recv == 0)
        { //no ping responses
            if (Animations::AnimationManager::getInstance()->getCurrentAnimation() != Animations::SlowOn::getInstance()){
                ESP.reset();
                Serial.println("Ping Failed!!!!!!!!!");
            }
        }
        return true;
    });
    ping.begin(WiFi.gatewayIP(), 5, 1000);
    //ping.begin(IPAddress(192,168,11,99), 5, 1000);
}