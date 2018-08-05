/*
 ============================================================================
 Name        : Jogo-da-cobrinha_OpenGL.c
 Author      : Kalpa Vrikcha
 Version     :
 Copyright   : License GPL
 Description : To compile this file use:
 gcc -lGL -lGLU -lglut main.c -o main && ./main
 ============================================================================
 */

#include <GL/glut.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define VALOR_COMIDA		10
#define DIMENSAO				10
#define START_X					140
#define START_Y					140
#define WIDTH					  300
#define HEIGHT					300

typedef struct celula {
	int x, y, px, py;
	struct celula *prox, *ante;
} Celula;
Celula *cabeca;

enum { LEFT, RIGHT, UP, DOWN };

int direcao = RIGHT;
int delay = 60;
int winWidth = WIDTH;
int winHeight = HEIGHT;
int tamanhoMinhocaInicial = 3;
int tamanhoMinhoca = 0;
int posicoesMinhoca[100] = {};

void desenhaMinhoca(Celula *celula);
void criarJanela(int argc, char *argv[]);
void desenha();
void alteraTamanhoJanela(GLsizei w, GLsizei h);
void atualizarTela(int value);
void controladorDoJogo(int key, int x, int y);
void moverMinhoca();
Celula *crescer(Celula *cab, int x, int y);
void atualizarPosicoes(Celula *cabeca);
void iniciarJogo();
void cliquesDoMouse(int button, int state, int x, int y);
void movimentoMouse( int x, int y);

int main(int argc, char *argv[]) {

	iniciarJogo();
	criarJanela(argc, argv);
	glutDisplayFunc(desenha);
	glutReshapeFunc(alteraTamanhoJanela);
	glutTimerFunc(30, atualizarTela, 1);
	glutSpecialFunc(controladorDoJogo);
	glutMouseFunc(cliquesDoMouse);
	glutPassiveMotionFunc(movimentoMouse);
	glutMainLoop();
	exit(0);
}

void movimentoMouse( int x, int y) {
	printf("(%d, %d)\n", x, y);
}

void desenha() {

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
	desenhaMinhoca(cabeca);
	glutSwapBuffers();
}

void desenhaMinhoca(Celula *cabeca) {

	Celula *c;
	for (c = cabeca; c != NULL; c = c->prox) {
		glColor4ub(0.0f, 1.0f, 0.0f, 0.0f);
		glBegin(GL_QUADS);
			glVertex2i(c->x, c->y);
			glVertex2i(c->x, c->y+DIMENSAO);
			glVertex2i(c->x+DIMENSAO, c->y+DIMENSAO);
			glVertex2i(c->x+DIMENSAO, c->y);
		glEnd();
		glColor4ub(0, 127, 0, 255);
		glBegin(GL_LINE_LOOP);
			glVertex2i(c->x, c->y);
			glVertex2i(c->x, c->y+DIMENSAO);
			glVertex2i(c->x+DIMENSAO, c->y+DIMENSAO);
			glVertex2i(c->x+DIMENSAO, c->y);
		glEnd();
	}
}

void criarJanela(int argc, char *argv[]) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("AnimaProject_V0.1-beta");
}

void alteraTamanhoJanela(GLsizei w, GLsizei h) {

	winWidth = w;
	winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void atualizarTela(int value) {

	glutTimerFunc(delay, atualizarTela, -1);
	moverMinhoca();
	glutPostRedisplay();
}

void controladorDoJogo(int key, int x, int y) {

	switch(key) {
		case GLUT_KEY_LEFT:
			direcao = (direcao != RIGHT) ? LEFT : RIGHT;
		break;
		case GLUT_KEY_UP:
			direcao = (direcao != DOWN) ? UP : DOWN;
		break;
		case GLUT_KEY_RIGHT:
			direcao = (direcao != LEFT) ? RIGHT : LEFT;
		break;
		case GLUT_KEY_DOWN:
			direcao = (direcao != UP) ? DOWN : UP;
		break;
	}
}

void moverMinhoca() {

	cabeca->px = cabeca->x;
	cabeca->py = cabeca->y;

	switch(direcao) {
		case LEFT:
			cabeca->x -= 10;
			cabeca->x = cabeca->x < 0 ?
				(winWidth - 10) : cabeca->x;
			break;
		case RIGHT:
			cabeca->x += 10;
			cabeca->x = cabeca->x > (winWidth - 10) ?
				0 : cabeca->x;
			break;
		case UP:
			cabeca->y += 10;
			cabeca->y = cabeca->y > (winHeight - 10) ?
				0 : cabeca->y;
			break;
		case DOWN:
			cabeca->y -= 10;
			cabeca->y = cabeca->y < 0 ?
				(winHeight - 10) : cabeca->y;
			break;
	}

	if(tamanhoMinhocaInicial > 0) {
		cabeca = crescer(cabeca, 0, 0);
		tamanhoMinhocaInicial--;
	}
	atualizarPosicoes(cabeca);
}

void atualizarPosicoes(Celula *cabeca) {

	Celula *c;
	c = cabeca->prox;
	do {
		c->px = c->x;
		c->py = c->y;
		c->x = c->ante->px;
		c->y = c->ante->py;
		c = c->prox;
	} while(c != NULL);
}

Celula *crescer(Celula *cab, int x, int y) {
	if(cab == NULL) {
		Celula *novaCelula = (Celula*)malloc(sizeof(Celula));
		novaCelula->x = x;
		novaCelula->y = y;
		novaCelula->px = novaCelula->x;
		novaCelula->py = novaCelula->y;
		novaCelula->prox = NULL;
		novaCelula->ante = NULL;
		cab = novaCelula;
		return cab;
	} else {
		Celula *novaCelula = (Celula*)malloc(sizeof(Celula));
		novaCelula->prox = NULL;
		Celula *pCabeca;
		for(pCabeca = cab; pCabeca->prox != NULL; pCabeca = pCabeca->prox);
		pCabeca->prox = novaCelula;
		novaCelula->x = pCabeca->px;
		novaCelula->y = pCabeca->py;
		novaCelula->px = novaCelula->x;
		novaCelula->py = novaCelula->y;
		novaCelula->ante = pCabeca;
		tamanhoMinhoca++;
		return cab;
	}
}

void iniciarJogo() {

	cabeca = NULL;
	cabeca = crescer(cabeca, START_X, START_Y);
}

void cliquesDoMouse(int button, int state, int x, int y) {

   if (button == GLUT_LEFT_BUTTON) {
	   if (state == GLUT_DOWN) {
		   printf("Clique em: (%d, %d)\n", x, y);
	   }
   }
   glutPostRedisplay();
}
