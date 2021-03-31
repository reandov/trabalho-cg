#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>

#include "ObjLoader.h"
#include "MaterialSamples.h"

// Inicializar
void init(GLFWwindow *window);
void teclado_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void reSize(int w, int h);

// Funções

// Variáveis
static float angulo = 0;
static unsigned blenderModelId;
static unsigned tree1, tree2, tree3, tree4, tree5, tree6;

void Desenha(float dt)
{

	glLoadIdentity();

	vec3 position(0.f, 5.f, 20.f);
	vec3 direction(0.f, 0.f, -5.f);
	vec3 up(0.f, 1.f, 0.f);
	vec3 look = position + direction;

	gluLookAt(
		position.x, position.y, position.z,
		look.x, look.y, look.z,
		up.x, up.y, up.z);

	float veloc_ang = 30.f * dt;
	angulo += veloc_ang;

	jade.ativar();
	glPushMatrix();
	glTranslatef(0.f, 0.f, -15.f);
	glRotatef(angulo, 0.f, 1.f, 0.f);
	glCallList(blenderModelId);
	glTranslatef(10.f, -1.f, -25.f);
	glCallList(tree1);
	glTranslatef(-20.f, -1.f, 25.f);
	glCallList(tree2);
	glTranslatef(-14.f, -1.f, 30.f);
	glCallList(tree3);
	glTranslatef(16.f, -1.f, -35.f);
	glCallList(tree4);
	glTranslatef(-22.f, -1.f, -10.f);
	glCallList(tree5);
	glTranslatef(-7.f, -1.f, -3.f);
	glCallList(tree6);
	glPopMatrix();

	// glPushMatrix();

	// glPopMatrix();

	if (angulo >= 360.0)
		angulo = 0.0;
}

int main()
{

	glfwInit();

	GLFWwindow *window = glfwCreateWindow(800, 600, "Tutorial Light - Blender", NULL, NULL);
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
	glfwSetKeyCallback(window, teclado_callback);

	glClearColor(0.19f, 0.6f, 0.8f, 1.f);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	float globalAmb[] = {0.1f, 0.1f, 0.1f, 1.f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb);

	float light0[4][4] = {
		{0.1f, 0.1f, 0.1f, 1.f},  // ambient
		{0.6f, 0.6f, 0.6f, 1.f},  // diffuse
		{0.8f, 0.8f, 0.8f, 0.8f}, // specular
		{0.f, 0.1f, 0.f, 0.f}	  // position
	};

	glLightfv(GL_LIGHT0, GL_AMBIENT, &light0[0][0]);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &light0[1][0]);
	glLightfv(GL_LIGHT0, GL_SPECULAR, &light0[2][0]);
	glLightfv(GL_LIGHT0, GL_POSITION, &light0[3][0]);

	ObjLoader::loadOBJ(blenderModelId, "assets/terrain.obj");
	ObjLoader::loadOBJ(tree1, "assets/tree.obj");
	ObjLoader::loadOBJ(tree2, "assets/tree.obj");
	ObjLoader::loadOBJ(tree3, "assets/tree.obj");
	ObjLoader::loadOBJ(tree4, "assets/tree.obj");
	ObjLoader::loadOBJ(tree5, "assets/tree.obj");
	ObjLoader::loadOBJ(tree6, "assets/tree.obj");
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

void teclado_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}
