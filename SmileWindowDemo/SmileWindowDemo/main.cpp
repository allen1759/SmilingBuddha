#include <iostream>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include "../../SmilingBuddha/SmilingBuddha/SmileWindow.h"

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

	std::shared_ptr<cv::Mat> textureMat = std::make_shared<cv::Mat>(cv::imread("D:\\1-0.jpg", cv::IMREAD_COLOR));
	SmileWindow smileWindow;
	smileWindow.SetTexture(textureMat);
	int i = 0;

	while (!glfwWindowShouldClose(window)) {
		// clear window
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);

		if (i < 1000) {
			smileWindow.SetTexture(textureMat);
			i++;
		}
		smileWindow.Draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}