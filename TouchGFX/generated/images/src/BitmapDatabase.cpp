// 4.14.0 0x411b8719
// Generated by imageconverter. Please, do not edit!

#include <BitmapDatabase.hpp>
#include <touchgfx/Bitmap.hpp>

extern const unsigned char image_logo2[]; // BITMAP_LOGO2_ID = 0, Size: 800x480 pixels
extern const unsigned char image_mybgblack[]; // BITMAP_MYBGBLACK_ID = 1, Size: 800x480 pixels

const touchgfx::Bitmap::BitmapData bitmap_database[] =
{
    { image_logo2, 0, 800, 480, 0, 0, 800, (uint8_t)(touchgfx::Bitmap::RGB888) >> 3, 480, (uint8_t)(touchgfx::Bitmap::RGB888) & 0x7 },
    { image_mybgblack, 0, 800, 480, 0, 0, 800, (uint8_t)(touchgfx::Bitmap::RGB888) >> 3, 480, (uint8_t)(touchgfx::Bitmap::RGB888) & 0x7 }
};

namespace BitmapDatabase
{
const touchgfx::Bitmap::BitmapData* getInstance()
{
    return bitmap_database;
}

uint16_t getInstanceSize()
{
    return (uint16_t)(sizeof(bitmap_database) / sizeof(touchgfx::Bitmap::BitmapData));
}
}