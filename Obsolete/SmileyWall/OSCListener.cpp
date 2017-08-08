/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#include "OSCListener.h"
#include <iostream>

using namespace std;
using namespace osc;

void OSCListener::ProcessMessage( const ReceivedMessage& m, 
	const IpEndpointName& remoteEndpoint )
{
	processInteractionStateMsg(m);
	processSmileDetectionMsg(m);
	processEyePosMsg(m);
	processTakePhotoMsg(m);
	processTakeVideoMsg(m);
}

void OSCListener::processInteractionStateMsg(const ReceivedMessage& m)
{
	try	{
		if (strcmp(m.AddressPattern(), "/interactionState") == 0) {
			bool state = false;
			osc::ReceivedMessageArgumentStream oscArgStream = m.ArgumentStream();
			oscArgStream >> state >> osc::EndMessage;
			interactionState = (state)? INTERACTION_START : INTERACTION_END;
			/*if (state == chairFlag) {
				cout << "chair: " << chairFlag << " " << chairCounter << "\n";
				chairCounter++;
				if (chairCounter > chairThreshold) {
					interactionState = (state)? INTERACTION_START : INTERACTION_END;
				}
			} else {
				cout << "chair: " << chairFlag << " " << chairCounter << "\n";
				chairFlag = state;
				chairCounter = 1;
			}*/
		}
	}
	catch (Exception& e) {
		cerr << "error while parsing message in OSCListener: "
			<< m.AddressPattern() << ": " << e.what() << endl;
	}
}

void OSCListener::processSmileDetectionMsg(const ReceivedMessage& m)
{
	try	{
		/*if (strcmp(m.AddressPattern(), "/smileDetection") == 0) {
			osc::ReceivedMessageArgumentStream oscArgStream = m.ArgumentStream();
			oscArgStream >> smileState >> osc::EndMessage;
		}*/
	}
	catch (Exception& e) {
		cerr << "error while parsing message in OSCListener: "
			<< m.AddressPattern() << ": " << e.what() << endl;
	}
}

void OSCListener::processEyePosMsg(const ReceivedMessage& m)
{
	try {
		if (strcmp(m.AddressPattern(), "/PoG") == 0) {
			osc::ReceivedMessageArgumentStream oscArgStream = m.ArgumentStream();
			oscArgStream >> eyePos.x >> eyePos.y >> osc::EndMessage;
		}
	}
	catch (Exception& e) {
		cerr << "error while parsing message in OSCListener: "
			<< m.AddressPattern() << ": " << e.what() << endl;
	}
}

void OSCListener::processTakePhotoMsg(const ReceivedMessage& m)
{
	try {
		if (strcmp(m.AddressPattern(), "/photoPath") == 0) {
			const char *path = NULL;
			osc::ReceivedMessageArgumentStream oscArgStream = m.ArgumentStream();
			oscArgStream >> path >> osc::EndMessage;
			takePhotoPath = path;
		}
	}
	catch (Exception& e) {
		cerr << "error while parsing message in OSCListener: "
			<< m.AddressPattern() << ": " << e.what() << endl;
	}
}

void OSCListener::processTakeVideoMsg(const ReceivedMessage& m)
{
	try {
		if (strcmp(m.AddressPattern(), "/smileDetection") == 0) {
			const char *path = NULL;
			osc::ReceivedMessageArgumentStream oscArgStream = m.ArgumentStream();
			oscArgStream >> path >> grade >> osc::EndMessage;
			if (takeVideoPath != path) {
				takeVideoPath = path;
				newVideo = true;
			}
			startReadTime = cv::getTickCount() / cv::getTickFrequency();
		}
	}
	catch (Exception& e) {
		cerr << "error while parsing message in OSCListener: "
			<< m.AddressPattern() << ": " << e.what() << endl;
	}
}