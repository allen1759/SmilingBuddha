/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "../../SmilingBuddha/SmilingBuddha/VideoPool.h"

int main()
{
	VideoPool * videoPool = VideoPool::GetInstance();
	
	std::vector<std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>>> videos;
	std::vector<cv::Mat> windows(36);

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 9; ++j) {
			videos.push_back(videoPool->GetSmileVideoList(i, j));
		}
	}

	int k = 0;
	while (k < 1000000) {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 9; ++j) {
				windows[i * 9 + j] = *videos[i * 9 + j]->at(k % videos[i * 9 + j]->size());
				//if (k < videos[i * 9 + j]->size()) {
				//	windows[i * 9 + j] = *videos[i * 9 + j]->at(k);
				//}
				//else {
				//	windows[i * 9 + j] = *videos[i * 9 + j]->back();
				//}
			}
		}

		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 9; ++j) {
				cv::imshow(std::to_string(i * 9 + j), windows[i * 9 + j]);
				cv::moveWindow(std::to_string(i * 9 + j), j * 150, i * 200);
			}
		}
		cv::waitKey(30);
		k++;
	}

	return 0;
}