#include <GL/glut.h>
#include <math.h>

void inicio();
void desenha();
void toFazendoNada();
void teclado(unsigned char tecla, int x, int y);

float anguloInicial = M_PI / 6.0;
float velocidade = 0.01;

int main(int argc, char** argv) {
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); // profundidade, um framebuffer e sistema de cores RGB 255, 255, 255
    glutInitWindowPosition(100, 100); // 100 pixels para a direito e 100 pixels para baixo
    glutInitWindowSize(500, 500); // quantidade em pixels
    glutCreateWindow("Primitivas");

    inicio();

    glutKeyboardFunc(teclado); //funcao que vai tratar as entradas de teclado
    glutIdleFunc(toFazendoNada);
    glutDisplayFunc(desenha);

    glutMainLoop();

    return 0;
}

void inicio() {
    glClearColor(0,0,0,0); // limpar as cores da caixa
}

//quantidade de divisões e o raio
void desenhaCirculo(int n, float raio) {
    float delta = 2.0 * M_PI / n; //circulo dividido n vezes (radianos)
    for(int i = 0; i < n; i++) {
        float angulo = anguloInicial + i * delta;
        float x = raio * cos(angulo);
        float y = raio * sin(angulo);

        glVertex2f(x, y);
    }
}

void teclado(unsigned char tecla, int x, int y) {
    switch(tecla) {
        case 'v':
            velocidade = velocidade - 0.001;
            break;
        case 'b':
            velocidade = velocidade + 0.01;
            break;
        case 'i':
            velocidade = velocidade * (-1);
            break;
    }
}

void toFazendoNada() {
    anguloInicial += velocidade * 2.0 * M_PI / 360.0;
    if(anguloInicial > 2 * M_PI) anguloInicial -= 2 * M_PI;

    glutPostRedisplay(); //redesenhar o poligono apos alterar o angulo inicial ^
}

void desenha() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    //xMin, xMax, yMin, yMax, zMin, zMax
    glOrtho(-25, 25, -25, 25, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //GL_POINTS         -> pontos
    //GL_LINES          -> segmentos de reta de 2 em 2
    //GL_LINE_STRIP     -> linha conectada em sequencia 
    //GL_LINE_LOOP      -> uma linha conectada fechada
    //GL_TRIANGLES      -> triangulos
    //GL_TRIANGLE_STRIP -> 
    //GL_TRIANGLE_FAN   -> leque de triangulos
    //GL_POLYGON        -> poligonos
    glBegin(GL_POLYGON);
        desenhaCirculo(8, 25);
    glEnd();

    //glFlush(); // um bufer
    glutSwapBuffers(); //alterna entre buffers
}