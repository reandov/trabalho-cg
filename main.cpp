#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>

#include "ObjLoader.h"
#include "MaterialSamples.h"

// Inicializar 
void init(GLFWwindow* window);
void teclado_callback(GLFWwindow * window, int key, int scancode, int action, int mods);
void reSize(int w, int h);
void esfera(unsigned& id);

// Variáveis de Posição
vec3 floorPos(0.f, 0.f, -5.f);
vec3 lightPos(0.f, 0.f, 10.f);


// Variáveis
static float angulo = 0;
static bool moveLuz = true;
static unsigned blenderModelId0, blenderModelId1, blenderModelId2, blenderModelId3;
static unsigned esferaId;

void Desenha(float dt) {

	glLoadIdentity();

	vec3 position(0.f, 0.f, 0.f);
	vec3 direction(0.f, 0.f, -3.f);
	vec3 up(0.f, 1.f, 0.f);
	vec3 look = position + direction;// direction;

	gluLookAt(
		position.x, position.y, position.z,
		look.x, look.y, look.z,
		up.x, up.y, up.z);


	float veloc_ang = 20.f*dt;

	bronze.ativar();
	glTranslatef(floorPos.x, floorPos.y, floorPos.z);
	glCallList(blenderModelId0);

	jade.ativar();
	if (moveLuz) {
		angulo += veloc_ang;
		glRotatef(angulo, 0.f, 1.f, 0.f);
	} else {
		glRotatef(angulo, 0.f, 1.f, 0.f);
	}
	
	
	glTranslatef(lightPos.x, lightPos.y, lightPos.z);
	glCallList(esferaId);

	float lightposition[] = {lightPos.x, lightPos.y, lightPos.z};
 	glLightfv(GL_LIGHT0, GL_POSITION, lightposition);

	// glTranslatef(5.f, 0.f, 5.f);
	// glCallList(blenderModelId1);

	// glTranslatef(10.f, 0.f, 10.f);
	// glCallList(blenderModelId2);

	// glTranslatef(-10.f, 0.f, 15.f);
	// glCallList(blenderModelId3);

	if (angulo >= 360.0)
		angulo = 0.0;
}

int main() {

	glfwInit();

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Trabalho CG", NULL, NULL);

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


void init(GLFWwindow* window) {
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, teclado_callback);

	glClearColor(0.f, 0.f, 0.f, 1.f);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);


	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	float globalAmb[] = { 0.1f, 0.1f, 0.1f, 1.f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb);

	float light0[3][4] = {
				{0.5f, 0.5f, 0.5f, 5.f}, // ambient
				{0.8f, 0.8f, 0.8f, 1.f}, // diffuse
				{ 1.f,  1.f,  1.f, 1.f}, // specular
	};

	glLightfv(GL_LIGHT0, GL_AMBIENT, &light0[0][0]);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &light0[1][0]);
	glLightfv(GL_LIGHT0, GL_SPECULAR, &light0[2][0]);

	ObjLoader::loadOBJ(blenderModelId0, "./assets/monkey.obj");
	// ObjLoader::loadOBJ(blenderModelId1, "./assets/tree.obj");
	// ObjLoader::loadOBJ(blenderModelId2, "./assets/tree.obj");
	// ObjLoader::loadOBJ(blenderModelId3, "./assets/tree.obj");
	esfera(esferaId);
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

void esfera(unsigned& id)
{
	std::vector<std::vector<vec3> > pontos;
	std::vector<std::vector<vec3> > normals;

	float raio = 1.0f;
	unsigned nStacks = 10;
	unsigned nSectors = 10;
	const float PI = 3.14159265359;
	const float TWO_PI = 2 * PI;
	const float HALF_PI = PI / 2.0;

	float deltaPhi = PI / nStacks;
	float deltaTheta = TWO_PI / nSectors;


	for (unsigned i = 0; i <= nStacks; i++)
	{
		float phi = -HALF_PI + i * deltaPhi;
		float temp = raio * cos(phi);
		float y = raio * sin(phi);

		std::vector<vec3> pts;

		for (unsigned j = 0; j <= nSectors; j++)
		{
			float theta = j * deltaTheta;
			float x = temp * sin(theta);
			float z = temp * cos(theta);

			float s = theta / TWO_PI;
			GLfloat t = phi / PI + 0.5;

			pts.push_back(vec3(x, y, z));
		}// next j

		pontos.push_back(pts);
		normals.push_back(pts);
	}// next i

	id = glGenLists(1);
	glNewList(id, GL_COMPILE);
	glPolygonMode(GL_FRONT, GL_FILL);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);

	for (unsigned i = 0; i < nStacks; i++)
	{
		glBegin(GL_TRIANGLE_STRIP);

		for (unsigned j = 0; j <= nSectors; j++)
		{
			glNormal3fv(&normals[i][j].x); glVertex3fv(&pontos[i][j].x);
			glNormal3fv(&normals[i + 1][j].x); glVertex3fv(&pontos[i + 1][j].x);
		}
		glEnd();
	}
	glDisable(GL_CULL_FACE);
	glEndList();
}

void teclado_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		moveLuz = !moveLuz;
	}
}