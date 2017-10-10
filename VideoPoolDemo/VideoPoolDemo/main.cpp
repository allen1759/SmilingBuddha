/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "../../SmilingBuddha/SmilingBuddha/VideoPool.h"


std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> GetBlendingSequence(const std::shared_ptr<cv::Mat> mat1, const std::shared_ptr<cv::Mat> mat2)
{
	std::shared_ptr<std::vector<std::shared_ptr<cv::Mat>>> arr = std::make_shared<std::vector<std::shared_ptr<cv::Mat>>>();
	const int SIZE = 15;
	for (int i = 0; i < SIZE; ++i) {
		std::shared_ptr<cv::Mat> dst = std::make_shared<cv::Mat>();
		double weight = static_cast<double>(i) / (SIZE - 1);
		cv::addWeighted(*mat1, (1 - weight), *mat2, weight, 0.0, *dst);
		arr->push_back(dst);
	}

	return arr;
}

int main()
{
	VideoPool * videoPool = VideoPool::GetInstance();
	
	std::vector<std::shared_ptr<const std::vector<std::shared_ptr<cv::Mat>>>> videos;
	std::vector<cv::Mat> windows(36);

	const int ROWS = 4;
	const int COLS = 9;

	// Smile Video Demo
	//for (int i = 0; i < ROWS; ++i) {
	//	for (int j = 0; j < COLS; ++j) {
	//		videos.push_back(videoPool->GetSmileVideoList(i, j));
	//	}
	//}


	// Direction Video Demo
	for (int i = 0; i < 1; ++i) {
		for (int j = 0; j < COLS; ++j) {
			std::shared_ptr<const std::vector<std::shared_ptr<cv::Mat>>> tmp;
			std::vector<std::shared_ptr<cv::Mat>> arr;
			tmp = videoPool->GetDirectionVideo(i, j, ActorVideoSet::NEUTRAL);
			arr.insert(arr.end(), tmp->rbegin(), tmp->rend());
			arr.insert(arr.end(), tmp->begin(), tmp->end());

			tmp = videoPool->GetMorphingVideo(i, j, ActorVideoSet::LEFT_DOWN);
			tmp = GetBlendingSequence(tmp->front(), tmp->back());
			arr.insert(arr.end(), tmp->begin(), tmp->end());
			tmp = videoPool->GetDirectionVideo(i, j, ActorVideoSet::LEFT_DOWN);
			arr.insert(arr.end(), tmp->begin(), tmp->end());
			arr.insert(arr.end(), tmp->rbegin(), tmp->rend());
			tmp = videoPool->GetMorphingVideo(i, j, ActorVideoSet::LEFT_DOWN);
			tmp = GetBlendingSequence(tmp->front(), tmp->back());
			arr.insert(arr.end(), tmp->rbegin(), tmp->rend());

			tmp = videoPool->GetMorphingVideo(i, j, ActorVideoSet::UP);
			tmp = GetBlendingSequence(tmp->front(), tmp->back());
			arr.insert(arr.end(), tmp->begin(), tmp->end());
			tmp = videoPool->GetDirectionVideo(i, j, ActorVideoSet::UP);
			arr.insert(arr.end(), tmp->begin(), tmp->end());
			arr.insert(arr.end(), tmp->rbegin(), tmp->rend());
			tmp = videoPool->GetMorphingVideo(i, j, ActorVideoSet::UP);
			tmp = GetBlendingSequence(tmp->front(), tmp->back());
			arr.insert(arr.end(), tmp->rbegin(), tmp->rend());

			videos.push_back(std::make_shared<std::vector<std::shared_ptr<cv::Mat>>>(arr));
		}
	}





	int k = 0;
	while (true) {
		for (int i = 0; i < ROWS; ++i) {
			for (int j = 0; j < COLS; ++j) {
				windows[i * COLS + j] = *videos[i * COLS + j]->at(k % videos[i * COLS + j]->size());
			}
		}

		for (int i = 0; i < ROWS; ++i) {
			for (int j = 0; j < COLS; ++j) {
				cv::imshow(std::to_string(i * COLS + j), windows[i * COLS + j]);
				cv::moveWindow(std::to_string(i * COLS + j), j * 150, i * 220);
			}
		}
		cv::waitKey(100);
		k++;
	}

	return 0;
}