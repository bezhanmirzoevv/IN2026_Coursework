#ifndef __HIGHSCORETABLE_H__
#define __HIGHSCORETABLE_H__
#include "GameDisplay.h"
#include "GUILabel.h"
#include <map>
using namespace std;

class HighScoreTable 
{
public:
	//void printScores(GameDisplay a, GUILabel b);
	void SaveScores(string s, int b);

protected:
	map<string, int> scoreMap;
};

#endif
