#include "Image.h"

Image::Image() {
  code = 0; 
  width = 0;
  height = 0;
  offset = 0;
  vOffset = 0;
  fileName = "";
}
Image::Image(int code) {
  code = code;
  switch (code) {
    case 1:
      //clear
      height = 48;
      width = 52;
      offset = 26;
      vOffset = 28;
	    fileName = "clear";
      break;
    case 2:
      //fewClouds
      height = 48;
      width = 72;
      offset = 36;
      vOffset = 28;
      fileName = "fewClouds";
      break;
    case 3:
      //scatteredClouds
      height = 48;
      width = 72;
      offset = 36;
      vOffset = 28;
      fileName = "scatteredClouds";
      break;
    case 4: 
      //brokenClouds
      height = 48;
      width = 72;
      offset = 36;
      vOffset = 28;
      fileName = "brokenClouds";
      break;
    case 9: 
      //showerRain
      height = 52;
      width = 64;
      offset = 32;
      vOffset = 30;
      fileName = "showerRain";
      break;
    case 10:
      //rain
      height = 48;
      width = 64;
      offset = 32;
      vOffset = 28;
      fileName = "rain";
      break;
    case 11: 
      //thunderstorm
      height = 58;
      width = 72;
      offset = 36;
      vOffset = 33;
      fileName = "thunderstorm";
      break;
    case 13: 
      //snow
      height = 48;
      width = 52;
      offset = 26;
      vOffset = 28;
      fileName = "snow";
      break;
    case 50:
      //mist
      height = 48;
      width = 64;
      offset = 32;
      vOffset = 28;
      fileName = "mist";
      break;
  }//switch

  //return image;
}
int Image::getWidth() {
  return width;
}
int Image::getHeight() {
  return height;
}
int Image::getOffset() {
  return offset;
}
int Image::getVOffset() {
  return vOffset;
}
String Image::getFileName() {
  return fileName;
}
