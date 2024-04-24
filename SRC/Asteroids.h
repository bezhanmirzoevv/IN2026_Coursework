#ifndef __ASTEROIDS_H__
#define __ASTEROIDS_H__

#include "GameUtil.h"
#include "GameSession.h"
#include "IKeyboardListener.h"
#include "IGameWorldListener.h"
#include "IScoreListener.h" 
#include "ScoreKeeper.h"
#include "Player.h"
#include "Computer.h"
#include "IPlayerListener.h"
#include "IComputerListener.h"
#include "HighScoreTable.h"


class GameObject;
class Spaceship;
class ComputerSpaceShip;
class GUILabel;
class GUIButton;

class Asteroids : public GameSession, public IKeyboardListener, public IGameWorldListener, public IScoreListener, public IPlayerListener, public IComputerListener
{
public:
	Asteroids(int argc, char* argv[]);
	virtual ~Asteroids(void);

	virtual void Start(void);
	virtual void Stop(void);

	// Declaration of IKeyboardListener interface ////////////////////////////////

	void OnKeyPressed(uchar key, int x, int y);
	void OnKeyReleased(uchar key, int x, int y);
	void OnSpecialKeyPressed(int key, int x, int y);
	void OnSpecialKeyReleased(int key, int x, int y);

	// Declaration of IScoreListener interface //////////////////////////////////

	void OnScoreChanged(int score);

	// Declaration of the IPlayerListener interface //////////////////////////////

	void OnPlayerKilled(int lives_left);

	void OnComputerKilled(int lives_left);

	// Declaration of IGameWorldListener interface //////////////////////////////

	void OnWorldUpdated(GameWorld* world) {}
	void OnObjectAdded(GameWorld* world, shared_ptr<GameObject> object) {}
	void OnObjectRemoved(GameWorld* world, shared_ptr<GameObject> object);

	// Override the default implementation of ITimerListener ////////////////////
	void OnTimer(int value);

private:
	shared_ptr<Spaceship> mSpaceship;
	shared_ptr<ComputerSpaceShip> mComputerSpaceShip;
	shared_ptr<GUILabel> mScoreLabel;
	shared_ptr<GUILabel> mLivesLabel;
	shared_ptr<GUILabel> mGameOverLabel;
	shared_ptr<GUILabel> mStartLabel1;
	shared_ptr<GUILabel> mStartLabel2;
	shared_ptr<GUILabel> mHighScoreLabel1;
	shared_ptr<GUILabel> mHighScoreLabel2;
	shared_ptr<GUILabel> mPlayerNameInput;
	shared_ptr<GUILabel> mPlayerName;


	uint mLevel;
	uint mAsteroidCount;
	uint mScreen;

	void ResetSpaceship();
	void ResetComputerSpaceShip();
	shared_ptr<GameObject> CreateSpaceship();
	shared_ptr<GameObject> CreateComputerSpaceShip();
	void CreateGUI();
	void CreateAsteroids(const uint num_asteroids);
	shared_ptr<GameObject> CreateExplosion();

	const static uint SHOW_GAME_OVER = 0;
	const static uint START_NEXT_LEVEL = 1;
	const static uint CREATE_NEW_PLAYER = 2;
	const static uint START_SCREEN = 3;
	const static uint RESPAWN_ASTEROIDS = 4;
	const static uint START_GAME = 5;

	ScoreKeeper mScoreKeeper;
	Player mPlayer;
	Computer mComputer;
	HighScoreTable mhighscoretable;
};

#endif