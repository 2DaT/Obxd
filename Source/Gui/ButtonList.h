#pragma once
#include "../Source/Engine/SynthEngine.h"
class ButtonList  : public ComboBox{
private :
	int count;
public:
	ButtonList()
	{
		count = 0;
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


};