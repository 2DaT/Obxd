#pragma once
#include "../Source/Engine/SynthEngine.h"
class ButtonList  : public ComboBox{
private :
	int count;
	Image kni;
	int w2,h2;
public:
	ButtonList(Image k,int fh) :ComboBox("cb")
	{
		kni = k;
		count = 0;
		h2 =fh;
		w2 = k.getWidth();
	}
	//int addItem
	void addChoise(String name)
	{
		addItem(name,++count);
	}
	float getValue()
	{
		return ((getSelectedId()-1)/ (float)(count-1));
	}
	void setValue(float val,int notify)
	{
		setSelectedId((int)(val*(count -1) + 1),notify);
	}
	void paintOverChildren(Graphics& g)
	{
		int ofs = getSelectedId()-1;
				g.drawImage(kni, 0, 0, getWidth(), getHeight(),
					0, h2*ofs, w2, h2);
	}


};