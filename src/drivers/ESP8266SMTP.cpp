/* 
*  ESP8266SMTPHelper class helps send e-mails
*  using Arduino core for ESP8266 WiFi chip
*  by Boris Shobat
*  May 11 2017
*  https://github.com/CosmicBoris/ESP8266SMTP/
*/

#include "ESP8266SMTP.hpp"
#include <base64.h>
#include "../utils.hpp"
ESP8266SMTPHelper::ESP8266SMTPHelper(const char* login, const char* password) :
	_base64_login(strdup(login)),
	_base64_password(strdup(password)){}

ESP8266SMTPHelper::~ESP8266SMTPHelper()
{
	if(_subject) delete[] _subject;
	if(_from) delete[] _from;
	if(_base64_login) delete[] _base64_login;
	if(_base64_password) delete[] _base64_password;
	
}

ESP8266SMTPHelper& ESP8266SMTPHelper::Subject(const char* s)
{
	if(_subject) 
		delete[] _subject;

	_subject = strdup(s);
	return *this;
}

ESP8266SMTPHelper& ESP8266SMTPHelper::setPort(uint16_t port)
{
	_smtp_port = port;
	return *this;
}

ESP8266SMTPHelper& ESP8266SMTPHelper::setServer(const char *server)
{
	if(_smtp_server)
		delete[] _smtp_server;

	_smtp_server = strdup(server);
	return *this;
}

ESP8266SMTPHelper& ESP8266SMTPHelper::setFrom(const char* from)
{
	if(_from)
		delete[] _from;

	_from = strdup(from);
	return *this;
}

ESP8266SMTPHelper& ESP8266SMTPHelper::setEmail(const char* m)
{
	if(_emailAddress)
		delete[] _emailAddress;
	if(_base64_login)
		delete[] _base64_login;
	_emailAddress = strdup(m);
	_base64_login = strdup(base64::encode(m).c_str());

	return *this;
}

ESP8266SMTPHelper& ESP8266SMTPHelper::setPassword(const char* password)
{
	if(_base64_password) 
		delete[] _base64_password;

	_base64_password = strdup(base64::encode(password).c_str());
	return *this;
}

ESP8266SMTPHelper& ESP8266SMTPHelper::setForGmail()
{
	_smtp_port = 465;
	setServer("smtp.gmail.com");
	return *this;
}

char* ESP8266SMTPHelper::getBase64Email()
{
	return _base64_login;
}

char* ESP8266SMTPHelper::getBase64Password()
{
	return _base64_password;
}

const char* ESP8266SMTPHelper::getError()
{
	return _error;
}

String ESP8266SMTPHelper::getLastResponce()
{
	return _serverResponce;
}

bool ESP8266SMTPHelper::AwaitSMTPResponse(WiFiClientSecure &client, std::function<void(void)> whileWaiting, const String &resp, uint16_t timeOut)
{
	uint32_t timeAwait = millis() + timeOut;
	while(!client.available()) {
		//whileWaiting();
		if(millis() > timeAwait) {
			_error = "SMTP Response TIMEOUT!";
			return false;
		}
	}
	_serverResponce = client.readStringUntil('\n');
#if defined(GS_SERIAL_LOG_LEVEL_1) || defined(GS_SERIAL_LOG_LEVEL_2) 
	Serial.println(_serverResponce);
#endif
	return !resp || _serverResponce.indexOf(resp) != -1;
}

bool ESP8266SMTPHelper::Send(const String &to, const String &message, std::function<void(void)> whileWaiting)
{
	if(!_smtp_server) {
		_error = "SMTP server not set.";
		return false;
	}

	WiFiClientSecure client;
	client.setInsecure();

#if defined(GS_SERIAL_LOG_LEVEL_2)
	Serial.print(F("Connecting to: "));
	Serial.println(_smtp_server);
#endif
	if(!client.connect(_smtp_server, _smtp_port)) {
		_error = "Could not connect to mail server";
		return false;
	}
	if(!AwaitSMTPResponse(client, whileWaiting, "220")) {
		_error = "Connection error";
		return false;
	}

#if defined(GS_SERIAL_LOG_LEVEL_2)
	Serial.println(SMTP_HELO);
#endif
	client.println(SMTP_HELO);
	if(!AwaitSMTPResponse(client, whileWaiting, "250")) {
		_error = "identification error";
		return false;
	}

#if defined(GS_SERIAL_LOG_LEVEL_2)
	Serial.println(FPSTR(SMTP_AUTH));
#endif
	client.println(SMTP_AUTH);
	AwaitSMTPResponse(client, whileWaiting);

#if defined(GS_SERIAL_LOG_LEVEL_2)
	Serial.println(F("BASE64_LOGIN:"));
#endif
	client.println(_base64_login);
	AwaitSMTPResponse(client, whileWaiting);

#if defined(GS_SERIAL_LOG_LEVEL_2)
	Serial.println(F("BASE64_PASSWORD:"));
#endif
	client.println(_base64_password);
	if(!AwaitSMTPResponse(client, whileWaiting, "235")) {
		_error = "SMTP AUTH error";
		return false;
	}
	
	String tmp;
	tmp.reserve(46); // 14 length of SMTP_FROM + 32 for email, trying to prevent reallocation of string buffer when string changes
	tmp = SMTP_FROM;
	tmp.replace("$", _emailAddress);

#if defined(GS_SERIAL_LOG_LEVEL_2)
	Serial.println(tmp);
#endif
	client.println(tmp);
	AwaitSMTPResponse(client, whileWaiting);
	
	bool oneRecepient = to.indexOf(',') == -1;
	
	if(oneRecepient) {
		tmp = SMTP_RCPT;
		tmp.replace("$", to);
		
#if defined(GS_SERIAL_LOG_LEVEL_2)
		Serial.println(tmp);
#endif
		client.println(tmp);
		AwaitSMTPResponse(client, whileWaiting);
	} else {
		char *toCopy = strdup(to.c_str()); // make copy becouse strtok modifyes original string
		char *sz_r = strtok(toCopy, ",");
		while(sz_r) {
			while(*sz_r == ' ') ++sz_r;    // skip spaces after comma.
				
			tmp = SMTP_RCPT;
			tmp.replace("$", sz_r);
			
#if defined(GS_SERIAL_LOG_LEVEL_2)
			Serial.println(tmp);
#endif
			client.println(tmp);
			AwaitSMTPResponse(client, whileWaiting);
			
			sz_r = strtok(NULL, ",");
		}
		delete[] toCopy;
	}

#if defined(GS_SERIAL_LOG_LEVEL_2)
	Serial.println("DATA");
#endif
	client.println("DATA");
	if(!AwaitSMTPResponse(client, whileWaiting, "354")) {
		_error = "SMTP DATA error";
		return false;
	}
//Date goes here C: Date: Thu, 21 May 2008 05:33:29 -0700 

	tmp = F("From: <$>");
	if(_from != nullptr) 
		tmp.replace(" ", _from);
	tmp.replace("$",_emailAddress);
#if defined(GS_SERIAL_LOG_LEVEL_2)
	Serial.println(tmp);
#endif
	client.println(tmp);

	tmp = F("To: <$>");
	if(oneRecepient) {
		tmp.replace("$", to);
#if defined(GS_SERIAL_LOG_LEVEL_2)
		Serial.println(tmp);
#endif
		client.println(tmp);
	} else {
		char *rec = strtok((char*)to.c_str(), ",");
		while(rec) {
			while(*rec == ' ') ++rec;    // skip spaces after comma.
				
			tmp.replace("$", rec);
#if defined(GS_SERIAL_LOG_LEVEL_2)
			Serial.println(tmp);
#endif
			client.println(tmp);
			rec = strtok(NULL, ",");
			tmp = F("Cc: <$>");
		}
	}

	String fullpacket=SMTP_SUB+String(_subject)+"\r\n"+HTML_HEAD+String(message)+HTML_END;
	client.println(fullpacket);

	if(!AwaitSMTPResponse(client, whileWaiting, "250")) {
		_error = "Sending message error";
		return false;
	}
	client.println("QUIT");
	if(!AwaitSMTPResponse(client, whileWaiting, "221")) {
		_error = "SMTP QUIT error";
		return false;
	}
	return true;
}

ESP8266SMTPHelper SMTP;