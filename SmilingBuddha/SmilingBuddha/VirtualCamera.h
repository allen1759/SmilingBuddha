/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _VIRTUAL_CAMERA_H
#define _VIRTUAL_CAMERA_H

class VirtualCamera
{
private:
	VirtualCamera();

public:
	~VirtualCamera();

	static VirtualCamera * GetInstance();

private:
	static VirtualCamera *instance;

};

#endif // !_VIRTUAL_CAMERA_H
