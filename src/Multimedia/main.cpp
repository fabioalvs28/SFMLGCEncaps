#include "Core/pch.h"
#include "GCImage.h"
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
    if (img.SaveBMP("images/test.bmp"))
    {
        std::cout << "IMG Success" << std::endl;
    }
    img.InverseBMP("images/test.bmp");
    img.SaveBMP("images/copy.bmp");

    img.LoadBMP("images/copy.bmp");
    if (img.Premultiply())
    {
        std::cout << "Premultiplied image successfully" << std::endl;

        if (img.SaveBMP("images/premultiplied.bmp"))
        {
            std::cout << "Saved as premultiplied.bmp" << std::endl;
        }
        else
        {
            std::cerr << "Failed to save the premultiplied image." << std::endl;
        }
    }
    else
    {
        std::cerr << "Failed to create the premultiplied image." << std::endl;
    }




    if (img.LoadBMP("images/test.bmp") && img2.LoadBMP("images/copy.bmp"))
    {
        if (img.BlendSTD(img2, 255)) {
            if (img.SaveBMP("images/blended_std.bmp")) {
                std::cout << "Blended (STD) image successfully saved as blended_std.bmp" << std::endl;
            }
            else {
                std::cerr << "Failed to save the blended (STD) image." << std::endl;
            }
        }
        else {
            std::cerr << "Blend (STD) operation failed." << std::endl;
        }

        if (img.LoadBMP("images/test.bmp") && img2.LoadBMP("images/copy.bmp")) {
            if (img.BlendPRE(img, 255)) {
                if (img.SaveBMP("images/blended_pre.bmp")) {
                    std::cout << "Blended (PRE) image successfully saved as blended_pre.bmp" << std::endl;
                }
                else {
                    std::cerr << "Failed to save the blended (PRE) image." << std::endl;
                }
            }
            else {
                std::cerr << "Blend (PRE) operation failed." << std::endl;
            }
        }
        else {
            std::cerr << "Failed to reload one or both images for PRE blend." << std::endl;
        }
    }
    else {
        std::cerr << "Failed to load one or both images." << std::endl;

    }
    return 0;
}