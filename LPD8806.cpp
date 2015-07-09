#include "LPD8806.h"

#include <cmath>
#include <iostream>
#include <cstdlib>

LPD8806::LPD8806()
{
	nLeds_ = 0;
	lastIndex_ = 0;
	buffer_ = 0;
	chOrder_ = LPD8806::CHANNEL_ORDER[2];
	masterBrightness_ = 1.0;

	setUp();
}

LPD8806::LPD8806(const int &nLeds)
{
	nLeds_ = nLeds;

	lastIndex_ = nLeds_ - 1;

	buffer_ = new uint8_t *[nLeds_];

	for(int i=0; i<nLeds_; i++)
	{
		buffer_[i] = new uint8_t[3];
	}

	chOrder_ = LPD8806::CHANNEL_ORDER[2];
	masterBrightness_ = 1.0;

	setUp();
}

LPD8806::LPD8806(const int &nLeds, const int &chOrder)
{
	nLeds_ = nLeds;

	lastIndex_ = nLeds_ - 1;

	buffer_ = new uint8_t *[nLeds_];

	for(int i=0; i<nLeds_; i++)
	{
		buffer_[i] = new uint8_t[3];
	}

	if(chOrder >= 0 && chOrder <= 2)
	{
		chOrder_ = LPD8806::CHANNEL_ORDER[chOrder];
	}
	else
	{
		chOrder_ = LPD8806::CHANNEL_ORDER[2];
	}

	masterBrightness_ = 1.0;

	setUp();
}

LPD8806::~LPD8806()
{
	delete [] buffer_;
}

bool LPD8806::update()
{
	for(int i=0; i<nLeds_; i++)
	{
		if(spi->send(buffer_[i], 3) == -1)
			return false;
	}

	if(spi->send(zeros_, 4) == -1)
			return false;

	return true;
}

bool LPD8806::allOff()
{
	fillRGB(0,0,0);
	update();
	fillRGB(0,0,0);
	update();

	return true;
}

bool LPD8806::fill(const ColorRGB &color, int start, int end)
{
	if(start < 0)
	{
		start = 0;
	}

	if(end == 0 || end > lastIndex_)
	{
		end = lastIndex_;
	}

	//std::cout << "LPD8806::fill() < color:" << (int)color.r << "," << (int)color.g << "," << (int)color.b << std::endl;
	for(int i=start; i<end+1; i++)
	{
		setPixel(i, color);
	}

	return true;
}

bool LPD8806::fillRGB(uint8_t r, uint8_t g, uint8_t b, int start, int end)
{
	ColorRGB color(r,g,b);

	fill(color, start, end);

	return true;
}

bool LPD8806::fillHSV(uint8_t h, uint8_t s, uint8_t v, int start, int end)
{
	ColorHSV hsv(h,s,v);
	ColorRGB rgb = hsv.getColorRGB();

	fill(rgb, start, end);

	return true;
}

bool LPD8806::fillHue(uint8_t h, int start, int end)
{
	ColorHSV hsv(h,0,0);
	ColorRGB rgb = hsv.getColorRGB();

	fill(rgb, start, end);

	return true;
}

bool LPD8806::fillOff(int start, int end)
{
	fillRGB(0,0,0,start,end);

	return true;
}

bool LPD8806::strobe(float t_on, float t_off)
{
	fillRGB(255,255,255);
	update();
	sleep(t_on);

	fillOff();
	update();
	sleep(t_off);

	return true;
}

void LPD8806::setPixel(int pixel,const ColorRGB &color)
{
	if(pixel < 0 || pixel > nLeds_)
	{
		return;
	}

	buffer_[pixel][chOrder_[0]] = gamma_[color.r];
	buffer_[pixel][chOrder_[1]] = gamma_[color.g];
	buffer_[pixel][chOrder_[2]] = gamma_[color.b];
}

void LPD8806::setRGB(int pixel, uint8_t r, uint8_t g, uint8_t b)
{
	ColorRGB rgb(r,g,b);
	setPixel(pixel, rgb);
}

void LPD8806::setHSV(int pixel, uint8_t h, uint8_t s, uint8_t v)
{
	ColorHSV hsv(h,s,v);
	setPixel(pixel, hsv.getColorRGB());
}

void LPD8806::setHue(int pixel, uint8_t h)
{
	ColorHSV hsv(h,255,255);
	setPixel(pixel, hsv.getColorRGB());
}

void LPD8806::setOff(int pixel)
{
	ColorRGB rgb(0,0,0);
	setPixel(pixel,rgb);
}

ColorRGB LPD8806::wheelColor(int wheelPos)
{
	ColorRGB color;

	if(wheelPos < 0)
		wheelPos = 0;
	if(wheelPos > 384)
		wheelPos = wheelPos % 384;

	if(wheelPos < 128)
	{
		color.r = 127 - wheelPos % 128;
		color.g = wheelPos % 128;
		color.b = 0;
	}
	else if(wheelPos < 256)
	{
		color.g = 127 - wheelPos % 128;
		color.b = wheelPos % 128;
		color.r = 0;
	}
	else
	{
		color.b = 127 - wheelPos % 128;
		color.r = wheelPos % 128;
		color.g = 0;
	}
	
	return color;
}

void LPD8806::anim_Rainbow(int start, int end)
{
	ColorRGB rgb;
	int size, wheelPos=0, hue;

	if(end == 0 || end > lastIndex_)
	{
		end = lastIndex_;
	}

	size = (end-start) + 1;

	for(int i=0; i<size; i++)
	{
		wheelPos = (i+rainbowStep_) % 384;
		rgb = wheelColor(wheelPos);
		hue = (i + larsonStep_);
		setPixel(start + i, rgb);
	}

	rainbowStep_++;
	if(rainbowStep_>384)
	{
		rainbowStep_ = 0;
	}
}

void LPD8806::anim_RainbowCycle(int start, int end)
{
	ColorRGB rgb;
	int size, wheelPos=0;

	if(end == 0 || end > lastIndex_)
	{
		end = lastIndex_;
	}

	size = (end-start) + 1;

	for(int i=0; i<size; i++)
	{
		wheelPos = (i * (384 / size) + rainbowCycleStep_) % 384;
		rgb = wheelColor(wheelPos);
		setPixel(start + i, rgb);
	}

	rainbowCycleStep_++;
	if(rainbowCycleStep_>384)
	{
		rainbowCycleStep_ = 0;
	}
}

void LPD8806::anim_Stripe(ColorRGB color1, ColorRGB color2, int space, int &step, int start, int end)
{
	int size;
	ColorRGB *curColor;

	if(step <= space)
	{
		curColor = &color1;
	}
	else
	{
		curColor = &color2;
	}

	if(end == 0 || end > lastIndex_)
	{
		end = lastIndex_;
	}

	size = (end-start) + 1;

	for(int i=0; i<size; i++)
	{
		if(((i-step)%space) == 0 && i > 0)
		{
			if(curColor == &color1)
			{
				curColor = &color2;
			}
			else
			{
				curColor = &color1;
			}
		}
		
		setPixel(start + i, *curColor);
	}

	step++;
	if(step > (space*2))
	{
		step = 1;
	}
}

void LPD8806::anim_StripeReverse(ColorRGB color1, ColorRGB color2, int space, int &step, int start, int end)
{
	int size;
	ColorRGB *curColor;

	if(step < space)
	{
		curColor = &color1;
	}
	else
	{
		curColor = &color2;
	}

	if(end == 0 || end > lastIndex_)
	{
		end = lastIndex_;
	}

	size = (end-start) + 1;

	for(int i=0; i<size; i++)
	{
		if(((i+step)%space) == 0 && i > 0)
		{
			if(curColor == &color1)
			{
				curColor = &color2;
			}
			else
			{
				curColor = &color1;
			}
		}
		
		setPixel(start + i, *curColor);
	}

	step++;
	if(step >= (space*2))
	{
		step = 0;
	}
}

void LPD8806::anim_Line(ColorRGB bg, ColorRGB line, int space, int start, int end)
{
	int size;
	ColorRGB *curColor;

	if(end == 0 || end > lastIndex_)
	{
		end = lastIndex_;
	}

	size = (end-start) + 1;

	for(int i=0; i<size; i++)
	{
		curColor = &bg;
		if(i >= lineStep_ && i < lineStep_ + space)
		{
			curColor = &line;
		}
		setPixel(start + i, *curColor);
	}

	lineStep_++;
	if(lineStep_ > nLeds_)
	{
		lineStep_ = 0;
	}
}

void LPD8806::anim_PulseSaw(ColorRGB color, int start, int end)
{
	int size;

	if(end == 0 || end > lastIndex_)
	{
		end = lastIndex_;
	}

	size = (end-start) + 1;

	for(int i=0; i<size; i++)
	{
		setPixel(start + i,ColorRGB(color.r, color.g, color.b, sawStep_));
	}

	sawStep_ += 0.01;
	if(sawStep_ > 0.99)
	{
		sawStep_ = 0;
	}
}

void LPD8806::anim_PulseSine(ColorRGB color, int start, int end)
{
	int size;

	if(end == 0 || end > lastIndex_)
	{
		end = lastIndex_;
	}

	size = (end-start) + 1;

	for(int i=0; i<size; i++)
	{
		setPixel(start + i,ColorRGB(color.r, color.g, color.b, sineStep_));
	}

	sineStep_ += 0.01*sineDir_;
	
	if(sineStep_ > 0.99 || sineStep_ < 0)
	{
		sineDir_ *= -1;
	}
}

void LPD8806::anim_ColorDrift(int colStart, int colSpan, int start, int end)
{
	int size;

	if(end == 0 || end > lastIndex_)
	{
		end = lastIndex_;
	}

	size = (end-start) + 1;

	fill(wheelColor(colStart + driftStep_));

	driftStep_ += driftDir_;
	if(driftStep_ == colSpan || driftStep_ < 0)
	{
		driftDir_ *= -1;
	}
}

void LPD8806::anim_SpeckleSingle(ColorRGB bg, ColorRGB speck, int nSpeck, int start, int end)
{
	int size;
	int speckLoc[nSpeck];
	ColorRGB *color;

	if(end == 0 || end > lastIndex_)
	{
		end = lastIndex_;
	}

	size = (end-start) + 1;

	for (int i = 0; i < nSpeck; i++)
	{
		speckLoc[i] = rand() % size;
	}

	for (int i = 0; i < size; i++)
	{
		color = &bg;

		for (int j = 0; j < nSpeck; j++)
		{
			if(i == speckLoc[j])
			{
				color = &speck;
			}
		}

		setPixel(start + i, *color);
	}
}

void LPD8806::anim_SpeckleDouble(ColorRGB bg, ColorRGB speck1, ColorRGB speck2, int nSpeck, int start, int end)
{
	int size;
	int speckLoc[nSpeck];
	ColorRGB *color;

	if(end == 0 || end > lastIndex_)
	{
		end = lastIndex_;
	}

	size = (end-start) + 1;

	for (int i = 0; i < nSpeck; i++)
	{
		speckLoc[i] = rand() % size;
	}

	for (int i = 0; i < size; i++)
	{
		color = &bg;

		for (int j = 0; j < nSpeck; j++)
		{
			if(i == speckLoc[j] && j <= nSpeck/2)
			{
				color = &speck1;
			}
			else if(i == speckLoc[j] && j > nSpeck/2)
			{
				color = &speck2;
			}
		}

		setPixel(start + i, *color);
	}
}

void LPD8806::anim_ColorWipe(ColorRGB color, int start, int end)
{
	if(end == 0 || end > lastIndex_)
	{
		end = lastIndex_;
	}

	if(wipeStep_ == 0)
	{
		fillOff();
	}

	setPixel(start + wipeStep_, color);

	wipeStep_++;
	if(start + wipeStep_ > end)
	{
		wipeStep_ = 0;
	}
}

void LPD8806::anim_ColorChase(ColorRGB color, int start, int end)
{
	if(end == 0 || end > lastIndex_)
	{
		end = lastIndex_;
	}

	if(chaseStep_ == 0)
	{
		setOff(end);
	}
	else
	{
		setOff(start + chaseStep_ -1);
	}

	setPixel(start + chaseStep_, color);

	chaseStep_++;
	if(start + chaseStep_ > end)
	{
		chaseStep_ = 0;
	}
}

void LPD8806::anim_ChaseMelting(ColorRGB color, int dir, int start, int end)
{
	if(end == 0 || end > lastIndex_)
	{
		end = lastIndex_;
	}

	if(dir == 1)	//Full to empty
	{
		fill(color, start, end-meltStep_);
		if((start + chaseStep_) == (end-meltStep_))
		{
			meltStep_++;
			if(end-meltStep_ == 0)
			{
				meltStep_ = 0;
			}
		}

		if((start + chaseStep_) > (end-meltStep_))
		{
			setOff(start + chaseStep_ -1);
		}
		else if(chaseStep_ == 0)
		{
			setOff(end);
		}

		setPixel(start+chaseStep_, color);

		chaseStep_++;
		if(start + chaseStep_ > end)
		{
			chaseStep_ = 0;
		}
	}
	else //Empty to full
	{
		fill(color, start, start+meltStep_);
		std::cout << "Fill: " << start+meltStep_ << std::endl;
		if((end - chaseStep_) == (start + meltStep_))
		{
			meltStep_++;
			std::cout << "MeltStep Inc" << std::endl;
			if((start+meltStep_) == end)
			{
				meltStep_ = 0;
				std::cout << "MeltStep Reset" << std::endl;
			}
		}

		if((end - chaseStep_) > (start + meltStep_))
		{
			setOff(end - chaseStep_ +1);
			std::cout << "SetOff: " << end - chaseStep_ +1 << std::endl;
		}

		setPixel(end-chaseStep_, color);
		std::cout << "setPixel: " << end-chaseStep_ << std::endl;

		chaseStep_++;
		if(end - chaseStep_ == start)
		{
			chaseStep_ = 0;
			std::cout << "ChaseStep reset" << std::endl;
		}
	}
}

void LPD8806::anim_WheelCycle(const int &colStart,const int &colSpan,const int &space, int start, int end)
{
	int size, wheelPos=0;
	int incPerPix = (colSpan/space);
	ColorRGB rgb;

	if(end == 0 || end > lastIndex_)
	{
		end = lastIndex_;
	}

	size = (end-start) + 1;

	for(int i=0; i< size; i++)
	{
		wheelPos = ((i * incPerPix + wheelStep_) % colSpan) + colStart;
		rgb = wheelColor(wheelPos);
		setPixel(start + i, rgb);
	}

	wheelStep_++;
	if(wheelStep_ > colSpan)
	{
		wheelStep_ = 0;
	}
}

void LPD8806::anim_LarsonScanner(ColorRGB color, int tail, float fade, int start, int end)
{
	int size, tl, tr;
	float level;

	if(end == 0 || end > lastIndex_)
	{
		end = lastIndex_;
	}

	size = end - start + 1;
	tail++;
	
	if(tail >= size / 2)
	{
		tail = (size/2) -1;
	}

	larsonLast_ = start + larsonStep_;
	setPixel(larsonLast_, color);

	tl = tail;
	if(larsonLast_ + tl > end)
	{
		tl = end -larsonLast_;
	}

	tr = tail;
	if(larsonLast_ - tr < start)
	{
		tr = larsonLast_ - start;
	}

	for(int l=1; l<tl + 1; l++)
	{
		level = ((float)(tail - l) / (float)(tail)) * fade;
		setRGB(larsonLast_ + l, color.r * level, color.g * level, color.b * level );
	}

	if(larsonLast_ + tl + 1 <= end)
	{
		setOff(larsonLast_ + tl + 1);
	}

	for(int r=0; r<tr +1; r++)
	{
		level = ((float)(tail - r) / (float)(tail)) * fade;
		setRGB(larsonLast_ - r, color.r * level, color.g * level, color.b * level );
	}

	if(larsonLast_ - tr - 1 >= start)
	{
		setOff(larsonLast_ - tr - 1);
	}

	if(start + larsonLast_ == end)
	{
		larsonDir_ = ~larsonDir_;
	}
	else if(larsonStep_ == 0)
	{
		larsonDir_ = ~larsonDir_;
	}

	larsonStep_ += larsonDir_;
}

void LPD8806::anim_LarsonRainbow(ColorRGB color, int tail, float fade, int start, int end)
{
	int size, hue;

	if(end == 0 || end > lastIndex_)
	{
		end = lastIndex_;
	}
	size = end - start + 1;

	hue = (larsonStep_ * ( 360/size ));
	ColorHSV hsv(hue,255,255);

	anim_LarsonScanner(hsv.getColorRGB(), tail, fade, start, end);
}

void LPD8806::anim_SineWave(ColorRGB color, int cycles, int start, int end)
{
	int size;
	float y;

	if(end == 0 || end > lastIndex_)
	{
		end = lastIndex_;
	}
	size = end - start + 1;

	for(int i=0; i<size; i++)
	{
		y = sin(PI * (float)cycles * (float)(waveStep_ * i) / (float)size);
		if(y >= 0.0)
		{
			y = 1.0 - y;
			ColorRGB rgb(255 - (float)(255 - color.r) * y, 255 - (float)(255 - color.g) * y, 255 - (float)(255 - color.b) * y);
			setPixel(start+i, rgb);
		}
		else
		{
			y += 1.0;
			ColorRGB rgb((float)(color.r * y), (float)(color.g * y), (float)(color.b * y));
			setPixel(start+i, rgb);
		}
	}

	waveStep_ += 1;
}

void LPD8806::anim_StepThird(ColorRGB color)
{
	if(thirdStep_ == 0)
	{
		fillOff(2*(nLeds_/3), nLeds_);
	}
	else
	{
		fillOff((thirdStep_-1)*(nLeds_/3),thirdStep_*(nLeds_/3));
	}
	
	fill(color,thirdStep_*(nLeds_/3), (++thirdStep_)*(nLeds_/3));

	if(thirdStep_ > 2)
	{
		thirdStep_ = 0;
	}
}

void LPD8806::anim_RandQuater(ColorRGB bg, ColorRGB colStep, int start, int end)
{
	int size, quatFill;
	ColorRGB *col;

	if(end == 0 || end > lastIndex_)
	{
		end = lastIndex_;
	}
	size = end - start + 1;

	quatFill = rand() % 4;

	for (int i = 0; i < size; i++)
	{
		col = &bg;
		if(i >= quatFill*(nLeds_/4) && i < (quatFill+1)*(nLeds_/4))
		{
			col = &colStep;
		}

		setPixel(start + i, *col);
	}
}

void LPD8806::anim_BlockFill(ColorRGB colOld, ColorRGB colNew, int blockSize, int start, int end)
{
	int size;

	if(end == 0 || end > lastIndex_)
	{
		end = lastIndex_;
	}
	size = end - start + 1;

	if(blockStep_ == 0)
	{
		fill(colOld,start,end);
	}
	else
	{
		fill(colNew, start + (size/2), (start + (size/2)) + (blockStep_*(size/blockSize)));
		fill(colNew, (start + (size/2)) - (blockStep_*(size/blockSize)), start + (size/2));
	}

	blockStep_++;
	if(blockStep_ > (blockSize/2)-1)
	{
		blockStep_ = 0;
	}
}

void LPD8806::setUp()
{
	rainbowStep_ =0; 
	rainbowCycleStep_ =0; 
	larsonStep_ =0; 
	wipeStep_ =0; 
	chaseStep_ =0; 
	waveStep_ =0;
	larsonDir_ =0; 
	larsonLast_ =0;
	thirdStep_ =0;
	stripeStep_ =0;
	sawStep_ =0;
	sineStep_ =0;
	sineDir_ =1;
	lineStep_ =0;
	wheelStep_ =0;
	driftStep_ =0; 
	driftDir_ =1;
	blockStep_ =0;
	meltStep_ =0;

	spi = new gnublin_spi();
	spi->setSpeed(SPI_SPEED);
	spi->setCS(CHIP_SELECT);
	spi->setLSB(SPI_LSB);

	zeros_ = new uint8_t [4];

	for(int i=0; i<4; i++)
	{
		zeros_[i] = 0x00;
	}

	gamma_ = new uint8_t [256];

	for(int i=0; i<256; i++)
	{
		gamma_[i] = (uint8_t)(0x80 | ((uint8_t)(pow((float)i/255.0, 2.5)*127.0 +0.5)));
	}
}

const uint8_t LPD8806::CHANNEL_ORDER[3][3] = 
		{
			{0, 1, 2},		//RGB
			{1, 0, 2},		//GRB
			{1, 2, 0}		//BRG
		};
