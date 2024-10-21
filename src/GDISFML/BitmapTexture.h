#pragma once
#include <Windows.h>
#include <stdint.h>

class BitmapTexture
{
public:
	BitmapTexture();
	~BitmapTexture();

	bool LoadFile(const char* fileName);

	BITMAPFILEHEADER* m_fileHeader;
	BITMAPINFOHEADER* m_infoHeader;
	BITMAPINFO* m_info;
	uint8_t* m_pixels;
};