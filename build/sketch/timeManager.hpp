#ifndef TIMEMANAGER
#define TIMEMANAGER
#include <ESP8266WiFi.h>
#include <time.h>      // time() ctime()
#include <sys/time.h>  // struct timeval
#include <coredecls.h> // settimeofday_cb()

//https://github.com/esp8266/Arduino/blob/master/libraries/esp8266/examples/NTP-TZ-DST/NTP-TZ-DST.ino
#define TZ -5     // (utc+) TZ in hours
#define DST_MN 60 // use 60mn for summer time in some countries
#define TZ_MN ((TZ)*60)
#define TZ_SEC ((TZ)*3600)
#define DST_SEC ((DST_MN)*60)

class TimeManager
{
private:
    static timeval cbtime; // time set in callback
    static bool cbtime_set;

    static void time_is_set(void)
    {
        gettimeofday(&cbtime, NULL);
        cbtime_set = true;
        Serial.println("------------------ settimeofday() was called ------------------");
    }

public:
    static void setup()
    {
        settimeofday_cb(time_is_set);
        configTime(TZ_SEC, DST_SEC, "pool.ntp.org");
    }
    static time_t getTime()
    {
        time_t now;
        now = time(nullptr);
        return now;
    }
};

#endif