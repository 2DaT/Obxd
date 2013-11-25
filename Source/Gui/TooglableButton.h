#pragma once
#include "../Source/Engine/SynthEngine.h"
class TooglableButton : public TextButton
{
public:

	bool toogled;
	TooglableButton() : TextButton()
	{
		this->
		toogled = false;
	}
	void clicked()
	{
		toogled = !toogled;
		//this->setColour(1,Colours::blue);
		if(toogled)
			this->setColour(TextButton::ColourIds::buttonColourId,Colours::lightgreen);
		else
			this->removeColour(TextButton::ColourIds::buttonColourId);
		//this->setColour(DrawableButton::ColourIds::backgroundColourId,Colours::lightpink);
		Button::clicked();
	};
	void setValue(float state,int notify)
	{
		if(state > 0.5)
			toogled = true;
		else toogled = false;
		if(toogled)
			this->setColour(TextButton::ColourIds::buttonColourId,Colours::lightgreen);
		else
			this->removeColour(TextButton::ColourIds::buttonColourId);
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
};