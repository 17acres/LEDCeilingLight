#ifndef TIMEMANAGER
#define TIMEMANAGER
#include <ESP8266WiFi.h>
#include <time.h>      // time() ctime()
#include <sys/time.h>  // struct timeval
#include <coredecls.h> // settimeofday_cb()
#include <TZ.h>

//https://github.com/esp8266/Arduino/blob/master/libraries/esp8266/examples/NTP-TZ-DST/NTP-TZ-DST.ino

class TimeManager
{
private:
    static timeval cbtime; // time set in callback
    static bool cbtime_set;

    static void time_is_set(void)
    {
        gettimeofday(&cbtime, NULL);
        cbtime_set = true;
    }

public:
    static void setup()
    {
        settimeofday_cb(time_is_set);
        configTime(TZ_America_Detroit, "pool.ntp.org");
    }
    static time_t getTime()
    {
        time_t now;
        now = time(nullptr)-8*60*60;//fix weird 8 hour bug
        return now;
    }
    //Check for 1970 time
    static bool isValid(time_t *time)
    {
        return (localtime(time)->tm_year) > 70;
    }
    static bool isReady()
    {
        time_t now = getTime();
        return isValid(&now) && cbtime_set;
    }
};

#endif