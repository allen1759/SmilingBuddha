/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#pragma once
#ifndef OSCLISTENER_H
#define OSCLISTENER_H

#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include "osc/OscReceivedElements.h"
#include "osc/OscPacketListener.h"

typedef enum InteractionState {
	PRELUDE = -1,
	INTERACTION_END = 0,
	INTERACTION_START = 1
} InteractionState;

const int chairThreshold = 3;

/*!
 * \brief
 * A listener for open sound control (OSC).
 * 
 * There are some communications with OSC:
	- Pressure detection: communicate a bool value 
		controls the beginning or the end of interactive stage.
	- Taking a photo: get a user's photo.
	- Eye detection: get a user's eye position we guess.
 */

class OSCListener: public osc::OscPacketListener
{
public:
	OSCListener(): osc::OscPacketListener(), interactionState(PRELUDE),
		eyePos(cv::Point2f(0.0f, 0.0f)), newVideo(false) {}

	/*!
	*  \brief initialize messages.
	* 
	*/
	inline void initMessages() {
		interactionState = PRELUDE;
		eyePos = cv::Point2f(0.0f, 0.0f);
		takePhotoPath.clear();
		smileState = false;
	}

protected:
	/*!
	*  \brief process a received message.
	*  \param m a received message.
		\param remoteEndpoint information about the sender.
	*/
	void ProcessMessage( const osc::ReceivedMessage& m, 
		const IpEndpointName& remoteEndpoint );

private:
	void processInteractionStateMsg(const osc::ReceivedMessage& m);
	void processSmileDetectionMsg(const osc::ReceivedMessage& m);
	void processEyePosMsg(const osc::ReceivedMessage& m);
	void processTakePhotoMsg(const osc::ReceivedMessage& m);
	void processTakeVideoMsg(const osc::ReceivedMessage& m);

public:
	InteractionState interactionState;
	cv::Point2f eyePos;
	std::string takePhotoPath;
	std::string takeVideoPath;
	bool smileState;
	bool newVideo;
	double startReadTime;
	double currentTickTime;
	float grade;
};
#endif // OSCLISTENER_H