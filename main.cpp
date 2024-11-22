#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

#define M_PI 3.14159265358979323846

// Declaración de funciones
void menu(int value);
void displayEsfera(void);
void displayCono(void);
void displayCilindro(void);
void inicializaVista(void);
void inicializaVentana(int argc, char** argv);
void reshape(int width, int height);
void anillo(float R, float y, int N);
float calculoX(float R, int N, int n);
float calculoZ(float R, int N, int n);
float fEsfera(float y, float H);
void esferaAlambre(int N);
float fCono(float y);
void conoAlambre(int N);
float fCilindro(float y);
void cilindroAlambre(int N);
void formaAlambre(float H, int N, float(*f)(float y));
void keyboard(unsigned char key, int x, int y);
void idle(void);

// Variables globales
int opcionActual = 1; // 1 = Esfera, 2 = Cono, 3 = Cilindro
GLfloat rotacionX = 0.0f, rotacionY = 0.0f;
GLfloat traslacionX = 0.0f, traslacionY = 0.0f;
GLfloat escala = 1.0f;

// Devuelve el valor de x del punto que gira en el plano 
float calculoX(float R, int N, int n) {
    return R * cos(n * (2 * M_PI) / N);
}

// Devuelve el valor de z 
float calculoZ(float R, int N, int n) {
    return R * sin(n * (2 * M_PI) / N);
}

// Dibuja un anillo de radio R con N divisiones
void anillo(float R, float y, int N) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < N; i++) {
        float x = calculoX(R, N, i);
        float z = calculoZ(R, N, i);
        glVertex3f(x, y, z);
    }
    glEnd();
}

// Calcula el radio del anillo de la esfera
float fEsfera(float y, float H) {
    float radioBase = 50.0; // Radio en el centro de la esfera
    return radioBase * sqrt(1.0f - (y * y) / (H * H / 4.0f));
}

// Dibujamos la esfera
void esferaAlambre(int N) {
    float H = 100.0f; // Altura total de la esfera
    for (int i = 0; i < N; i++) {
        float y = i * H / N - (H / 2); // Calcula el nivel y de cada anillo
        float r = fEsfera(y, H);       // Calcula el radio del anillo en función de y
        anillo(r, y, N);
    }
}

// Calcula el radio del anillo para el cono
float fCono(float y) {
    float H = 100.0f; // Altura total del cono
    return 50.0f * (1.0f - (y + H / 2) / H); // Reduce el radio conforme aumenta la altura
}

// Dibujamos el cono
void conoAlambre(int N) {
    formaAlambre(100.0f, N, fCono);
}

// Calcula el radio del anillo para el cilindro (constante)
float fCilindro(float y) {
    return 50.0f; // Radio constante
}

// Dibujamos el cilindro
void cilindroAlambre(int N) {
    formaAlambre(100.0f, N, fCilindro);
}

// Forma genérica para crear figuras alámbricas
void formaAlambre(float H, int N, float(*f)(float y)) {
    int finerN = 2 * N; // Duplicamos las divisiones
    for (int i = 0; i < finerN; i++) {
        float y = i * H / finerN - (H / 2);  // Más divisiones a lo largo de la altura
        float r = f(y);           // Obtenemos el radio
        anillo(r, y, N);          // Y dibujamos un anillo
    }
}

// Menú para cambiar entre figuras
void menu(int value) {
    opcionActual = value;
    glutPostRedisplay(); // Redibuja la ventana
}

// Display para dibujar la esfera
void displayEsfera(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 0.0f, 1.0f); // Color morado
    glLoadIdentity();
    glTranslatef(traslacionX, traslacionY, -4.0f); // Aplicar traslación
    glScalef(escala, escala, escala);             // Aplicar escalado
    glRotatef(rotacionX, 1.0f, 0.0f, 0.0f);       // Rotar en X
    glRotatef(rotacionY, 0.0f, 1.0f, 0.0f);       // Rotar en Y
    esferaAlambre(20);
    glFlush();
}

// Display para dibujar el cono
void displayCono(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 0.0f, 0.0f); // Color rojo
    glLoadIdentity();
    glTranslatef(traslacionX, traslacionY, -4.0f); // Aplicar traslación
    glScalef(escala, escala, escala);             // Aplicar escalado
    glRotatef(rotacionX, 1.0f, 0.0f, 0.0f);       // Rotar en X
    glRotatef(rotacionY, 0.0f, 1.0f, 0.0f);       // Rotar en Y
    conoAlambre(20);
    glFlush();
}

// Display para dibujar el cilindro
void displayCilindro(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 0.0f, 1.0f); // Color azul
    glLoadIdentity();
    glTranslatef(traslacionX, traslacionY, -4.0f); // Aplicar traslación
    glScalef(escala, escala, escala);             // Aplicar escalado
    glRotatef(rotacionX, 1.0f, 0.0f, 0.0f);       // Rotar en X
    glRotatef(rotacionY, 0.0f, 1.0f, 0.0f);       // Rotar en Y
    cilindroAlambre(20);
    glFlush();
}

// Display principal que llama al dibujo correspondiente
void display(void) {
    if (opcionActual == 1) {
        displayEsfera();
    }
    else if (opcionActual == 2) {
        displayCono();
    }
    else if (opcionActual == 3) {
        displayCilindro();
    }
}

// Configura la vista inicial
void inicializaVista(void) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-100.0f, 100.0f, -100.0f, 100.0f, -100.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Ajusta la vista en caso de redimensionar la ventana
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    inicializaVista();
}

// Control de teclado
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'w': rotacionX -= 5.0f; break; // Rotar arriba
    case 's': rotacionX += 5.0f; break; // Rotar abajo
    case 'a':
        rotacionY -= 5.0f; break; // Rotar izquierda
    case 'd': rotacionY += 5.0f; break; // Rotar derecha
    case 'i': traslacionY += 0.1f; break; // Mover arriba
    case 'k': traslacionY -= 0.1f; break; // Mover abajo
    case 'j': traslacionX -= 0.1f; break; // Mover izquierda
    case 'l': traslacionX += 0.1f; break; // Mover derecha
    case '+': escala += 0.1f; break; // Agrandar
    case '-': escala -= 0.1f; break; // Reducir
    case 27: exit(0); break; // Salir con la tecla Esc
    }
    glutPostRedisplay(); // Redibujar la ventana
}

// Configura la ventana y el menú
void inicializaVentana(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(200, 30);
    glutCreateWindow("Menu: Esfera, Cono y Cilindro");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard); // Vincular el teclado

    // Crear el menú
    glutCreateMenu(menu);
    glutAddMenuEntry("Esfera", 1);
    glutAddMenuEntry("Cono", 2);
    glutAddMenuEntry("Cilindro", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON); // Botón derecho del mouse activa el menú
}

// Inicializa OpenGL
void iniciaOpenGL(int argc, char** argv) {
    inicializaVentana(argc, argv);
    inicializaVista();
    glutMainLoop();
}

// Función principal
int main(int argc, char** argv) {
    iniciaOpenGL(argc, argv);
    return 0;
}
