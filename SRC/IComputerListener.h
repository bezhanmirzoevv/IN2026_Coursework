#ifndef __ICOMPUTERLISTENER_H__
#define __ICOMPUTERLISTENER_H__

class IComputerListener
{
public:
	virtual void OnComputerKilled(int lives_left) = 0;
};

#endif
