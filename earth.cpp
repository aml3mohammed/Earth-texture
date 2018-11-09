#include <windows.h>
#include <iostream>
#include <GL/glut.h>
#include <cmath>
#include "imageloader.h"
#include <GL/glu.h>
#include <stdlib.h>


using namespace std;

void handleKeypress(unsigned char, int, int);
void handleResize(int, int);
GLuint loadTexture(Image*);
void initRendering();

void drawScene();
void drawWindow();
void drawBackground();
void drawGrandfather();

GLuint textureBg;
GLuint textureWood;
GLuint textureImage;



void handleResize(int w, int h) {
	const float ar = (float) w / (float) h;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}
static void idle(void)
{
    glutPostRedisplay();
}

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
	case 27: //Escape key
		exit(0);
	}
}

void initRendering() {
    glClearColor(0.8 , 0.8 , 0.8 , 1) ;
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	Image*image = loadBMP("earth.bmp");
	textureWood = loadTexture(image);
	delete image;

}

void drawScene() {
	const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLUquadric *mapping = gluNewQuadric();
    gluQuadricTexture(mapping,GL_TRUE);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureWood);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPushMatrix();
        glColor3d(0.5,0.7,0.9);
        glTranslated(0,0.4,-6);
        glRotated(-30,1,0,1);
        glRotated(a,1,0,0);
        gluSphere(mapping,1,50,50);
    glPopMatrix();


    gluDeleteQuadric(mapping);
	glDisable(GL_TEXTURE_2D);
	glutSwapBuffers();
}



//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
		0,                            //0 for now
		GL_RGB,                       //Format OpenGL uses for image
		image->width, image->height,  //Width and height
		0,                            //The border of the image
		GL_RGB, //GL_RGB, because pixels are stored in RGB format
		GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
						  //as unsigned numbers
		image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}
const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 5.0f };
const GLfloat light_diffuse[]  = { 5.0f, 5.0f, 5.0f, 5.0f };
const GLfloat light_specular[] = { 5.0f, 5.0f, 5.0f, 5.0f };
const GLfloat light_position[] = { 5.0f, 5.0f, 5.0f, 1.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("Earth");
	initRendering();
	glutReshapeFunc(handleResize);

	glutDisplayFunc(drawScene);
    glutIdleFunc(idle);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
	glutKeyboardFunc(handleKeypress);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

	glutMainLoop();
	return 0;
}
