#include "types.h"

uint8_t  write_column  (uint8_t *buf, uint32_t size, uint32_t col);
uint8_t  write_row     (uint8_t *buf, uint32_t size, uint32_t row);
uint8_t  write_pixel   (uint8_t  rgb, uint32_t col,  uint8_t row);