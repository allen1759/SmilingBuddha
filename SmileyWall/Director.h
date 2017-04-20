/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 *
 */
#pragma once
#ifndef DIRECTOR_H
#define DIRECTOR_H
#define _AFXDLL
#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <Windows.h>
#include <digitalv.h>	// MCI_DGV_PLAY_REPEAT
#include <string>
#include <map>
#include "Picture.h"
#include "InitState.h"
#include "OSCListener.h"
#include "ImagesPool.h"


/*!
  \def SIMULATOR
	- 1: Simulator mode.
		- Show window bar and autoresize window.
		- Use a usb camera to capture image with OpenCV.
		- Take photo by a usb camera.
	- 0: In a exhibition, you must use this mode.
		- Show window bar and autoresize window.
		- Use a usb camera to capture image with OpenCV.
		- Take photo by a digital single-lens camera.
*/
#define SIMULATOR 0
#define MAX_VOLUME 1000
enum KeyNumber {ESC = 0x1b, LEFT_ARROW = 0x250000, UP_ARROW = 0x260000, 
	RIGHT_ARROW = 0x270000, DOWN_ARROW = 0x280000, ENTER = 13};
typedef enum AnimationType{ANISTATRT, SMILEwave2PHOTO, PHOTOVIDEO, PHOTOwave2SMILE, MORPH2CENTER, CENTER2DIR, DIRANI, CENTERANI, MORPH2NEU, WAVE2VIDEO, WAVE2BUDDHA, TOBLACK} AnimationType;

const std::string winName = "ZhengMeiXiang"; //!< Window name
const cv::Size webcamImageSize(640, 480);	//!< Webcam Image size
const cv::Size paintingSize(2048, 1200);		//!< Painting size
const cv::Size numPicturesInPainting(9, 4); //!< the numbers of pictures in row and column
const int numPhotos = numPicturesInPainting.width * numPicturesInPainting.height; //!< the total number pictures in the painting
const int numSmilePhotos = 57;                            
const int numTextPhotos = 44;
const int textPosFromCenter[] = {2, 3, 4, 4};
const int numsmilePhotos=30;//!< smile photo number

const int peopleIndex[] ={35,4,46,13,32,44,2,3,8,39,14,33,43,5,17,15,20,1,36,23,40,34,28,9,16,31,21,7,41,19,10,18,45,38,37,29};
const int numLookPeople = 36; // 36
const int numLookNeu = 50;
const int numLookSmooth = 15;
const int numPerLookPerPersonPhotos = 55; //?
const int numRandomPhotos = 200; // Number of random photos per person
const int numUserPhotos = 40;
const int numSmileVideos = 36;
const int videoTime = 5;
const double animation_time = 5.5;
const double animation_timeoffset = 0.1;
const double smooth_time = 0.5;
const double eye_stay_time = 3;
const int userTopNum = 1;

const int numBuddhaPhotos = 5;
const int slotIndex[] = {13, 12, 3, 4, 5, 14, 23, 22, 21, 20, 11, 2, 6, 15, 24, 33, 32, 31, 30, 29, 28, 19, 10, 1, 7, 16, 25, 34, 27, 18, 9, 0, 8, 17, 26, 35};

const int smileDatectionPort = 9998; //!< Port for sending OSC messages to smile datection
const int musicPlayerPort = 6754; //!< Port for sending OSC messages to music player

const int nineBox[] = {12, 13, 14, 21, 22, 23, 30, 31, 32};

const double bsec = 0.7;
const double msec = 1.12;
const int cbc_frame = 28;

/*!
 * \brief
 * The most top controller for ZhengMeiXiang.
 * 
 * Director controls all animations, receives all inputs such as 
 OSC, key events, and mouse events, plays background music, and takes user's photo.
 * 
 */
class Director
{
public:
	Director(): oscListener(new OSCListener()), animationState(new InitState), 
		imagesPool(new ImagesPool), startTickCount(0), painting(cv::Mat(paintingSize, CV_8UC3)), 
		canRecord(false), videoNum(0), currentVideoNum(0), smileVideoNum(0), readVideoFin(true) {
			for (int i=0; i<userTopNum; i++) {
				userTopScore[i] = 0;
			}
	}
	
	/*!
	*  \brief The artwork actions! This is a starting point.
	
	Initialize all variables and processes anything you want,
	such as recording, animation control, and so on.
	*/
	void action();

	/*!
	*  \brief Play animation in the painting, i.e. play all animations for the all pictures.
	*/
	void playAnimation();

	/*!
	*  \brief All animations for the all pictures were end or not.
	*  \return
		- true: all animations were end.
		- false: otherwise.
	*/
	bool isAllAnimationEnd();

	/*!
	*  \brief Play background music.
	*  \param musicPath a music file path.
		\param loop true: music plays repeatedly.
					false: music plays once.
	*/
	/*void playMusic(int i, const bool loop = false);

	void stopMusic(int i);

	int getVolume(int i);

	void setVolume(int i, int volume);

	void fadeInMusic(int index, double fadeStartTime, double sec);

	void fadeOutMusic(int index, double fadeStartTime, double sec);*/

	void playMusic(const std::string &musicPath, const bool loop = true, const bool stop = true);

	void stopMusic();

	//void setVolume(int volume);

	/*!
	*  \brief A getter for OSCListener.
	*  \return a OSCListener pointer.
	*/
	inline OSCListener* getOSCListener() const {return oscListener;}

	/*!
	*  \brief A setter for startTickCount.
	*  
		set startTickCount to be start time when a audience starts interacting with the artwork.
	*/
	inline void setStartTickCount() {startTickCount = cv::getTickCount();}

	/*!
	*  \brief A getter for animationState.
	*  
		There are many animation states in interaction. One state, one animation.
		Hence, just change the animation state in order to change animation.
		\param newAnimationState a new animation state that is set.
		\param clearAnimation true: clear the current animation of all pictures.
							  false: remain the current animation of all pictures.
	*/
	void setAnimationState(AnimationState *newAnimationState, const bool &clearAnimation = false);

	/*!
	*  \brief Get a picture iterator located in the index.
	*  \param index row-major order, i.e. if you want to access picture at (x, y), 
	then index = y * width + x.
	*  \return a picture iterator.
	*/
	std::vector<Picture>::iterator getPictureAt(const unsigned int &index);

	/*!
	*  \brief Get a picture iterator located at (row, col).
	*  \param row row.
		\param col column.
	*  \return a picture iterator.
	*/
	std::vector<Picture>::iterator getPictureAt(
		const unsigned int &row, const unsigned int &col);

	/*!
	*  \brief Take a photo.
	*  \param photo return photo taken.
	*/
	void takePhoto(cv::OutputArray &photo);

	/*!
	*  \brief Get a eye position from a OSC Message.
	*  \return a eye position that the unit of x-axis and y-axis are float numbers.
	*/
	cv::Point2f getEyePosWithOSCMessage() const;

	bool getSmileStateWithOSCMessage() const;

	/*!
	*  \brief Get a 1-dimension eye position.
	*  \param normalEyePos a normalized eye position, 
				i.e. 0 <= eyePos.x <= 1 and 0 <= eyePos.y <= 1.
	*  \return a eye position in row-major order.
	*/
	int get1DEyePos(const cv::Point2f &normalEyePos) const;

	/*!
	*  \brief Get a 2-dimension eye position.
	*  \param normalEyePos a normalized eye position, 
				i.e. 0 <= eyePos.x <= 1 and 0 <= eyePos.y <= 1.
	*  \return a eye position that the unit of x-axis is row and that of y-axis is column.
	*/
	cv::Point get2DEyePos(const cv::Point2f &normalEyePos) const;

	/*!
	*  \brief Play background music.
	*  \param musicPath a music file path.
		\param loop true: music plays repeatedly.
					false: music plays once.
	*/
	/*inline void stopMusic() { PlaySound(NULL, NULL, SND_FILENAME); }

	inline void playMusic(const std::string &musicPath, const bool loop = true, const bool stop = true) const {
		if (loop) {
			if (stop)
				PlaySound(musicPath.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
			else
				PlaySound(musicPath.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NOSTOP);
		}
		else {
			if (stop)
				PlaySound(musicPath.c_str(), NULL, SND_FILENAME | SND_ASYNC);
			else
				PlaySound(musicPath.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
		}
		
	}*/
	
	

	/*!
	*  \brief A getter for interactive duration.
	*  \return duration (sec).
		\param tickCount compute duration from startTickCount to tickCount
	*/
	inline float getDuration(const int64 &tickCount) {
		return static_cast<float>(tickCount - startTickCount) /
			static_cast<float>(cv::getTickFrequency());
	}

	/*!
	*  \brief A getter for current time.
	*  \return current time.
	*/
	inline int64 getCurrentTime() const { return cv::getTickCount(); }

	/*!
	*  \brief A getter for ImagesPool.
	*  \return a ImagesPool pointer.
	*/
	inline ImagesPool* getImagesPool() const {return imagesPool;}

	/*!
	*  \brief A setter for canRecord.
	*  \param recorded - true: record the user. -false: Don't record the user.
	*/
	inline void setCanRecord(const bool &recorded) {canRecord = recorded;}

	/*!
	*  \brief A getter for canRecord.
	*  \return canRecord - true: record the user. -false: Don't record the user.
	*/
	inline bool getCanRecord() const {return canRecord;}

	/*!
	*  \brief A getter for recordImage.
	*  \return a image that the camera records.
	*/
	inline cv::Mat getRecordImage() const {return recordImage;}

	/*!
	*  \brief A getter for captureImage.
	*  \return a image that the DSLR captures.
	*/
	inline cv::Mat getCaptureImage() const {return captureImage;}

	/*!
	*  \brief A getter for painting.
	*  \return a painting for ZhengMeiXiang.
	*/
	inline cv::Mat getPainting() const {return painting;}

	/*!
	*  \brief A generator for permutation.
	*  \param rules You can assign rules for permutation. the permutation will
	*			follow this rules. For example, rules[0] = 2 and rules[9] = 16. Then
	*			permutations[0] = 2, permutations[9] = 16, and the others are random numbers.
	*  \return a permutation following rules.
	*/
	std::vector<int> genPermutations(std::map<int, int> &rules = std::map<int, int>());
	
	inline cv::Rect getWebcamRoi() const {return webcamRoi;}

	inline void setWebcamRoi(const cv::Rect &roi) {webcamRoi = roi;}

	inline std::vector<cv::Mat> getCaptureImageVec() const {return captureImageVec;}

	inline void clearCaptureImageVec() {captureImageVec.clear();}

	/*!
	*  \Take video from smile detect.
	*  \If now read video return false.
	*/
	bool takeVideoFromSmileDetect();

	bool isVideoReadFin() {return readVideoFin;}

	int getVideoNum() {return videoNum;}
	void upVideoNum() {videoNum ++;}

	int getCurrentVideoNum() {return currentVideoNum;}
	void setCurrentVideoNum(int num) {currentVideoNum = num;}
	void clearOscVideoInfo();
	void sendOSCBoolMessage(const std::string &ipAddress, const int &port, 
		const std::string &name, const bool &message);
	void sendTCPOSCBoolMessage(const std::string &ipAddress, const int &port, 
		const std::string &name, const bool &message);
	void sendOSCStringMessage(const std::string &ipAddress, const int &port, 
		const std::string &name, const char* message);

	//void sendTopUserVideo ();

private:
	bool readData();
	//bool initCamera();
	void processGlobalKeyEvent(const int key);
	void processGlobalOSC();
	//void record();
	void takePhotoByCamera(cv::Mat& photo);
	/*void sendOSCBoolMessage(const std::string &ipAddress, const int &port, 
	const std::string &name, const bool &message);*/
	static void threadToReadVideo( void* arg );

private:
	OSCListener *oscListener;
	AnimationState *animationState;
	ImagesPool *imagesPool;
	int64 startTickCount;
	cv::Mat painting;
	std::vector<Picture> pictures;
	bool canRecord;
	cv::Mat recordImage;	// Image got from web-camera
	cv::Mat captureImage;	// Image got from DSLR camera
	std::vector<cv::Mat> captureImageVec;	// All images got from DSLR camera in interaction
	cv::Rect webcamRoi;
	bool readVideoFin;
	int videoNum;
	int currentVideoNum;
	int smileVideoNum;

	int userTopIndex;
	string userTopPath[userTopNum];
	float userTopScore[userTopNum];
};

#endif	// DIRECTOR_H