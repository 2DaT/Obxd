#pragma once
#include "../Source/Engine/SynthEngine.h"
class Knob  : public Slider
{
public:
	//Knob(Image image, const int numFrames, const bool stripIsHorizontal) : Slider("Knob")
	//{
		
	//	setTextBoxStyle(NoTextBox, 0, 0, 0);
	//	setSliderStyle(RotaryVerticalDrag);
	//	setRange(0.0f, 1.0f, 0.001f);
	//}
	Knob(Image k,int fh) : Slider("Knob")
	{
		h2 =fh;
		w2 = k.getWidth();
		numFr = k.getHeight() / h2;
		kni = k;
	};
	
	void paint(Graphics& g)
	{
		int ofs = (int)((getValue() - getMinimum()) / (getMaximum() - getMinimum()) * (numFr - 1));
				g.drawImage(kni, 0, 0, getWidth(), getHeight(),
					0, h2*ofs, w2, h2);

	}
private:
	Image kni;
	int fh,numFr;
	int w2,h2;
};