# 1 "n:\\classmate\\LEDCeilingLight\\Adafruit_TLC59711_Dan.cpp"
# 1 "n:\\classmate\\LEDCeilingLight\\Adafruit_TLC59711_Dan.cpp"
/***************************************************

  This is a library for our Adafruit 12-channel PWM/LED driver



  Pick one up today in the adafruit shop!

  ------> http://www.adafruit.com/products/1455



  Two SPI Pins are required to send data: clock and data pin.



  Adafruit invests time and resources providing this open source code,

  please support Adafruit and open-source hardware by purchasing

  products from Adafruit!



  Written by Limor Fried/Ladyada for Adafruit Industries.

  BSD license, all text above must be included in any redistribution

 ****************************************************/
# 17 "n:\\classmate\\LEDCeilingLight\\Adafruit_TLC59711_Dan.cpp"
//Modified by Dan for adjustable speed

# 20 "n:\\classmate\\LEDCeilingLight\\Adafruit_TLC59711_Dan.cpp" 2
# 21 "n:\\classmate\\LEDCeilingLight\\Adafruit_TLC59711_Dan.cpp" 2

SPISettings SPI_SETTINGS(500000, 1, SPI_MODE0);

/*!

 *  @brief  Instantiates a new Adafruit_TLC59711 class

 *  @param  n

 *          number of connected drivers

 *  @param  c

 *          clock pin

 *  @param  d

 *          data pin

 */
# 33 "n:\\classmate\\LEDCeilingLight\\Adafruit_TLC59711_Dan.cpp"
Adafruit_TLC59711::Adafruit_TLC59711(uint8_t n, uint8_t c, uint8_t d) {
  numdrivers = n;
  _clk = c;
  _dat = d;

  BCr = BCg = BCb = 0x7F; // default 100% brigthness

  pwmbuffer = (uint16_t *)({ static const char mem_debug_file[] __attribute__((section( "\".irom.text." "Adafruit_TLC59711_Dan.cpp" "." "40" "." "35" "\""))) __attribute__((aligned(4))) = "Adafruit_TLC59711_Dan.cpp"; pvPortCalloc(2, 12 * n, mem_debug_file, 40); });
}

/*!

 *  @brief  Instantiates a new Adafruit_TLC59711 class

 *  @param  n

 *          number of connected drivers

 *  @param  c

 *          clock pin

 *  @param  d

 *          data pin

 *  @param  r

 *          data clock speed

 */
# 54 "n:\\classmate\\LEDCeilingLight\\Adafruit_TLC59711_Dan.cpp"
Adafruit_TLC59711::Adafruit_TLC59711(uint8_t n, uint8_t c, uint8_t d, uint32_t r) {
  numdrivers = n;
  _clk = c;
  _dat = d;
  SPI_SETTINGS._clock=r;
  BCr = BCg = BCb = 0x7F; // default 100% brigthness

  pwmbuffer = (uint16_t *)({ static const char mem_debug_file[] __attribute__((section( "\".irom.text." "Adafruit_TLC59711_Dan.cpp" "." "61" "." "36" "\""))) __attribute__((aligned(4))) = "Adafruit_TLC59711_Dan.cpp"; pvPortCalloc(2, 12 * n, mem_debug_file, 61); });
}

/*!

 *  @brief  Instantiates a new Adafruit_TLC59711 class using provided SPI

 *  @param  n

 *          number of connected drivers

 *  @param  *theSPI

 *          spi object

 */
# 71 "n:\\classmate\\LEDCeilingLight\\Adafruit_TLC59711_Dan.cpp"
Adafruit_TLC59711::Adafruit_TLC59711(uint8_t n, SPIClass *theSPI) {
  numdrivers = n;
  _clk = -1;
  _dat = -1;
  _spi = theSPI;

  BCr = BCg = BCb = 0x7F; // default 100% brigthness

  pwmbuffer = (uint16_t *)({ static const char mem_debug_file[] __attribute__((section( "\".irom.text." "Adafruit_TLC59711_Dan.cpp" "." "79" "." "37" "\""))) __attribute__((aligned(4))) = "Adafruit_TLC59711_Dan.cpp"; pvPortCalloc(2, 12 * n, mem_debug_file, 79); });
}

/*!

 *  @brief  Write data throught SPI at MSB

 *  @param  d

 *          data

 */
# 87 "n:\\classmate\\LEDCeilingLight\\Adafruit_TLC59711_Dan.cpp"
void Adafruit_TLC59711::spiwriteMSB(uint8_t d) {
  if (_clk >= 0) {
    uint32_t b = 0x80;
    //  b <<= (bits-1);
    for (; b != 0; b >>= 1) {
      digitalWrite(_clk, 0x0);
      if (d & b)
        digitalWrite(_dat, 0x1);
      else
        digitalWrite(_dat, 0x0);
      digitalWrite(_clk, 0x1);
    }
  } else {
    _spi->transfer(d);
  }
}

/*!

 *  @brief  Writes PWM buffer to board

 */
# 107 "n:\\classmate\\LEDCeilingLight\\Adafruit_TLC59711_Dan.cpp"
void Adafruit_TLC59711::write() {
  if (_clk < 0) {
    _spi->beginTransaction(SPI_SETTINGS);
  }

  uint32_t command;

  // Magic word for write
  command = 0x25;

  command <<= 5;
  // OUTTMG = 1, EXTGCK = 0, TMGRST = 1, DSPRPT = 1, BLANK = 0 -> 0x16
  command |= 0x16;

  command <<= 7;
  command |= BCr;

  command <<= 7;
  command |= BCg;

  command <<= 7;
  command |= BCb;

  (__extension__({uint32_t state; __asm__ __volatile__("rsil %0," "15" : "=a" (state) :: "memory"); state;}));
  for (uint8_t n = 0; n < numdrivers; n++) {
    spiwriteMSB(command >> 24);
    spiwriteMSB(command >> 16);
    spiwriteMSB(command >> 8);
    spiwriteMSB(command);

    // 12 channels per TLC59711
    for (int8_t c = 11; c >= 0; c--) {
      // 16 bits per channel, send MSB first
      spiwriteMSB(pwmbuffer[n * 12 + c] >> 8);
      spiwriteMSB(pwmbuffer[n * 12 + c]);
    }
  }

  if (_clk >= 0)
    delayMicroseconds(200);
  else
    delayMicroseconds(2);
    _spi->endTransaction();

  (__extension__({uint32_t state; __asm__ __volatile__("rsil %0," "0" : "=a" (state) :: "memory"); state;}));
}

/*!

 *  @brief  Set PWM value on selected channel

 *  @param  chan

 *          one from 12 channel (per driver) so there is 12 * number of drivers

 *  @param  pwm

 *          pwm value

 */
# 161 "n:\\classmate\\LEDCeilingLight\\Adafruit_TLC59711_Dan.cpp"
void Adafruit_TLC59711::setPWM(uint8_t chan, uint16_t pwm) {
  if (chan > 12 * numdrivers)
    return;
  pwmbuffer[chan] = pwm;
}

/*!

 *  @brief  Set RGB value for selected LED

 *  @param  lednum

 *          selected LED number that for which value will be set

 *  @param  r

 *          red value

 *  @param g

 *          green value

 *  @param b

 *          blue value

 */
# 178 "n:\\classmate\\LEDCeilingLight\\Adafruit_TLC59711_Dan.cpp"
void Adafruit_TLC59711::setLED(uint8_t lednum, uint16_t r, uint16_t g,
                               uint16_t b) {
  setPWM(lednum * 3, r);
  setPWM(lednum * 3 + 1, g);
  setPWM(lednum * 3 + 2, b);
}

/*!

 *  @brief  Set the brightness of LED channels to same value

 *  @param  BC

 *          Brightness Control value

 */
# 190 "n:\\classmate\\LEDCeilingLight\\Adafruit_TLC59711_Dan.cpp"
void Adafruit_TLC59711::simpleSetBrightness(uint8_t BC) {
  if (BC > 127) {
    BC = 127; // maximum possible value since BC can only be 7 bit
  } else if (BC < 0) {
    BC = 0;
  }

  BCr = BCg = BCb = BC;
}

/*!

 *  @brief  Set the brightness of LED channels to specific value

 *  @param  bcr

 *          Brightness Control Red value

 *  @param  bcg

 *          Brightness Control Green value

 *  @param  bcb

 *          Brightness Control Blue value

 */
# 209 "n:\\classmate\\LEDCeilingLight\\Adafruit_TLC59711_Dan.cpp"
void Adafruit_TLC59711::setBrightness(uint8_t bcr, uint8_t bcg, uint8_t bcb) {
  if (bcr > 127) {
    bcr = 127; // maximum possible value since BC can only be 7 bit
  } else if (bcr < 0) {
    bcr = 0;
  }

  BCr = bcr;

  if (bcg > 127) {
    bcg = 127; // maximum possible value since BC can only be 7 bit
  } else if (bcg < 0) {
    bcg = 0;
  }

  BCg = bcg;

  if (bcb > 127) {
    bcb = 127; // maximum possible value since BC can only be 7 bit
  } else if (bcb < 0) {
    bcb = 0;
  }

  BCb = bcb;
}

/*!

 *  @brief  Begins SPI connection if there is not empty PWM buffer

 *  @return If successful returns true, otherwise false

 */
# 239 "n:\\classmate\\LEDCeilingLight\\Adafruit_TLC59711_Dan.cpp"
boolean Adafruit_TLC59711::begin() {
  if (!pwmbuffer)
    return false;

  if (_clk >= 0) {
    pinMode(_clk, 0x01);
    pinMode(_dat, 0x01);
  } else {
    _spi->begin();
  }
  return true;
}
# 1 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino"
# 2 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino" 2
# 3 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino" 2
# 4 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino" 2

# 6 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino" 2

# 8 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino" 2
# 9 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino" 2
# 10 "n:\\classmate\\LEDCeilingLight\\CeilingLight.ino" 2
HvLeds *hvLeds;
AddrLeds *addrLeds;
Animations::AnimationManager *animMan;

void setup()
{
    Serial.begin(115200);//Do this before RX pin is repurposed for dma LEDs (i2s)
    hvLeds = HvLeds::getInstance();
    addrLeds = AddrLeds::getInstance();
    animMan = Animations::AnimationManager::getInstance();

    //gdbstub_init();

    EEPROM.begin(sizeof(unsigned int));
    unsigned int numSeconds;
    EEPROM.get(0, numSeconds);
    Serial.print("On Mode Second Count ");
    Serial.println(numSeconds);

    pinMode(2, 0x01);
    digitalWrite(2, 0x0);
    digitalWrite(2, 0x1);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    TimeManager::setup();
    WebServer::setup();
    EmailSender::setup();
    EmailSender::sendEmail("Ceiling light started - On Mode Second Count: "+String(numSeconds));
}

void loop()
{
    EmailSender::runSpooler(Utils::doUpdates);
}
