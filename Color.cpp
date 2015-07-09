#include "Color.h"

#include <iostream>

ColorRGB::ColorRGB(unsigned char red, unsigned char green, unsigned char blue, double brightn)
{
    bright = brightn;
    r = (unsigned char)((double)red * bright);
    g = (unsigned char)((double)green * bright);
    b = (unsigned char)((double)blue * bright);

    //std::cout << "ColorRGB() < color:" << (int)r << "," << (int)g << "," << (int)b << std::endl;
}

ColorHSV ColorRGB::getColorHSV()
{
    ColorHSV hsv;
    unsigned char rgb_min = MIN3(r, g, b);
    unsigned char rgb_max = MAX3(r, g, b);

    hsv.v = rgb_max;
    if(hsv.v == 0)
    {
        hsv.h = hsv.s = 0;
        return hsv;
    }

    hsv.s = 255*long(rgb_max - rgb_min)/hsv.v;
    if(hsv.s == 0)
    {
        hsv.h = 0;
        return hsv;
    }

    if(rgb_max == r)
    {
        hsv.h = 0 + 43*(g - b)/(rgb_max - rgb_min);
    }
    else if(rgb_max == g)
    {
        hsv.h = 85 + 43*(b - r)/(rgb_max - rgb_min);
    }
    else
    {
        hsv.h = 174 + 43*(r - g)/(rgb_max - rgb_min);
    }

    return hsv;
}

void ColorRGB::incBrightness(float inc)
{
    if(bright == 1.0)
    {
        bright = 0.0;
    }
    else
        bright += inc;

    r = (unsigned char)((double)r * bright);
    g = (unsigned char)((double)g * bright);
    b = (unsigned char)((double)b * bright);
}

ColorHSV::ColorHSV(unsigned char hue, unsigned char sat, unsigned char val)
{
    h = hue;
    s = sat;
    v = val;
}

ColorRGB ColorHSV::getColorRGB()
{
    ColorRGB rgb;
    unsigned char region, remainder, p, q, t;

    if (s == 0)
    {
        rgb.r = v;
        rgb.g = v;
        rgb.b = v;
        return rgb;
    }

    region = h / 43;
    remainder = (h - (region * 43)) * 6; 

    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * remainder) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0:
            rgb.r = v; rgb.g = t; rgb.b = p;
            break;
        case 1:
            rgb.r = q; rgb.g = v; rgb.b = p;
            break;
        case 2:
            rgb.r = p; rgb.g = v; rgb.b = t;
            break;
        case 3:
            rgb.r = p; rgb.g = q; rgb.b = v;
            break;
        case 4:
            rgb.r = t; rgb.g = p; rgb.b = v;
            break;
        default:
            rgb.r = v; rgb.g = p; rgb.b = q;
            break;
    }

    return rgb;
}
