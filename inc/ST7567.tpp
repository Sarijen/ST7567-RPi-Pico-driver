
template<typename Board>
void ST7567<Board>::drawString(uint8_t x, uint8_t y, font_table font, string str) {
  if (x > WIDTH || y > HEIGHT) {return;}

  size_t glyph_size = (font.width + 7) / 8 * font.height;
  uint8_t current_char = 0;
  uint8_t char_offset = font.width + 1;

  for (char c : str) {
    if (c > 32 && c < 127) {
      uint8_t char_index = c - 33;

      drawImage(
        x + (char_offset * current_char),
        y,
        font.width,
        font.height,
        span<const uint8_t>{
          &font.characters[char_index * glyph_size],
          glyph_size,
        }
      );
    }
    current_char++;
  }
}


template<typename Board>
void ST7567<Board>::drawImage(uint8_t x, uint8_t y, uint8_t width, uint8_t height, span<const uint8_t> image) {
  if (x > WIDTH || y > HEIGHT) {return;}

  uint8_t bytes_per_row = (width / 8);
  if ((width % 8) != 0) {bytes_per_row += 1;}

  for (int8_t img_x = 0; img_x < width; img_x++) {
    for (int8_t img_y = 0; img_y < height; img_y++) {

      uint16_t byte_index = (img_y * bytes_per_row) + (img_x / 8);
      uint8_t bit_index = 7 - (img_x % 8);

      if (!((image[byte_index] >> bit_index) & 1)) {
        plotPixel(img_x + x , img_y + y); 
      }
    }
  }
}


template<typename Board>
void ST7567<Board>::drawRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
  if (x > WIDTH || y > HEIGHT) {return;}

  uint8_t rect_dx = x + width;
  uint8_t rect_dy = y + height;

  for (uint8_t rect_x = x; rect_x < rect_dx; rect_x++) {

    for (uint8_t rect_y = y; rect_y < rect_dy; rect_y++) {

      if (rect_x == x || rect_y == y || rect_x == rect_dx - 1 || rect_y == rect_dy - 1) {
        plotPixel(rect_x, rect_y);
      }
    }
  }
}


template<typename Board>
void ST7567<Board>::plotPixel(uint8_t x, uint8_t y) {
  uint8_t page = y / 8; // Pages are 8-bits high
  uint16_t byte_index = (WIDTH * page) + x;
  uint8_t bit_position = (y % 8);

  framebuffer[byte_index] |= (1 << bit_position);
}


template<typename Board>
void ST7567<Board>::flip(bool horizontally, bool vertically) {
  sendCommand(horizontally ? COM_DIR_REVERSE : COM_DIR_NORMAL);
  sendCommand(vertically ? SEG_DIR_REVERSE : SEG_DIR_NORMAL);
}


template<typename Board>
void ST7567<Board>::setContrast(uint8_t EVvalue, uint8_t RegRatioValue) {
  if (EVvalue > MAX_EV_VALUE) {EVvalue = MAX_EV_VALUE;}
  if (RegRatioValue > MAX_RR_VALUE) {RegRatioValue = MAX_RR_VALUE;}

  sendCommand(REGULATION_RATIO | RegRatioValue);
  sendCommand(EV_ADJUST);
  sendCommand(EV_SET | EVvalue);
};


template<typename Board>
void ST7567<Board>::sendFramebuffer() {
  uint16_t current_page_index;
  uint16_t byte;

  for (uint8_t current_page = 0; current_page < PAGE_COUNT; current_page++) {
    sendCommand(SET_PAGE_ADDR | current_page);
    sendCommand(SET_COL_ADDR_HIGH); 
    sendCommand(SET_COL_ADDR_LOW);

    current_page_index = current_page * WIDTH;

    for (byte = current_page_index;
         byte < current_page_index + WIDTH; byte++) {
      sendPixelData(framebuffer[byte]);
    }
  }
}


template<typename Board>
void ST7567<Board>::clearFramebuffer() {
  framebuffer.fill(0);
}


template<typename Board>
void ST7567<Board>::hardwareReset() {
  Board::rst(GPIO_LOW);
  Board::delay_us(5);
  Board::rst(GPIO_HIGH);
  Board::delay_us(5);
}


template<typename Board>
void ST7567<Board>::sendCommand(uint8_t command) {
  Board::cs(GPIO_LOW);
  Board::spi_write(command);
  Board::cs(GPIO_HIGH);
}


template<typename Board>
void ST7567<Board>::sendPixelData(uint8_t data) {
  Board::cs(GPIO_LOW);
  Board::dc(GPIO_HIGH);

  Board::spi_write(data);

  Board::dc(GPIO_LOW);
  Board::cs(GPIO_HIGH);
}


template<typename Board>
void ST7567<Board>::init() {
  hardwareReset();

  sendCommand(DISPLAY_OFF);
  sendCommand(BIAS_SELECT);
  sendCommand(
    POWER_CONTROL
    | Booster_ON
    | Regulator_ON
    | Follower_ON
  );
  Board::delay_us(50);

  constexpr uint8_t defaultEVvalue = 18;
  constexpr uint8_t defaultRRvalue = 5;
  setContrast(defaultEVvalue, defaultRRvalue);

  sendCommand(SET_START_LINE);
  sendCommand(DISPLAY_ON);

  clearFramebuffer();
}
