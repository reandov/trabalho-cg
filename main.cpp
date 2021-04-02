#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>

#include "ObjLoader.h"
#include "MaterialSamples.h"
#include "Camera.h"
#include "Textura.h"

// Inicializar
void init(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void teclado_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void reSize(int w, int h);

// Variáveis
static float angulo = 0;
static float lastMousePos = 0.0;
static bool firstTimeMouse = true;
static unsigned modelID[5];

enum
{
	GRASS,
	WOOD,
	LEAVES
};
Textura CuboTex[3];

Camera camera(vec3(0, 3, 0));

void Desenha(float dt)
{

	glLoadIdentity();

	camera.ativar();

	float veloc_ang = 25.f * dt;
	angulo += veloc_ang;

	bronze.ativar();

	/////////////////////////////////////////////////////////

	glPushMatrix();

	glTranslatef(0.f, -2.f, 0.f);
	glCallList(modelID[0]);

	glPopMatrix();

	glPushMatrix();

	glTranslatef(2.f, -2.f, 0.f);
	glCallList(modelID[0]);

	glTranslatef(-4.f, 0.f, 0.f);
	glCallList(modelID[0]);

	glPopMatrix();

	/////////////////////////////////////////////////////////

	glPushMatrix();

	glTranslatef(0.f, -2.f, 2.f);
	glCallList(modelID[0]);

	glPopMatrix();

	glPushMatrix();

	glTranslatef(2.f, -2.f, 2.f);
	glCallList(modelID[0]);

	glTranslatef(-4.f, 0.f, 0.f);
	glCallList(modelID[0]);

	glPopMatrix();

	/////////////////////////////////////////////////////////

	glPushMatrix();

	glTranslatef(0.f, -2.f, -2.f);
	glCallList(modelID[0]);

	glPopMatrix();

	glPushMatrix();

	glTranslatef(2.f, -2.f, -2.f);
	glCallList(modelID[0]);

	glTranslatef(-4.f, 0.f, 0.f);
	glCallList(modelID[0]);

	glPopMatrix();

	/////////////////////////////////////////////////////////

	glPushMatrix();

	glTranslatef(0.f, 0.f, 0.f);
	glCallList(modelID[1]);

	glTranslatef(0.f, 2.f, 0.f);
	glCallList(modelID[1]);

	glTranslatef(0.f, 2.f, 0.f);
	glCallList(modelID[1]);

	glTranslatef(0.f, 2.f, 0.f);
	glCallList(modelID[1]);

	glPopMatrix();

	/////////////////////////////////////////////////////////

	glPushMatrix();

	glTranslatef(0.f, 8.f, 0.f);
	glCallList(modelID[2]);

	glPopMatrix();

	glPushMatrix();

	glTranslatef(2.f, 6.f, 0.f);
	glCallList(modelID[2]);

	glPopMatrix();

	glPushMatrix();

	glTranslatef(-2.f, 6.f, 0.f);
	glCallList(modelID[2]);

	glPopMatrix();

	glPushMatrix();

	glTranslatef(0.f, 6.f, 2.f);
	glCallList(modelID[2]);

	glPopMatrix();

	glPushMatrix();

	glTranslatef(0.f, 6.f, -2.f);
	glCallList(modelID[2]);

	glPopMatrix();

	glPushMatrix();

	glTranslatef(2.f, 6.f, 2.f);
	glCallList(modelID[2]);

	glPopMatrix();

	glPushMatrix();

	glTranslatef(2.f, 6.f, -2.f);
	glCallList(modelID[2]);

	glPopMatrix();

	glPushMatrix();

	glTranslatef(-2.f, 6.f, 2.f);
	glCallList(modelID[2]);

	glPopMatrix();

	glPushMatrix();

	glTranslatef(-2.f, 6.f, -2.f);
	glCallList(modelID[2]);

	glPopMatrix();

	glPushMatrix();

	glTranslatef(4.f, 6.f, 0.f);
	glCallList(modelID[2]);

	glPopMatrix();

	glPushMatrix();

	glTranslatef(-4.f, 6.f, 0.f);
	glCallList(modelID[2]);

	glPopMatrix();

	glPushMatrix();

	glTranslatef(0.f, 6.f, -4.f);
	glCallList(modelID[2]);

	glPopMatrix();

	glPushMatrix();

	glTranslatef(0.f, 6.f, 4.f);
	glCallList(modelID[2]);

	glPopMatrix();

	if (angulo >= 360.0)
		angulo = 0.0;
}

int main()
{

	glfwInit();

	GLFWwindow *window = glfwCreateWindow(1280, 720, "Trabalho CG", NULL, NULL);
	init(window);

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
		reSize(width, height);

		// -- Draw Objects --
		Desenha(dt);

		glfwSwapBuffers(window);
		running = !glfwWindowShouldClose(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void init(GLFWwindow *window)
{
	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, teclado_callback);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glClearColor(0.19f, 0.6f, 0.8f, 1.f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	CuboTex[GRASS].load("./assets/textures/grass.png");
	CuboTex[WOOD].load("./assets/textures/wood.png");
	CuboTex[LEAVES].load("./assets/textures/leaves.png");

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	float globalAmb[] = {0.1f, 0.1f, 0.1f, 1.f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb);

	float light0[4][4] = {
		{0.1f, 0.1f, 0.1f, 1.f}, // ambient
		{0.9f, 0.9f, 0.9f, 9.f}, // diffuse
		{1.f, 1.f, 1.f, 1.f},	 // specular
		{0.f, 15.f, 0.f, 1.f}};

	glLightfv(GL_LIGHT0, GL_AMBIENT, &light0[0][0]);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &light0[1][0]);
	glLightfv(GL_LIGHT0, GL_SPECULAR, &light0[2][0]);
	glLightfv(GL_LIGHT0, GL_POSITION, &light0[3][0]);

	Textura *tex1 = &CuboTex[GRASS];
	ObjLoader::loadOBJ(modelID[0], tex1, "assets/block.obj");

	Textura *tex2 = &CuboTex[WOOD];
	ObjLoader::loadOBJ(modelID[1], tex2, "assets/block.obj");

	Textura *tex3 = &CuboTex[LEAVES];
	ObjLoader::loadOBJ(modelID[2], tex3, "assets/block.obj");
}

void reSize(int w, int h)
{
	glViewport(0, 0, w, h);

	float aspect = (float)w / (float)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, aspect, 0.1, 500.0);
	glMatrixMode(GL_MODELVIEW);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
	float dx;

	if (firstTimeMouse)
	{
		dx = 0;
		lastMousePos = xpos;
		firstTimeMouse = false;
	}

	dx = xpos - lastMousePos;
	lastMousePos = xpos;

	camera.updateYaw(dx);
	camera.update();
}

void teclado_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
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
}
