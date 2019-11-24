#include "emailSender.hpp"
#include "animationManager.hpp"
#include "noGit/auth.hpp"
#include "ESP8266SMTP.hpp"
#include "timeManager.hpp"
std::queue<EmailSender::EmailContents> EmailSender::emailQueue;
void EmailSender::setup()
{
    SMTP.setEmail(email).setPassword(pwd).setFrom("Mr. Ceiling Light").setForGmail();
}
void EmailSender::sendEmail(String subject, String body)
{
    emailQueue.push((EmailContents){subject, body, TimeManager::getTime()});
}
void EmailSender::sendEmail(String subject)
{
    sendEmail(subject, "Dan couldn't be bothered to write a body. With warm regards, Mr. Ceiling Light");
}

void EmailSender::sendDebugEmail(String subject, String body)
{
    if(DEBUG_EMAILS)
        sendEmail(subject,body);
}
void EmailSender::sendDebugEmail(String subject)
{
    if(DEBUG_EMAILS)
        sendEmail(subject,"Dan couldn't be bothered to write a body. This is a tedious debug email that should be disabled eventually. With warm regards, Mr. Ceiling Light");
}

void EmailSender::runSpooler(std::function<void(void)> whileWaiting)
{
    whileWaiting();
    if (
        ((Animations::AnimationManager::getInstance()->getCurrentAnimation() == Animations::On::getInstance()) ||
         (Animations::AnimationManager::getInstance()->getCurrentAnimation() == Animations::Off::getInstance())) &&
        !emailQueue.empty() && TimeManager::isReady())
    {
        EmailContents thisEmail = emailQueue.front();
        emailQueue.pop();
        SMTP.Subject(thisEmail.subject.c_str());
        time_t messageTime=thisEmail.sendTime;
        String timeString=asctime(localtime(&messageTime));
        if(!TimeManager::isValid(&messageTime))
        {
            messageTime=TimeManager::getTime();
            timeString=asctime(localtime(&messageTime));
            timeString+=" **TIME WAS ZERO, REPLACED WITH SEND TIME**";
        }
        if (SMTP.Send(emailDest, thisEmail.body + "<br>Sent at: " + timeString, whileWaiting))
            Serial.println("Message sent");
        else
        {
            Serial.print("Sending Error ");
            Serial.println(SMTP.getError());
        }
    }
}