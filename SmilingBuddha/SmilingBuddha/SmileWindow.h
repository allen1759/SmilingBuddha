/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _SMILE_WINDOW_H
#define _SMILE_WINDOW_H

#include <memory>

#include <opencv2/core/core.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "VirtualCamera.h"

class SmileWindow
{
public:
	SmileWindow();

	~SmileWindow();

	void Draw();

	void SetPosition(int x, int y);

	void SetTexture(std::shared_ptr<cv::Mat> textureMat);

	//void UpdateTexture();

	static void SetScale(int w, int h);

	static glm::vec2 GetScale();

private:
	VirtualCamera *virtualCamera;

	float vertices[12] = {
		-0.5f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f
	};

	float textureCoords[8] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	short indices[6] = { 0, 1, 2, 0, 2, 3 };

	char *vertexShaderCode =
		"#version 400\n"
		"attribute vec3 aPosition;"
		"attribute vec2 aTexCoord;"
		"varying vec2 vTexCoord;"
		"void main() {"
		"  vTexCoord = aTexCoord;"
		"  gl_Position = vec4(aPosition, 1.0);"
		"}";

	char *fragShaderCode =
		"#version 400\n"
		"uniform sampler2D uTexture;"
		"varying vec2 vTexCoord;"
		"void main() {"
		"  gl_FragColor = texture2D(uTexture, vTexCoord);"
		"}";
	
	GLuint vertexBufferHandle;
	GLuint textureCoordHandle;
	GLuint textureHandle;
	GLuint shaderProgram;

	const int positionZ = 0;
	glm::vec3 position;
	static const int scaleZ = 1;
	static glm::vec3 scale;
};

#endif // !_SMILE_WINDOW_H
