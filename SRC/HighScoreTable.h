#ifndef __HIGHSCORETABLE_H__
#define __HIGHSCORETABLE_H__
#include "GameDisplay.h"
#include "GUILabel.h"
#include <map>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class GUILabel;

class HighScoreTable 
{
public:
	void printScores(GameDisplay* a);
	void removeScores(GameDisplay* a);
	void SaveScores(string s, int b);
	std::map<std::string, int> sortMapByValue(const std::map<std::string, int>& inputMap);


	int labelpointer;
	map<string, int> scoreMap;

	shared_ptr<GUILabel> player1Label;
	shared_ptr<GUILabel> player1Labelscore;
	shared_ptr<GUILabel> player2Label;
	shared_ptr<GUILabel> player2Labelscore;
	shared_ptr<GUILabel> player3Label;
	shared_ptr<GUILabel> player3Labelscore;
	shared_ptr<GUILabel> player4Label;
	shared_ptr<GUILabel> player4Labelscore;
	shared_ptr<GUILabel> player5Label;
	shared_ptr<GUILabel> player5Labelscore;
};

#endif
