#include <iostream>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>

static const float vertices[12] = {
	-0.5f, 0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.5f, 0.5f, 0.0f };

static const float textureCoords[8] = {
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f
};

static const short indices[6] = { 0, 1, 2, 0, 2, 3 };

static const char *vertexShaderCode =
"#version 400\n"
"attribute vec3 aPosition;"
"attribute vec2 aTexCoord;"
"varying vec2 vTexCoord;"
"void main() {"
"  vTexCoord = aTexCoord;"
"  gl_Position = vec4(aPosition, 1.0);"
"}";

static const char *fragmentShaderCode =
"#version 400\n"
"uniform sampler2D uTexture;"
"varying vec2 vTexCoord;"
"void main() {"
"  gl_FragColor = texture2D(uTexture, vTexCoord);"
"}";

// uniform: const
// attribute: variable
// varying: interpolation

int main()
{
	// initialization of glfw
	glfwInit();
	GLFWwindow *window = glfwCreateWindow(1920, 1080, "OpenGL", NULL, NULL);
	// set the target window
	glfwMakeContextCurrent(window);

	// for OpenGL 2.0+
	glewExperimental = GL_TRUE;
	glewInit();


	GLuint vertexBufferHandle;
	glGenBuffers(1, &vertexBufferHandle);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices, GL_STATIC_DRAW);

	GLuint textureCoordHandle;
	glGenBuffers(1, &textureCoordHandle);
	glBindBuffer(GL_ARRAY_BUFFER, textureCoordHandle);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), textureCoords, GL_STATIC_DRAW);

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, 0);
	glCompileShader(vertexShader);

	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragmentShaderCode, 0);
	glCompileShader(fragShader);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);

	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);

	cv::Mat textureMat = cv::imread("resources//unsmile19201.jpg", cv::IMREAD_COLOR);
	glActiveTexture(GL_TEXTURE0);
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureMat.cols, textureMat.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, textureMat.data);



	while (!glfwWindowShouldClose(window)) {
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
		glBindBuffer(GL_ARRAY_BUFFER, textureCoordHandle);
		glVertexAttribPointer(texureHandle, 2, GL_FLOAT, false, 0, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		int sampler = glGetUniformLocation(shaderProgram, "uTexture");
		glUniform1i(sampler, 0);
		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}