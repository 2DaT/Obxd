#pragma once
#include "../Source/Engine/SynthEngine.h"
class Knob  : public Slider
{
	Knob(Image image, const int numFrames, const bool stripIsHorizontal) : Slider("Knob")
	{
		
		setTextBoxStyle(NoTextBox, 0, 0, 0);
		setSliderStyle(RotaryVerticalDrag);
		setRange(0.0f, 1.0f, 0.001f);
	}

	//void paint(Graphics& g)
	//{
	//	g.drawImageTransformed(kni,AffineTransform::rotation(((getValue() - getMinimum())/(getMaximum() - getMinimum()))*float_Pi - float_Pi*2));
	//}
private:
	Image kni;
};