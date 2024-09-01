#ifndef IMAGE_H
#define IMAGE_H
#include <Arduino.h>
class Image {
  public:
    Image();
    Image(int code);
    int getWidth();
    int getHeight();
    int getOffset();
    int getVOffset();
    String getFileName();
  private:
    //static Image* instance;
    int code;
    int width;
    int height;
    int offset;
    int vOffset;
    String fileName;
};
#endif