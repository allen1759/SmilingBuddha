/**
* Created by Yu-Ci Huang <j810326@gmail.com>
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _RAY_H
#define _RAY_H

#include <string>

#include <glm/glm.hpp>

class Ray {
public:
	Ray()
	{
		this->origin = glm::vec3(0.0f, 0.0f, 0.0f);
		this->direction = glm::vec3(0.0f, 0.0f, 1.0f);
	}

	Ray(glm::vec3 origin, glm::vec3 direction)
	{
		this->origin = origin;
		this->direction = direction;
	}

	void SetOrigin(glm::vec3 origin)
	{
		this->origin = origin;
	}

	void SetDirection(glm::vec3 direction)
	{
		this->direction = direction;
	}

	glm::vec3 GetOrigin()
	{
		return origin;
	}

	glm::vec3 GetDirection()
	{
		return direction;
	}

	std::string ToString()
	{
		std::string thisString = "(" + std::to_string(origin.x) + ", " + std::to_string(origin.y) + ", " + std::to_string(origin.z) + ")" + ", "
			+ "(" + std::to_string(direction.x) + ", " + std::to_string(direction.y) + ", " + std::to_string(direction.z) + ")";
		return thisString;
	}

private:
	glm::vec3 origin;
	glm::vec3 direction;
};

#endif