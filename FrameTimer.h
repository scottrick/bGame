//
//  FrameTimer.h
//  sdlBase
//
//  Created by ScottA on 2/3/16.
//  Copyright © 2016 Scott Atkins. All rights reserved.
//

#ifndef FrameTimer_hpp
#define FrameTimer_hpp

#include <chrono>

class FrameTimer {
public:
	FrameTimer(unsigned int updatesPerSecond, unsigned int maxDrawsPerSecond);

	void start();
	void updateWithCurrentTick();

	bool shouldUpdate();
	bool shouldRedraw();

	void signalUpdateFinished();
	void signalRedrawFinished();

	float getSecondsPerUpdate();

	int getRedrawsPerformedLastSecond(); //get FPS
	int getUpdatesPerformedLastSecond(); //get updates/second

protected:


private:
	int updatesPerSecond;
	int redrawsPerSecond;

	__int64 ticksPerUpdate;
	__int64 ticksPerRedraw;
	__int64 nextGameUpdateTick;
	__int64 nextGameRedrawTick;

	__int64 currentTick;
	__int64 prevCounterTick;

	float secondsPerUpdate;

	int prevUpdatesCounter;
	int prevRedrawsCounter;
	int currUpdatesCounter;
	int currRedrawsCounter;

	__int64 getCurrentTime();
};

#endif /* FrameTimer_hpp */