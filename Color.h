#ifndef COLOR_H
#define COLOR_H

#define MIN3(x,y,z) ((y) <= z ? \
						((x) <= (y) ? (x) : (y)) \
						: \
						((x) <= (z) ? (x) : (z)))

#define MAX3(x,y,z) ((y) >= z ? \
						((x) >= (y) ? (x) : (y)) \
						: \
						((x) >= (z) ? (x) : (z)))

class ColorHSV;

class ColorRGB 
{
public:
	ColorRGB() : r(0), g(0), b(0), bright(1.0) 
	{
		
	}

	ColorRGB(unsigned char red, unsigned char green, unsigned char blue, double brightn = 1.0);

	unsigned char r,g,b;
	double bright;
	ColorHSV getColorHSV();
	void incBrightness(float);
};

class ColorHSV
{
	public:
	ColorHSV() : h(0), s(0), v(0) { }

	ColorHSV(unsigned char, unsigned char, unsigned char);

	unsigned char h,s,v;
	ColorRGB getColorRGB();
};

#endif
