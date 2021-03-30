// Importações
#include <GLFW/glfw3.h> 
#include <GL/glut.h>
#include <GL/glu.h>

#include "./headers/ObjLoader.h"

// Assinaturas
void init(GLFWwindow* window);
void teclado_callback(GLFWwindow * window, int key, int scancode, int action, int mods);
void reSize(int w, int h);
void draw(float dt);

// Variáveis Globais
static float angulo = 0; 
static unsigned blenderModelId; 

int main(int argc, char** argv) {
  glfwInit();

  GLFWwindow *window = glfwCreateWindow(800, 600, "Trabalho CG", NULL, NULL);

  init(window); 

  float lastTime = 0.0; 

  bool running = true; 

  while (running) {
    float currentTime = (float)glfwGetTime();
    float dt = currentTime - lastTime;
    lastTime = currentTime;

    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int width, height; 
    glfwGetFramebufferSize(window, &width, &height); 
    reSize(width, height);

    // Desenha Objetos
    draw(dt);

    glfwSwapBuffers(window);
    running = !glfwWindowShouldClose(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  
  return 0;
}

void init(GLFWwindow * window) {
  glfwMakeContextCurrent(window); 
  glfwSetKeyCallback(window, teclado_callback); 

  glClearColor(0.f, 0.f, 0.f, 1.f);

  glEnable(GL_DEPTH_TEST);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

  float globalAmb[] = {0.1f, 0.1f, 0.1f, 0.1f};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb);

  float light0[4][4] = {
				{0.1f, 0.1f, 0.1f, 1.f}, // ambiente
				{0.8f, 0.8f, 0.8f, 1.f}, // difusa
				{ 1.f,  1.f,  1.f, 1.f }, // especular
				{0.f, 0.f, 1.f, 1.f}    // posição
	};

	glLightfv(GL_LIGHT0, GL_AMBIENT, &light0[0][0]);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &light0[1][0]);
	glLightfv(GL_LIGHT0, GL_SPECULAR, &light0[2][0]);
	glLightfv(GL_LIGHT0, GL_POSITION, &light0[3][0]);

	ObjLoader::loadOBJ(blenderModelId, "../assets/cube.obj");
}

void draw(float dt) {
  glLoadIdentity();
  
  vec3 position(0.f, 0.f, 0.f);
  vec3 direction(0.f, 0.f, -3.f);
  vec3 up(0.f, 0.f, 0.f);
  vec3 look = position + direction;

  gluLookAt(position.x, position.y, position.z,
            look.x, look.y, look.z,
            up.x, up.y, up.z);

  float veloc_ang = 25.f*dt;
  angulo += veloc_ang;

  glPushMatrix();
    glTranslatef(0.f, 0.f, -5.f);
    glRotatef(angulo, 1.f, 0.f, 0.f);
    glRotatef(angulo, 0.f, 1.f, 0.f);
    glRotatef(angulo, 0.f, 0.f, 1.f);
    glCallList(blenderModelId);
  glPopMatrix(); 

  if (angulo >= 360) {
    angulo = 0.0;
  }
}

void reSize(int w, int h) {
	glViewport(0, 0, w, h);

	float aspect = (float)w / (float)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, aspect, 0.1, 500.0);
	glMatrixMode(GL_MODELVIEW);
}

void teclado_callback(GLFWwindow * window, int key, int scancode, int action, int mods) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}