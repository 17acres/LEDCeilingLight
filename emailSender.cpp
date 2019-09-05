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

void EmailSender::runSpooler(std::function<void(void)> whileWaiting)
{
    Utils::delayUpdate(11); //Make sure light fully turns off before sending mails
    if (
        ((Animations::AnimationManager::getInstance()->getCurrentAnimation() == Animations::On::getInstance()) ||
         (Animations::AnimationManager::getInstance()->getCurrentAnimation() == Animations::Off::getInstance())) &&
        !emailQueue.empty())
    {
        EmailContents thisEmail = emailQueue.front();
        emailQueue.pop();
        SMTP.Subject(thisEmail.subject.c_str());
        if (SMTP.Send(emailDest, thisEmail.body + "<br>Sent at: " + asctime(localtime(&thisEmail.sendTime)), whileWaiting))
            Serial.println("Message sent");
        else
        {
            Serial.print("Sending Error ");
            Serial.println(SMTP.getError());
        }
    }
    else
    {
        whileWaiting();
    }
}