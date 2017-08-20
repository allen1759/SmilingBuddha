/**
* Created by Zhi-Wei Yang <allen1759@gmail.com>.
* Copyright(c) 2015-2017 imLab. All rights reserved.
*
*/

#include "SmileWindow.h"

float SmileWindow::scale[3];

SmileWindow::SmileWindow()
{
	virtualCamera = VirtualCamera::GetInstance();


	// Generate vertices buffer.
	glGenBuffers(1, &vertexBufferHandle);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices, GL_STATIC_DRAW);
	
	// Generate texture coordinate buffer.
	glGenBuffers(1, &textureCoordHandle);
	glBindBuffer(GL_ARRAY_BUFFER, textureCoordHandle);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), textureCoords, GL_STATIC_DRAW);

	// Load vertex shader and fragment shader.
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, 0);
	glCompileShader(vertexShader);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragShaderCode, 0);
	glCompileShader(fragShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);

	// Detech useless shader.
	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);


	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &textureHandle);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

SmileWindow::~SmileWindow()
{
}

void SmileWindow::Draw()
{
	glUseProgram(shaderProgram);

	int positionHandle = glGetAttribLocation(shaderProgram, "aPosition");
	glEnableVertexAttribArray(positionHandle);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
	glVertexAttribPointer(positionHandle, 3, GL_FLOAT, false, 0, 0);

	int texureHandle = glGetAttribLocation(shaderProgram, "aTexCoord");
	glEnableVertexAttribArray(texureHandle);
	glBindBuffer(GL_ARRAY_BUFFER, textureCoordHandle);
	glVertexAttribPointer(texureHandle, 2, GL_FLOAT, false, 0, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureHandle);

	int sampler = glGetUniformLocation(shaderProgram, "uTexture");
	glUniform1i(sampler, 0);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
}

void SmileWindow::SetPosition(int x, int y)
{
	position[0] = x;
	position[1] = y;
}

void SmileWindow::SetTexture(std::shared_ptr<cv::Mat> textureMat)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureMat->cols, textureMat->rows, 0, GL_BGR, GL_UNSIGNED_BYTE, textureMat->data);
}

void SmileWindow::SetScale(int w, int h)
{
	scale[0] = 0.0f;
	//scale[0] = w;
	//scale[1] = h;
}

//float * SmileWindow::GetScale()
//{
//	return SmileWindow::scale;
//}
