/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _VIRTUAL_CAMERA_H
#define _VIRTUAL_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class VirtualCamera
{
private:
	VirtualCamera();

public:
	~VirtualCamera();

	static VirtualCamera * GetInstance();

	glm::mat4x4 GetMVPMatrix();

private:
	static VirtualCamera *instance;

	const int RESOLUTION_WIDTH = 1920;
	const int RESOLUTION_HEIGHT = 1080;

	glm::mat4 mvpMatrix;
};

#endif // !_VIRTUAL_CAMERA_H
