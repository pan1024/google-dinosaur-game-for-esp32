#include<Arduino.h>
class Object
{
private:
    /* data */
  
public:
    uint8_t width=0;
    uint8_t height=0;
    int index_x=0;
    int index_y=0;
    const uint16_t *data;
    Object(uint8_t width,uint8_t height,int index_x,int index_y,const uint16_t *data);
    ~Object();
};

