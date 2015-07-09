#include "../LPD8806.h"

#include <iostream>
#include <cstdlib>

#define N_LEDS	64

#define SWITCH_PIN 4

int main()
{
	usleep(2000000);

	LPD8806 strip(N_LEDS);

	gnublin_gpio gpio;
	gpio.pinMode(SWITCH_PIN,INPUT);

	strip.allOff();

	ColorRGB RGBpurple(160,32,240);
	ColorRGB RGByellow(255,255,0);
	ColorRGB RGBgreen(50,205,50);

	while(gpio.digitalRead(4))
	{
		strip.anim_SineWave(RGBgreen,4);
		strip.update();
		usleep(120000);
	}
	while(!(gpio.digitalRead(4)));
	
	while(gpio.digitalRead(4))
	{
		strip.anim_Stripe(RGBpurple,RGByellow,4);
		strip.update();
		usleep(40000);
	}
	while(!(gpio.digitalRead(4)));

	for(int j=0; j<4; j++)
	{
		int strobeAt = rand() % 50 + 50;
		int strobeN = rand() % 10 + 3;

		for(int i=0; i<100; i++)
		{
			strip.anim_PulseSaw(RGBgreen);
			strip.update();
			usleep(15000);

			if(i == strobeAt)
			{
				for (int n = 0; n < strobeN; n++)
				{
					strip.fill(ColorRGB(255,255,255));
					strip.update();
					usleep(30000);
					strip.fillOff();
					strip.update();
					usleep(30000);
				}
			}
		}
	}

	while(gpio.digitalRead(4))
	{
		strip.anim_PulseSaw(RGBpurple);
		strip.update();
		usleep(15000);
	}
	while(!(gpio.digitalRead(4)));

	while(gpio.digitalRead(4))
	{
		strip.fill(ColorRGB(255,255,255));
		strip.update();
	}
	while(!(gpio.digitalRead(4)));

	for(int j=0; j<6; j++)
	{
		for (int i = 0; i < 32; i++)
		{
			strip.anim_SpeckleSingle(RGBpurple,RGBgreen,5);
			strip.update();
			usleep(50000);
		}
	}

	for(int j=0; j<20; j++)
	{
		strip.fill(ColorRGB(255,255,255));
		strip.update();
		usleep(30000);
		strip.fillOff();
		strip.update();
		usleep(30000);
	}

	for(int j=0; j<6; j++)
	{
		for (int i = 0; i < 32; i++)
		{
			strip.anim_SpeckleSingle(RGBpurple,RGBgreen,5);
			strip.update();
			usleep(50000);
		}
	}

	for(int j=0; j<20; j++)
	{
		strip.fill(ColorRGB(255,255,255));
		strip.update();
		usleep(30000);
		strip.fillOff();
		strip.update();
		usleep(30000);
	}

	while(gpio.digitalRead(4))
	{
		strip.anim_SpeckleDouble(RGBpurple,RGBgreen,RGByellow,20);
		strip.update();
		usleep(30000);
	}
	while(!(gpio.digitalRead(4)));

	strip.allOff();
	return 0;
}
