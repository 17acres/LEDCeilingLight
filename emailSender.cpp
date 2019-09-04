#include "emailSender.hpp"
#include "animationManager.hpp"
#include "noGit/auth.hpp"
#include "ESP8266SMTP.hpp"


std::queue<EmailSender::EmailContents> EmailSender::emailQueue;
void EmailSender::setup()
{
    SMTP.setEmail(email).setPassword(pwd).setFrom("Mr. Ceiling Light").setForGmail();
}
void EmailSender::sendEmail(String subject, String body)
{
    emailQueue.push((EmailContents){subject, body});
}
void EmailSender::sendEmail(String subject)
{
    sendEmail(subject, "Dan couldn't be bothered to write a body. With warm regards, Mr. Ceiling Light");
}

void EmailSender::runSpooler(std::function<void(void)> whileWaiting)
{
    if (emailQueue.empty())
    {
        whileWaiting();
    }
    else if (
        (Animations::AnimationManager::getInstance()->getCurrentAnimation() != Animations::FunOn::getInstance()) &&
        (Animations::AnimationManager::getInstance()->getCurrentAnimation() != Animations::FadeOff::getInstance()))
    {
        EmailContents thisEmail = emailQueue.front();
        emailQueue.pop();
        SMTP.Subject(thisEmail.subject.c_str());
        if (SMTP.Send(emailDest, thisEmail.body, whileWaiting))
            Serial.println("Message sent");
        else
        {
            Serial.print("Sending Error ");
            Serial.println(SMTP.getError());
        }
    }
}