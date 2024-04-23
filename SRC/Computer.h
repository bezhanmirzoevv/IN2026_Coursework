#ifndef __COMPUTER_H__
#define __COMPUTER_H__

#include "GameUtil.h"

#include "GameObject.h"
#include "GameObjectType.h"
#include "IComputerListener.h"
#include "IGameWorldListener.h"

class Computer : public IGameWorldListener
{
public:
	Computer() { mLives = 30; }
	virtual ~Computer() {}

	void OnWorldUpdated(GameWorld* world) {}

	void OnObjectAdded(GameWorld* world, shared_ptr<GameObject> object) {}

	void OnObjectRemoved(GameWorld* world, shared_ptr<GameObject> object)
	{
		if (object->GetType() == GameObjectType("Spaceship")) {
			mLives -= 1;
			FireComputerKilled();
		}
	}

	void AddListener(shared_ptr<IComputerListener> listener)
	{
		mListeners.push_back(listener);
	}

	void FireComputerKilled()
	{
		// Send message to all listeners
		for (ComputerListenerList::iterator lit = mListeners.begin();
			lit != mListeners.end(); ++lit) {
			(*lit)->OnComputerKilled(mLives);
		}
	}

private:
	int mLives;

	typedef std::list< shared_ptr<IComputerListener> > ComputerListenerList;

	ComputerListenerList mListeners;
};

#endif