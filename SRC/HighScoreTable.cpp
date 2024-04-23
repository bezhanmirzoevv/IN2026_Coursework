#include "HighScoreTable.h"
#include "GameSession.h"
#include "GUIComponent.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
using namespace std;


void HighScoreTable::printScores(GameDisplay* a) {
	float linedisplacement = 0;
	std::ifstream file("HighScores.txt");
	std::string name;
	int score;
	labelpointer = 1;
	while (file >> name >> score) {

		std::ostringstream msg_stream;
		msg_stream << score;
		// Get the score message as a string
		std::string score_msg = msg_stream.str();

		switch (labelpointer) {
		case 1: {
			player1Label = shared_ptr<GUILabel>(new GUILabel(name));
			player1Labelscore = shared_ptr<GUILabel>(new GUILabel(score_msg));
			player1Label->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
			player1Labelscore->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
			player1Label->SetVerticalAlignment(GUIComponent::GUI_VALIGN_BOTTOMMIDDLEMIDDLE);
			player1Labelscore->SetVerticalAlignment(GUIComponent::GUI_VALIGN_BOTTOMMIDDLEMIDDLE);
			shared_ptr<GUIComponent> player_1_component = static_pointer_cast<GUIComponent>(player1Label);
			shared_ptr<GUIComponent> player_1score_component = static_pointer_cast<GUIComponent>(player1Labelscore);
			a->GetContainer()->AddComponent(player_1_component, GLVector2f(0.25f, 0.7f - linedisplacement));
			a->GetContainer()->AddComponent(player_1score_component, GLVector2f(0.75f, 0.7f - linedisplacement));
		}
			break;
		case 2: {
			player2Label = shared_ptr<GUILabel>(new GUILabel(name));
			player2Labelscore = shared_ptr<GUILabel>(new GUILabel(score_msg));
			player2Label->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
			player2Labelscore->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
			player2Label->SetVerticalAlignment(GUIComponent::GUI_VALIGN_BOTTOMMIDDLEMIDDLE);
			player2Labelscore->SetVerticalAlignment(GUIComponent::GUI_VALIGN_BOTTOMMIDDLEMIDDLE);
			shared_ptr<GUIComponent> player_2_component = static_pointer_cast<GUIComponent>(player2Label);
			shared_ptr<GUIComponent> player_2score_component = static_pointer_cast<GUIComponent>(player2Labelscore);
			a->GetContainer()->AddComponent(player_2_component, GLVector2f(0.25f, 0.7f - linedisplacement));
			a->GetContainer()->AddComponent(player_2score_component, GLVector2f(0.75f, 0.7f - linedisplacement));
		}
			break;
		case 3: {
			player3Label = shared_ptr<GUILabel>(new GUILabel(name));
			player3Labelscore = shared_ptr<GUILabel>(new GUILabel(score_msg));
			player3Label->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
			player3Labelscore->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
			player3Label->SetVerticalAlignment(GUIComponent::GUI_VALIGN_BOTTOMMIDDLEMIDDLE);
			player3Labelscore->SetVerticalAlignment(GUIComponent::GUI_VALIGN_BOTTOMMIDDLEMIDDLE);
			shared_ptr<GUIComponent> player_3_component = static_pointer_cast<GUIComponent>(player3Label);
			shared_ptr<GUIComponent> player_3score_component = static_pointer_cast<GUIComponent>(player3Labelscore);
			a->GetContainer()->AddComponent(player_3_component, GLVector2f(0.25f, 0.7f - linedisplacement));
			a->GetContainer()->AddComponent(player_3score_component, GLVector2f(0.75f, 0.7f - linedisplacement));
		}
			break;
		case 4: {
			player4Label = shared_ptr<GUILabel>(new GUILabel(name));
			player4Labelscore = shared_ptr<GUILabel>(new GUILabel(score_msg));
			player4Label->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
			player4Labelscore->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
			player4Label->SetVerticalAlignment(GUIComponent::GUI_VALIGN_BOTTOMMIDDLEMIDDLE);
			player4Labelscore->SetVerticalAlignment(GUIComponent::GUI_VALIGN_BOTTOMMIDDLEMIDDLE);
			shared_ptr<GUIComponent> player_4_component = static_pointer_cast<GUIComponent>(player4Label);
			shared_ptr<GUIComponent> player_4score_component = static_pointer_cast<GUIComponent>(player4Labelscore);
			a->GetContainer()->AddComponent(player_4_component, GLVector2f(0.25f, 0.7f - linedisplacement));
			a->GetContainer()->AddComponent(player_4score_component, GLVector2f(0.75f, 0.7f - linedisplacement));
		}
			break;
		case 5: {
			player5Label = shared_ptr<GUILabel>(new GUILabel(name));
			player5Labelscore = shared_ptr<GUILabel>(new GUILabel(score_msg));
			player5Label->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
			player5Labelscore->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
			player5Label->SetVerticalAlignment(GUIComponent::GUI_VALIGN_BOTTOMMIDDLEMIDDLE);
			player5Labelscore->SetVerticalAlignment(GUIComponent::GUI_VALIGN_BOTTOMMIDDLEMIDDLE);
			shared_ptr<GUIComponent> player_5_component = static_pointer_cast<GUIComponent>(player5Label);
			shared_ptr<GUIComponent> player_5score_component = static_pointer_cast<GUIComponent>(player5Labelscore);
			a->GetContainer()->AddComponent(player_5_component, GLVector2f(0.25f, 0.7f - linedisplacement));
			a->GetContainer()->AddComponent(player_5score_component, GLVector2f(0.75f, 0.7f - linedisplacement));
		}
			break;
		}
		linedisplacement += 0.05f;
		labelpointer++;
	}
	file.close();
}

void HighScoreTable::removeScores(GameDisplay* a) {
	if (labelpointer > 1) {
		player1Label->SetVisible(false);
		player1Labelscore->SetVisible(false);
	}
	if (labelpointer > 2) {
		player2Label->SetVisible(false);
		player2Labelscore->SetVisible(false);
	}
	if (labelpointer > 3) {
		player3Label->SetVisible(false);
		player3Labelscore->SetVisible(false);
	}
	if (labelpointer > 4) {
		player4Label->SetVisible(false);
		player4Labelscore->SetVisible(false);
	}
	if (labelpointer > 5) {
		player5Label->SetVisible(false);
		player5Labelscore->SetVisible(false);
	}

}


void HighScoreTable::SaveScores(string playerName, int playerScore)
{
	std::ifstream file("HighScores.txt");
	std::string name;
	int score;
	int store = 0;
	while (file >> name >> score) {
		scoreMap.insert(make_pair(name, score));
		if (name == playerName) {
			if (score < playerScore) {
				store = 1;
				scoreMap[playerName] = playerScore;
			}else {
				store = 3;
			}
			break;
		}
	}
	file.close();

	switch (store) {
	case 0:
	{
		std::ofstream file3;
		file3.open("HighScores.txt", std::ios_base::app); // append instead of overwrite
		file3 << playerName << " " << playerScore;
		file3.close();
	}
		break;

	case 1:
	{
		std::fstream file1("HighScores.txt");
		for (auto& pair : scoreMap) {
			file1 << pair.first << " " << pair.second << endl;
		}
		file1.close();
	}
		break;
	}
}
