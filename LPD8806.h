#ifndef LPD8806_H
#define LPD8806_H

#include <stdint.h>
#include "gnublin.h"
#include "Color.h"

#define R	0
#define G	1
#define B	2

#define BOARD		RASPBERRY_PI
#define SPI_SPEED	32000000
#define CHIP_SELECT	0
#define SPI_LSB		0

#define PI 3.14159265

class LPD8806
{
public:
	LPD8806();									//default constructor
	LPD8806(const int&);						//no. of leds argument construnctor
	LPD8806(const int&, const int&);			//no. of leds, channel order argument construnctor
	~LPD8806();									//destructor

	bool update();
	bool allOff();
	bool fill(const ColorRGB&, int =0, int =0);
	bool fillRGB(uint8_t, uint8_t, uint8_t, int =0, int =0);
	bool fillHSV(uint8_t, uint8_t, uint8_t, int =0, int =0);
	bool fillHue(uint8_t, int =0, int =0);
	bool fillOff(int =0, int =0);
	void setRGB(int, uint8_t, uint8_t, uint8_t);
	void setHSV(int, uint8_t, uint8_t, uint8_t);
	void setHue(int, uint8_t);
	void setOff(int);
	ColorRGB wheelColor(int);

	void anim_Rainbow(int =0, int =0);
	void anim_RainbowCycle(int =0, int =0);
	void anim_Stripe(ColorRGB, ColorRGB, int, int&, int =0, int =0);
	void anim_StripeReverse(ColorRGB, ColorRGB, int, int&, int =0, int =0);
	void anim_Line(ColorRGB, ColorRGB, int, int =0, int =0);
	void anim_PulseSaw(ColorRGB, int =0, int =0);
	void anim_PulseSine(ColorRGB, int =0, int =0);
	void anim_ColorDrift(int, int, int =0, int =0);
	void anim_SpeckleSingle(ColorRGB, ColorRGB, int, int =0, int =0);
	void anim_SpeckleDouble(ColorRGB, ColorRGB, ColorRGB, int, int =0, int =0);
	void anim_ColorWipe(ColorRGB, int =0, int =0);
	void anim_ColorChase(ColorRGB, int =0, int =0);
	void anim_ChaseMelting(ColorRGB, int, int =0, int =0);
	void anim_WheelCycle(const int&,const int&,const int&, int =0, int =0);
	void anim_LarsonScanner(ColorRGB, int =2, float =0.75, int=0, int =0);
	void anim_LarsonRainbow(ColorRGB, int =2, float =0.75, int=0, int =0);
	void anim_SineWave(ColorRGB, int, int=0, int =0);

	void anim_StepThird(ColorRGB);
	void anim_RandQuater(ColorRGB, ColorRGB, int =0, int =0);

	void anim_BlockFill(ColorRGB, ColorRGB, int, int=0, int=0);

	bool strobe(float, float);
	void setPixel(int,const ColorRGB&);

	// bool setNLeds(const int&);					//set the number of leds in strip
	// bool setChOrder(const int&);				//set the channel order of the strip

	// bool getNLeds(const int&);					//get the number of leds in strip
	// bool getChOrder(const int&);				//get the channel order of the strip

	static const uint8_t CHANNEL_ORDER[3][3];

private:
	void setUp();

	const uint8_t		*chOrder_;
	uint8_t 	**buffer_;
	uint8_t		*zeros_;
	uint8_t		*gamma_;
	int 		nLeds_;
	int 		lastIndex_;
	float		masterBrightness_;
	gnublin_spi *spi;
	int			rainbowStep_, rainbowCycleStep_, larsonStep_, wipeStep_, chaseStep_, waveStep_;
	int			larsonDir_, larsonLast_; 

	int			thirdStep_, stripeStep_, sineDir_, lineStep_, wheelStep_, driftStep_, driftDir_;
	int			blockStep_, meltStep_;
	float		sawStep_, sineStep_;
};
	
#endif
