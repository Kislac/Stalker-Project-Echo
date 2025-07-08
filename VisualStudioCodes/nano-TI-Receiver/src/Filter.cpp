#include <Filter.h>

//private:

Filter::Filter(uint8_t size)
{
        i = 0;
        sum = 0;
        buffer_size = size;
        buffer = new uint16_t[buffer_size];
        for( int x = 0 ; x != buffer_size; x++ ) buffer[x] = 0;
} // filter::filter(void)

uint16_t Filter::MovingAvg(int16_t value){
    //Moving avarage filter that requires only the last input to calculate the filtered value. Stores n number of previous data, and returns the filtered value
  
  int16_t   ret         = 0;

  ret = buffer[i]; //using the return variable as a temporary store for the oldest value in the dataset
  buffer[i] = value;
  i++;
  if (i > buffer_size - 1){
    i = 0; 
  }
  sum += (value- ret);
  ret = sum / buffer_size;
  return ret; 
}