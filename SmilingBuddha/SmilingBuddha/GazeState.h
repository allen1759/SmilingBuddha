/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _GAZE_STATE_H
#define _GAZE_STATE_H

#include "InteractionState.h"

#include <chrono>

#include "Ray.h"
#include "VideoPool.h"
#include "Video.h"

class GazeState : public InteractionState
{
public:
	GazeState(Director *director, std::chrono::high_resolution_clock::time_point startTime);

	virtual ~GazeState();

	virtual void Update() override;

	virtual void OnSmile() override;

protected:
	std::shared_ptr<Video> GetActorDirectionVideo(int row, int col, int direction, bool loop, bool reverse);

	/**
	* Calculate the row/col of gaze grid from head pose, ensure that row/col are in legal range.
	*
	* @param headPose  head pose of user.
	* @param row  the correspond row number, pass by reference.
	* @param col  the correspond column number, pass by reference.
	*
	*/
	void HeadPost2RowCol(Ray headPose, int &row, int &col);

	void MoveChangeBackgroundAnimationToOtherGrid(int row, int col);


	const int ROW_COUNT;
	const int COL_COUNT;
	const int ROW_CENTER;
	const int COL_CENTER;
	const float PROJECTION_WIDTH;
	const float PROJECTION_HEIGHT;

	const float GAZE_STATE_TIME = 20.0f;
	const float ACTOR_VIDEO_TIME = 3.0f;
	const float CHANGE_BACKGROUND_TIME = 3.0f;

	VideoPool *videoPool;

	bool switchToBroadcastState;

	int lastGazeRow;
	int lastGazeCol;

	float elapsedTime;
	std::chrono::high_resolution_clock::time_point startTime;
};

#endif // !_GAZE_STATE_H
