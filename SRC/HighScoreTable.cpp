#include "HighScoreTable.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
using namespace std;

/*HighScoreTable::printScores(GameDisplay a, GUILabel b) {
	std::ifstream file("HighScores.txt");
	if (file.is_open()) {
		std::string playername;
		int score;
		while (file >> playername >> score) {
			ScoresMap.insert([playername, score])
		}
		file.close();
	}
	return ScoresMap;

	string example = ScoresMap.at("bezhan")
}
*/

void HighScoreTable::SaveScores(string playerName, int playerScore)
{
	std::ifstream file("HighScores.txt");
	std::string name;
	int score;
	bool store = false;
	while (file >> name >> score) {
		scoreMap.insert(make_pair(name, score));
		if (name == playerName) {
			if (score < playerScore) {
				store = true;
				scoreMap[playerName] = playerScore;
			}
			break;
		}
	}
	file.close();
	if (store) {
		std::fstream file("HighScores.txt");
		for (auto& pair : scoreMap) {
			file << pair.first << " " << pair.second << endl;
		}
	}
	file.close();
	
}
