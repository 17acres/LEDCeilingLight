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
    };
    typedef struct EmailStruct EmailContents;
    static std::queue<EmailContents> emailQueue;
    static void setup();
    static void sendDebugEmail(String subject, String body);
    static void sendDebugEmail(String subject);
    static void sendEmail(String subject, String body);
    static void sendEmail(String subject);
    static void runSpooler(std::function<void(void)> whileWaiting);
};
#endif