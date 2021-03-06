/**
* Created by Yu-Ci Huang <j810326@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#ifndef _VIDEO_RENDERER_H
#define _VIDEO_RENDERER_H

#include <mutex>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "Video.h"

class VideoRenderer
{
public:
	static VideoRenderer *GetInstance();

private:
	VideoRenderer();

public:
	~VideoRenderer();

	void SetVideo(std::shared_ptr<Video> video);

	void RenderLoop();

private:
	static VideoRenderer *instance;

	const int RESOLUTION_WIDTH;
	const int RESOLUTION_HEIGHT;
	GLFWwindow *window;

	GLuint vertexBufferHandle;
	float vertices[12] = {
		-1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f };

	GLuint texCoordBufferHandle;
	float texCoords[8] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	short indices[6] = {
		0, 1, 2,
		0, 2, 3
	};

	GLuint shaderProgram;
	char *vertexShaderCode =
		"#version 400\n"
		"attribute vec3 aPosition;"
		"attribute vec2 aTexCoord;"
		"varying vec2 vTexCoord;"
		"void main() {"
		"  vTexCoord = aTexCoord;"
		"  gl_Position = vec4(aPosition, 1.0);"
		"}";

	char *fragmentShaderCode =
		"#version 400\n"
		"uniform sampler2D uTexture;"
		"varying vec2 vTexCoord;"
		"void main() {"
		"  gl_FragColor = texture2D(uTexture, vTexCoord);"
		"}";

	GLuint texture;

	std::shared_ptr<Video> video;
	std::mutex videoMutex;
};

#endif // !_VIRTUAL_CAMERA_H
