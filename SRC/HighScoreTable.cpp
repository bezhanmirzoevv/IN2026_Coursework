#include <fstream>
#include "HighScoreTable.h"
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

void HighScoreTable::SaveScores(string s, int b)
{
	std::ofstream file;
	file.open("HighScores.txt", std::ios_base::app);
	file << s << " " << b << endl;
	file.close();
}
