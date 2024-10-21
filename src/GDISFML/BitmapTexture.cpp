#include "pch.h"


#include "BitmapTexture.h"

BitmapTexture::BitmapTexture()
{
    m_fileHeader = new BITMAPFILEHEADER;
    m_infoHeader = new BITMAPINFOHEADER;
    m_info = new BITMAPINFO;
    m_pixels = nullptr;
};
BitmapTexture::~BitmapTexture()
{
    delete[] m_pixels;
    delete m_fileHeader;
    delete m_infoHeader;
    //delete m_info;
};

bool BitmapTexture::LoadFile(const char* fileName)
{
    // Open file
    FILE* file;
    fopen_s(&file, fileName, "rb");

    if (file == nullptr) {
        return false; 
    }

    // Read data struct & grb pixels
    fseek(file, 0, SEEK_END);

    int size = ftell(file);
    uint8_t bufferHeader[54] = {};

    m_pixels = new uint8_t[size - 54];
    fseek(file, 0, SEEK_SET);
    fread(m_fileHeader, sizeof(uint8_t), 14, file);
    fread(m_infoHeader, sizeof(uint8_t), 40, file);

    m_infoHeader->biBitCount = 32;

    m_info = (BITMAPINFO*)m_infoHeader;

    fread(m_pixels, sizeof(uint8_t), size - m_fileHeader->bfOffBits, file);
    fclose(file);



    // convert grb to grba pixels
    int numPixels = (size - m_fileHeader->bfOffBits) / 3;
    uint8_t* rgbaPixels = new uint8_t[numPixels * 4]; 


    for (int i = 0; i < numPixels; ++i) {
        int bgrIndex = i * 3;  
        int bgraIndex = i * 4; 

        rgbaPixels[bgraIndex] = m_pixels[bgrIndex];       
        rgbaPixels[bgraIndex + 1] = m_pixels[bgrIndex + 1];   
        rgbaPixels[bgraIndex + 2] = m_pixels[bgrIndex + 2];   

        if (rgbaPixels[bgraIndex + 2] == 255 && rgbaPixels[bgraIndex + 1] == 0 && rgbaPixels[bgraIndex] == 255) {
            rgbaPixels[bgraIndex + 3] = 0;
        }
        else {
            rgbaPixels[bgraIndex + 3] = 255; 
        }
    }

    m_pixels = rgbaPixels;
    
    // This is need for alpha blending after (we couldn't make it one time in load of texture, we put it in each draw texture AlphaBlend)

    return true;
}