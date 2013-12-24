#pragma once
#include "../Source/Engine/SynthEngine.h"
class TooglableButton : public ImageButton
{
public:
	bool toogled;
	TooglableButton(Image k) :ImageButton()
	{
		//this->setImages
		kni = k;
		toogled = false;
		width = kni.getWidth();
		height = kni.getHeight();
		w2=width;
		h2 = height / 2;
		this->setClickingTogglesState(true);
	}
	void clicked()
	{
		toogled = !toogled;
		//this->setColour(1,Colours::blue);
		//if(toogled)
		//	this->setColour(TextButton::ColourIds::buttonColourId,Colours::lightgreen);
		//else
		//	this->removeColour(TextButton::ColourIds::buttonColourId);
		//this->setColour(DrawableButton::ColourIds::backgroundColourId,Colours::lightpink);
		Button::clicked();

	};
	void paintButton(Graphics& g, bool isMouseOverButton, bool isButtonDown)
	{
		        int offset = 0;
        if (toogled)
        {
            offset = 1;
        }
		g.drawImage(kni, 0, 0, getWidth(), getHeight(),
				0, offset *h2, w2,h2);
	}
	void setValue(float state,int notify)
	{
		if(state > 0.5)
			toogled = true;
		else toogled = false;
		repaint();
	}
	float getValue()
	{
		if(toogled)
			return 1;
		else return 0;
	}
	//void paint(Graphics& g)
	//{
	//	g.drawImageTransformed(kni,AffineTransform::rotation(((getValue() - getMinimum())/(getMaximum() - getMinimum()))*float_Pi - float_Pi*2));
	//}
private:
	Image kni;
	int width,height,w2,h2;
};