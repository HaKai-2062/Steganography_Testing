#include <iostream>

#include "ImageHandler.h"
#include <cstdio>
#include <memory>

// bitmap file header (14 bytes)
struct BitmapFileHeader
{
	unsigned char   bitmapType[2];     // 2 bytes
	int             fileSize;          // 4 bytes
	short           reserved1;         // 2 bytes
	short           reserved2;         // 2 bytes
	unsigned int    offsetBits;        // 4 bytes
} bfh;

// Bitmap image header (40 bytes)
struct BitmapImageHeader
{
	unsigned int    sizeHeader;         // 4 bytes
	unsigned int    width;              // 4 bytes
	unsigned int    height;             // 4 bytes
	short int       planes;             // 2 bytes
	short int       bitCount;           // 2 bytes
	unsigned int    compression;        // 4 bytes
	unsigned int    imageSize;          // 4 bytes
	unsigned int    ppmX;               // 4 bytes
	unsigned int    ppmY;               // 4 bytes
	unsigned int    clrUsed;            // 4 bytes
	unsigned int    clrImportant;       // 4 bytes
} bih;

void Bitmap::SaveImage()
{
	FILE* image;
	int imageSize = m_Width * m_Height;

	// 3colors/pixel and 54 hader size
	int fileSize = imageSize * 3 + 54;
	
	// pixels per meter https://www.wikiwand.com/en/Dots_per_inch
	int pixelPerMeter = static_cast<int>(m_Dpi * 39.375);

	memcpy_s(&bfh.bitmapType, 2, "BM", 2);
	bfh.fileSize = fileSize;
	bfh.reserved1 = 0;
	bfh.reserved2 = 0;
	bfh.offsetBits = 0;

	bih.sizeHeader = sizeof(bih);
	bih.width = m_Width;
	bih.height = m_Height;
	bih.planes = 1;
	bih.bitCount = 24;
	bih.compression = 0;
	bih.imageSize = fileSize;
	bih.ppmX = pixelPerMeter;
	bih.ppmY = pixelPerMeter;
	bih.clrUsed = 0;
	bih.clrImportant = 0;

	fopen_s(&image, m_FileName, "wb");
	if (!image)
	{
		std::cout << "Error creating the image!" << std::endl;
		system("pause");
		return;
	}
	fwrite(&bfh, 1, 14, image);
	fwrite(&bih, 1, sizeof(bih), image);

	for (int i = 0; i < imageSize; i++)
	{
		rgbData BGR = m_PixelData[i];
		uint8_t color[3] = {
			BGR.b,BGR.g, BGR.r
		};

		fwrite(color, 1, sizeof(color), image);
	}
	fclose(image);
}

void Bitmap::DrawImage()
{
	m_PixelData = new rgbData[m_Width * m_Height];

	srand(static_cast<unsigned int>(time(NULL)));

	for (int x = 0; x < m_Width; x++)
	{
		for (int y = 0; y < m_Height; y++)
		{
			int index = y * m_Width + x;
			uint8_t color = rand() % 256;
			m_PixelData[index].r = color;
			m_PixelData[index].g = color;
			m_PixelData[index].b = color;
		}
	}
}