#include "BmpLoader.h"
#include <stdio.h>
#include <iostream>

BmpLoader::BmpLoader(const char* filename)
{
    FILE* file;
    if (fopen_s(&file, filename, "rb") != 0)
    {
        std::cout << "File not found" << std::endl;
        // Handle the error by returning or throwing an exception
        throw std::runtime_error("File not found");
    }

    fread(&bfh, sizeof(BITMAPFILEHEADER), 1, file);
    if (bfh.bfType != 0x4D42)
    {
        fclose(file);
        std::cout << "Not a valid bitmap" << std::endl;
        // Handle the error by returning or throwing an exception
        throw std::runtime_error("Not a valid bitmap");
    }

    fread(&bih, sizeof(BITMAPINFOHEADER), 1, file);
    if (bih.biSizeImage == 0)
        bih.biSizeImage = bih.biHeight * bih.biWidth * 3;
    textureData = new unsigned char[bih.biSizeImage];
    fseek(file, bfh.bfOffBits, SEEK_SET);
    fread(textureData, 1, bih.biSizeImage, file);
    unsigned char temp;
    for (int i = 0; i < bih.biSizeImage; i += 3)
    {
        temp = textureData[i];
        textureData[i] = textureData[i + 2];
        textureData[i + 2] = temp;
    }

    iWidth = bih.biWidth;
    iHeight = bih.biHeight;
    fclose(file);
}

BmpLoader::~BmpLoader()
{
    delete[] textureData;
}









