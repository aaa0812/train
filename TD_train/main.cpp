#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "draw_scene.hpp"
#include "tools/shaders.hpp"
#include <iostream>
#include "grid.hpp"
#include "custom_shapes.hpp"
#include "tools/camera.hpp"

using namespace glbasimac;
using namespace STP3D;

/* Window properties */
static const unsigned int WINDOW_WIDTH = 1200;
static const unsigned int WINDOW_HEIGHT = 800;
static const char WINDOW_TITLE[] = "Train";
static float aspectRatio = 1.0f;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;

static bool displayGrid = false;

float posX = 0;
float posY = 0;
float posZ = 10;

Vector3D pos_camera = Vector3D(posX, posY, posZ);
FPSCamera cam(pos_camera, Vector3D(0.0, 1.0, -1));

/* Error handling function */
void onError(int error, const char *description)
{
	std::cout << "GLFW Error (" << error << ") : " << description << std::endl;
}

void onWindowResized(GLFWwindow * /*window*/, int width, int height)
{
	aspectRatio = width / (float)height;

	glViewport(0, 0, width, height);
	std::cerr << "Setting 3D projection" << std::endl;
	myEngine.set3DProjection(90.0, aspectRatio, Z_NEAR, Z_FAR);
}

void onKey(GLFWwindow *window, int key, int /*scancode*/, int action, int /*mods*/)
{
	int is_pressed = (action == GLFW_PRESS);
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;
	case GLFW_KEY_L:
		if (is_pressed)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case GLFW_KEY_P:
		if (is_pressed)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case GLFW_KEY_R:
		if (is_pressed)
			displayGrid = !displayGrid;
		break;
	case GLFW_KEY_T:
		if (is_pressed)
			flag_anim_rot_arm = !flag_anim_rot_arm;
		break;
	case GLFW_KEY_W:
		cam.stepFront(1);
		break;
	case GLFW_KEY_D:
		cam.stepRight(1);
		break;
	case GLFW_KEY_A:
		cam.stepLeft(1);
		break;
	case GLFW_KEY_S:
		cam.stepBack(1);
		break;
	case GLFW_KEY_UP:
		cam.lookUp(1);
		break;
	case GLFW_KEY_DOWN:
		cam.lookDown(1);
		break;
	case GLFW_KEY_RIGHT:
		cam.lookRight(1);
		std::cout << cam;
		break;
	case GLFW_KEY_LEFT:
		cam.lookLeft(1);
		std::cout << cam;
		break;
	default:
		std::cerr << "Touche non gérée " << key << std::endl;
	}
}

void onMouseButton(GLFWwindow *window, int button, int action, int /*mods*/)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		std::cout << "Pressed in " << xpos << " " << ypos << std::endl;
	}
}

int main(int argc, char **argv)
{
	const std::string path{(argc >= 2) ? argv[1] : "grid.json"};
	std::ifstream file(path);
	if (!file)
	{
		std::cerr << "Unable to open : " << path << "\n";
		return 1;
	}
	nlohmann::json data = nlohmann::json::parse(file);

	const GridConfig gridConfig = data.get<GridConfig>();
	std::cout << gridConfig.size_grid << std::endl;

	/* GLFW initialisation */
	GLFWwindow *window;
	if (!glfwInit())
		return -1;

	/* Try to uncomment this for MAC OS if it did not work */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	/* Callback to a function if an error is rised by GLFW */
	glfwSetErrorCallback(onError);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (!window)
	{
		// If no context created : exit !
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	std::cout << "Loading GL extension" << std::endl;
	// Intialize glad (loads the OpenGL functions)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return -1;
	}

	glfwSetWindowSizeCallback(window, onWindowResized);
	glfwSetKeyCallback(window, onKey);
	glfwSetMouseButtonCallback(window, onMouseButton);

	std::cout << "Engine init" << std::endl;
	// TO DO EX01 part 2

	myEngine.mode2D = false;
	myEngine.initGL();
	onWindowResized(window, WINDOW_WIDTH, WINDOW_HEIGHT);
	CHECK_GL;

	initScene(gridConfig);
	initShapes();
	double elapsedTime{0.0};

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Get time (in second) at loop beginning */
		double startTime = glfwGetTime();

		/* Render begins here */
		glClearColor(0.8f, 0.8f, 0.8f, 0.0f);

		// TO DO EX01 part 2
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		myEngine.mvMatrixStack.loadIdentity();
		// TO DO EX01 part 3
		/* Fix camera position */
		// Old cam
		/* Vector3D pos_camera =
		Vector3D(dist_zoom * cos(deg2rad(angle_theta)) * cos(deg2rad(angle_phy)),
		dist_zoom * sin(deg2rad(angle_theta)) * cos(deg2rad(angle_phy)),
		dist_zoom * sin(deg2rad(angle_phy)));
		Vector3D viewed_point = Vector3D(0.0, 0.0, 0.0);
		Vector3D up_vector = Vector3D(0.0, 0.0, 1.0);
		Matrix4D viewMatrix = Matrix4D::lookAt(pos_camera, viewed_point, up_vector);
		myEngine.updateMvMatrix(); */

		// New cam
		myEngine.setViewMatrix(cam.viewMatrix);

		drawScene(startTime, displayGrid);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		/* Elapsed time computation from loop begining */
		elapsedTime = glfwGetTime() - startTime;
		/* If to few time is spend vs our wanted FPS, we wait */
		while (elapsedTime < FRAMERATE_IN_SECONDS)
		{
			glfwWaitEventsTimeout(FRAMERATE_IN_SECONDS - elapsedTime);
			elapsedTime = glfwGetTime() - startTime;
		}
	}

	glfwTerminate();
	return 0;
}
