// Projeto.cpp : Pedro Afonso Ferreira Lopes Martins N.º 2019216826
// Desenvolvido no âmbito da cadeira de Computação Gráfica da Universidade de Coimbra, 3º ano da licenciatura de Engenharia Informática. v.1.1

#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "RgbImage.h"

//Variáveis gerais do programa.
int projecao = 0, is_on = 0, left_click = 0, right_click = 0;
double joystick_x = 0, joystick_z = 0, is_pressed_x = 0, is_pressed_y = 0, was_pressed = 0;
double rotate_y = 0, x_axis = 0.05, x_pressed = 0, rotate_man = 0;
double rotate_x = 0, y_axis = 0.05, y_pressed = 0;

//Texto e ecrã.
GLint wScreen = 1920, hScreen = 1080;
GLfloat	SIZE = 12.0;
char     texto[30];

//Variáveis observador.
GLfloat  rVisao = 3, aVisao = +0.5 * 3.14159, incVisao = 1;
GLfloat  obsP[] = { rVisao * cos(aVisao), 0.0, rVisao * sin(aVisao) };
float	 anguloZ = 35;

//Posição default dos vértices, NÃO ALTERAR!
GLfloat tam = 0.5;

//Array de vértices para construção de um paralelepípedo.
static GLfloat vertices[] = {
    // Esquerda
    -tam,  -tam,  tam,	// 0 
    -tam,   tam,  tam,	// 1 
    -tam,   tam, -tam,	// 2 
    -tam,  -tam, -tam,	// 3 
    // Direita
    tam,  -tam,  tam,	// 4 
    tam,   tam,  tam,	// 5 
    tam,   tam, -tam,	// 6 
    tam,  -tam, -tam,	// 7 
    // Cima
    -tam,  tam,  tam,	// 8 
    -tam,  tam, -tam,	// 9 
    tam,  tam, -tam,	// 10 
    tam,  tam,  tam,	// 11 
    //
    -tam, -tam, tam,    // 12
    -tam, -tam, -tam,   // 13
    tam, -tam, -tam,    // 14
    tam, -tam, tam,     // 15
    //
    tam, -tam, tam,     // 16
    tam, tam, tam,      // 17
    -tam, tam, tam,     // 18
    -tam, -tam, tam,    // 19
    //
    tam, -tam, -tam,    // 20
    tam, tam, -tam,     // 21
    -tam, tam, -tam,    // 22
    -tam, -tam, -tam,   // 23

};

//Array de normais para construção de um paralelepípedo.
static GLfloat normais[] = {
    // Esquerda
      -1.0, 0.0, 0.0,
      -1.0, 0.0, 0.0,
      -1.0, 0.0, 0.0,
      -1.0, 0.0, 0.0,
      // Direita
      1.0, 0.0, 0.0,
      1.0, 0.0, 0.0,
      1.0, 0.0, 0.0,
      1.0, 0.0, 0.0,
      // Cima
      0.0, 1.0, 0.0,
      0.0, 1.0, 0.0,
      0.0, 1.0, 0.0,
      0.0, 1.0, 0.0,
      //
      0.0, -1.0, 0.0,
      0.0, -1.0, 0.0,
      0.0, -1.0, 0.0,
      0.0, -1.0, 0.0,
      //
      0.0, 0.0, 1.0,
      0.0, 0.0, 1.0,
      0.0, 0.0, 1.0,
      0.0, 0.0, 1.0,
      //
      0.0, 0.0, -1.0,
      0.0, 0.0, -1.0,
      0.0, 0.0, -1.0,
      0.0, 0.0, -1.0,
};

//Array de coordenadas de textura para construção de um paralelepípedo.
static GLfloat texturas[] = {
0, 0,
1, 0,
1, 1,
0, 1,
0, 0,
1, 0,
1, 1,
0, 1,
0, 0,
1, 0,
1, 1,
0, 1,
0, 0,
1, 0,
1, 1,
0, 1,
0, 0,
1, 0,
1, 1,
0, 1,
0, 0,
1, 0,
1, 1,
0, 1 };

//Atribuição de faces a parâmetros previamente definidos nos arrays.
static GLuint esquerda[] = { 0, 1,  2,  3 };
static GLuint direita[] = { 4, 7,  6,  5 };
static GLuint cima[] = { 8, 9, 10, 11 };
static GLuint baixo[] = { 12, 13, 14, 15 };
static GLuint tras[] = { 16, 17, 18, 19 };
static GLuint frente[] = { 20, 21, 22, 23 };
RgbImage imag;

GLUquadricObj* esfera = gluNewQuadric();
GLuint   texture[3];

//Definições da luz do teto (pontual).
GLfloat localPos[4] = { 0.0, 5.0, 0.0, 1.0 };
GLfloat localCorAmb[4] = { 1.0, 0, 0, 1.0 };
GLfloat localCorDif[4] = { 1.0, 0, 0, 1.0 };
GLfloat localCorEsp[4] = { 1.0, 0, 0, 1.0 };
GLint   ligaTeto = 0;		 //:::   'T'  

//Definições da luz ambiente.
GLint   Dia = 1;     //:::   'Z'  
GLfloat intensidadeDia = 1.0;
GLfloat luzGlobalCorAmb[4] = { intensidadeDia, intensidadeDia, intensidadeDia, 1.0 };

//Definições da luz de foco central.
GLint ligaFoco = 1;          //:::   'F'
GLfloat intensidadeF = 1;    //:::   'V'
GLint   luzR = 0;		 	 //:::   'R'  
GLint   luzG = 1;			 //:::   'G'  
GLint   luzB = 0;            //:::   'B'

//Estado da malha (1 ATIVADO | 0 DESLIGADO).
GLint   malha = 1;           //:::   'C'

//Especularidade dos botões (De 0 a 1).
GLfloat shine = 0.4;        //:::    'X'

//Função para definir a luzes a ser utilizadas.
void defineLuzes() {
    GLfloat localCorDifF[4] = { luzR * intensidadeF, luzG * intensidadeF, luzB * intensidadeF, 1.0 };
    GLfloat Foco_direccao[] = { 0, -1, 0, 0 };	// -Y
    GLfloat Foco_direccao2[] = { 0, -1, 0, 0 };	// -Y
    GLfloat Foco1_cor[] = { 1, 0, 0, 0.1 };	// RED
    GLfloat Foco_ak = 1.0;
    GLfloat Foco_al = 0.05f;
    GLfloat Foco_aq = 0.0f;
    GLfloat Foco_Expon = 2;		// Foco, SPOT_Exponent
    GLfloat Pos1[] = { 0, 4, 0, 1.0 };   // Posicao (0, 4, 0)
    GLfloat Pos2[] = { -0.4, 1, 0.4, 1.0 };   // Posicao (0, 4, 0)
    GLfloat aberturaFoco = 5;

    //Luz Ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCorAmb);

    //Luz do Teto
    glLightfv(GL_LIGHT0, GL_POSITION, localPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, localCorAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, localCorDif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, localCorEsp);

    //Foco Central
    glLightfv(GL_LIGHT1, GL_POSITION, Pos1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, localCorDifF);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, Foco_ak);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, Foco_al);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, Foco_aq);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, aberturaFoco);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, Foco_direccao);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, Foco_Expon);


    //Foco luz RED/GREEN
    glLightfv(GL_LIGHT2, GL_POSITION, Pos2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, Foco1_cor);
    glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, Foco_ak);
    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, Foco_al);
    glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, Foco_aq);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, aberturaFoco);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, Foco_direccao2);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, Foco_Expon);

}

//Função para definir os materiais a ser utilizados na cena.
void initMaterials(int spec) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Gold
    if (spec == 0) {

        GLfloat  goldAmb[] = { 0.24725 ,0.1995 ,0.0745, 1 };
        GLfloat  goldDif[] = { 0.75164 ,0.60648 ,0.22648, 1 };
        GLfloat  goldSpec[] = { 0.628281 ,0.555802 ,0.366065, 1 };
        GLint  goldCoef = 0.4 * 128;

        glMaterialfv(GL_FRONT, GL_AMBIENT, goldAmb);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, goldDif);
        glMaterialfv(GL_FRONT, GL_SPECULAR, goldSpec);
        glMaterialf(GL_FRONT, GL_SHININESS, goldCoef);

    }
    //Ruby
    else if (spec == 1) {

        GLfloat  rubyAmb[] = { 0.1745 ,0.01175 ,0.01175, 1 };
        GLfloat  rubyDif[] = { 0.61424 ,0.04136 ,0.04136, 1 };
        GLfloat  rubySpec[] = { 0.727811 ,0.626959 ,0.626959, 1 };
        GLint  rubyCoef = 0.6 * 128;

        glMaterialfv(GL_FRONT, GL_AMBIENT, rubyAmb);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, rubyDif);
        glMaterialfv(GL_FRONT, GL_SPECULAR, rubySpec);
        glMaterialf(GL_FRONT, GL_SHININESS, rubyCoef);

    }
    //Turquoise
    else if (spec == 2) {

        GLfloat  turqAmb[] = { 0.1 ,0.18725 ,0.1745, 1 };
        GLfloat  turqDif[] = { 0.396 ,0.74151 ,0.69102, 1 };
        GLfloat  turqSpec[] = { 0.297254 ,0.30829 ,0.306678, 1 };
        GLint  turqCoef = 0.1 * 128;

        glMaterialfv(GL_FRONT, GL_AMBIENT, turqAmb);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, turqDif);
        glMaterialfv(GL_FRONT, GL_SPECULAR, turqSpec);
        glMaterialf(GL_FRONT, GL_SHININESS, turqCoef);
    }
    //Silver
    else if (spec == 3) {

        GLfloat  silvAmb[] = { 0.19225 ,0.19225 ,0.19225, 1 };
        GLfloat  silvDif[] = { 0.50754 ,0.50754 ,0.50754, 1 };
        GLfloat  silvSpec[] = { 0.508273 ,0.508273 ,0.508273, 1 };
        GLint  silvCoef = 0.4 * 128;

        glMaterialfv(GL_FRONT, GL_AMBIENT, silvAmb);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, silvDif);
        glMaterialfv(GL_FRONT, GL_SPECULAR, silvSpec);
        glMaterialf(GL_FRONT, GL_SHININESS, silvCoef);
    }
    //Obsidian
    else if (spec == 4) {

        GLfloat  obsiAmb[] = { 0.05375 ,0.05 ,0.06625, 1 };
        GLfloat  obsiDif[] = { 0.18275 ,0.17 ,0.22525, 1 };
        GLfloat  obsiSpec[] = { 0.332741 ,0.328634 ,0.346435, 1 };
        GLint  obsiCoef = 0.3 * 128;

        glMaterialfv(GL_FRONT, GL_AMBIENT, obsiAmb);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, obsiDif);
        glMaterialfv(GL_FRONT, GL_SPECULAR, obsiSpec);
        glMaterialf(GL_FRONT, GL_SHININESS, obsiCoef);
    }
    //Gold Changeable
    else if (spec == 5) {

        GLfloat  goldAmb[] = { 0.24725 ,0.1995 ,0.0745, 1 };
        GLfloat  goldDif[] = { 0.75164 ,0.60648 ,0.22648, 1 };
        GLfloat  goldSpec[] = { 0.628281 ,0.555802 ,0.366065, 1 };
        GLint  goldCoef = shine * 128;

        glMaterialfv(GL_FRONT, GL_AMBIENT, goldAmb);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, goldDif);
        glMaterialfv(GL_FRONT, GL_SPECULAR, goldSpec);
        glMaterialf(GL_FRONT, GL_SHININESS, goldCoef);
    }

}

// Função para desenhar os 3 eixos (x, y e z).
void drawEixos() {
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0.5 * SIZE, 0, 0);
    glEnd();
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0.5 * SIZE, 0);
    glEnd();
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 0.5 * SIZE);
    glEnd();
}

//Função para iniciar as texturas a ser utilizadas (criação de objeto 2D, load da imagem, modo de mapeamento, e definição de parametros de filtragem e repetição) 
void initTexturas()
{
    // Base do comando
    glGenTextures(1, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    imag.LoadBmpFile("metal.bmp");
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
        imag.GetNumCols(),
        imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
        imag.ImageData());

    //SkyBox
    glGenTextures(1, &texture[1]);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    imag.LoadBmpFile("housings.bmp");
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
        imag.GetNumCols(),
        imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
        imag.ImageData());

    //Botões
    glGenTextures(1, &texture[2]);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    imag.LoadBmpFile("carbono.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
        imag.GetNumCols(),
        imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
        imag.ImageData());

}

// Função para desenhar a preto os contornos de paralelepípedos.
void drawOutline(int num) {

    glColor3f(0.0, 0.0, 0.0);
    switch (num) {
    case 0:
        glBegin(GL_LINE_LOOP);
        glVertex3f(-tam, -tam, tam);
        glVertex3f(-tam, tam, tam);
        glVertex3f(-tam, tam, -tam);
        glVertex3f(-tam, -tam, -tam);
        glEnd();
        break;
    case 1:
        glBegin(GL_LINE_LOOP);
        glVertex3f(tam, -tam, tam);
        glVertex3f(tam, tam, tam);
        glVertex3f(tam, tam, -tam);
        glVertex3f(tam, -tam, -tam);
        glEnd();
        break;
    case 2:
        glBegin(GL_LINE_LOOP);
        glVertex3f(-tam, tam, tam);
        glVertex3f(-tam, tam, -tam);
        glVertex3f(tam, tam, -tam);
        glVertex3f(tam, tam, tam);
        glEnd();
        break;
    case 3:
        glBegin(GL_LINE_LOOP);
        glVertex3f(-tam, -tam, tam);
        glVertex3f(-tam, -tam, -tam);
        glVertex3f(tam, -tam, -tam);
        glVertex3f(tam, -tam, tam);
        glEnd();
        break;
    case 4:
        glBegin(GL_LINE_LOOP);
        glVertex3f(tam, -tam, tam);
        glVertex3f(tam, tam, tam);
        glVertex3f(-tam, tam, tam);
        glVertex3f(-tam, -tam, tam);
        glEnd();
        break;
    case 5:
        glBegin(GL_LINE_LOOP);
        glVertex3f(tam, -tam, -tam);
        glVertex3f(tam, tam, -tam);
        glVertex3f(-tam, tam, -tam);
        glVertex3f(-tam, -tam, -tam);
        glEnd();
        break;
    }

}

// Função para desenhar com determinada cor, textura ou mesh).
void drawSquare(int num, int textura, int mesh) {
    GLint dim = 360;
    GLfloat med_dim = (GLfloat)dim;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    drawOutline(0);
    drawOutline(1);
    drawOutline(2);
    drawOutline(3);
    drawOutline(4);
    drawOutline(5);

    switch (num) {
    case 0:
        glColor3f(1.0, 0.0, 0.0); // Red
        break;
    case 1:
        glColor3f(0.5f, 1.0f, 1.0f); // Green
        break;
    case 2:
        glColor3f(0.0, 0.0, 1.0); // Blue
        break;
    case 3:
        glColor3f(0.0, 0.0, 0.0); // Black
        break;
    case 4:
        glColor3f(1.0, 1.0, 1.0); // White
        break;
    case 5:
        glColor3f(0.1f, 0.1f, 0.1f); // Dark Grey
        break;
    case 6:
        glColor3f(1.0, 0.5, 1.0); // White
        break;
    case 7:
        glColor3f(0.5, 1.0, 1.0); // White
        break;
    case 8:
        break;

    }

    //Se o utilizador quer utilizar a mesh na parte de cima do polígono.
    if (mesh == 1) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, esquerda);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, direita);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        //Cima 
        glPushMatrix();
        glRotatef(90, 1, 0, 0);
        glTranslatef(-0.5, -0.5, -0.5);
        glBegin(GL_QUADS);
        for (int i = 0; i < dim; i++)
            for (int j = 0; j < dim; j++) {
                glTexCoord2f((float)j / dim, (float)i / dim);
                glVertex3d((float)j / med_dim, (float)i / med_dim, 0);
                glTexCoord2f((float)(j + 1) / dim, (float)i / dim);
                glVertex3d((float)(j + 1) / med_dim, (float)i / med_dim, 0);
                glTexCoord2f((float)(j + 1) / dim, (float)(i + 1) / dim);
                glVertex3d((float)(j + 1) / med_dim, (float)(i + 1) / med_dim, 0);
                glTexCoord2f((float)j / dim, (float)(i + 1) / dim);
                glVertex3d((float)j / med_dim, (float)(i + 1) / med_dim, 0);
            }
        glEnd();
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, baixo);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, tras);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, frente);
        glDisable(GL_TEXTURE_2D);
        return;
    }

    //Se o utilizador quer aplicar textura no objeto
    if (textura == 1) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, esquerda);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, direita);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, cima);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, baixo);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, tras);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, frente);
        glDisable(GL_TEXTURE_2D);
    }

    //Desenho padrão de um paralelepipedo sem texturas e mesh.
    else {
        glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, esquerda);
        glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, direita);
        glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, cima);
        glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, baixo);
        glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, tras);
        glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, frente);
    }
}

// Função para desenhar uma esfera. 
void drawSphere(GLfloat radius, GLfloat ncount, GLfloat vcount) {
    GLfloat angy;
    GLfloat angxz = 0;
    GLfloat y, xandy;

    for (GLfloat i = 0; i < vcount; i++) {
        angy = 90 - (i * (180 / vcount));
        xandy = radius * cos(angy);
        y = cos(angy) * radius;

        for (GLfloat j = 0; j < ncount; j++) {
            angxz = (360 / ncount) * j;
            float exangy, exangxz;

            if (i + 1 == vcount) {
                exangy = 360;
            }
            else {
                exangy = angy - (360 / vcount);
            }

            if (j + 1 == ncount) {
                exangxz = 180;
            }
            else {
                exangxz = angxz + (180 / ncount);
            }



            glBegin(GL_LINE_LOOP);
            glNormal3f(cos(angxz) * sin(angy), y / radius, sin(angxz) * sin(angy));
            glVertex3f(cos(angxz) * sin(angy) * radius, y, sin(angxz) * sin(angy) * radius);
            glNormal3f(cos(angxz) * sin(exangy), cos(exangy), sin(angxz) * sin(exangy));
            glVertex3f(cos(angxz) * sin(exangy) * radius, cos(exangy) * radius, sin(angxz) * sin(exangy) * radius);
            glNormal3f(cos(exangxz) * sin(exangy), cos(exangy), sin(exangxz) * sin(exangy));
            glVertex3f(cos(exangxz) * sin(exangy) * radius, cos(exangy) * radius, sin(exangxz) * sin(exangy) * radius);
            glNormal3f(cos(exangxz) * sin(angy), y / radius, sin(exangxz) * sin(angy));
            glVertex3f(cos(exangxz) * sin(angy) * radius, y, sin(exangxz) * sin(angy) * radius);
            glEnd();

        }
    }
}

//Função para criar dois círculos em posições diferentes de y.
void drawCircle(GLfloat intensity, GLfloat radius) {

    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= intensity; i++) {
        //glNormal3f(cos(i * 360 / intensity), 1, sin(i * 360 / intensity));
        glNormal3f(0, 0, -1);
        glVertex2f((radius * cos(i * 360 / intensity)), (radius * sin(i * 360 / intensity)));
    }
    glEnd();
}

//Função para criar um cilindro.
void drawCilinder(GLfloat res, GLfloat radius, GLfloat tamtop, GLfloat tambot) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    for (int j = 0; j < res; j++) {
        GLfloat ang1 = 360 / res * j;
        GLfloat ang2 = 0;

        if (j + 1 == res) {
            ang2 = 360;
        }
        else {
            ang2 = ang1 + (360 / res);
        }
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture[2]);

        glBegin(GL_QUADS);
        glTexCoord2f(1, 1);
        glNormal3f(cos(ang1), 0, sin(ang1));
        glVertex3f(radius * cos(ang1), tamtop, radius * sin(ang1));

        glTexCoord2f(0, 1);
        glNormal3f(cos(ang2), 0, sin(ang2));
        glVertex3f(radius * cos(ang2), tamtop, radius * sin(ang2));

        glTexCoord2f(0, 0);
        glNormal3f(cos(ang2), 0, sin(ang2));
        glVertex3f(radius * cos(ang2), tambot, radius * sin(ang2));

        glTexCoord2f(1, 0);
        glNormal3f(cos(ang1), 0, sin(ang1));
        glVertex3f(radius * cos(ang1), tambot, radius * sin(ang1));
        glEnd();

        glDisable(GL_TEXTURE_2D);
    }
}

//Função para desenhar a esfera com textura para a SkyBox.
void drawEsfera() {

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    gluQuadricDrawStyle(esfera, GLU_FILL);
    gluQuadricNormals(esfera, GLU_SMOOTH);
    gluQuadricTexture(esfera, GL_TRUE);
    gluSphere(esfera, 60.0, 100, 100);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

//Função para premir botão da direita.
void pressed_Right() {
    if (is_pressed_x == 1) {
        glTranslatef(0.2, x_axis - 0.06, -0.2);
        x_axis += 0.02;
        glTranslatef(0, x_axis, 0);
    }
    if (is_pressed_x == 0) {
        glTranslatef(0.2, x_axis, -0.2);
    }
    is_pressed_x = 0;

}

//Função para premir botão da esquerda.
void pressed_Left() {
    if (is_pressed_y == 1) {
        glTranslatef(0.2, y_axis - 0.06, 0.2);
        y_axis += 0.02;
        glTranslatef(0, y_axis, 0);
    }
    if (is_pressed_y == 0) {
        glTranslatef(0.2, y_axis, 0.2);
    }
    is_pressed_y = 0;

}

//Função para desenhar texto no ecrã do utilizador.
void desenhaTexto(char* string, GLfloat x, GLfloat y, GLfloat z) {
    glRasterPos3f(x, y, z);
    while (*string)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *string++);
}

//Função responsável por desenhar a cena no ecrã do utilizador.
void drawScene() {

    drawEsfera();

    glPushMatrix();

    glDisable(GL_LIGHTING);
    drawEixos();
    glEnable(GL_LIGHTING);

    glRotatef(rotate_x, 0.0, 0.0, 1.0);
    glRotatef(rotate_y, 0.0, 1.0, 0.0);

    initMaterials(3);

    glScalef(2, 2, 2);

    //Base do comando.
    glPushMatrix();
    glScalef(0.6, 0.1, 0.6);
    if (malha == 1) {
        drawSquare(8, 1, 1);
    }
    else {
        drawSquare(8, 1, 0);
    }
    glPopMatrix();

    initMaterials(5);
    //Botão direito principal
    glPushMatrix();
    pressed_Right();
    glScalef(0.05, 0.1, 0.05);
    glPushMatrix();
    glTranslatef(0, 0.5, 0);
    glRotatef(90, 1, 0, 0);
    drawCircle(360, 1);
    glPopMatrix();
    drawCilinder(360, 1, 0.5, -0.5);
    glPopMatrix();

    //Botão esquerdo principal
    glPushMatrix();
    pressed_Left();
    glScalef(0.05, 0.1, 0.05);
    glPushMatrix();
    glTranslatef(0, 0.5, 0);
    glRotatef(90, 1, 0, 0);
    drawCircle(360, 1);
    glPopMatrix();
    drawCilinder(360, 1, 0.5, -0.5);
    glPopMatrix();

    initMaterials(3);
    //Disco giratório.
    glPushMatrix();
    glColor3f(0, 0, 0);
    glRotatef(-rotate_man, 0, 1, 0);
    glScalef(0.05, 0.1, 0.05);
    glPushMatrix();
    glTranslatef(0, 0.01, 0);
    glBegin(GL_LINES);
    glVertex3f(0.0, 1.0, 0.0);
    glVertex3f(-1.0, 1.0, -1.0);
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, 1, 0);
    glRotatef(90, 1, 0, 0);
    drawCircle(360, 1.5);
    glPopMatrix();
    drawCilinder(360, 1.5, 1, -0.5);
    glPopMatrix();

    //Joystick
    glPushMatrix();
    glTranslatef(-0.2, 0.1, 0.0);
    glRotatef(joystick_z * 100, 1, 0, 0);
    glRotatef(joystick_x * 100, 0, 0, 1);
    glPushMatrix();
    glTranslatef(0, 0.2, 0.0);
    drawSphere(0.1, 160, 160);
    glPopMatrix();
    glScalef(0.02, 0.3, 0.02);
    initMaterials(4);
    drawSquare(8, 0, 0);
    glPopMatrix();

    //Base do joystick
    glPushMatrix();
    glTranslatef(-0.2, 0.03, 0.0);
    drawSphere(0.05, 160, 160);
    glPopMatrix();

    initMaterials(3);

    //Luz do comando
    glDisable(GL_LIGHTING);

    glPushMatrix();
    if (is_on == 0) {
        glDisable(GL_LIGHT2);
        glColor4f(0, 1, 0, 0.1);
    }
    else {
        glEnable(GL_LIGHT2);
        glColor4f(1, 0, 0, 0.1);
    }
    glTranslatef(-0.2, 0.06, 0.2);
    drawSphere(0.03, 160, 160);
    glPopMatrix();

    glEnable(GL_LIGHTING);

    //Interruptor do comando
    glPushMatrix();

    if (is_on == 0) {
        glPushMatrix();
        glRotatef(45, 0, 1, 0);
        glTranslatef(0.2, 0, -0.21);
        glRotatef(90, 1, 0, 0);
        glScalef(0.05, 0.05, 0.05);
        drawSquare(8, 0, 0);
        glPopMatrix();
    }
    else {
        glPushMatrix();
        glRotatef(45, 0, 1, 0);
        glTranslatef(0.28, 0, -0.135);
        glRotatef(90, 1, 0, 0);
        glScalef(0.05, 0.05, 0.05);
        drawSquare(8, 0, 0);
        glPopMatrix();
    }

    glPushMatrix();
    glTranslatef(0.05, 0, -0.277);
    glRotatef(90, 1, 0, 0);
    glScalef(0.1, 0.05, 0.05);
    drawSquare(8, 0, 0);
    glPopMatrix();
    glPopMatrix();

    initMaterials(4);

    //Bases de borracha do comando (Material: Silver)
    glPushMatrix();
    glPushMatrix();
    glTranslatef(-0.22, -0.06, 0.22);
    drawSphere(0.02, 80, 80);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.22, -0.06, -0.22);
    drawSphere(0.02, 80, 80);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.22, -0.06, -0.22);
    drawSphere(0.02, 80, 80);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.22, -0.06, 0.22);
    drawSphere(0.02, 80, 80);
    glPopMatrix();
    glPopMatrix();

    glPopMatrix();

    initMaterials(3);
}
//Função principal do programa. Responsável por algumas definições, projeção/transformação visual e desenho da cena.
void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //1 - Viewport (Desenho do texto informativo)
    glViewport(0 * wScreen, 0.4 * hScreen, 0.2 * wScreen, 0.4 * hScreen);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-20, 20, -20, 20, -100, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 30, 0, 0, 0, 0, 0, 0, -1);
    glDisable(GL_LIGHTING);

    //Desenho e layout do texto
    glColor3f(1, 1, 1);
    sprintf_s(texto, 30, "%d - Luz Ambiente 'Z'", Dia);
    desenhaTexto(texto, -15, 0, -20);
    sprintf_s(texto, 30, "%d - Luz Teto 'T' ", ligaTeto);
    desenhaTexto(texto, -15, 0, -18);
    sprintf_s(texto, 30, "%d - Luz Foco 'F' ", ligaFoco);
    desenhaTexto(texto, -15, 0, -16);
    sprintf_s(texto, 30, "      %d - Cor Red 'R' ", luzR);
    desenhaTexto(texto, -15, 0, -14);
    sprintf_s(texto, 30, "      %d - Cor Green 'G' ", luzG);
    desenhaTexto(texto, -15, 0, -12);
    sprintf_s(texto, 30, "      %d - Cor Blue 'B' ", luzB);
    desenhaTexto(texto, -15, 0, -10);
    sprintf_s(texto, 30, "      %4.2f - Intensidade 'V' ", intensidadeF);
    desenhaTexto(texto, -15, 0, -8);
    sprintf_s(texto, 30, "%d - Estado do comando 'O' ", !is_on);
    desenhaTexto(texto, -15, 0, -6);
    sprintf_s(texto, 30, "%d - Malha 'C'", malha);
    desenhaTexto(texto, -15, 0, -4);
    sprintf_s(texto, 30, "%4.2f - Especularidade 'X'", shine);
    desenhaTexto(texto, -15, 0, -2);

    if (projecao == 0) {
        sprintf_s(texto, 30, "PERSPETIVA - Projecao 'P' ");
    }
    else {
        sprintf_s(texto, 30, "ORTOGONAL - Projecao 'P' ");
    }

    desenhaTexto(texto, -15, 0, 0);
    sprintf_s(texto, 30, "%d - Cliques Esquerdo 'Q' ", left_click);
    desenhaTexto(texto, -15, 0, 2);
    sprintf_s(texto, 30, "%d - Cliques Direito 'E' ", right_click);
    desenhaTexto(texto, -15, 0, 4);

    //2 - Viewport (Desenho da cena)
    glViewport(0, 0, wScreen, hScreen);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (projecao == 0) {
        gluPerspective(anguloZ, (float)wScreen / hScreen, 0.1, 300);
    }

    if (projecao == 1) {
        glOrtho(-5, 5, -5, 5, -5, 5);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(obsP[0], obsP[1], obsP[2], 0, obsP[1], 0, 0, 1, 0);

    glEnable(GL_LIGHTING);
    if (ligaFoco == 1) {
        glEnable(GL_LIGHT1);
    }
    else {
        glDisable(GL_LIGHT1);
    }
    if (ligaTeto == 1) {
        glEnable(GL_LIGHT0);
    }
    else {
        glDisable(GL_LIGHT0);
    }
    defineLuzes();

    drawScene();

    glEnable(GL_LIGHTING);
    glFlush();
    glutSwapBuffers();

}

void Timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(10, Timer, 1);
}

// Função para o teclado (Q para premir botão esquerdo / E para premir botão esquerdo)
void keyboard(unsigned char key, int x, int y) {

    switch (key) {

        //Rotação direira do disco giratório.
    case 'M': case 'm':
        if (rotate_man < 0) {
            printf("Limite mínimo excedido!\n");
        }
        else {
            rotate_man -= 5;
        }
        glutPostRedisplay();
        break;

        //Rotação esquerda do disco giratório.
    case 'N': case 'n':
        if (rotate_man > 270) {
            printf("Limite máximo excedido!\n");
        }
        else {
            rotate_man += 5;
        }
        glutPostRedisplay();
        break;

        //Desligar/Ligar o comando (Comando desligado impossibilita quaisquer rotações!).
    case 'O': case 'o':
        if (is_on == 0) {
            is_on = 1;
            rotate_x = 0;
            rotate_y = 0;
        }
        else {
            is_on = 0;
        }
        glutPostRedisplay();
        break;

        //Mudança de projeção (Ortogonal -> Prespetiva OU Prespetiva -> Ortogonal).
    case 'P': case 'p':
        if (projecao == 0) {
            projecao = 1;
        }
        else {
            projecao = 0;
        }
        glutPostRedisplay();
        break;

        //Aumentar/Diminuir especularidade dos dois botões centrais do comando (Valores entre 0 e 1 com incremento de 0.1).
    case 'x': case 'X':
        shine = shine + 0.05;
        if (shine > 1.05) shine = 0;
        glutPostRedisplay();
        break;

        //Ativação da malha (mesh) na base do comando, conforme a preferência do utilizador).
    case 'c':case 'C':
        malha = !malha;
        glutPostRedisplay();
        break;

        //Ligar/Desligar a luz do teto (Luz Pontual sem Foco).
    case 't':case 'T':
        ligaTeto = !ligaTeto;
        glutPostRedisplay();
        break;

        //Aumentar/Diminuir intensidade do foco incidente no meio do comando (Valores entre 0 e 1 com incremento de 0.1).
    case 'v': case 'V':
        intensidadeF = intensidadeF + 0.1;
        if (intensidadeF > 1.1) intensidadeF = 0;
        glutPostRedisplay();
        break;

        //Ligar/Desligar a componente vermelho do foco central.
    case 'r':case 'R':
        luzR = (luzR + 1) % 2;
        glutPostRedisplay();
        break;

        //Ligar/Desligar a componente verde do foco central.
    case 'g':case 'G':
        luzG = (luzG + 1) % 2;
        glutPostRedisplay();
        break;

        //Ligar/Desligar a componente azul do foco central.
    case 'b':case 'B':
        luzB = (luzB + 1) % 2;
        glutPostRedisplay();
        break;

        //Ligar/Desligar a luz ambiente do cenário.
    case 'z':	case 'Z':
        Dia = !Dia;
        if (Dia) {
            luzGlobalCorAmb[0] = 1.0;
            luzGlobalCorAmb[1] = 1.0;
            luzGlobalCorAmb[2] = 1.0;
        }
        else {
            luzGlobalCorAmb[0] = 0.0;
            luzGlobalCorAmb[1] = 0.0;
            luzGlobalCorAmb[2] = 0.0;
        }
        printf("%d", Dia);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCorAmb);
        glutPostRedisplay();
        break;

        //Ligar/Desligar o foco central.
    case 'f': case 'F':
        ligaFoco = !ligaFoco;
        if (ligaFoco)
            glEnable(GL_LIGHT1);
        else
            glDisable(GL_LIGHT1);
        glutPostRedisplay();
        break;

        //Afastar joysticker da posição inicial do observador.
    case 'I': case 'i':
        if (joystick_x <= 0.1) {
            joystick_x += 0.02;
        }
        glutPostRedisplay();
        break;

        //Mover joysticker para a esquerda.
    case 'J': case 'j':
        if (joystick_z <= 0.1) {
            joystick_z += 0.02;
        }
        glutPostRedisplay();
        break;

        //Aproximar joysticker da posição inicial do observador.
    case 'K': case 'k':
        if (joystick_x >= -0.1) {
            joystick_x -= 0.02;
        }
        glutPostRedisplay();
        break;

        //Mover joysticker para a direita.
    case 'L': case 'l':
        if (joystick_z >= -0.1) {
            joystick_z -= 0.02;
        }
        glutPostRedisplay();
        break;

        //Girar comando (+) segundo o eixo Z.
    case 'W': case 'w':
        if (is_on == 0)
            rotate_x += 5;
        glutPostRedisplay();
        break;

        //Girar comando (-) segundo o eixo Y.
    case 'A': case 'a':
        if (is_on == 0)
            rotate_y -= 5;
        glutPostRedisplay();
        break;

        //Girar comando (-) segundo o eixo Z.
    case 'S': case 's':
        if (is_on == 0)
            rotate_x -= 5;
        glutPostRedisplay();
        break;

        //Girar comando (+) segundo o eixo Y.
    case 'D': case 'd':
        if (is_on == 0)
            rotate_y += 5;
        glutPostRedisplay();
        break;

        //Clique do botão direito e acréscimo à contagem de cliques efetuados.
    case 'E': case 'e':
        if (x_axis < 0.05) {
            x_axis = 0.05;
        }

        right_click++;
        is_pressed_x = 1;

        if (x_pressed == 0) {
            printf("1 | x_axis = %f\n", x_axis);
            x_axis -= 0.02;

        }

        glutPostRedisplay();
        break;

        //Clique do botão esquerdo e acréscimo à contagem de cliques efetuados.
    case 'Q': case 'q':
        if (y_axis < 0.05) {
            y_axis = 0.05;
        }

        is_pressed_y = 1;
        left_click++;

        if (y_pressed == 0) {
            y_axis -= 0.02;

        }

        glutPostRedisplay();
        break;

        //Default
    case 27:
        exit(0);
        break;
    }

}

// Função para setas do teclado.
void keysNotAscii(int key, int x, int y) {

    if (key == GLUT_KEY_UP) {
        obsP[1] = obsP[1] + 0.2;
    }
    else if (key == GLUT_KEY_DOWN) {
        obsP[1] = obsP[1] - 0.2;
    }
    else if (key == GLUT_KEY_LEFT) {
        if (projecao == 0) {
            aVisao = aVisao + 0.1;
        }
        else {
            aVisao = aVisao + 45;
        }
    }
    else if (key == GLUT_KEY_RIGHT) {
        if (projecao == 0) {
            aVisao = aVisao - 0.1;
        }
        else {
            aVisao = aVisao - 45;
        }
    }

    if (obsP[1] > SIZE) {
        obsP[1] = SIZE;
    }
    if (obsP[1] < -SIZE) {
        obsP[1] = -SIZE;
    }

    obsP[0] = rVisao * cos(aVisao);
    obsP[2] = rVisao * sin(aVisao);

    glutPostRedisplay();

}

// Função main do programa.
int main(int argc, char* argv[]) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1920, 1080);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Controller | COMMANDS(More available in the left TAB): Button Click [Q/E] | Move around [Setas] | Rotate around Z-Axis [W/A/Z/D] | Joystick [I/J/K/L]");
    glEnable(GL_NORMALIZE);

    initTexturas();
    glEnable(GL_DEPTH_TEST);

    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glEnableClientState(GL_VERTEX_ARRAY);
    glNormalPointer(GL_FLOAT, 0, normais);
    glEnableClientState(GL_NORMAL_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, texturas);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glutSpecialFunc(keysNotAscii);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutTimerFunc(10, Timer, 1);

    glutMainLoop();

    return 0;

}