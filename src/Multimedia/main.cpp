#include "pch.h"
#include "GCImage.h"
#include "GCFile.h"
#include <cstdint>
#include <iostream>

int main()
{
    GCImage img;
    GCImage img2;

    img.CreateEmptyImage(1600, 1200, 32);
    img.Fill(255, 10, 75, 255);
    for (int i = 0; i < 600; i++)
    {
        int red = (455 - i * 10) / 2;
        int green = (i * 2) / 4;
        int blue = i + 5;

        // S'assure que les valeurs de couleur sont dans la plage valide
        if (red < 0) red = 0;
        if (red > 255) red = 255;
        if (green < 0) green = 0;
        if (green > 255) green = 255;
        if (blue < 0) blue = 0;
        if (blue > 255) blue = 255;

        img.DrawRect(i, i, i, i, blue, green, red, 255);
    }
    for (int i = 0; i < 500; i++)
    {

        img.DrawCircle(1300, 300, i / 2, 255, 0, 255, 255);
    }
    img.SaveBMP("images/test.bmp");
    img.InverseBMP("images/test.bmp");
    img.SaveBMP("images/copy.bmp");

    img.LoadBMP("images/copy.bmp");
    img.Premultiply();
    img.SaveBMP("images/premultiplied.bmp");
    img.LoadBMP("images/test.bmp");

    img2.LoadBMP("images/copy.bmp");
    img.BlendSTD(img2, 255);
    img.SaveBMP("images/blended_std.bmp");
    img.LoadBMP("images/test.bmp");

    img.BlendPRE(img, 255);
    img.SaveBMP("images/blended_pre.bmp");

    img.Load(&img2);
    img.SaveBMP("images/loaded.bmp");

    GCFile file2("images/test.png");
    img.SavePNG(&file2);
    file2.Close();

    return 0;
}