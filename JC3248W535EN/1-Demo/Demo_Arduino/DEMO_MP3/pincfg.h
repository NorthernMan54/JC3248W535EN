#ifndef _PINCFG_H_
#define _PINCFG_H_

#define TFT_BLK_ON_LEVEL 1
#define TFT_BLK 1
#define TFT_RST -1
#define TFT_CS 45
#define TFT_SCK 47
#define TFT_SDA0 21
#define TFT_SDA1 48
#define TFT_SDA2 40
#define TFT_SDA3 39
#define TFT_TE 38

#define TOUCH_PIN_NUM_I2C_SCL 8
#define TOUCH_PIN_NUM_I2C_SDA 4
#define TOUCH_PIN_NUM_INT 3
#define TOUCH_PIN_NUM_RST -1

#define SD_MMC_D0 13
#define SD_MMC_CLK 12
#define SD_MMC_CMD 11

#define AUDIO_I2S_PORT I2S_NUM_0
#define AUDIO_I2S_MCK_IO -1 // MCK
#define AUDIO_I2S_BCK_IO 42 // BCK
#define AUDIO_I2S_LRCK_IO 2   // LCK
#define AUDIO_I2S_DO_IO 41  // DIN

#define BAT_ADC_PIN 5

#endif