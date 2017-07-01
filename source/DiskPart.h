#ifndef DISKPART_H
#define DISKPART_H

#include "PosChart.h"
#include "enums.h"

//Holds the geometric aspects of a button
class DiskPart 
{
	//Both innerRing and outerRing can take values [0,4]
	//0 is the middle point, 1-2-3 are the next outer rings, 4 is the ring outside the screen
	int innerRing, outerRing;
	//Both startSlice and stopSlice can take values [0,11]
	//0 represents 6 o'clock and increasing numbers go ccw
	//To draw a full circle, set startSlice==stopSlice
	int startSlice, stopSlice;
	//These keep the original location (before merge) of the button.
	//loop: 0->3 rings going outwards
	//row: indexed similar to start/stopSlice
	int loop, row;

public:
	DiskPart ();
	DiskPart (int inpLoop, int inpRow);
	int getOrigLoop () const;
	int getOrigRow () const;
	void mergeRadially (PosChart &positions);
	void mergeLaterally (PosChart &positions, lateralMergePhases ph);
	void getShape (int &loopStart, int &loopEnd, int &rowStart, int &rowEnd);
	bool isInDisk (int inpLoop, int inpRow);
};

DiskPart::DiskPart ()
{
}
DiskPart::DiskPart (int inpLoop, int inpRow)
{
	loop = inpLoop;
	row = inpRow;

	innerRing = loop;
	outerRing = loop+1;

	if (loop == 0)
	{
		startSlice = row * 6;
		stopSlice = ((row + 1) * 6) % 12;
	}
	else if (loop == 3)
	{
		startSlice = row * 4;
		stopSlice = ((row + 1) * 4) % 12;
	}
	else
	{
		startSlice = row;
		stopSlice = (row + 1) % 12;
	}
}

int DiskPart::getOrigLoop () const
{
	return loop;
}
int DiskPart::getOrigRow () const
{
	return row;
}


//This is the merge that will happen first.
void DiskPart::mergeRadially (PosChart &positions)
{
	switch (loop)
	{
	case 0:
		//middle loop cannot merge radially
		break;
	case 1:
		if (positions.getLoop (2)[row]==0)
		{
			positions.fillPos (2, row);
			outerRing = 3;
		}
		break;
	case 2:
		if (positions.getLoop (1)[row]==0)
		{
			positions.fillPos (1, row);
			innerRing = 1;
		}
		break;
	}
}

//Radial merge already happened. Lateral merge can happen in three phases:
//First phase merges non-radially-merged buttons (ccw).
//Second phase merges radially-merged buttons (ccw).
//Third phase merges any remaining empty spaces (cw).
void DiskPart::mergeLaterally (PosChart &positions, lateralMergePhases ph)
{
	switch (ph)
	{
		//only merge ccw if button is not merged radially
	case firstPhase:
		//check if button is not merged radially
		if (outerRing - innerRing == 1)
		{
			if (loop == 0)
			{
				//the first loop has only two buttons
				if  (positions.getLoop (loop)[(row + 1) % 2] == 0)
				{
					positions.fillPos (loop, (row + 1) % 2);
					startSlice = stopSlice = 0;
				}
			}
			else
			{
				//other loops have 12 buttons
				if  (positions.getLoop (loop)[(row + 1) % 12] == 0)
				{
					positions.fillPos (loop, (row + 1) % 12);
					stopSlice = (row + 2) % 12;
				}
			}
		}
		break;

		//only merge ccw if button is merged radially
	case secondPhase:
		//check if button is merged radially
		if (outerRing - innerRing == 2)
		{
			//check if both nearby positions are available
			if  ((positions.getLoop (1)[(row + 1) % 12] == 0) && (positions.getLoop (2)[(row + 1) % 12] == 0))
				{
					positions.fillPos (1, (row + 1) % 12);
					positions.fillPos (2, (row + 1) % 12);
					stopSlice = (row + 2) % 12;
				}
		}
		break;

		//always merge button if possible in cw
	case thirdPhase:
		if (outerRing - innerRing == 1)
		{
			//don't need to merge middle again
			if (loop != 0)
			{
				if  (positions.getLoop (loop)[(row + 11) % 12] == 0)
				{
					positions.fillPos (loop, (row + 11) % 12);
					startSlice = (row + 11) % 12;
				}
			}
		}
		else if (outerRing - innerRing == 2)
		{
			if  ((positions.getLoop (1)[(row + 11) % 12] == 0) && (positions.getLoop (2)[(row + 11) % 12] == 0))
				{
					positions.fillPos (1, (row + 11) % 12);
					positions.fillPos (2, (row + 11) % 12);
					startSlice = (row + 11) % 12;
				}
		}
		break;
	}
}

void DiskPart::getShape (int &loopStart, int &loopEnd, int &rowStart, int &rowEnd)
{
	loopStart = innerRing;
	loopEnd = outerRing;
	rowStart = startSlice;
	rowEnd = stopSlice;
}

bool DiskPart::isInDisk (int inpLoop, int inpRow)
{
	switch (inpLoop)
	{
	case 0:
		inpRow *= 6;	//0 for upper half and 1 for lower half. we want 0 for upper half and 6 for lower half.
		if (innerRing != 0)
			return false;
		break;
	case 1:
		if ((innerRing >= 2) || (outerRing <= 1))
			return false;
		break;
	case 2:
		if (outerRing <= 2)
			return false;
		break;
	case 3:
		return false;
	}

	if ((startSlice == 0) && (stopSlice == 0))
		return true;

	int tempStopSlice;
	if (stopSlice <= startSlice)
	{
		tempStopSlice = stopSlice + 12;
		if (inpRow == 0)
			inpRow = 12;
	}
	else
		tempStopSlice = stopSlice;

	if ((inpRow >= startSlice) && (inpRow < tempStopSlice))
		return true;
	else
		return false;
}


#endif