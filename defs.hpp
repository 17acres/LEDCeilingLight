#ifndef defs
#define defs

#pragma region Pins
#define TEMP_PIN A0
#define TLC_CLK_PIN D5
#define TLC_DATA_PIN D7
#define BUF_OE_PIN D1
#define SW_SENSE_PIN D2
#define ADDR_DATA D3
#pragma endregion

#define region LED Channels
#define TOP_R_CHAN 5
#define TOP_G_CHAN 9
#define TOP_B_CHAN 0
#define TOP_W_CHAN 11

#define BOT_R_CHAN 4
#define BOT_G_CHAN 8
#define BOT_B_CHAN 6
#define BOT_W_CHAN 10
#pragma endregion

#define NUM_LEDS 48
#define HALF_LEDS 24

#define DEBUG_EMAILS false

#endif