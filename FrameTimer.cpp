//
//  FrameTimer.cpp
//  sdlBase
//
//  Created by ScottA on 2/3/16.
//  Copyright © 2016 Scott Atkins. All rights reserved.
//

#include "FrameTimer.h"

#include <sstream>

FrameTimer::FrameTimer(unsigned int updatesPerSecond, unsigned int maxDrawsPerSecond) {
	this->updatesPerSecond = updatesPerSecond;
	this->redrawsPerSecond = maxDrawsPerSecond;

	ticksPerUpdate = 1000 / updatesPerSecond;
	ticksPerRedraw = 1000 / redrawsPerSecond;

	secondsPerUpdate = 1.0f / updatesPerSecond;

	prevRedrawsCounter = 0;
	prevUpdatesCounter = 0;
	currRedrawsCounter = 0;
	currUpdatesCounter = 0;
}

__int64 FrameTimer::getCurrentTime() {
	auto now = std::chrono::system_clock::now();
	auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
	auto epoch = now_ms.time_since_epoch();
	auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
	return value.count();
}

void FrameTimer::start() {
	currentTick = getCurrentTime();
	nextGameUpdateTick = currentTick + ticksPerUpdate;
	nextGameRedrawTick = currentTick + ticksPerRedraw;
	prevCounterTick = currentTick;
}

void FrameTimer::updateWithCurrentTick() {
	currentTick = getCurrentTime();

	while (currentTick > prevCounterTick + 1000) {
		prevCounterTick += 1000;
		prevUpdatesCounter = currUpdatesCounter;
		prevRedrawsCounter = currRedrawsCounter;
		currUpdatesCounter = 0;
		currRedrawsCounter = 0;
	}
}

bool FrameTimer::shouldUpdate() {
	return nextGameUpdateTick <= currentTick;
}

bool FrameTimer::shouldRedraw() {
	return nextGameRedrawTick <= currentTick;
}

void FrameTimer::signalUpdateFinished() {
	currUpdatesCounter++;
	nextGameUpdateTick += ticksPerUpdate;
}

void FrameTimer::signalRedrawFinished() {
	currRedrawsCounter++;
	while (nextGameRedrawTick <= currentTick) {
		nextGameRedrawTick += ticksPerRedraw;
	}
}

float FrameTimer::getSecondsPerUpdate() {
	return secondsPerUpdate;
}

int FrameTimer::getUpdatesPerformedLastSecond() {
	return prevUpdatesCounter;
}

int FrameTimer::getRedrawsPerformedLastSecond() {
	return prevRedrawsCounter;
}
