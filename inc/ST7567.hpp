#pragma once

#include <array>
#include <span>
#include <string>

using std::span;
using std::string;


typedef struct {
  const uint8_t width;
  const uint8_t height;
  const uint8_t* characters;
} font_table;

struct Board {
  static void cs(bool enable);
  static void dc(bool mode);
  static void rst(bool reset);
  static void spi_write(const uint8_t data);
  static void delay_us(const uint8_t duration);
};


template<typename Board>
class ST7567 {
  public:
    void init();
    void hardwareReset();

    void setContrast(uint8_t EVvalue, uint8_t RegRatioValue);
    void flip(bool horizontally, bool vertically);

    void sendFramebuffer();
    void clearFramebuffer();

    void plotPixel(uint8_t x, uint8_t y);
    void drawRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
    void drawFilledRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height);

    void drawImage(uint8_t x, uint8_t y, uint8_t width, uint8_t height, span<const uint8_t> image);
    void drawString(uint8_t x, uint8_t y, font_table font, string str);

  private:
    // INSTRUCTIONS
    // (Read instructions are not supported over serial communication)
    static constexpr uint8_t DISPLAY_OFF =            0b10101110;
    static constexpr uint8_t DISPLAY_ON =             DISPLAY_OFF | 0b1;
    static constexpr uint8_t SET_START_LINE =         0b01000000;

    static constexpr uint8_t SET_PAGE_ADDR =          0b10110000;
    static constexpr uint8_t SET_COL_ADDR_HIGH =      0b00010000;
    static constexpr uint8_t SET_COL_ADDR_LOW =       0b00000000;

    static constexpr uint8_t SEG_DIR_NORMAL =         0b10100000;
    static constexpr uint8_t SEG_DIR_REVERSE =        SEG_DIR_NORMAL | 0b1;
    static constexpr uint8_t INVERSE_DISPLAY_OFF =    0b10100110;
    static constexpr uint8_t INVERSE_DISPLAY_ON =     INVERSE_DISPLAY_OFF | 0b1;

    static constexpr uint8_t ALL_PIXEL_OFF =          0b10100100;
    static constexpr uint8_t ALL_PIXEL_ON =           ALL_PIXEL_OFF | 0b1;
    static constexpr uint8_t BIAS_SELECT =            0b10100011;   

    static constexpr uint8_t SOFTWARE_RESET =         0b11100010;
    static constexpr uint8_t COM_DIR_NORMAL =         0b11000000;
    static constexpr uint8_t COM_DIR_REVERSE =        COM_DIR_NORMAL | 0b1000;

    static constexpr uint8_t POWER_CONTROL =          0b00101000;
    static constexpr uint8_t Booster_ON   =           0b00000100;
    static constexpr uint8_t Regulator_ON =           0b00000010;
    static constexpr uint8_t Follower_ON  =           0b00000001;

    static constexpr uint8_t REGULATION_RATIO =       0b00100000;
    static constexpr uint8_t EV_ADJUST =              0b10000001;
    static constexpr uint8_t EV_SET =                 0b00000000;
    static constexpr uint8_t NOP =                    0b11100011;

    static constexpr bool GPIO_HIGH = 1;
    static constexpr bool GPIO_LOW = 0;

    static constexpr uint8_t WIDTH = 128;
    static constexpr uint8_t HEIGHT = 64;
    static constexpr uint8_t PAGE_COUNT = 8;

    static constexpr uint8_t MAX_EV_VALUE = 63;
    static constexpr uint8_t MAX_RR_VALUE = 7;

    std::array<uint8_t, WIDTH * HEIGHT / 8> framebuffer;

    void sendCommand(uint8_t command);
    void sendPixelData(uint8_t data);
};

#include "ST7567.tpp"
