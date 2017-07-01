#ifndef BUTTONSET_H
#define BUTTONSET_H

#include <vector>
#include "Button.h"
#include "PosChart.h"
#include "ValidChar.h"
#include "enums.h"


using std::vector;

//set of active buttons
class ButtonSet
{
	vector<Button> buttons;
	PosChart positions;
	void initPositions();
	void mergeButtons ();

public:
	ButtonSet ();
	ButtonSet (vector<ValidChar> writings);
	vector<Button>& getButtons();
	bool getButtonWithField (int inpLoop, int inpRow, Button& but);
};
ButtonSet::ButtonSet ()
{}
ButtonSet::ButtonSet (vector<ValidChar> writings)
{
	for (vector<ValidChar>::size_type i = 0; i != writings.size () ; i++)
		buttons.push_back (Button (writings[i]));
	initPositions ();
	mergeButtons ();
}

//fills positions with the oriinal (non-merged) values
void ButtonSet::initPositions()
{
	for (vector<Button>::size_type i = 0; i != buttons.size () ; i++)
		positions.fillPos (buttons[i].getButtonShape ().getOrigLoop (), buttons[i].getButtonShape ().getOrigRow ());
}

//ValidChars and Buttons will already be sorted wrt frequency
//Simply merge them in order
void ButtonSet::mergeButtons ()
{
	 for (vector<Button>::size_type i = 0; i != buttons.size () ; i++)
		 buttons[i].getButtonShape ().mergeRadially (positions);
	 for (vector<Button>::size_type i = 0; i != buttons.size () ; i++)
		 buttons[i].getButtonShape ().mergeLaterally (positions,firstPhase);
	 for (vector<Button>::size_type i = 0; i != buttons.size () ; i++)
		 buttons[i].getButtonShape ().mergeLaterally (positions,secondPhase);
	 for (vector<Button>::size_type i = 0; i != buttons.size () ; i++)
		 buttons[i].getButtonShape ().mergeLaterally (positions,thirdPhase);

}
vector<Button>& ButtonSet::getButtons()
{
	return buttons;
}

bool ButtonSet::getButtonWithField (int inpLoop, int inpRow, Button& but)
{
	for (vector<Button>::size_type i = 0; i < buttons.size(); i++)
	{
		if (buttons[i].getButtonShape().isInDisk(inpLoop, inpRow))
		{
			but = buttons[i];
			return true;
		}
	}
	return false;
}

#endif