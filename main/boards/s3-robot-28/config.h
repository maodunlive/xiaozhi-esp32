#ifndef _BOARD_CONFIG_H_
#define _BOARD_CONFIG_H_

#include <driver/gpio.h>

// ===================================================================
// S3-Robot-28 : ESP32-S3-N16R8 + 2.8" ST7789(320x240) + INMP441 + MAX98357A
// 引脚与现有 MicroPython 仪表盘完全一致，刷机不需要动任何一根线
// ===================================================================

// ---------- 音频：纯 I2S，无编解码芯片 ----------
#define AUDIO_INPUT_SAMPLE_RATE  16000
#define AUDIO_OUTPUT_SAMPLE_RATE 24000
#define AUDIO_I2S_METHOD_SIMPLEX

// INMP441：L/R 接 GND（左声道），VDD 3.3V
#define AUDIO_I2S_MIC_GPIO_WS   GPIO_NUM_5   // 麦克风 WS / LRC
#define AUDIO_I2S_MIC_GPIO_SCK  GPIO_NUM_4   // 麦克风 SCK / BCLK
#define AUDIO_I2S_MIC_GPIO_DIN  GPIO_NUM_6   // 麦克风 SD / DOUT

// MAX98357A：SD 悬空（不能接 GND），GAIN 悬空 = 9dB
#define AUDIO_I2S_SPK_GPIO_DOUT GPIO_NUM_17  // 功放 DIN
#define AUDIO_I2S_SPK_GPIO_BCLK GPIO_NUM_15  // 功放 BCLK
#define AUDIO_I2S_SPK_GPIO_LRCK GPIO_NUM_16  // 功放 LRC

// ---------- 按键与灯 ----------
#define BOOT_BUTTON_GPIO        GPIO_NUM_0
#define BUILTIN_LED_GPIO        GPIO_NUM_48
#define TOUCH_BUTTON_GPIO       GPIO_NUM_NC
#define VOLUME_UP_BUTTON_GPIO   GPIO_NUM_NC
#define VOLUME_DOWN_BUTTON_GPIO GPIO_NUM_NC

// ---------- 屏幕：2.8" ST7789 320x240，7 针模块（背光直连 VCC）----------
#define DISPLAY_BACKLIGHT_PIN GPIO_NUM_NC
#define DISPLAY_MOSI_PIN      GPIO_NUM_11
#define DISPLAY_CLK_PIN       GPIO_NUM_12
#define DISPLAY_DC_PIN        GPIO_NUM_9
#define DISPLAY_RST_PIN       GPIO_NUM_13
#define DISPLAY_CS_PIN        GPIO_NUM_10

#define LCD_TYPE_ST7789_SERIAL
#define DISPLAY_WIDTH   320
#define DISPLAY_HEIGHT  240
#define DISPLAY_MIRROR_X false
#define DISPLAY_MIRROR_Y true      // MicroPython 版是「倒置行序」，对应镜像 Y
#define DISPLAY_SWAP_XY true       // 横屏
#define DISPLAY_INVERT_COLOR    false   // 实测这块屏不需要反色
#define DISPLAY_RGB_ORDER  LCD_RGB_ELEMENT_ORDER_RGB   // MADCTL 的 BGR 位要清掉
#define DISPLAY_OFFSET_X  0
#define DISPLAY_OFFSET_Y  0
#define DISPLAY_BACKLIGHT_OUTPUT_INVERT false
#define DISPLAY_SPI_MODE 0

// ---------- 预留 ----------
// GPIO18 给 MCP 点灯演示占着；以后接舵机时改成 GPIO_NUM_NC 并回收
#define LAMP_GPIO GPIO_NUM_18

#endif // _BOARD_CONFIG_H_
