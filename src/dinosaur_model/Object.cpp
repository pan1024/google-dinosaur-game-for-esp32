#include<dinosaur_model/Object.h>
Object::Object(uint8_t width,uint8_t height,int index_x,int index_y,const uint16_t *data)
{
    this->width=width;
    this->height=height;
    this->index_x=index_x;
    this->index_y=index_y;
    this->data=data;
}

Object::~Object()
{
}
