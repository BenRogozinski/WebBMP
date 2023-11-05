#include "Arduino.h"
#include <WebBMP.h>

// Converts 8 bit RGB to 16 bit RGB565 format
uint16_t color565(uint8_t red, uint8_t green, uint8_t blue) {
  return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | (blue >> 3);
}

// Convert the http data stream to a bitmap in RAM
uint8_t* getBMPData(Stream &stream, int &imageWidth, int &imageHeight) {
  uint8_t *imageBuffer = new uint8_t[maxImageSize];
  uint8_t header[54];
  if (stream.readBytes(header, sizeof(header)) != sizeof(header)) {
    return NULL;
  }

  if (header[0] != 'B' || header[1] != 'M') {
    return NULL;
  }

  imageWidth = header[18] + (header[19] << 8);
  imageHeight = header[22] + (header[23] << 8);

  uint32_t dataOffset = 0;
  for (int i = 3; i >= 0; i--) {
    dataOffset = (dataOffset << 8) | header[i+10];
  }

  stream.readBytes(header, (dataOffset - sizeof(header)));

  if (imageWidth > maxImageWidth || imageHeight > maxImageHeight) {
    return NULL;
  }

  int bytesPerRow = (imageWidth * 3 + 3) & ~3;
  uint8_t paddingBytes = bytesPerRow - imageWidth * 3;

  for (int row = imageHeight - 1; row >= 0; row--) {
    for (int col = 0; col < imageWidth; col++) {
      uint8_t b = stream.read();
      uint8_t g = stream.read();
      uint8_t r = stream.read();
      uint16_t color = color565(r, g, b);

      // Calculate the position in the imageBuffer
      int bufferPos = 2 * (row * imageWidth + col);
      imageBuffer[bufferPos] = color & 0xFF; // Low byte
      imageBuffer[bufferPos + 1] = color >> 8; // High byte
    }

    // Skip the padding bytes
    for (int i = 0; i < paddingBytes; i++) {
      stream.read();
    }
  }

  return imageBuffer;
}