#include <stdint.h>

class Filter {
private:
        uint8_t i;
        uint8_t buffer_size;
        int16_t sum;
        uint16_t *buffer;
public:
        Filter(uint8_t size = 16);
        uint16_t MovingAvg(int16_t new_value);
}; // class filter