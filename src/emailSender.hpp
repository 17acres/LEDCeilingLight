#ifndef EMAILSENDER
#define EMAILSENDER
#include <queue>
#include <WString.h>
#include <functional>
#include "timeManager.hpp"
class EmailSender
{
public:
    struct EmailStruct
    {
        String subject;
        String body;
        time_t sendTime;
        bool isLog;
    };
    typedef struct EmailStruct EmailContents;
    static std::queue<EmailContents> emailQueue;
    static void setup();
    static void sendDebugEmail(String subject, String body, bool isLog);
    static void sendDebugEmail(String subject, bool isLog);
    static void sendEmail(String subject, String body, bool isLog);
    static void sendEmail(String subject, bool isLog);
    static void runSpooler(std::function<void(void)> whileWaiting);
    static void sendActualSMTP();
    static void sendActualMQTT();
};
#endif