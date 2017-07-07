#ifndef POSCHART_H
#define POSCHART_H

#include <algorithm>
#include <vector>

using std::fill_n;
using std::vector;

class PosChart
{
	vector<int> loop0;
	vector<int> loop1;
	vector<int> loop2;
public:
	PosChart();
	void fillPos(int loop, int row);
	const vector<int>& PosChart::getLoop(int loop) const;
};



PosChart::PosChart()
{
	//initialize all elements with 0
	loop0 = vector<int>(2, 0);
	loop1 = vector<int>(12, 0);
	loop2 = vector<int>(12, 0);
}

void PosChart::fillPos(int loop, int row)
{
	switch (loop)
	{
	case 0:
		loop0[row] = 1;
		break;
	case 1:
		loop1[row] = 1;
		break;
	case 2:
		loop2[row] = 1;
		break;
	}
}

const vector<int>& PosChart::getLoop(int loop) const
{
	switch (loop)
	{
	case 0:
		return loop0;
	case 1:
		return loop1;
	case 2:
		return loop2;
	}
}

#endif