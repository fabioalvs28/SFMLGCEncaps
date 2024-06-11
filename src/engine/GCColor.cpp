#include "pch.h"
#include "GCColor.h"

GCColor::GCColor() {
    r = 0;
    g = 0;
    b = 0;
    a = 255;
}

GCColor::GCColor(int r, int g, int b, int a = 255) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

GCColor::GCColor(const char* hexaCode) {
    int start = 0;
    if (hexaCode[0] == '#') start++;
    int size;
    for (size = 0; hexaCode[size] != '\0'; size++);
    size -= start;

    int temp[4] = { 255, 255, 255, 255 };
    int charIndex;
    for (int colorIndex = 0; colorIndex < size / 2; colorIndex++)
    {
        charIndex = start + colorIndex * 2;
        if ('0' <= hexaCode[charIndex] && hexaCode[charIndex] <= '9') temp[colorIndex] = hexaCode[charIndex] - '0';
        else if ('a' <= hexaCode[charIndex] && hexaCode[charIndex] <= 'f') temp[colorIndex] = hexaCode[charIndex] - 'a' + 10;
        else if ('A' <= hexaCode[charIndex] && hexaCode[charIndex] <= 'F') temp[colorIndex] = hexaCode[charIndex] - 'A' + 10;
        temp[colorIndex] *= 16;
        if ('0' <= hexaCode[charIndex + 1] && hexaCode[charIndex + 1] <= '9') temp[colorIndex] += hexaCode[charIndex + 1] - '0';
        else if ('a' <= hexaCode[charIndex + 1] && hexaCode[charIndex + 1] <= 'f') temp[colorIndex] += hexaCode[charIndex + 1] - 'a' + 10;
        else if ('A' <= hexaCode[charIndex + 1] && hexaCode[charIndex + 1] <= 'F') temp[colorIndex] += hexaCode[charIndex + 1] - 'A' + 10;
    }
    r = temp[0];
    g = temp[1];
    b = temp[2];
    a = temp[3];
}

GCColor::GCColor(float hue, float saturation, float light, float alpha = 1.0f) {
    if (saturation == 0) {
        r = g = b = light * 255;
        a = alpha * 255;
    }
    else
    {
        float const q = light < 0.5f ? light * (1 + saturation) : light + saturation - light * saturation;
        float const p = 2 * light - q;
        r = round(HSLToRGB(p, q, hue + 1.f / 3) * 255);
        g = round(HSLToRGB(p, q, hue) * 255);
        b = round(HSLToRGB(p, q, hue - 1.f / 3) * 255);
        a = alpha * 255;
    }
}

GCColor::~GCColor()
{
}

float GCColor::HSLToRGB(float p, float q, float t)
{
    if (t < 0)
        t += 1;
    if (t > 1)
        t -= 1;
    if ((6 * t) < 1)
        return p + (q - p) * 6 * t;
    if ((2 * t) < 1)
        return q;
    if ((3 * t) < 2)
        return (p + (q - p) * ((2.f / 3) - t) * 6);

    return p;
}