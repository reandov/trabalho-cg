// Standard Importations
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <random>

// Headers Importations
#include "headers/ObjLoader.h"
#include "headers/MaterialSamples.h"
#include "headers/Camera.h"
#include "headers/Texture.h"

// OpenGL Initializations
void init(GLFWwindow *window);
void draw(float dt);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void resize(int w, int h);

// Needed Functions Initializations
void generateTerrain(unsigned modelID);
void generateTrees(int xpos, int zpos, int height, unsigned modelID1, unsigned modelID2);
void generateRandomPositions();

// Necessary Variables
static float lastMousePosX = 0.0;
static float lastMousePosY = 0.0;
static float angle = 0;
static bool firstTimeMouse = true;
static bool isLightMoving = true;

// Model Variables and Initializations
enum
{
	GRASS,
	WOOD,
	LEAVES
};

Texture textures[3];
static unsigned modelID[5];

// Other Initializations
vec3 lightPos(15.f, 0.f, 120.f);
std::vector<vec3> randomPositions;
Camera camera(vec3(0, 3, 0));

int main()
{
	// Initializing GLFW
	glfwInit();

	// Creating window and initializing it
	GLFWwindow *window = glfwCreateWindow(1280, 720, "Trabalho CG", NULL, NULL);
	init(window);

	// Main loop
	float lastTime = 0.0;
	bool running = true;
	while (running)
	{
		float currentTime = (float)glfwGetTime();
		float dt = currentTime - lastTime;
		lastTime = currentTime;

		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		resize(width, height);

		draw(dt);

		glfwSwapBuffers(window);
		running = !glfwWindowShouldClose(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

// Initialization functions - Runs at the start of a window
void init(GLFWwindow *window)
{
	// Creating the context of the window
	glfwMakeContextCurrent(window);

	// Keyboard and Mouse callbacks
	glfwSetKeyCallback(window, keyboardCallback);
	glfwSetCursorPosCallback(window, mouseCallback);

	// Disabling cursor on the screen
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	generateRandomPositions();
	/////////////////////////////////
	// Enabling OpenGL features

	// Depth
	glEnable(GL_DEPTH_TEST);

	// Textures
	glEnable(GL_TEXTURE_2D);

	// Face cutting (faces not facing camera will be cutted a.k.a. clipping)
	glEnable(GL_CULL_FACE);

	// Enable OpenGL's Lighting engine
	glEnable(GL_LIGHTING);

	// Enable Light 0 (note: standard OpenGL only supports 8 lights at the max)
	glEnable(GL_LIGHT0);

	// Set texture environment parameters
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Related to glEnable(GL_CULL_FACE), this function remove the back faces
	glCullFace(GL_BACK);
	/////////////////////////////////

	// Clearing background color (note: in this case, I'm chaning to sky blue color)
	glClearColor(0.19f, 0.6f, 0.8f, 1.f);

	/////////////////////////////////
	// Setting up the light0
	// Configuring global ambient light
	float globalAmb[] = {0.4f, 0.4f, 0.4f, 1.f};
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb);

	// Matrix containing the light0 configuration
	float light0[3][4] = {
		// Ambient
		{0.8f, 0.8f, 0.8f, 1.f},
		// Diffuse
		{0.9f, 0.9f, 0.9f, 1.f},
		// Specular
		{0.1f, 0.1f, 0.1f, 1.f}};

	// Setting every property of the light
	glLightfv(GL_LIGHT0, GL_AMBIENT, &light0[0][0]);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &light0[1][0]);
	glLightfv(GL_LIGHT0, GL_SPECULAR, &light0[2][0]);
	/////////////////////////////////

	// Loading textures
	textures[GRASS].load("./assets/textures/grass.png");
	textures[WOOD].load("./assets/textures/wood.png");
	textures[LEAVES].load("./assets/textures/leaves.png");

	// Loading block module and applying every texture individually
	Texture *tex1 = &textures[GRASS];
	ObjLoader::loadOBJ(modelID[0], tex1, "assets/block.obj");

	// Loading block module and applying every texture individually
	Texture *tex2 = &textures[WOOD];
	ObjLoader::loadOBJ(modelID[1], tex2, "assets/block.obj");

	// Loading block module and applying every texture individually
	Texture *tex3 = &textures[LEAVES];
	ObjLoader::loadOBJ(modelID[2], tex3, "assets/block.obj");
}

// Drawing function
void draw(float dt)
{
	// Loading identity
	glLoadIdentity();

	// Activating camera
	camera.activate();

	// Calculating angular velocity
	float angular_velocity = 25.f * dt;

	// Activating default material
	chrome.activate();

	// Generating terrain
	generateTerrain(modelID[0]);

	// Generating trees
	// generateTrees(0, 0, 4, modelID[1], modelID[2]);
	// generateTrees(30, 30, 4, modelID[1], modelID[2]);
	// generateTrees(15, 15, 7, modelID[1], modelID[2]);
	// generateTrees(20, 2, 8, modelID[1], modelID[2]);
	// generateTrees(2, 20, 5, modelID[1], modelID[2]);
	// generateTrees(4, 8, 4, modelID[1], modelID[2]);

	for (int i = 0; i < 20; i++)
	{
		generateTrees(randomPositions[i].x, randomPositions[i].y, randomPositions[i].z, modelID[1], modelID[2]);
	}

	// Activating light
	light.activate();

	// If light is moving, keep applying rotation and recalculating the angle
	if (isLightMoving)
	{
		angle += angular_velocity;
		glRotatef(angle, 5.f, 0.f, 0.f);
	}
	else
	{
		glRotatef(angle, 5.f, 0.f, 0.f);
	}

	// Translating the light's position
	glTranslatef(lightPos.x, lightPos.y, lightPos.z);

	// Calling a cube to be the light source
	glCallList(modelID[2]);

	// Storing light's position
	float lightposition[] = {lightPos.x, lightPos.y, lightPos.z, 1.f};

	// Updating light position using the OpenGL native function
	glLightfv(GL_LIGHT0, GL_POSITION, lightposition);

	// Angle limiter
	if (angle >= 360.0)
		angle = 0.0;
}

// Function used to draw a simple terrain by passing a model (in which it'll be used as ground block)
void generateTerrain(unsigned modelID)
{
	float tamX = 0.f;
	float tamZ = 0.f;
	for (int i = 0; i < 32; i++)
	{
		tamX = 0.f;
		for (int j = 0; j < 32; j++)
		{
			glPushMatrix();

			glTranslatef(tamX, -2.f, tamZ);
			glCallList(modelID);

			glPopMatrix();

			tamX += 2.f;
		}
		tamZ += 2.f;
	}
}

// Function to generate a tree
void generateTrees(int xpos, int zpos, int height, unsigned modelID1, unsigned modelID2)
{
	float increment = 0.0f;

	for (int i = 0; i < height; i++)
	{
		glPushMatrix();

		glTranslatef(xpos, increment, zpos);
		glCallList(modelID1);

		glPopMatrix();

		increment += 2.f;
	}

	float tamX = xpos - 4;
	float tamZ = zpos - 4;
	for (int i = 0; i < 5; i++)
	{
		tamX = xpos - 4;
		for (int j = 0; j < 5; j++)
		{
			glPushMatrix();

			glTranslatef(tamX, increment, tamZ);
			glCallList(modelID2);

			glPopMatrix();

			glPushMatrix();

			glTranslatef(tamX, increment + 2, tamZ);
			glCallList(modelID2);

			glPopMatrix();

			tamX += 2.f;
		}
		tamZ += 2.f;
	}

	tamX = xpos - 2;
	tamZ = zpos - 2;
	for (int i = 0; i < 3; i++)
	{
		tamX = xpos - 2;
		for (int j = 0; j < 3; j++)
		{
			glPushMatrix();

			glTranslatef(tamX, increment + 4, tamZ);
			glCallList(modelID2);

			glPopMatrix();

			tamX += 2.f;
		}
		tamZ += 2.f;
	}

	glPushMatrix();

	glTranslatef(xpos, increment + 6, zpos);
	glCallList(modelID2);

	glPopMatrix();
}

// Used to resize the window and mantain aspect ratio
void resize(int w, int h)
{
	glViewport(0, 0, w, h);

	float aspect = (float)w / (float)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, aspect, 0.1, 500.0);
	glMatrixMode(GL_MODELVIEW);
}

// Mouse callback function
void mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
	float dx, dy;

	if (firstTimeMouse)
	{
		dx = 0;
		lastMousePosX = xpos;
		lastMousePosY = ypos;
		firstTimeMouse = false;
	}

	dx = xpos - lastMousePosX;
	dy = ypos - lastMousePosY;

	float sensitivity = 0.2f;
	dx *= sensitivity;
	dy *= sensitivity;

	if (dy > 89.0f)
	{
		dy = 89.0f;
	}
	else if (dy < -89.0f)
	{
		dy = -89.0f;
	}

	lastMousePosX = xpos;
	lastMousePosY = ypos;

	camera.updateYaw(dx);
	camera.updatePitch(dy);
	camera.update();
}

// Keyboard callback function
void keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		// Close window on ESC keypress
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	else if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		// Forward
		camera.forward();
	}
	else if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		// Back
		camera.back();
	}
	else if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		// Left
		camera.left();
	}
	else if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		// Right
		camera.right();
	}
	else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		// If light is moving, this will deny it's move
		isLightMoving = !isLightMoving;
	}
}

// Generate N vec3 containing random xpos, zpos and height
void generateRandomPositions()
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(0, 62); // distribution in range [1, 6]

	for (int i = 0; i < 20; i++)
	{
		float x = dist6(rng);
		float z = dist6(rng);
		float height = dist6(rng);

		if (height > 16)
			height = height / 4;

		vec3 rand(x, z, height);
		randomPositions.push_back(rand);
	}
}