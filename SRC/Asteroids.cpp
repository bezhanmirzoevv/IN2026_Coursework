#include "Asteroid.h"
#include "Asteroids.h"
#include "Animation.h"
#include "AnimationManager.h"
#include "GameUtil.h"
#include "GameWindow.h"
#include "GameWorld.h"
#include "GameDisplay.h"
#include "Spaceship.h"
#include "ComputerSpaceShip.cpp"
#include "BoundingShape.h"
#include "BoundingSphere.h"
#include "GUILabel.h"
#include "Explosion.h"
#include "GUIButton.h"
#include <fstream>
#include "HighScoreTable.cpp"

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/** Constructor. Takes arguments from command line, just in case. */
Asteroids::Asteroids(int argc, char* argv[])
	: GameSession(argc, argv)
{
	mLevel = 0;
	mAsteroidCount = 0;
	mScreen = 2;
}

/** Destructor. */
Asteroids::~Asteroids(void)
{
}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////

/** Start an asteroids game. */
void Asteroids::Start()
{
	// Create a shared pointer for the Asteroids game object - DO NOT REMOVE
	shared_ptr<Asteroids> thisPtr = shared_ptr<Asteroids>(this);

	// Add this class as a listener of the game world
	mGameWorld->AddListener(thisPtr.get());

	// Add this as a listener to the world and the keyboard
	mGameWindow->AddKeyboardListener(thisPtr);

	// Add a score keeper to the game world
	mGameWorld->AddListener(&mScoreKeeper);

	// Add this class as a listener of the score keeper
	mScoreKeeper.AddListener(thisPtr);

	// Create an ambient light to show sprite textures
	GLfloat ambient_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat diffuse_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
	glEnable(GL_LIGHT0);

	Animation* explosion_anim = AnimationManager::GetInstance().CreateAnimationFromFile("explosion", 64, 1024, 64, 64, "explosion_fs.png");
	Animation* asteroid1_anim = AnimationManager::GetInstance().CreateAnimationFromFile("asteroid1", 128, 8192, 128, 128, "asteroid1_fs.png");
	Animation* spaceship_anim = AnimationManager::GetInstance().CreateAnimationFromFile("spaceship", 128, 128, 128, 128, "spaceship_fs.png");


	// Create some asteroids and add them to the world
	CreateAsteroids(10);

	//Create the GUI
	CreateGUI();

	// Add a player (watcher) to the game world
	mGameWorld->AddListener(&mPlayer);
	mGameWorld->AddListener(&mComputer);

	//Spawn the Compter space ship
	mGameWorld->AddObject(CreateComputerSpaceShip());
	

	// Add this class as a listener of the player
	mPlayer.AddListener(thisPtr);
	mComputer.AddListener(thisPtr);


	// Start the game
	GameSession::Start();


}

/** Stop the current game. */
void Asteroids::Stop()
{
	// Stop the game
	GameSession::Stop();
}

// PUBLIC INSTANCE METHODS IMPLEMENTING IKeyboardListener /////////////////////

void Asteroids::OnKeyPressed(uchar key, int x, int y)
{
	switch (mScreen) {
		//game screen
	case 1:
		switch (key) {
		case ' ':
			mSpaceship->Shoot();
			break;
		default:
			break;
		}
		break;

		//start screen 
	case 2:
		switch (key) {
		case ' ':
			mScreen = 1;
			mStartLabel1->SetVisible(false);
			mHighScoreLabel1->SetVisible(false);
			mScoreLabel->SetVisible(true);
			mLivesLabel->SetVisible(true);

			// remove the demo spaceship and reset world
			mGameWorld->FlagForRemoval(mComputerSpaceShip->GetThisPtr());

			//reset score to start game
			mScoreKeeper.ResetScore();
			mScoreKeeper.FireScoreChanged();
		
			// Create a spaceship and add it to the world
			mGameWorld->AddObject(CreateSpaceship());
			break;
		case 's':
			mScreen = 3;
			mStartLabel1->SetVisible(false);
			mStartLabel2->SetVisible(true);
			mHighScoreLabel1->SetVisible(false);
			mHighScoreLabel2->SetVisible(true);
			mhighscoretable.printScores(mGameDisplay);
			break;
		default:
			break;
		}
		break;

		//high score screen
	case 3:
		switch (key) {
		case ' ':
			mScreen = 1;
			mStartLabel2->SetVisible(false);
			mHighScoreLabel2->SetVisible(false);
			mScoreLabel->SetVisible(true);
			mLivesLabel->SetVisible(true);
			mhighscoretable.removeScores(mGameDisplay);
			// Create a spaceship and add it to the world
			mGameWorld->AddObject(CreateSpaceship());
			break;
		default:
			break;
		}
		break;

	default:
		break;
	}
}

void Asteroids::OnKeyReleased(uchar key, int x, int y) {}

void Asteroids::OnSpecialKeyPressed(int key, int x, int y)
{
	if (mScreen == 1) {
		switch (key)
		{
			// If up arrow key is pressed start applying forward thrust
		case GLUT_KEY_UP: mSpaceship->Thrust(10);  /*mComputerSpaceShip->Thrust(10);*/ break;
			// If left arrow key is pressed start rotating anti-clockwise
		case GLUT_KEY_LEFT: mSpaceship->Rotate(90);  /*mComputerSpaceShip->Rotate(90);*/ break;
			// If right arrow key is pressed start rotating clockwise
		case GLUT_KEY_RIGHT: mSpaceship->Rotate(-90);  /*mComputerSpaceShip->Rotate(-90);*/ break;
			// Default case - do nothing
		default: break;
		}
	}
}

void Asteroids::OnSpecialKeyReleased(int key, int x, int y)
{
	if (mScreen == 1) {
		switch (key)
		{
			// If up arrow key is released stop applying forward thrust
		case GLUT_KEY_UP: mSpaceship->Thrust(0); /*mComputerSpaceShip->Thrust(0);*/ break;
			// If left arrow key is released stop rotating
		case GLUT_KEY_LEFT: mSpaceship->Rotate(0); /*mComputerSpaceShip->Rotate(0);*/ break;
			// If right arrow key is released stop rotating
		case GLUT_KEY_RIGHT: mSpaceship->Rotate(0);/* mComputerSpaceShip->Rotate(0);*/ break;
			// Default case - do nothing
		default: break;
		}
	}
}


// PUBLIC INSTANCE METHODS IMPLEMENTING IGameWorldListener ////////////////////

void Asteroids::OnObjectRemoved(GameWorld* world, shared_ptr<GameObject> object)
{
	if (object->GetType() == GameObjectType("Asteroid"))
	{
		shared_ptr<GameObject> explosion = CreateExplosion();
		explosion->SetPosition(object->GetPosition());
		explosion->SetRotation(object->GetRotation());
		mGameWorld->AddObject(explosion);
		mAsteroidCount--;
		if (mScreen != 1) {
			mComputerSpaceShip->SetVelocity(GLVector3f(0, 0, 0));
		}
		if (mAsteroidCount <= 0)
		{
			if (mScreen == 1) {
				SetTimer(500, START_NEXT_LEVEL);
			}else {
				SetTimer(500, RESPAWN_ASTEROIDS);
			}
		}

	}
}

// PUBLIC INSTANCE METHODS IMPLEMENTING ITimerListener ////////////////////////

void Asteroids::OnTimer(int value)
{
	if (value == CREATE_NEW_PLAYER)
	{
		mSpaceship->Reset();
		mGameWorld->AddObject(mSpaceship);

	}
	if (value == RESPAWN_ASTEROIDS)
	{
		CreateAsteroids(10);
	}

	if (value == START_NEXT_LEVEL)
	{
		mLevel++;
		int num_asteroids = 10 + 2 * mLevel;
		CreateAsteroids(num_asteroids);
	}

	if (value == SHOW_GAME_OVER)
	{
		mGameOverLabel->SetVisible(true);
		//store the score in the file
		mhighscoretable.SaveScores("jack", mScoreKeeper.getScore());
	}

	if (value == START_SCREEN)
	{
		mGameOverLabel->SetVisible(true);
	}
}

// PROTECTED INSTANCE METHODS /////////////////////////////////////////////////
shared_ptr<GameObject> Asteroids::CreateSpaceship()
{
	// Create a raw pointer to a spaceship that can be converted to
	// shared_ptrs of different types because GameWorld implements IRefCount
	mSpaceship = make_shared<Spaceship>();
	mSpaceship->SetBoundingShape(make_shared<BoundingSphere>(mSpaceship->GetThisPtr(), 4.0f));
	shared_ptr<Shape> bullet_shape = make_shared<Shape>("bullet.shape");
	mSpaceship->SetBulletShape(bullet_shape);
	Animation* anim_ptr = AnimationManager::GetInstance().GetAnimationByName("spaceship");
	shared_ptr<Sprite> spaceship_sprite =
		make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	mSpaceship->SetSprite(spaceship_sprite);
	mSpaceship->SetScale(0.1f);
	// Reset spaceship back to centre of the world
	mSpaceship->Reset();
	// Return the spaceship so it can be added to the world
	return mSpaceship;

}

shared_ptr<GameObject> Asteroids::CreateComputerSpaceShip()
{
	// Create a raw pointer to a spaceship that can be converted to
	// shared_ptrs of different types because GameWorld implements IRefCount
	mComputerSpaceShip = make_shared<ComputerSpaceShip>();
	mComputerSpaceShip->SetBoundingShape(make_shared<BoundingSphere>(mComputerSpaceShip->GetThisPtr(), 4.0f));
	shared_ptr<Shape> bullet_shape = make_shared<Shape>("bullet.shape");
	mComputerSpaceShip->SetBulletShape(bullet_shape);
	Animation* anim_ptr = AnimationManager::GetInstance().GetAnimationByName("spaceship");
	shared_ptr<Sprite> spaceship_sprite =
		make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	mComputerSpaceShip->SetSprite(spaceship_sprite);
	mComputerSpaceShip->SetScale(0.1f);
	// Reset spaceship back to centre of the world
	mComputerSpaceShip->Reset();
	// Return the spaceship so it can be added to the world
	return mComputerSpaceShip;

}

void Asteroids::CreateAsteroids(const uint num_asteroids)
{
	mAsteroidCount = num_asteroids;
	for (uint i = 0; i < num_asteroids; i++)
	{
		Animation* anim_ptr = AnimationManager::GetInstance().GetAnimationByName("asteroid1");
		shared_ptr<Sprite> asteroid_sprite
			= make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
		asteroid_sprite->SetLoopAnimation(true);
		shared_ptr<GameObject> asteroid = make_shared<Asteroid>();
		asteroid->SetBoundingShape(make_shared<BoundingSphere>(asteroid->GetThisPtr(), 10.0f));
		asteroid->SetSprite(asteroid_sprite);
		asteroid->SetScale(0.2f);
		//Change the asteroids position if it spawns on top of player
		mGameWorld->AddObject(asteroid);

	}
}

void Asteroids::CreateGUI()
{
	// Add a (transparent) border around the edge of the game display
	mGameDisplay->GetContainer()->SetBorder(GLVector2i(10, 10));
	// Create a new GUILabel and wrap it up in a shared_ptr
	mScoreLabel = make_shared<GUILabel>("Score: 0");
	// Set the vertical alignment of the label to GUI_VALIGN_TOP
	mScoreLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_TOP);
	// Set the visibility of the label to false (hidden)
	mScoreLabel->SetVisible(false);
	// Add the GUILabel to the GUIComponent  
	shared_ptr<GUIComponent> score_component
		= static_pointer_cast<GUIComponent>(mScoreLabel);
	mGameDisplay->GetContainer()->AddComponent(score_component, GLVector2f(0.0f, 1.0f));

	// Create a new GUILabel and wrap it up in a shared_ptr
	mLivesLabel = make_shared<GUILabel>("Lives: 3");
	// Set the vertical alignment of the label to GUI_VALIGN_BOTTOM
	mLivesLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_BOTTOM);
	// Set the visibility of the label to false (hidden)
	mLivesLabel->SetVisible(false);
	// Add the GUILabel to the GUIComponent  
	shared_ptr<GUIComponent> lives_component = static_pointer_cast<GUIComponent>(mLivesLabel);
	mGameDisplay->GetContainer()->AddComponent(lives_component, GLVector2f(0.0f, 0.0f));

	// Create a new GUILabel and wrap it up in a shared_ptr
	mGameOverLabel = shared_ptr<GUILabel>(new GUILabel("GAME OVER"));
	// Set the horizontal alignment of the label to GUI_HALIGN_CENTER
	mGameOverLabel->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
	// Set the vertical alignment of the label to GUI_VALIGN_MIDDLE
	mGameOverLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_MIDDLE);
	// Set the visibility of the label to false (hidden)
	mGameOverLabel->SetVisible(false);
	// Add the GUILabel to the GUIContainer  
	shared_ptr<GUIComponent> game_over_component
		= static_pointer_cast<GUIComponent>(mGameOverLabel);
	mGameDisplay->GetContainer()->AddComponent(game_over_component, GLVector2f(0.5f, 0.5f));

	// Create a new GUILabel and wrap it up in a shared_ptr
	mStartLabel1 = shared_ptr<GUILabel>(new GUILabel("Start (Space)"));
	mStartLabel2 = shared_ptr<GUILabel>(new GUILabel("Start (Space)"));
	// Set the horizontal alignment of the button to GUI_HALIGN_CENTER
	mStartLabel1->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
	mStartLabel2->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
	// Set the vertical alignment of the button to GUI_VALIGN_MIDDLE
	mStartLabel1->SetVerticalAlignment(GUIComponent::GUI_VALIGN_BOTTOMMIDDLEMIDDLE);
	mStartLabel2->SetVerticalAlignment(GUIComponent::GUI_VALIGN_BOTTOMMIDDLEMIDDLE);
	// Set the visibility of the button to true (visible)
	mStartLabel2->SetVisible(false);
	// Add the GUILabel to the GUIContainer  
	shared_ptr<GUIComponent> start_label_component = static_pointer_cast<GUIComponent>(mStartLabel1);
	shared_ptr<GUIComponent> start_label_component2 = static_pointer_cast<GUIComponent>(mStartLabel2);
	mGameDisplay->GetContainer()->AddComponent(start_label_component, GLVector2f(0.5f, 0.3f));
	mGameDisplay->GetContainer()->AddComponent(start_label_component2, GLVector2f(0.5f, 0.1f));

	// Create a new GUILabel and wrap it up in a shared_ptr
	mHighScoreLabel1 = shared_ptr<GUILabel>(new GUILabel("High Score Table (s)"));
	mHighScoreLabel2 = shared_ptr<GUILabel>(new GUILabel("High Score Table"));
	// Set the horizontal alignment of the button to GUI_HALIGN_CENTER
	mHighScoreLabel1->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
	mHighScoreLabel2->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
	// Set the vertical alignment of the button to GUI_VALIGN_MIDDLE
	mHighScoreLabel1->SetVerticalAlignment(GUIComponent::GUI_VALIGN_BOTTOMMIDDLEMIDDLE);
	mHighScoreLabel2->SetVerticalAlignment(GUIComponent::GUI_VALIGN_TOP);
	// Set the visibility of the button to true (visible)
	mHighScoreLabel1->SetVisible(true);
	mHighScoreLabel2->SetVisible(false);
	// Add the GUILabel to the GUIContainer  
	shared_ptr<GUIComponent> high_score_component = static_pointer_cast<GUIComponent>(mHighScoreLabel1);
	shared_ptr<GUIComponent> high_score_component2 = static_pointer_cast<GUIComponent>(mHighScoreLabel2);
	mGameDisplay->GetContainer()->AddComponent(high_score_component, GLVector2f(0.5f, 0.25f));
	mGameDisplay->GetContainer()->AddComponent(high_score_component2, GLVector2f(0.5f, 0.9f));

}

void Asteroids::OnScoreChanged(int score)
{
	// Format the score message using an string-based stream
	std::ostringstream msg_stream;
	msg_stream << "Score: " << score;
	// Get the score message as a string
	std::string score_msg = msg_stream.str();
	mScoreLabel->SetText(score_msg);
}

void Asteroids::OnPlayerKilled(int lives_left)
{
	shared_ptr<GameObject> explosion = CreateExplosion();
	explosion->SetPosition(mSpaceship->GetPosition());
	explosion->SetRotation(mSpaceship->GetRotation());
	mGameWorld->AddObject(explosion);

	// Format the lives left message using an string-based stream
	std::ostringstream msg_stream;
	msg_stream << "Lives: " << lives_left;
	// Get the lives left message as a string
	std::string lives_msg = msg_stream.str();
	mLivesLabel->SetText(lives_msg);

	if (lives_left > 0)
	{
		SetTimer(1000, CREATE_NEW_PLAYER);
	}
	else
	{
		SetTimer(500, SHOW_GAME_OVER);
	}
}

void Asteroids::OnComputerKilled(int lives_left)
{
	
}

shared_ptr<GameObject> Asteroids::CreateExplosion()
{
	Animation* anim_ptr = AnimationManager::GetInstance().GetAnimationByName("explosion");
	shared_ptr<Sprite> explosion_sprite =
		make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	explosion_sprite->SetLoopAnimation(false);
	shared_ptr<GameObject> explosion = make_shared<Explosion>();
	explosion->SetSprite(explosion_sprite);
	explosion->Reset();
	return explosion;
}




