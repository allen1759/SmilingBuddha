/*!
*\author Zheng-Xiang Ke
* 
*\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
*/
#define MORE_FOR_UP_AND_DOWN 1	

#include "Director.h"
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include "AllAnimationStates.h"
#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"
#include <process.h>
#include "TCPClient.h"

//cv::VideoCapture capture(0);

using namespace std;
using namespace cv;
using namespace osc;

const int delay = 1000 / 35; //!< delay time
const int cameraOSCPort = 7000; //!< Port for sending OSC messages
const int outWallControllerOSCPort = 4649; //!< Port for sending OSC messages

const int oscMessageBufferSize = 1024; //!< Buffer size for a OSC message
const string webcamRoiFilePath = "webcamROI.txt"; //!< Webcam ROI file path
const string settingFilePath = "settings.txt"; //!< Setting file path
const string starPhotosDirPath = "Stars/"; //!< Star photo Directory path
const string smilePhotosDirPath = "Smiles/"; //!< Smile photo Directory path
const string textPhotosDirPath = "Texts/"; //!< Text photo Directory path
const string buddhaPhotosDirPath = "Buddhas/"; //!< Text photo Directory path
const string smileUserDirPath = "D:/SmileVideo2/"; //!< Smile photo Directory path

const string ltPhotosDirPath  = "Photo/LT/";
const string lbPhotosDirPath  = "Photo/LB/";
const string rtPhotosDirPath  = "Photo/RT/";
const string rbPhotosDirPath  = "Photo/RB/";
const string cPhotosDirPath  = "Photo/C/";

const string lookPhotosDirPath = "D:/Direction/"; //!< Direction photo Directory path
const string smileFacePhotosDirPath = "smile.jpg"; //!< Smile face photo Directory path

const Size pictureDefaultSize(110, 150);

Point2f normalMousePos(0.0f, 0.0f); //!< normalized mouse pos

const int numMusic = 3;
MCIDEVICEID dev;
char* music[numMusic] = { "Musics/smile1.wav", "Musics/smile2.wav", "Musics/bell.wav" };
MCI_OPEN_PARMS mciOpen;
MCI_PLAY_PARMS mciPlay;

typedef struct videoData
{
	bool* readVideoFin;
	int* videoNum;
	int* currentVideoNum;
	cv::Size size;
	std::vector<std::vector<cv::Mat> > *currentUserpool;
	//std::vector<std::vector<cv::Mat> > *crazyUserpool;
	std::vector<std::vector<cv::Mat> > *userpool;
	string path;
	//string crazyPath;
	double startTime;
} ReadVideoData;

ReadVideoData data;


void onMouse(int event, int x, int y, int flags, void *param)
{
	normalMousePos = Point2f(static_cast<float>(x) / paintingSize.width, 
		static_cast<float>(y) / paintingSize.height);
	switch (event) {
	case CV_EVENT_LBUTTONDOWN:
		break;
	default:
		break;
	}
}

void Director::action()
{
	setMouseCallback(winName, onMouse, 0);

	if (!readData()) {
		cerr << "Read data error!" << endl;
		return;
	}

	/*if (!initCamera()) {
	cerr << "Init camera error!" << endl;
	return;
	}*/



	int key = 0;
	while(key != ESC) {
		processGlobalKeyEvent(key);
		processGlobalOSC();
		//record();

		animationState->processTime(this, getCurrentTime());
		animationState->processKeyEvent(this, key);
		//animationState->processMouseEvent(this, get2DEyePos(normalMousePos));
		animationState->processOSC(this, oscListener);
		animationState->processAnimation(this);

		playAnimation();

		imshow(winName, painting);
		key = waitKey(delay);
	}
}

void Director::playAnimation()
{
	for (vector<Picture>::iterator pictureIterator = pictures.begin();
		pictureIterator != pictures.end(); ++pictureIterator)
		pictureIterator->playAnimation();
}

bool Director::isAllAnimationEnd()
{
	for (std::vector<Picture>::iterator pictureIterator = pictures.begin();
		pictureIterator != pictures.end(); ++pictureIterator) {
			if (!pictureIterator->getAnimation()->animationEnded())
				return false;
	}

	return true;
}

bool Director::readData()
{
	// Read webcam ROI
	ifstream ifsRoi(webcamRoiFilePath.c_str());
	if (ifsRoi.bad()) {
		cerr << "Read the webcam ROI file: " << webcamRoiFilePath << " error" << endl;
		return false;
	}
	ifsRoi >> webcamRoi.x >> webcamRoi.y >> webcamRoi.width >> webcamRoi.height;

	// Read setting
	ifstream ifs(settingFilePath.c_str());
	if (ifs.bad()) {
		cerr << "Read the setting file: " << settingFilePath << " error" << endl;
		return false;
	}
	int numRead = 0;
	while(ifs.good() && numRead < numPhotos) {
		Rect rect;
		ifs >> rect.x >> rect.y >> rect.width >> rect.height;
		pictures.push_back(Picture(painting(rect), rect));
		++ numRead;
	}

	// Read all stars' photos
	for (int i = 0; i < numPhotos; ++i) {
		ostringstream oss;
		oss << starPhotosDirPath << "imgSrc" 
			<< setfill('0') << setw(2) << i << ".jpg";
		Mat starPhoto = imread(oss.str());
		if (starPhoto.data == NULL) {
			cerr << "Read the star photo: " << oss.str() << " error" << endl;
			return false;
		}
		Mat resizeStarPhoto;
		resize(starPhoto, resizeStarPhoto, pictures[i].size(), 0, 0, INTER_AREA);

		imagesPool->starPhotos.push_back(resizeStarPhoto);
	}

	// Read all smiles' photos
	/*
	for (int i = 0; i < numSmilePhotos; ++i) {
		ostringstream oss;
		oss << smilePhotosDirPath << "smile" 
			<< setfill('0') << setw(2) << i << ".jpg";
		Mat smilePhoto = imread(oss.str());
		if (smilePhoto.data == NULL) {
			cerr << "Read the smile photo: " << oss.str() << " error" << endl;
			return false;
		}
		Mat resizeSmilePhoto;
		resize(smilePhoto, resizeSmilePhoto, pictures[0].size(), 0, 0, INTER_AREA);

		imagesPool->smilePhotos.push_back(resizeSmilePhoto);
	}
	*/

	// Read all texts' photos
	/*
	for (int i = 0; i < numTextPhotos; ++i) {
		ostringstream oss;
		oss << textPhotosDirPath << setfill('0') << setw(2) << i << ".jpg";
		Mat textPhoto = imread(oss.str());
		if (textPhoto.data == NULL) {
			cerr << "Read the text photo: " << oss.str() << " error" << endl;
			return false;
		}
		Mat resizeTextPhoto;
		resize(textPhoto, resizeTextPhoto, pictures[0].size(), 0, 0, INTER_AREA);

		imagesPool->textPhotos.push_back(resizeTextPhoto);
	}
	*/

	// Read random photos
	for (int n = 0; n < numLookPeople; n++) {
		/*if (n == 43) {
		continue;
		}*/
		int i = peopleIndex[n];
		std::vector<Mat> perPerson;
		for (int j = 0; j < numRandomPhotos; j++) {
			ostringstream oss;
			oss << lookPhotosDirPath << i << "/Ran/" << i << "-" << j << ".jpg";
			Mat randomPhoto = imread(oss.str());
			if (randomPhoto.data == NULL) {
				cerr << "Read the random photo: " << oss.str() << " error" << endl;
				return false;
			}
			Mat resizeRandomPhoto;
			resize(randomPhoto, resizeRandomPhoto, pictures[0].size(), 0, 0, INTER_AREA);

			perPerson.push_back(resizeRandomPhoto);
		}
		imagesPool->randomPhotos.push_back(perPerson);
	}

	{
		ostringstream oss;
		oss << smileFacePhotosDirPath;
		Mat smilePhoto = imread(oss.str());
		if (smilePhoto.data == NULL) {
			cerr << "Read the smile face photo: " << oss.str() << " error" << endl;
			return false;
		}
		Mat resizeSmilePhoto;
		resize(smilePhoto, resizeSmilePhoto, pictures[0].size(), 0, 0, INTER_AREA);

		imagesPool->smileFacePhotos.push_back(resizeSmilePhoto);
	}

	// Read all looks' photos
	for (int n=0; n<numLookPeople; n++) {
		/*if (n == 43) {
		continue;
		}*/
		int i = peopleIndex[n];
		//int i = n;
		//N->LU->U->RU->L->C->R->LD->D->RD, smooth
		std::vector<std::vector<cv::Mat>> perPersonSet;
		//add neutral set
		std::vector<cv::Mat> nLookSet;
		for (int j = 0; j < numLookNeu; ++j) {
			ostringstream oss;
			oss << lookPhotosDirPath << i << "/Neu/" 
				<< i << "-" << j << ".jpg";;
			Mat nLookPhoto = imread(oss.str());
			if (nLookPhoto.data == NULL) {
				cerr << "Read the Neutral look photo: " << oss.str() << " error" << endl;
				return false;
			}
			Mat resizeNLookPhoto;
			resize(nLookPhoto, resizeNLookPhoto, pictures[0].size(), 0, 0, INTER_AREA);

			nLookSet.push_back(resizeNLookPhoto);
		}
		perPersonSet.push_back(nLookSet);

		//add LU set
		std::vector<cv::Mat> luLookSet;
		for (int j = 0; j < numPerLookPerPersonPhotos; ++j) {
			ostringstream oss;
			oss << lookPhotosDirPath << i << "/left_down/" 
				<< i << "-" << j << ".jpg";;
			Mat luLookPhoto = imread(oss.str());
			if (luLookPhoto.data == NULL) {
				cerr << "Read the left up look photo: " << oss.str() << " error" << endl;
				return false;
			}
			Mat resizeLULookPhoto;
			resize(luLookPhoto, resizeLULookPhoto, pictures[0].size(), 0, 0, INTER_AREA);

			luLookSet.push_back(resizeLULookPhoto);
		}
		perPersonSet.push_back(luLookSet);

		//add U set
		std::vector<cv::Mat> uLookSet;
		for (int j = 0; j < numPerLookPerPersonPhotos; ++j) {
			ostringstream oss;
			oss << lookPhotosDirPath << i << "/down/" 
				<< i << "-" << j << ".jpg";
			Mat uLookPhoto = imread(oss.str());
			if (uLookPhoto.data == NULL) {
				cerr << "Read the up look photo: " << oss.str() << " error" << endl;
				return false;
			}
			Mat resizeULookPhoto;
			resize(uLookPhoto, resizeULookPhoto, pictures[0].size(), 0, 0, INTER_AREA);

			uLookSet.push_back(resizeULookPhoto);
		}
		perPersonSet.push_back(uLookSet);

		//add RU set
		std::vector<cv::Mat> ruLookSet;
		for (int j = 0; j < numPerLookPerPersonPhotos; ++j) {
			ostringstream oss;
			oss << lookPhotosDirPath << i << "/right_down/" 
				<< i << "-" << j << ".jpg";
			Mat ruLookPhoto = imread(oss.str());
			if (ruLookPhoto.data == NULL) {
				cerr << "Read the right up look photo: " << oss.str() << " error" << endl;
				return false;
			}
			Mat resizeRULookPhoto;
			resize(ruLookPhoto, resizeRULookPhoto, pictures[0].size(), 0, 0, INTER_AREA);

			ruLookSet.push_back(resizeRULookPhoto);
		}
		perPersonSet.push_back(ruLookSet);

		//add L set
		std::vector<cv::Mat> lLookSet;
		for (int j = 0; j < numPerLookPerPersonPhotos; ++j) {
			ostringstream oss;
			oss << lookPhotosDirPath << i << "/left/" 
				<< i << "-" << j << ".jpg";
			Mat lLookPhoto = imread(oss.str());
			if (lLookPhoto.data == NULL) {
				cerr << "Read the left look photo: " << oss.str() << " error" << endl;
				return false;
			}
			Mat resizeLLookPhoto;
			resize(lLookPhoto, resizeLLookPhoto, pictures[0].size(), 0, 0, INTER_AREA);

			lLookSet.push_back(resizeLLookPhoto);
		}
		perPersonSet.push_back(lLookSet);

		//add C set
		std::vector<cv::Mat> cLookSet;
		for (int j = 0; j < numPerLookPerPersonPhotos; ++j) {
			ostringstream oss;
			oss << lookPhotosDirPath << i << "/center/" 
				<< i << "-" << j << ".jpg";
			Mat cLookPhoto = imread(oss.str());
			if (cLookPhoto.data == NULL) {
				cerr << "Read the center look photo: " << oss.str() << " error" << endl;
				return false;
			}
			Mat resizeCLookPhoto;
			resize(cLookPhoto, resizeCLookPhoto, pictures[0].size(), 0, 0, INTER_AREA);

			cLookSet.push_back(resizeCLookPhoto);
		}
		perPersonSet.push_back(cLookSet);

		//add R set
		std::vector<cv::Mat> rLookSet;
		for (int j = 0; j < numPerLookPerPersonPhotos; ++j) {
			ostringstream oss;
			oss << lookPhotosDirPath << i << "/right/" 
				<< i << "-" << j << ".jpg";
			Mat rLookPhoto = imread(oss.str());
			if (rLookPhoto.data == NULL) {
				cerr << "Read the right look photo: " << oss.str() << " error" << endl;
				return false;
			}
			Mat resizeRLookPhoto;
			resize(rLookPhoto, resizeRLookPhoto, pictures[0].size(), 0, 0, INTER_AREA);

			rLookSet.push_back(resizeRLookPhoto);
		}
		perPersonSet.push_back(rLookSet);

		//add LD set
		std::vector<cv::Mat> ldLookSet;
		for (int j = 0; j < numPerLookPerPersonPhotos; ++j) {
			ostringstream oss;
			oss << lookPhotosDirPath << i << "/left_up/" 
				<< i << "-" << j << ".jpg";
			Mat ldLookPhoto = imread(oss.str());
			if (ldLookPhoto.data == NULL) {
				cerr << "Read the left down look photo: " << oss.str() << " error" << endl;
				return false;
			}
			Mat resizeLDLookPhoto;
			resize(ldLookPhoto, resizeLDLookPhoto, pictures[0].size(), 0, 0, INTER_AREA);

			ldLookSet.push_back(resizeLDLookPhoto);
		}
		perPersonSet.push_back(ldLookSet);

		//add D set
		std::vector<cv::Mat> dLookSet;
		for (int j = 0; j < numPerLookPerPersonPhotos; ++j) {
			ostringstream oss;
			oss << lookPhotosDirPath << i << "/up/" 
				<< i << "-" << j << ".jpg";
			Mat dLookPhoto = imread(oss.str());
			if (dLookPhoto.data == NULL) {
				cerr << "Read the down look photo: " << oss.str() << " error" << endl;
				return false;
			}
			Mat resizeDLookPhoto;
			resize(dLookPhoto, resizeDLookPhoto, pictures[0].size(), 0, 0, INTER_AREA);

			dLookSet.push_back(resizeDLookPhoto);
		}
		perPersonSet.push_back(dLookSet);

		//add RD set
		std::vector<cv::Mat> rdLookSet;
		for (int j = 0; j < numPerLookPerPersonPhotos; ++j) {
			ostringstream oss;
			oss << lookPhotosDirPath << i << "/right_up/" 
				<< i << "-" << j << ".jpg";
			Mat rdLookPhoto = imread(oss.str());
			if (rdLookPhoto.data == NULL) {
				cerr << "Read the right up look photo: " << oss.str() << " error" << endl;
				return false;
			}
			Mat resizeRDLookPhoto;
			resize(rdLookPhoto, resizeRDLookPhoto, pictures[0].size(), 0, 0, INTER_AREA);

			rdLookSet.push_back(resizeRDLookPhoto);
		}
		perPersonSet.push_back(rdLookSet);

		//add smooth set
		for (int j=0; j<1; j++) {
			for (int k=j+1; k<10; k++) {
				std::vector<cv::Mat> smoothLookSet;
				for (int m = 1; m < numLookSmooth+1; ++m) {
					ostringstream oss;
					oss << lookPhotosDirPath << i << "/" << j << "-" << k << "/" 
						<< i << "-" << setfill('0') << setw(2) << m << ".bmp";
					Mat smLookPhoto = imread(oss.str());
					if (smLookPhoto.data == NULL) {
						cerr << "Read the smooth look photo: " << oss.str() << " error" << endl;
						return false;
					}
					Mat resizeSMLookPhoto;
					resize(smLookPhoto, resizeSMLookPhoto, pictures[0].size(), 0, 0, INTER_AREA);

					smoothLookSet.push_back(resizeSMLookPhoto);
				}
				perPersonSet.push_back(smoothLookSet);
			}
		}
		printf("%d\n", imagesPool->allLookPhotos.size());
		imagesPool->allLookPhotos.push_back(perPersonSet);


	}

	// Read all video' photos
	for (int j=0; j< 0;j++) {
		int index[] = {27, 47, 48, 49, 50, 51, 52, 57, 58, 59, 60, 62, 63, 64, 65, 66, 67, 68, 69, 70, 166, 168, 162, 134, 131, 109, 20};
		std::vector<cv::Mat> videoSet;
		for (int i = 1; i <= numUserPhotos; ++i) {
			ostringstream oss;
			oss << "D:\\SmileVideo\\Photo\\video" << index[j] <<"_" << i << ".jpg";
			Mat videoPhoto = imread(oss.str());
			if (videoPhoto.data == NULL) {
				cerr << "Read the video photo: " << oss.str() << " error" << endl;
				return false;
			}
			Mat resizeVideoPhoto;
			resize(videoPhoto, resizeVideoPhoto, pictures[0].size());

			videoSet.push_back(resizeVideoPhoto);
		}
		imagesPool->userPhotos.push_back(videoSet);
		videoNum++;
	}

	// Read all buddhas' photos
	
	for (int i = 0; i < numBuddhaPhotos; ++i) {
		Mat buddhaPhoto;
		ostringstream oss;
		if (i == 0) {
			oss << buddhaPhotosDirPath << 1 << ".png";
			buddhaPhoto = imread(oss.str(), CV_LOAD_IMAGE_UNCHANGED);
			if (buddhaPhoto.data == NULL) {
				cerr << "Read the buddha photo: " << oss.str() << " error" << endl;
				return false;
			}
			imagesPool->buddhaPhotos.push_back(buddhaPhoto);
		}
		else {
			oss << buddhaPhotosDirPath << i+1 << ".png";
			buddhaPhoto = imread(oss.str());
			if (buddhaPhoto.data == NULL) {
				cerr << "Read the buddha photo: " << oss.str() << " error" << endl;
				return false;
			}

			Mat resizeBuddhaPhoto;
			resize(buddhaPhoto, resizeBuddhaPhoto, pictures[0].size(), 0, 0, INTER_AREA);

			imagesPool->buddhaPhotos.push_back(resizeBuddhaPhoto);
		}
	}

	// Read slot user photos
	ostringstream ossSlotFileName;
	ossSlotFileName << smileUserDirPath << "slot.txt";
	ifstream slotFile;
	slotFile.open(ossSlotFileName.str(), ifstream::in);
	
	while (!slotFile.eof()) {
		string slotLine;
		getline(slotFile, slotLine);
		SlotUser user;
		int videoIndex;
		stringstream ss;
		ss << slotLine;
		ss >> user.index;
		ss >> videoIndex;
		if (!videoIndex)
			continue;
		for (int i = 1; i <= numUserPhotos; ++i) {
			ostringstream oss;
			oss << smileUserDirPath << "/video" << videoIndex << "_" << i << ".jpg";
			Mat smileVideoPhoto = imread(oss.str());
			if (smileVideoPhoto.data == NULL) {
				cerr << "Read the smile video photo: " << oss.str() << " error" << endl;
				return false;
			}
			Mat resizeSmileVideoPhoto;
			resize(smileVideoPhoto, resizeSmileVideoPhoto, pictures[0].size(), 0, 0, INTER_AREA);
			user.photos.push_back(resizeSmileVideoPhoto);
		}
		imagesPool->slotUserPhotos.push_back(user);
	}
	slotFile.close();


	// Read all smile video photos
	for (int i = 1; i <= numSmileVideos - imagesPool->slotUserPhotos.size(); ++i) {
		std::vector<cv::Mat> smileVideoSet;
		for (int j = 1; j <= numUserPhotos; ++j) {
			ostringstream oss;
			oss << smileUserDirPath << "/video" << i << "_" << j << ".jpg";
			Mat smileVideoPhoto = imread(oss.str());
			if (smileVideoPhoto.data == NULL) {
				cerr << "Read the smile video photo: " << oss.str() << " error" << endl;
				return false;
			}
			Mat resizeSmileVideoPhoto;
			resize(smileVideoPhoto, resizeSmileVideoPhoto, pictures[0].size(), 0, 0, INTER_AREA);

			smileVideoSet.push_back(resizeSmileVideoPhoto);
		}
		imagesPool->smileUserPhotos.push_back(smileVideoSet);
	}

	bool slotTaken[36];

	for (int i = 0; i < numSmileVideos; ++i)
		slotTaken[i] = false;

	imagesPool->smileVideoPhotos.resize(numSmileVideos);

	for (int i = 0; i < imagesPool->slotUserPhotos.size(); ++i) {
		imagesPool->smileVideoPhotos.at(imagesPool->slotUserPhotos.at(i).index) = imagesPool->slotUserPhotos.at(i).photos;
		slotTaken[imagesPool->slotUserPhotos.at(i).index] = true;
	}

	int userIndex = 0;
	for (int i = 0; i < numSmileVideos; ++i) {
		if (!slotTaken[slotIndex[i]]) {
			imagesPool->smileVideoPhotos.at(slotIndex[i]) = imagesPool->smileUserPhotos.at(userIndex);
			userIndex = userIndex + 1;
		}
	}

	return true;
}

//bool Director::initCamera()
//{
//	if (!capture.isOpened()) {
//		cerr << "Error: Can not open camera!" << endl;
//		return false;
//	}
//
//	return true;
//}

void Director::processGlobalKeyEvent(const int key)
{
	switch(key) {
	case 'r': case 'R':
		setAnimationState(new InitState);
		break;
	case 'l': case 'L':
		setAnimationState(new LayoutState);
		break;
	case 'c': case 'C':
		setAnimationState(new WebcamRoiState);
		break;
	default:
		break;
	}
}

void Director::processGlobalOSC()
{
	if (oscListener->interactionState == INTERACTION_END) {
		/*ostringstream oss;
		oss << smileUserDirPath << "time.txt";
		ofstream timeFile(oss.str(), ofstream::app);
		timeFile << "STANDUP: " << double(getCurrentTime())/getTickFrequency() << endl <<endl;
		timeFile.close();
		*/
		int num = imagesPool->currentUserPhotos.size();
		if (num > 0) {
			imagesPool->userPhotos.push_back(imagesPool->currentUserPhotos[num-1]);
			imagesPool->smileUserPhotos.insert(imagesPool->smileUserPhotos.begin(), imagesPool->currentUserPhotos[userTopIndex]);
			imagesPool->smileUserPhotos.pop_back();
			videoNum++;
		}

		bool slotTaken[36];

		for (int i = 0; i < numSmileVideos; ++i)
			slotTaken[i] = false;

		for (int i = 0; i < imagesPool->slotUserPhotos.size(); ++i) {
			imagesPool->smileVideoPhotos.at(imagesPool->slotUserPhotos.at(i).index) = imagesPool->slotUserPhotos.at(i).photos;
			slotTaken[imagesPool->slotUserPhotos.at(i).index] = true;
		}

		int userIndex = 0;
		for (int i = 0; i < numSmileVideos; ++i) {
			if (!slotTaken[slotIndex[i]]) {
				imagesPool->smileVideoPhotos.at(slotIndex[i]) = imagesPool->smileUserPhotos.at(userIndex);
				userIndex = userIndex + 1;
			}
		}

		imagesPool->currentUserPhotos.clear();
		//imagesPool->crazyUserPhotos.clear();
		currentVideoNum = 0;
		for (int i=0; i<userTopNum; i++) {
			if (userTopScore[i] != 0) {
				userTopScore[i] = 0;
			}
		}
		sendOSCBoolMessage("localhost", smileDatectionPort, "/interaction", false);
		sendOSCBoolMessage("localhost", musicPlayerPort, "/fade23", true);
		setAnimationState(new InitState);
	}
}

/*
void Director::sendTopUserVideo () {
	for (int i=0; i<userTopNum; i++) {
		if (userTopScore[i] != 0) {
			cout << "userTop" << i << ": " << userTopPath[i] << "_" << userTopScore[i] << "\n";
			sendOSCStringMessage("localhost", outWallControllerOSCPort, "/photoPath", userTopPath[i].c_str());
			userTopScore[i] = 0;
		}
	}
}
*/
//void Director::record()
//{
//	if (!canRecord)
//		return;
//
//	capture >> recordImage;
//	resize(recordImage(webcamRoi), recordImage, pictureDefaultSize);
//}

std::vector<Picture>::iterator Director::getPictureAt(const unsigned int &index) 
{
	std::vector<Picture>::iterator iter = pictures.begin();
	if (index >= pictures.size()) {
		std::cerr << "row: " << index / numPicturesInPainting.width << 
			", col: " << index % numPicturesInPainting.width << 
			" is out of bound!" << std::endl;
		return iter;
	}

	iter += index;
	return iter;
}

void Director::setAnimationState(AnimationState *newAnimationState, 
	const bool &clearAnimation) {
		if (animationState != NULL)
			delete animationState;
		animationState = newAnimationState;

		if (clearAnimation) {
			for (vector<Picture>::iterator iter = pictures.begin(); iter != pictures.end(); ++iter)
				iter->setAnimation(IdleAnimationEnum, cv::noArray(), cv::noArray());
		}
}

vector<Picture>::iterator Director::getPictureAt(
	const unsigned int &row, const unsigned int &col)
{
	int index = col * numPicturesInPainting.width + row;
	return getPictureAt(index);
}

void Director::takePhoto(OutputArray &photo)
{
	Mat image;
	takePhotoByCamera(image);
	image.convertTo(photo, -1);
}

void Director::takePhotoByCamera(cv::Mat& photo)
{
	captureImage = Mat();
#if SIMULATOR == 1
	capture >> captureImage;
#else
	sendOSCBoolMessage("localhost", cameraOSCPort, "/takePhoto", true);
	//	while(captureImage.empty()) {
	//		string photoPath = oscListener->takePhotoPath;
	//		Sleep(delay);
	//		if (!photoPath.empty()) {
	//			captureImage = imread(photoPath);
	//			photoPath.clear();
	//		}
	//	}
#endif
	//	oscListener->takePhotoPath.clear(); // Clear path after get photo for waiting next path
	//	resize(captureImage, captureImage, pictureDefaultSize);
	//	photo = captureImage;
	//	captureImageVec.push_back(captureImage);
}

bool Director::takeVideoFromSmileDetect() {
	string videoPath = oscListener->takeVideoPath;
	float grade = oscListener->grade;
	if (!readVideoFin || videoPath.empty() || !oscListener->newVideo) {
		return false;
	}
	//cout << "receiveVideo: " videoPath << " " << score << "\n";
	oscListener->newVideo = false;
	readVideoFin = false;
	data.currentVideoNum = &currentVideoNum;
	//data.videoNum = &videoNum;
	data.readVideoFin = &readVideoFin;
	data.currentUserpool = &(imagesPool->currentUserPhotos);
	//data.crazyUserpool = &(imagesPool->crazyUserPhotos);
	//data.userpool = &(imagesPool->userPhotos);
	data.size = pictures[0].size();
	unsigned found = videoPath.rfind("/");
	string stablePath = string(videoPath);
	if (found!=std::string::npos)
		stablePath.insert(found, "2");
	data.path = stablePath;
	data.startTime = oscListener->startReadTime;
	//data.crazyPath = videoPath;
	_beginthread(&Director::threadToReadVideo, 0, (void*)&data);
	unsigned found2 = stablePath.rfind("o");
	string outWallPath = string(stablePath);
	if (found2!=std::string::npos)
		outWallPath = outWallPath.substr(found2+1);
	cout << "OSC: " << stablePath << "_" << grade << "\n";
	for (int i=0; i<userTopNum; i++) {
		if (grade >= userTopScore[i]) {
			for (int j=userTopNum-1; j>i; j--) {
				userTopPath[j] = userTopPath[j-1];
				userTopScore[j] = userTopScore[j-1];
			}
			userTopPath[i] = outWallPath;
			userTopScore[i] = grade;
			userTopIndex = 0;
			break;
		}
		else {
			userTopIndex++;
			break;
		}

	}
	//sendOSCStringMessage("localhost", outWallControllerOSCPort, "/photoPath", outWallPath.c_str());
	return true;
}

void Director::threadToReadVideo( void* arg ) {
	ReadVideoData videoData = *((ReadVideoData*)arg);
	std::vector<Mat> userPhotos;
	//std::vector<Mat> crazyPhotos;
	for (int n = 1; n <= numUserPhotos; n++) {
		ostringstream oss1, oss2;
		oss1 << videoData.path << "_" << n << ".jpg";
		//oss2 << videoData.crazyPath << "_" << n << ".jpg";
		Mat userPhoto = imread(oss1.str());
		//Mat crazyPhoto = imread(oss2.str());
		if (userPhoto.data == NULL) {
			cerr << "Read the user photo: " << oss1.str() << " error" << endl;
			*(videoData.readVideoFin) = true;
			return;
		}
		Mat resizedUserPhoto;
		//Mat resizedCrazyUserPhoto;
		resize(userPhoto, resizedUserPhoto, videoData.size, 0, 0, INTER_AREA);
		//resize(crazyPhoto, resizedCrazyUserPhoto, videoData.size, 0, 0, INTER_AREA);
		userPhotos.push_back(resizedUserPhoto);
		//crazyPhotos.push_back(resizedCrazyUserPhoto);
	}
	(*(videoData.currentUserpool)).push_back(userPhotos);
	//(*(videoData.crazyUserpool)).push_back(crazyPhotos);
	//(*(videoData.userpool)).push_back(userPhotos);
	*(videoData.currentVideoNum) += 1;
	//*(videoData.videoNum) += 1;
	//cout << "read need " << cv::getTickCount() / cv::getTickFrequency() - videoData.startTime << "s\n";
	*(videoData.readVideoFin) = true;
	//cout<<"this is new thread.\n";
}

void Director::playMusic(const std::string &musicPath, const bool loop, const bool stop) {
	if (loop) {
		if (stop)
			PlaySound(musicPath.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		else {
			ostringstream oss;
			oss << "play " << musicPath << " repeat";
			mciSendString(oss.str().c_str(), NULL, 0, NULL);
		}
	}
	else {
		if (stop)
			PlaySound(musicPath.c_str(), NULL, SND_FILENAME | SND_ASYNC);
		else {
			ostringstream oss;
			oss << "play " << musicPath;
			mciSendString(oss.str().c_str(), NULL, 0, NULL);
		}
	}
}

void Director::stopMusic() {

}

//void Director::playMusic(const std::string &musicPath, const bool loop, const bool stop)
//{
//	if (stop) {
//		stopMusic();
//	}
//	mciOpen.lpstrDeviceType =
//		(LPCSTR) MCI_DEVTYPE_WAVEFORM_AUDIO;
//	mciOpen.lpstrElementName = musicPath.c_str();
//	if(mciSendCommand((MCIDEVICEID)NULL, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD)&mciOpen) == 0)
//		dev= mciOpen.wDeviceID;
//	else
//		mciOpen.wDeviceID = dev;
//
//	mciPlay.dwFrom = 0;
//
//	// MCI_DGV_PLAY_REPEAT is useful for mp3, and is unuseful for wav.
//	if (loop)
//		mciSendCommand(mciOpen.wDeviceID, MCI_PLAY, MCI_NOTIFY | MCI_FROM | MCI_DGV_PLAY_REPEAT, (DWORD)&mciPlay);
//	else
//		mciSendCommand(mciOpen.wDeviceID, MCI_PLAY, MCI_NOTIFY | MCI_FROM, (DWORD)&mciPlay);
//}
//
//void Director::stopMusic()
//{
//	mciOpen.wDeviceID = dev;
//	mciSendCommand(mciOpen.wDeviceID, MCI_CLOSE, (DWORD)NULL, (DWORD)NULL);
//}
//
//void Director::setVolume(int volume)
//{
//	mciOpen.wDeviceID = dev;
//
//	MCI_DGV_SETAUDIO_PARMS	mciSetAudioPara;
//	mciSetAudioPara.dwCallback	= (DWORD)0;
//	mciSetAudioPara.dwItem	 = MCI_DGV_SETAUDIO_VOLUME;
//	mciSetAudioPara.dwValue	 = volume; // Volume: (0-1000)
//	mciSendCommand(mciOpen.wDeviceID, MCI_SETAUDIO, MCI_DGV_SETAUDIO_VALUE |  MCI_DGV_SETAUDIO_ITEM, 
//		(DWORD)(LPVOID)&mciSetAudioPara);
//}

//int Director::getVolume(int index)
//{
//	mciOpen.wDeviceID = dev[index];
//
//	MCI_STATUS_PARMS mciStatusPara = {0};
//	mciStatusPara.dwItem = MCI_DGV_STATUS_VOLUME;
//	mciSendCommand(mciOpen.wDeviceID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)&mciStatusPara);
//
//	return mciStatusPara.dwReturn;
//}

//void Director::fadeInMusic(int index, double fadeStartTime, double sec)
//{
//	int volume = getVolume(index);
//	//printf("%d  ", volume);
//	if (volume < MAX_VOLUME) {
//		double duration = getTickCount() / getTickFrequency() - fadeStartTime;
//		int newVolume = static_cast<int>(duration * MAX_VOLUME / sec);
//		setVolume(index, min(MAX_VOLUME, newVolume));
//		//printf("fadeIn: %d || %f\n", newVolume, duration);
//	}
//}
//
//void Director::fadeOutMusic(int index, double fadeStartTime, double sec)
//{
//	int volume = getVolume(index);
//	if (volume > 0) {
//		double duration = getTickCount() / getTickFrequency()- fadeStartTime;
//		int newVolume = static_cast<int>(MAX_VOLUME - duration * MAX_VOLUME / sec);
//		setVolume(index, max(0, newVolume));
//		//printf("fadeOut: %d || %f\n", newVolume, duration);
//	}g
//}

void Director::clearOscVideoInfo() {
	oscListener->newVideo = false;
	cout << oscListener->newVideo << "\n";
	cout << oscListener->takeVideoPath << "\n";
}

Point2f Director::getEyePosWithOSCMessage() const
{
	return oscListener->eyePos;
}

bool Director::getSmileStateWithOSCMessage() const
{
	return oscListener->smileState;
}

int Director::get1DEyePos(const Point2f &normalEyePos) const
{
	Point2i realEyePos;
	realEyePos.x = int(normalEyePos.x * numPicturesInPainting.width);
	realEyePos.y = int(normalEyePos.y * numPicturesInPainting.height);
	if (realEyePos.x < 0)
		realEyePos.x = 0;
	if (realEyePos.y < 0)
		realEyePos.y = 0;
	if (realEyePos.x > numPicturesInPainting.width - 1)
		realEyePos.x = numPicturesInPainting.width - 1;
	if (realEyePos.y > numPicturesInPainting.height - 1)
		realEyePos.y = numPicturesInPainting.height - 1;
	int index = realEyePos.x + realEyePos.y * numPicturesInPainting.width;

	// old method
	/*
	Point2f realEyePos(normalEyePos.x * paintingSize.width, 
		normalEyePos.y * paintingSize.height);
	float minDistance = FLT_MAX;
	int index = 0;
	for (int i = 0; i < numPhotos; ++i) {
		Rect frame = pictures[i].getFrame();
#if MORE_FOR_UP_AND_DOWN == 1
		if (i/9 == 0) {
			frame.height += 115;
		} else if (i/9 == 1) {
			frame.y += 75;
			frame.height -= 75;
		} else if (i/9 == 2) {
			frame.y -= 40;
			frame.height -= 50;
		} else if (i/9 == 3) {
			frame.y -= 130;
			frame.height += 130;
		}
		if (i%9 == 0) {
			frame.width += 62;
		} else if (i%9 == 1) {
			frame.x += 32;
			frame.width -= 32;
		} else if (i%9 == 7) {
			frame.width -= 32;
		} else if (i%9 == 8) {
			frame.x -= 62;
			frame.width += 62;
		}
#endif
		if (realEyePos.x >= frame.x && realEyePos.x <= frame.x + frame.width &&
			realEyePos.y >= frame.y && realEyePos.y <= frame.y + frame.height)
		{
			return i;
		}

		Vec2d vec(realEyePos.x - (frame.x + frame.width / 2),
			realEyePos.y - (frame.y + frame.height / 2));
		float distance = static_cast<float>(norm(vec));
		if (distance < minDistance) {
			index = i;
			minDistance = distance;
		}
	}
	*/
	return index;
}

Point Director::get2DEyePos(const Point2f &normalEyePos) const
{
	int index = get1DEyePos(normalEyePos);
	return Point(index % numPicturesInPainting.width, index / numPicturesInPainting.width);
}

void Director::sendOSCBoolMessage(const string &ipAddress, const int &port, 
	const string &name, const bool &message)
{
	UdpTransmitSocket updTransmitSocket( IpEndpointName(ipAddress.c_str(), port) );
	char buffer[oscMessageBufferSize];
	OutboundPacketStream packetStream( buffer, oscMessageBufferSize );

	packetStream << BeginMessage( name.c_str() ) << message  << EndMessage;
	updTransmitSocket.Send( packetStream.Data(), packetStream.Size() );
}

void Director::sendOSCStringMessage(const string &ipAddress, const int &port, 
	const string &name, const char* message)
{
	UdpTransmitSocket updTransmitSocket( IpEndpointName(ipAddress.c_str(), port) );
	char buffer[oscMessageBufferSize];
	OutboundPacketStream packetStream( buffer, oscMessageBufferSize );

	packetStream << BeginMessage( name.c_str() ) << message  << EndMessage;
	updTransmitSocket.Send( packetStream.Data(), packetStream.Size() );
}

vector<int> Director::genPermutations(map<int, int> &rules)
{
	vector<int> permutations;
	for (int i = 0; i < numPhotos; ++i)
		permutations.push_back(i);

	for (map<int, int>::iterator mapIterator = rules.begin(); 
		mapIterator != rules.end(); mapIterator++) {
			swap(permutations[mapIterator->first], permutations[mapIterator->second]);
	}

	for (int i = 0; i < 3 * numPhotos; ++i) {
		int index1 = rand() % numPhotos;
		int index2 = rand() % numPhotos;

		if (rules.find(index1) != rules.end() || rules.find(index2) != rules.end())
			continue;

		swap(permutations[index1], permutations[index2]);
	}

	return permutations;
}
