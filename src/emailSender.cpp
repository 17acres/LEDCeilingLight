#include "emailSender.hpp"
#include "animation/animationManager.hpp"
#include "defs.hpp"
#include "../noGit/auth.hpp"
//#include "webServer.hpp"
#if DIRECT_EMAIL
#include "drivers/ESP8266SMTP.hpp"
#endif
#include "timeManager.hpp"
std::queue<EmailSender::EmailContents> EmailSender::emailQueue;
void EmailSender::setup()
{
#if DIRECT_EMAIL
    SMTP.setEmail(AUTH_EMAIL).setPassword(AUTH_PWD).setFrom("Mr. Ceiling Light").setForGmail();
#endif
}
void EmailSender::sendEmail(String subject, String body, bool isLog)
{
    subject.replace("\n","");
    body.replace("\n","");
    emailQueue.push((EmailContents){subject, body, TimeManager::getTime(), isLog});
}
void EmailSender::sendEmail(String subject, bool isLog)
{
    sendEmail(subject, "Dan couldn't be bothered to write a body. With warm regards, Mr. Ceiling Light", isLog);
}

void EmailSender::sendDebugEmail(String subject, String body, bool isLog)
{
    if (DEBUG_EMAILS)
        sendEmail(subject, body, isLog);
}
void EmailSender::sendDebugEmail(String subject, bool isLog)
{
    if (DEBUG_EMAILS)
        sendEmail(subject, "Dan couldn't be bothered to write a body. This is a tedious debug email that should be disabled eventually. With warm regards, Mr. Ceiling Light", isLog);
}

void EmailSender::runSpooler(std::function<void(void)> whileWaiting)
{
    whileWaiting();
    if (
        ((Animations::AnimationManager::getInstance()->getCurrentAnimation() == Animations::On::getInstance()) ||
         (Animations::AnimationManager::getInstance()->getCurrentAnimation() == Animations::Off::getInstance())) &&
        !emailQueue.empty() /*&& TimeManager::isReady() && WebServer::mqttClient.connected()*/)
    {
#if DIRECT_EMAIL
        sendActualSMTP();
#else
        sendActualMQTT();
#endif
    }
}

#if DIRECT_EMAIL
void EmailSender::sendActualSMTP()
{
    EmailContents thisEmail = emailQueue.front();
    emailQueue.pop();
    SMTP.Subject(thisEmail.subject.c_str());
    time_t messageTime = thisEmail.sendTime;
    String timeString = asctime(localtime(&messageTime));
    if (!TimeManager::isValid(&messageTime))
    {
        messageTime = TimeManager::getTime();
        timeString = asctime(localtime(&messageTime));
        timeString += " **TIME WAS ZERO, REPLACED WITH SEND TIME**";
    }
    if (SMTP.Send(AUTH_EMAIL_DEST, thisEmail.body + "<br>Sent at: " + timeString, whileWaiting))
        IFDEBUG(Serial.println("Message sent"));
    else
    {
        IFDEBUG(Serial.print("Sending Error "));
        IFDEBUG(Serial.println(SMTP.getError()));
    }
}
#else

void EmailSender::sendActualMQTT()
{
    EmailContents thisEmail = emailQueue.front();
    emailQueue.pop();

    time_t messageTime = thisEmail.sendTime;
    String timeString = asctime(localtime(&messageTime));
    timeString.trim();
    // if (!TimeManager::isValid(&messageTime))
    // {
    //     messageTime = TimeManager::getTime();
    //     timeString = asctime(localtime(&messageTime)); //asctime ends with /n
    //     timeString.trim();
    //     timeString += "<br> **TIME WAS ZERO, REPLACED WITH SEND TIME**";
    // }
    

    String emailStr = "<br>Subject: " + thisEmail.subject + "<br><br>Body:<br>" + thisEmail.body + "<br>Sent at: " + timeString;

    if (!thisEmail.isLog)
    {
        String mqttMsg = "{\"channel\":\"ceilinglight\",\"resource\":\"report\", \"data\":\"" + emailStr + "\", \"ispublic\": true, \"ts\":"+String((unsigned long)messageTime)+"000}";
        //WebServer::mqttClient.publish("ceilinglight/report", mqttMsg.c_str());
    }
    String mqttMsg = "{\"channel\":\"ceilinglight\",\"resource\":\"log\",\"data\":\"" + emailStr + "\", \"write\": true, \"ispublic\": true, \"ts\":"+String((unsigned long)messageTime)+"000}";
    IFDEBUG(Serial.println(mqttMsg));

    //int rc=WebServer::mqttClient.publish("ceilinglight/log", mqttMsg.c_str());
    //IFDEBUG(Serial.println(rc));
}
#endif
