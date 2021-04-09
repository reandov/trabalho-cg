// +===========================+
// |                           |
// |      PROTO-MINECRAFT      |
// |                           |
// +===========================+

// ~> Evandro Rodrigues 	- 402324
// ~> Leonardo David 		- 404074
// ~> Jordão Rodrigues 		- 403686

// Para rodar o código é necessário a GLFW3 (sudo apt install libglfw3)
// Para rodar o código de maneira rápida é só executar o shell script criado por nós: ./compiler.sh

// OBS: Os comentários das headers e demais arquivos estão em inglês, pois fizemos inicialmente assim, por ser mais fácil pra nós (em termos de leitura :) )

// Importações padrão ou do C++
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <random>

// Importações das Headers
#include "headers/ObjLoader.h"
#include "headers/MaterialSamples.h"
#include "headers/Camera.h"
#include "headers/Texture.h"

// Inicializaçõs do OpenGL
void init(GLFWwindow *window);
void draw(float dt);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void resize(int w, int h);

// Funções necessárias
void generateTerrain(unsigned modelID);													  // Geração do terreno (recebe um obj com textura para servir de modelo)
void generateTrees(int xpos, int zpos, int height, unsigned modelID1, unsigned modelID2); // Geração das árvores, em posições X e Z aleatórias, altura aleatória e recebe os dois objs.
void generateRandomPositions();															  // Função auxiliar que gera um vetor de vec3 para ser usando na geração das árvores

// Variáveis de inicialização
static float lastMousePosX = 0.0;
static float lastMousePosY = 0.0;
static float angle = 0;
static bool firstTimeMouse = true;
static bool isLightMoving = true;
static bool isNoClipOn = false;

// ENUM para cada tipo de bloco carregado.
enum
{
	GRASS,
	WOOD,
	LEAVES,
	GLOWSTONE
};

// Vetor de texturas
Texture textures[4];

// Vetor de Objetos.
static unsigned modelID[5];

// Outras inicializações e variáveis/vetores

// Vetor vec3 de posições aleatórias
std::vector<vec3> randomPositions;

// Posição da luz (que imita o sol)
vec3 lightPos(35.f, 0.f, 120.f);

// Posição do bloco de luz central do terreno
vec3 glowstonePos(32.f, 2.f, 32.f);

// Inicialização da câmera
Camera camera(vec3(0, 4, 0));

int main()
{
	// Inicialização da GLFW
	glfwInit();

	// Criação da janela e inicializando a mesma
	GLFWwindow *window = glfwCreateWindow(1280, 720, "Trabalho CG", NULL, NULL);
	init(window);

	// Loop principal
	float lastTime = 0.0;
	bool running = true;
	while (running)
	{
		// Calculamos o framerate da janela
		float currentTime = (float)glfwGetTime();
		float dt = currentTime - lastTime;
		lastTime = currentTime;

		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		resize(width, height);

		// Desenhamos o que precisa ser desenhado aqui
		draw(dt);

		glfwSwapBuffers(window);

		// Enquanto a janela estiver rodando
		running = !glfwWindowShouldClose(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

// Funçao Init roda ao instanciar uma janela nova
void init(GLFWwindow *window)
{
	// Criação do contexto da janela
	glfwMakeContextCurrent(window);

	// Callbacks do mouse e teclado
	glfwSetKeyCallback(window, keyboardCallback);
	glfwSetCursorPosCallback(window, mouseCallback);

	// Desabilitando cursor na janela
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Povoa o vetor de posições aleatórias aqui
	generateRandomPositions();

	/////////////////////////////////
	// Ligando algumas funcionalidades do OpenGL

	// Profundidade
	glEnable(GL_DEPTH_TEST);

	// Texturas
	glEnable(GL_TEXTURE_2D);

	// Face cutting (faces que não estão de frente pra câmera serão cortadas)
	glEnable(GL_CULL_FACE);

	// Habilitando o motor de iluminação do OpenGL
	glEnable(GL_LIGHTING);

	// Ligando a luz 0 (é estranho dizer isso dentro de um programa de computador :D)
	glEnable(GL_LIGHT0);

	// GL_MODULATE a textura sofra efeitos da iluminação
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Aqui só especificamos que a face a ser removida pelo Face Cutting será a de trás
	glCullFace(GL_BACK);
	/////////////////////////////////

	/////////////////////////////////
	// Configurando a luz ambiente
	float globalAmb[] = {0.4f, 0.4f, 0.4f, 1.f};
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb);

	// Matrizes de configuração da luz0 e luz1
	float light0[3][4] = {
		// Ambient
		{0.8f, 0.8f, 0.8f, 1.f},
		// Diffuse
		{0.9f, 0.9f, 0.9f, 1.f},
		// Specular
		{0.1f, 0.1f, 0.1f, 1.f}};

	float light1[3][4] = {
		// Ambient
		{0.8f, 0.8f, 0.8f, 1.f},
		// Diffuse
		{0.9f, 0.9f, 0.9f, 1.f},
		// Specular
		{0.1f, 0.1f, 0.1f, 1.f}};

	// Configurando cada componente da luz
	glLightfv(GL_LIGHT0, GL_AMBIENT, &light0[0][0]);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &light0[1][0]);
	glLightfv(GL_LIGHT0, GL_SPECULAR, &light0[2][0]);

	glLightfv(GL_LIGHT1, GL_AMBIENT, &light1[0][0]);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, &light1[1][0]);
	glLightfv(GL_LIGHT1, GL_SPECULAR, &light1[2][0]);
	/////////////////////////////////

	// Carregando as texturas
	textures[GRASS].load("./assets/textures/grass.png");
	textures[WOOD].load("./assets/textures/wood.png");
	textures[LEAVES].load("./assets/textures/leaves.png");
	textures[GLOWSTONE].load("./assets/textures/glowstone.png");

	// Carregando cada modelo .OBJ e aplicando a textura especificada
	Texture *tex1 = &textures[GRASS];
	ObjLoader::loadOBJ(modelID[0], tex1, "assets/block.obj");

	// Carregando cada modelo .OBJ e aplicando a textura especificada
	Texture *tex2 = &textures[WOOD];
	ObjLoader::loadOBJ(modelID[1], tex2, "assets/block.obj");

	// Carregando cada modelo .OBJ e aplicando a textura especificada
	Texture *tex3 = &textures[LEAVES];
	ObjLoader::loadOBJ(modelID[2], tex3, "assets/block.obj");

	// Carregando cada modelo .OBJ e aplicando a textura especificada
	Texture *tex4 = &textures[GLOWSTONE];
	ObjLoader::loadOBJ(modelID[3], tex4, "assets/block.obj");
}

// Função de desenho
void draw(float dt)
{
	// Carregando a Identidade
	glLoadIdentity();

	// Ativando a camera
	camera.activate();

	// Criando um ciclo de dia/noite
	if (angle < 150)
	{
		glClearColor(0.f, 0.f, 0.1f, 1.f);
	}
	else
	{
		glClearColor(0.19f, 0.6f, 0.8f, 1.f);
	}

	// Aqui ativamos o noclip baseado na localização da câmera (a posição) no plano cartesiano
	if ((camera.m_pos.x >= 32.f && camera.m_pos.x <= 34.f) && (camera.m_pos.z >= 32.f && camera.m_pos.z <= 34.f))
	{
		std::cout << "No Clip: " << isNoClipOn << std::endl;
		isNoClipOn = true;
	}

	// Seguindo a mesma ideia do noclip aqui é um sistema de colisão simples feito usando as proprias coordenadas
	if (!isNoClipOn)
	{
		if (camera.m_pos.y < 4 || camera.m_pos.y > 5)
		{
			camera.m_pos.y = 4;
		}

		if (camera.m_pos.x < 1)
		{
			camera.m_pos.x = 2;
		}
		else if (camera.m_pos.x > 63)
		{
			camera.m_pos.x = 62;
		}

		if (camera.m_pos.z < 1)
		{
			camera.m_pos.z = 2;
		}
		else if (camera.m_pos.z > 62)
		{
			camera.m_pos.z = 60;
		}
	}

	// Calculo da velocidade angular
	float angular_velocity = 25.f * dt;

	// Ativanto material do terreno
	chrome.activate();

	// Gerando o terreno
	generateTerrain(modelID[0]);

	// Gerando as arvores
	for (int i = 0; i < 20; i++)
	{
		generateTrees(randomPositions[i].x, randomPositions[i].y, randomPositions[i].z, modelID[1], modelID[2]);
	}

	// Ativando a luz central
	emerald.activate();
	glPushMatrix();
	glTranslatef(glowstonePos.x, glowstonePos.y, glowstonePos.z);
	glRotatef(angle, 0.f, 2.f, 0.f);
	glCallList(modelID[3]);
	glPopMatrix();

	// Armazenando a posição do bloco central pra ser usado na luz
	float glowstonePosition[] = {glowstonePos.x, glowstonePos.y, glowstonePos.z, 1.f};

	// Setando a localização da luz1 para a centro do mapa
	glLightfv(GL_LIGHT1, GL_POSITION, glowstonePosition);

	// Se isLightMoving for true continuamos a atualizar o angulo
	light.activate();
	if (isLightMoving)
	{
		angle += angular_velocity;
		glRotatef(angle, 5.f, 0.f, 0.f);
	}
	else
	{
		glRotatef(angle, 5.f, 0.f, 0.f);
	}

	// Transladando a posição da luz
	glTranslatef(lightPos.x, lightPos.y, lightPos.z);

	// Carregando o cubo que seria a luz0 (sol)
	glCallList(modelID[3]);

	// Armazenando a posição da luz
	float lightposition[] = {lightPos.x, lightPos.y, lightPos.z, 1.f};

	// Atualizando a posição da luz baseada na posição do cubo
	glLightfv(GL_LIGHT0, GL_POSITION, lightposition);

	// Delimitador no angulo
	if (angle >= 360.0)
		angle = 0.0;
}

// Função para gerar o terreno
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

// Função para gerar as arvores
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

// Usado para manter o aspect ratio da janela
void resize(int w, int h)
{
	glViewport(0, 0, w, h);

	float aspect = (float)w / (float)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, aspect, 0.1, 500.0);
	glMatrixMode(GL_MODELVIEW);
}

// Mouse callback
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

	float sensitivity = 0.1f;
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

// Keyboard callback
void keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		// Fecha janela ao apertar ESC
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	else if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		// Move a câmera pra frente
		camera.forward();
	}
	else if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		// Move a câmera pra trás
		camera.back();
	}
	else if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		// Move a câmera pra esquerda
		camera.left();
	}
	else if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		// Move a câmera pra direita
		camera.right();
	}
	else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		// Se a luz estiver movendo, espaço fará com que fique parada
		isLightMoving = !isLightMoving;
	}
	else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		// Gera novas posições aleatórias
		randomPositions.clear();
		generateRandomPositions();
	}
	else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		// Desativa o noclip
		if (isNoClipOn)
		{
			isNoClipOn = !isNoClipOn;
		}
	}
}

// Gera um vetor de vec3 contendo xpos, zpos and height
void generateRandomPositions()
{
	std::vector<int> availableMetrics = {};

	for (int i = 0; i < 32; i++)
	{
		availableMetrics.push_back(i * 2);
	}

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(0, 30);

	for (int i = 0; i < 20; i++)
	{
		float x = availableMetrics[dist6(rng)];
		float z = availableMetrics[dist6(rng)];
		float height = availableMetrics[dist6(rng)];

		if (height < 4)
		{
			height = 4;
		}
		else if (height > 16)
		{
			height = 4;
		}

		vec3 rand(x, z, height);
		randomPositions.push_back(rand);
	}
}