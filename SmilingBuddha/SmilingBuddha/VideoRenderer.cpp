/**
* Created by Yu-Ci Huang <j810326@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "VideoRenderer.h"

#include <iostream>
#include <chrono>

#include "Setting.h"

VideoRenderer *VideoRenderer::instance = NULL;

VideoRenderer * VideoRenderer::GetInstance()
{
	if (instance == NULL)
		instance = new VideoRenderer();

	return instance;
}

VideoRenderer::VideoRenderer()
	: RESOLUTION_WIDTH(Setting::GetInstance()->GetResolutionWidth()),
	  RESOLUTION_HEIGHT(Setting::GetInstance()->GetResolutionHeight())
{
	glfwInit();
	window = glfwCreateWindow(RESOLUTION_WIDTH, RESOLUTION_HEIGHT, "OpenGL", NULL, NULL);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	glGenBuffers(1, &vertexBufferHandle);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &texCoordBufferHandle);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), texCoords, GL_STATIC_DRAW);

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, 0);
	glCompileShader(vertexShader);

	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragmentShaderCode, 0);
	glCompileShader(fragShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);

	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

VideoRenderer::~VideoRenderer()
{
}

void VideoRenderer::SetVideo(Video *video)
{
	this->video = video;
}

void VideoRenderer::RenderLoop()
{
	std::chrono::high_resolution_clock::time_point start;
	std::chrono::high_resolution_clock::time_point end;
	std::chrono::microseconds duration;

	while (!glfwWindowShouldClose(window)) {
		start = std::chrono::high_resolution_clock::now();
		// clear window
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);

		glUseProgram(shaderProgram);
		int positionHandle = glGetAttribLocation(shaderProgram, "aPosition");
		glEnableVertexAttribArray(positionHandle);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
		glVertexAttribPointer(positionHandle, 3, GL_FLOAT, false, 0, 0);

		int texureHandle = glGetAttribLocation(shaderProgram, "aTexCoord");
		glEnableVertexAttribArray(texureHandle);
		glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
		glVertexAttribPointer(texureHandle, 2, GL_FLOAT, false, 0, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		
		//if (video) {
		//	std::shared_ptr<cv::Mat> videoFrame = video->GetFrame();
		//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, videoFrame->cols, videoFrame->rows, 0, GL_BGR, GL_UNSIGNED_BYTE, videoFrame->data);
		//}
		
		int sampler = glGetUniformLocation(shaderProgram, "uTexture");
		glUniform1i(sampler, 0);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

		glfwSwapBuffers(window);
		glfwPollEvents();

		end = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
		std::cout << "RENDER LOOP: " << duration.count() << std::endl;
	}
}

