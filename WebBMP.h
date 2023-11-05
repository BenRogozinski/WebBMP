/*
  A library to convert a http bitmap stream into an array
  WebBMP © 2023 by Ben Rogozinski is licensed under CC BY-NC-SA 4.0 
*/

#ifndef WEBBMP_H
#define WEBBMP_H

/*
  These settings are dependent on the needs of your project
  The larger the maximum image size is, the more RAM it will consume

  maxImageWidth - the maximum width of your bitmaps
  maxImageHeight - the maximum height of your bitmaps
  maxImageSize - The size of your image buffer (assumes RGB565 format)
*/
const int maxImageWidth = 48;
const int maxImageHeight = 48;
const int maxImageSize = maxImageWidth * maxImageHeight * 2;

// Function declarations
uint16_t color565(uint8_t red, uint8_t green, uint8_t blue);
uint8_t* getBMPData(Stream &stream, int &imageWidth, int &imageHeight);

#endif