
#include <stdlib.h>
#include <math.h>
#include <glut.h>
#include <iostream>
#include <windows.h>
#include <mmsystem.h>
#include <tchar.h>
#include <cstdlib>   // rand and srand
#include <string>
#include <iostream>



#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)

using namespace std;

GLdouble fovy = 55.0;
GLdouble zNear = 0.1;
GLdouble zFar = 200;

float timeElapsed = 0.0;
int FPS = 30;

class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = -0.0) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f& v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f& v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};

class Camera {
public:
	Vector3f eye, center, up;

	Camera(float eyeX = 0.0f, float eyeY = 3.0f, float eyeZ = 33.0f, float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
	}

	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		//center = center + right * d;
	}

	void moveY(float d) {
		 eye = eye + up.unit() * d;
	

		//center = center + up.unit() * d;
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		//center = center + view * d;
	}

	//void rotateX(float a) {
	//	Vector3f view = (center - eye).unit();
	//	Vector3f right = up.cross(view).unit();
	//	view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
	//	up = view.cross(right);
	//	center = eye + view;
	//}

	//void rotateY(float a) {
	//	Vector3f view = (center - eye).unit();
	//	Vector3f right = up.cross(view).unit();
	//	view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
	//	right = view.cross(up);
	//	center = eye + view;
	//}

	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}
};
Camera camera;


float groundLen = 10.0f;

float fenceR = 0.6;
float fenceG = 0.6;
float fenceB = 0.6;
float dcolor = -0.0005;


float santaX = 0.0;
float santaY = 0.0;
float santaZ = -3;
float santaTheta = 0;


float cubeunitlength = 1.5;

Vector3f gift1 = { 7.0f,0.0,4.0 };
Vector3f gift2 = { -9.0f,0.0,3.0f };
Vector3f gift3 = { -5.f, 0.0f, -6.0f };
Vector3f gift4 = { 8.0f, 0.0f, -3.5f };

int hitgf1 = 0;
int hitgf2 = 0;
int hitgf3 = 0;
int hitgf4 = 0;

float liftgiftup = 0.0;


float elfscale = 0.2;
float elfangle = 0.0;
float elfmovex = 0.0;
bool animgift = false;

float rotyz = 10.0f;

float treeTheta = 0.0;
bool treeRotate = false;

float movetorix = 0;
bool animtori = false;

bool snowmanjump = false;
float jumpsy = 0.0;
bool snowmanup = true;

string timeString = "";

float gametime = 61;
bool gameend = false;
bool gamewin = false;
bool gameover = false;



// http://stackoverflow.com/questions/12975341/to-string-is-not-a-member-of-std-says-so-g
namespace patch
{
	template < typename T > std::string to_string(const T& n)
	{
		std::ostringstream stm;
		stm << n;
		return stm.str();
	}
}


void drawSanta();

void drawBitmapText(string text, float x, float y, float z)
{	
	glDisable(GL_LIGHTING);
	if(gameover || gametime - timeElapsed <= 10 ) glColor3f(1.0, 0, 0);
	else if(gamewin) glColor3f(0, 1.0, 0);
	else glColor3f(1.0, 1.0, 1.0);

	glRasterPos3f(x, y, z);
	for (int i = 0; i < text.size(); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
	}

	glEnable(GL_LIGHTING);

}

void defaultMaterial() {

	GLfloat mat_specular[] = { 0.5f, 0.5f, 0.6f, 1.0f };
	GLfloat mat_diffuse[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat mat_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat mat_shininess = { 40.0f };
	/* define material properties for front face of all polygons */
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);


	glEnable(GL_COLOR_MATERIAL);
	// Set Material Properties which will be assigned by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);


}

void goldMaterial() {

	glDisable(GL_COLOR_MATERIAL);

	GLfloat mat_specular[]{ 0.797357, 0.723991, 0.208006, 1.0 };
	GLfloat mat_diffuse[]{ 0.34615, 0.3143, 0.0903, 1.0 };
	GLfloat mat_ambient[]{ 0.24725, 0.2245, 0.0645, 1.0 };
	GLfloat mat_shininess = 83.2;

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess * 128);
	glDisable(GL_COLOR_MATERIAL);
}

void enablelighting() {
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);

	defaultMaterial();

	/* Control lighting properties */
	//GLfloat light_ambient[] = { .5, .0, .0, 1.0 };
	GLfloat light_diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	//GLfloat light_specular[] = { .7, .0, .0, 1.0 };
	GLfloat light_position[] = { -2.31f, 0.10, 1.42f, 0.0f };

	/* set up ambient, diffuse, and specular components for light 0 */
	//glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);


	glShadeModel(GL_SMOOTH);
}

void setupCamera() {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, 800 / 600, 0.001, zFar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}

void collisionhandling() {

	if (santaX >= 9.5) {
		santaX = 9.5;
	}
	if (santaX <= -9.5) {
		santaX = -9.5;
	}
	if (santaZ >= 9.5) {
		santaZ = 9.5;
	}
	if (santaZ <= -9.5) {
		santaZ = -9.5;
	}


	if (!gameover)
	{
		if ((santaX <= gift1.x + cubeunitlength / 2) && (santaX >= gift1.x - cubeunitlength / 2) && (santaZ <= gift1.z + cubeunitlength / 2) && (santaZ >= gift1.z - cubeunitlength / 2))
		{
			if (!hitgf1) {
				PlaySound(TEXT("audio/giftpickup.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
				hitgf1 = 1;
			}
		}
		if ((santaX <= gift2.x + cubeunitlength / 2) && (santaX >= gift2.x - cubeunitlength / 2) && (santaZ <= gift2.z + cubeunitlength / 2) && (santaZ >= gift2.z - cubeunitlength / 2))
		{


			if (!hitgf2) {
				PlaySound(TEXT("audio/giftpickup.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
				hitgf2 = 1;
			}


		}
		if ((santaX <= gift3.x + cubeunitlength / 2) && (santaX >= gift3.x - cubeunitlength / 2) && (santaZ <= gift3.z + cubeunitlength / 2) && (santaZ >= gift3.z - cubeunitlength / 2))
		{
			if (!hitgf3) {
				PlaySound(TEXT("audio/giftpickup.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
				hitgf3 = 1;
			}


		}
		if ((santaX <= gift4.x + cubeunitlength / 2) && (santaX >= gift4.x - cubeunitlength / 2) && (santaZ <= gift4.z + cubeunitlength / 2) && (santaZ >= gift4.z - cubeunitlength / 2))
		{
			if (!hitgf4) {
				PlaySound(TEXT("audio/giftpickup.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
				hitgf4 = 1;
			}


		}
	
	
		if (hitgf1 && hitgf2 && hitgf3 && hitgf4) {

			gamewin = true;
			gameend = true;
		}
	}



};

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;
	float ratio = w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(fovy, ratio, zNear, zFar);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void drawSanta() {

	GLUquadricObj* pObj = gluNewQuadric();
	gluQuadricNormals(pObj, GLU_SMOOTH);

	glPushMatrix();


	//legs
	glPushMatrix();
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(0.30, 0.5f, 0.0f);
	glRotatef(90, 1.0f, 0.0f, 0.0f);
	gluCylinder(pObj, 0.2f, 0.2f, 0.3f, 26, 25);
	glTranslatef(0, 0, 0.5);
	glTranslatef(0, 0, -0.2);
	glColor3f(0, 0, 0);
	gluCylinder(pObj, 0.2f, 0.2f, 0.2f, 26, 25);
	glPopMatrix();
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(-0.3, 0.5f, 0.0f);
	glRotatef(90, 1.0f, 0.0f, 0.0f);
	gluCylinder(pObj, 0.2f, 0.2f, 0.3f, 26, 25);
	glTranslatef(0, 0, 0.5);
	glTranslatef(0, 0, -0.2);
	glColor3f(0, 0, 0);
	gluCylinder(pObj, 0.2f, 0.2f, 0.2f, 26, 25);
	glPopMatrix();
	glPopMatrix();


	//body
	glTranslatef(0.0f, 0.4f, 0.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glRotatef(-90, 1.0f, 0.0f, 0.0f);
	glutSolidCone(0.9f, 1.9f, 10, 10);

	//head
	glRotatef(90, 1.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 0.9f);
	glTranslatef(0.0f, 1.80f, 0.0f);
	glutSolidSphere(0.550f, 20, 20);

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0.2f, 0, 0.5f);
	glutSolidSphere(0.11f, 20, 20);
	glTranslatef(-0.4f, 0, 0);
	glutSolidSphere(0.11f, 20, 20);
	glPopMatrix();

	//tartor
	glTranslatef(0.0f, 0.4f, 0.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glRotatef(-90, 1.0f, 0.0f, 0.0f);
	glutSolidCone(0.5f, 0.95f, 10, 10);

	glColor3f(1.0f, 1.0f, 1.0f);
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glutSolidTorus(0.2f, 0.4f, 20, 20);

	glRotatef(-90, 0.0f, 0.0f, 1.0f);

	//arms
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(0.8, -0.0f, -1.66f);
	glRotatef(-50, 0.0f, 1.0f, 0.0f);
	gluCylinder(pObj, 0.15f, 0.19f, 0.85f, 26, 25);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidSphere(0.2, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(-0.8, -0.0f, -1.66f);
	glRotatef(50, 0.0f, 1.0f, 0.0f);
	gluCylinder(pObj, 0.15f, 0.19f, 0.85f, 26, 25);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidSphere(0.2, 25, 25);
	glPopMatrix();
	glPushMatrix();

	//rest of tartor
	glRotatef(90, 1.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(0.0f, 0.75f, 0.0f);
	glutSolidSphere(0.2f, 20, 20);

	//buttons
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(0.0f, -2.0f, 0.45f);
	glutSolidSphere(0.09f, 20, 20);

	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(0.0f, -0.4f, 0.2f);
	glutSolidSphere(0.09f, 20, 20);
	glPopMatrix();

	//decorations
	glTranslatef(0.0f, 0.0f, -0.94f);
	glutSolidTorus(0.179f, 0.21f, 50, 50);

	glTranslatef(0.0f, 0.0f, -1.18f);
	glutSolidTorus(0.19f, 0.8f, 50, 50);

	glTranslatef(0.0f, 0.0f, 0.65f);
	glutSolidTorus(0.1f, 0.55f, 50, 50);

	glPopMatrix();

}

void drawElF() {

	GLUquadricObj* pObj = gluNewQuadric();
	gluQuadricNormals(pObj, GLU_SMOOTH);

	glPushMatrix();


	//legs
	glPushMatrix();
	glPushMatrix();
	glColor3f(0, 1, 0.0f);
	glTranslatef(0.30, 0.5f, 0.0f);
	glRotatef(90, 1.0f, 0.0f, 0.0f);
	gluCylinder(pObj, 0.2f, 0.2f, 0.3f, 26, 25);
	glTranslatef(0, 0, 0.5);
	glTranslatef(0, 0, -0.2);
	glColor3f(0, 0, 0);
	gluCylinder(pObj, 0.2f, 0.2f, 0.2f, 26, 25);
	glPopMatrix();
	glPushMatrix();
	glColor3f(0, 1, 0.0f);
	glTranslatef(-0.3, 0.5f, 0.0f);
	glRotatef(90, 1.0f, 0.0f, 0.0f);
	gluCylinder(pObj, 0.2f, 0.2f, 0.3f, 26, 25);
	glTranslatef(0, 0, 0.5);
	glTranslatef(0, 0, -0.2);
	glColor3f(0, 0, 0);
	gluCylinder(pObj, 0.2f, 0.2f, 0.2f, 26, 25);
	glPopMatrix();
	glPopMatrix();


	//body
	glTranslatef(0.0f, 0.4f, 0.0f);
	glColor3f(0, 1, 0.0f);
	glRotatef(-90, 1.0f, 0.0f, 0.0f);
	glutSolidCone(0.7f, 1.9f, 10, 10);

	//head
	glRotatef(90, 1.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 0.9f);
	glTranslatef(0.0f, 1.80f, 0.0f);
	glutSolidSphere(0.550f, 20, 20);

	glPushMatrix();
	glColor3f(0, 0, 0);

	glTranslatef(-0.3, 0, 0.0f);
	glutSolidSphere(0.350f, 20, 20);
	glTranslatef(+0.6, 0, 0.0f);
	glutSolidSphere(0.350f, 20, 20);
	glPopMatrix();


	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0.2f, 0, 0.5f);
	glutSolidSphere(0.11f, 20, 20);
	glTranslatef(-0.4f, 0, 0);
	glutSolidSphere(0.11f, 20, 20);
	glPopMatrix();

	//tartor
	glTranslatef(0.0f, 0.4f, 0.0f);
	glColor3f(0, 1, 0.0f);
	glRotatef(-90, 1.0f, 0.0f, 0.0f);
	glutSolidCone(0.5f, 0.95f, 10, 10);

	glColor3f(1.0f, 1.0f, 1.0f);
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glutSolidTorus(0.2f, 0.4f, 20, 20);

	glRotatef(-90, 0.0f, 0.0f, 1.0f);


	glPushMatrix();

	//rest of tartor
	glRotatef(90, 1.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(0.0f, 0.75f, 0.0f);
	glutSolidSphere(0.2f, 20, 20);

	//buttons
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(0.0f, -2.0f, 0.45f);
	glutSolidSphere(0.09f, 20, 20);

	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(0.0f, -0.4f, 0.2f);
	glutSolidSphere(0.09f, 20, 20);
	glPopMatrix();

	//decorations
	glTranslatef(0.0f, 0.0f, -0.94f);
	glutSolidTorus(0.179f, 0.21f, 50, 50);

	glTranslatef(0.0f, 0.0f, -1.18f);
	/*glutSolidTorus(0.19f, 0.8f, 50, 50);*/

	glTranslatef(0.0f, 0.0f, 0.65f);
	glutSolidTorus(0.05f, 0.45f, 50, 50);


	glPopMatrix();

}

void drawTree() {


	glPushMatrix();

	//trunk
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glColor3f(0.8f, 0.5f, 0.3f);
	glRotatef(180 + 90, 1.0f, 0.0f, 0.0f);
	glutSolidCone(1.3f, 6.7f, 6, 25);
	glPopMatrix();

	glTranslatef(0.0f, 1.0f, 0.0f);

	//1st cone
	glPushMatrix();
	glColor3f(0.3, 0.6, 0.2f);
	glTranslatef(0.0f, 0.0f, 0.0f);
	glRotatef(180 + 90, 1.0f, 0.0f, 0.0f);
	glutSolidCone(2.5f, 5.7f, 5, 6);
	glPopMatrix();

	//2nd cone
	glPushMatrix();
	glColor3f(0.3, 0.6, 0.1f);
	glTranslatef(0.0f, 1.45f, 0.0f);
	glRotatef(180 + 90, 1.0f, 0.0f, 0.0f);
	glutSolidCone(2.5f, 5.5f, 5, 6);
	glPopMatrix();

	//3rd cone
	glPushMatrix();
	glColor3f(0.3, 0.6, 0.0f);
	glTranslatef(0.0f, 2.9f, 0.0f);
	glRotatef(180 + 90, 1.0f, 0.0f, 0.0f);
	glutSolidCone(2.5f, 4.5f, 5, 6);
	glPopMatrix();


	//stars
	glTranslatef(0.0f, 7.4f, 0.0f);
	glColor3f(1.0f, 1.0f, 0.0f);
	glRotatef(-45, 0.0f, 0.0f, 1.0f);
	glutSolidTorus(0.4, 0.7, 3, 3);
	glRotatef(45, 0.0f, 0.0f, 1.0f);
	glutSolidTorus(0.4, 0.7, 3, 3);

	glPopMatrix();


	//deco

	glPushMatrix();
	glTranslatef(0.0f, 6.4f, 0.0f);
	glColor3f(0.9f, 0, 0.0f);
	glRotatef(90, 1, 0, 0);
	glRotatef(rotyz, 0, 1, 0);
	glutSolidTorus(0.1, 1.2, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 4.4f, 0.0f);
	glColor3f(0.9f, 0, 0.0f);
	glRotatef(90, 1, 0, 0);
	glRotatef(rotyz, 0, 1, 0);
	glutSolidTorus(0.1, 2.2, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 2.4f, 0.0f);
	glColor3f(0.9f, 0, 0.0f);
	glRotatef(90, 1, 0, 0);
	glRotatef(rotyz, 0, 1, 0);
	glutSolidTorus(0.1, 2.2, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);

	goldMaterial();

	glPushMatrix();
	glTranslatef(0.0f, 5.8f, 1.3);
	glutSolidSphere(0.2, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 5.6f, -1.5);
	glutSolidSphere(0.2, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5f, 5.2f, 0);
	glutSolidSphere(0.2, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5f, 5.2f, 0);
	glutSolidSphere(0.2, 30, 30);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-0.4, 3.6, 1.8);
	glutSolidSphere(0.2, 30, 30);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0.6, 3.6, -1.8);
	glutSolidSphere(0.2, 30, 30);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0.8, 1.6, 1.8);
	glutSolidSphere(0.2, 30, 30);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-0.8, 1.6, -1.8);
	glutSolidSphere(0.2, 30, 30);
	glPopMatrix();




	glPopMatrix();

	defaultMaterial();



}

void drawSnowMan() {


	GLUquadricObj* pObj = gluNewQuadric();
	gluQuadricNormals(pObj, GLU_SMOOTH);

	glPushMatrix();


	glColor3f(1.0f, 1.0f, 1.0f);

	// Draw Body
	glTranslatef(0.0f, 0.65f, 0.0f);
	glutSolidSphere(1.0f, 20, 20);

	// Draw Head
	glTranslatef(0.0f, 1.50f, 0.0f);
	glutSolidSphere(0.70f, 20, 20);

	glPushMatrix();
	glTranslatef(0.0f, -0.47f, 0.0f);
	glColor3f(1.0f, 0.0f, 0.3f);
	glRotatef(90, 1, 0, 0);
	glutSolidTorus(0.35f, 0.4f, 30, 30);
	glPopMatrix();

	// Draw Eyes
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	glTranslatef(0.25f, 0.10f, 0.6f);
	glutSolidSphere(0.15f, 10, 10);
	glTranslatef(-0.5f, 0.0f, 0.0f);
	glutSolidSphere(0.15f, 10, 10);
	glPopMatrix();

	// Draw Nose

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.7f);
	glColor3f(1.0f, 0.5f, 0.5f);
	glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
	glutSolidCone(0.1f, 0.6f, 10, 2);
	glPopMatrix();


	//Draw Hat
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 1.0f, 0.0f);
	glRotatef(90, 1.0f, 0.0f, 0.0f);
	gluCylinder(pObj, 0.5f, 0.5f, 0.8f, 26, 13);

	//brim
	glTranslatef(0.0f, -0.1f, 0.4f);
	gluDisk(pObj, 0.3f, 0.80f, 26, 13);

	glPopMatrix();


	glPopMatrix();

}

void drawGift(Vector3f v) {


	glPushMatrix();

	glTranslatef(v.x, v.y + liftgiftup, v.z);
	glPushMatrix();
	glColor3f(0.5f, 0.0f, 0.3f);
	glPushMatrix();
	glTranslatef(0.0f, 0.75f, 0.0f);
	glutSolidCube(cubeunitlength);
	glPopMatrix();

	glPushMatrix();

	glBegin(GL_QUADS);
	glColor3f(0.1f, 0.7f, 0.4f);

	//Top


	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-(cubeunitlength / 2 + 0.05), cubeunitlength + 0.05, -0.2f);
	glVertex3f((cubeunitlength / 2 + 0.05) - 0, cubeunitlength + 0.05 + 0, -0.2f);
	glVertex3f((cubeunitlength / 2 + 0.05) - 0, cubeunitlength + 0.05 + 0, 0.2f);
	glVertex3f(-(cubeunitlength / 2 + 0.05), cubeunitlength + 0.05, 0.2f);

	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.2f, cubeunitlength + 0.05, -(cubeunitlength / 2 + 0.05));
	glVertex3f(0.2f, cubeunitlength + 0.05, -(cubeunitlength / 2 + 0.05));
	glVertex3f(0.2f, cubeunitlength + 0.05 + 0, (cubeunitlength / 2 + 0.05) - 0);
	glVertex3f(-0.2f, cubeunitlength + 0.05 + 0, (cubeunitlength / 2 + 0.05) - 0);



	//Front
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.2f, -0.0f, cubeunitlength / 2 + 0.05);
	glVertex3f(0.2f, -0.0f, cubeunitlength / 2 + 0.05);
	glVertex3f(0.2f, cubeunitlength + 0.05, cubeunitlength / 2 + 0.05);
	glVertex3f(-0.2f, cubeunitlength + 0.05, cubeunitlength / 2 + 0.05);

	//Back
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(-0.2f, -0, -(cubeunitlength / 2 + 0.05));
	glVertex3f(-0.2f, cubeunitlength + 0.05, -(cubeunitlength / 2 + 0.05));
	glVertex3f(0.2f, cubeunitlength + 0.05, -(cubeunitlength / 2 + 0.05));
	glVertex3f(0.2f, -0, -(cubeunitlength / 2 + 0.05));

	//Right
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(cubeunitlength / 2 + 0.05, 0, -0.2f);
	glVertex3f(cubeunitlength / 2 + 0.05, cubeunitlength + 0.05, -0.2f);
	glVertex3f(cubeunitlength / 2 + 0.05, cubeunitlength + 0.05, 0.2f);
	glVertex3f(cubeunitlength / 2 + 0.05, 0, 0.2f);

	//Left
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-(cubeunitlength / 2 + 0.05), 0, -0.2f);
	glVertex3f(-(cubeunitlength / 2 + 0.05), 0, 0.2f);
	glVertex3f(-(cubeunitlength / 2 + 0.05), cubeunitlength + 0.05, 0.2f);
	glVertex3f(-(cubeunitlength / 2 + 0.05), cubeunitlength + 0.05, -0.2f);

	glEnd();

	glPopMatrix();



	glPopMatrix();
	glPopMatrix();



}

void drawTorii() {

	// 0.6 7  movetorix-9

	GLUquadricObj* pObj = gluNewQuadric();
	gluQuadricNormals(pObj, GLU_SMOOTH);

	glDisable(GL_LIGHT0);

	glColor3f(1.0f, 0.0f, 0.0f);

	glPushMatrix();
	glTranslatef(-3, 3, -0);
	glScalef(0.5, 6, 0.5);
	glutSolidCube(1);
	glTranslatef(12, 0, 0);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 5, -0);
	glScalef(8, 0.5, 0.5);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 5.5, -0);
	glutSolidCube(0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 6, -0);
	glScalef(10, 0.5, 0.5);
	glutSolidCube(1);
	glTranslatef(0, 1, 0);
	glScalef(1.2, 1, 1);
	glColor3f(0, 0.0f, 0.0f);
	glutSolidCube(1);
	glPopMatrix();

	glEnable(GL_LIGHT0);

	glColor3f(1, 1, 0.9);

	glTranslatef(0, 4, 0);
	glutSolidSphere(0.5, 25, 25);

	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.55, 0.0f);
	glRotatef(90, 1.0f, 0.0f, 0.0f);
	gluCylinder(pObj, 0.2, 0.2, 0.1, 26, 13);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, -0.45, 0.0f);
	glRotatef(90, 1.0f, 0.0f, 0.0f);
	gluCylinder(pObj, 0.2, 0.2, 0.1, 26, 13);
	glPopMatrix();


	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.75, 0.0f);
	glRotatef(90, 1.0f, 0.0f, 0.0f);
	gluCylinder(pObj, 0.04, 0.04, 0.4, 26, 13);
	glPopMatrix();




}

void drawFence() {
	glColor3f(fenceR, fenceG, fenceB);

	glDisable(GL_LIGHTING);
	glTranslatef(0, 0.9f, 0);
	glScalef(0.8, 1.8, 0.2);
	glutSolidCube(1);
	glScalef(2, 0.4, 0.2);
	glutSolidCube(1);
	glEnable(GL_LIGHTING);


}

void drawScene() {


	// Draw ground
	glDisable(GL_LIGHTING);
	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
	glVertex3f(-groundLen, 0.0f, -groundLen);
	glVertex3f(-groundLen, 0.0f, groundLen);
	glVertex3f(groundLen, 0.0f, groundLen);
	glVertex3f(groundLen, 0.0f, -groundLen);
	glEnd();
	glEnable(GL_LIGHTING);

	//DrawFence
	for (float i = -9.8f; i <= 10.0f; (i = i + 1.6)) {
		glPushMatrix();
		glTranslatef(i, -0.01f, -10.0f);
		drawFence();
		glPopMatrix();
	}


	for (float i = -9.8f; i <= 10.0f; (i = i + 1.6)) {
		glPushMatrix();

		glTranslatef(-10.0f, -0.01f, i);
		glRotatef(90, 0, 1, 0);
		drawFence();
		glPopMatrix();
	}


	for (float i = -9.8f; i <= 10.0f; (i = i + 1.6)) {
		glPushMatrix();
		glTranslatef(10.0f, -0.01f, i);
		glRotatef(90, 0, 1, 0);
		drawFence();
		glPopMatrix();
	}

}

void Trees() {






	glPushMatrix();

	glTranslatef(7.0f, 0.0f, -7.0f);
	glRotatef(treeTheta, 0, 1.0f, 0);

	drawTree();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-7.0f, 0.0f, -7.0f);
	glRotatef(-treeTheta, 0, 1.0f, 0);

	drawTree();
	glPopMatrix();


}

void gifts() {
	glPushMatrix();

	if (!hitgf1) {
		drawGift(gift1);

		if (-elfmovex + gift1.x > -10) {
			glPushMatrix();

			glTranslatef(-elfmovex, liftgiftup, 0);
			glTranslatef(gift1.x, gift1.y, gift1.z);
			glRotatef(-elfangle, 0, 1, 0);
			glScalef(elfscale, elfscale, elfscale);
			drawElF();
			glPopMatrix();
		}
	}


	if (!hitgf2) {
		drawGift(gift2);

		if (elfmovex + gift2.x < 10) {
			glPushMatrix();
			glTranslatef(elfmovex, liftgiftup, 0);

			glTranslatef(gift2.x, gift2.y, gift2.z);
			glRotatef(elfangle, 0, 1, 0);
			glScalef(elfscale, elfscale, elfscale);
			drawElF();
			glPopMatrix();
		}
	}

	if (!hitgf3) {
		drawGift(gift3);

		if (elfmovex + gift3.x < 10) {

			glPushMatrix();
			glTranslatef(elfmovex, liftgiftup, 0);

			glTranslatef(gift3.x, gift3.y, gift3.z);
			glRotatef(elfangle, 0, 1, 0);
			glScalef(elfscale, elfscale, elfscale);
			drawElF();
			glPopMatrix();
		}
	}
	if (!hitgf4) {
		drawGift(gift4);

		if (-elfmovex + gift4.x > -10) {

			glPushMatrix();
			glTranslatef(-elfmovex, liftgiftup, 0);

			glTranslatef(gift4.x, gift4.y, gift4.z);
			glRotatef(-elfangle, 0, 1, 0);
			glScalef(elfscale, elfscale, elfscale);
			drawElF();
			glPopMatrix();
		}
		else 
		{
			animgift = false;
		}
	}


	glPopMatrix();



}


float snowfall = 0.0;
unsigned seed = time(0);

void snow() {

	srand(seed);

	glDisable(GL_LIGHTING);
	for (int i = 0; i <= 450; i++) {

			glPushMatrix();
			float randsign = rand() % 2;
			float randx = (randsign ? -1 : 1) * ((float(rand()) / float(RAND_MAX)) * 10);
			float randy =  rand() % 300;

			randsign = rand() % 2;
			float randz = (randsign ? -1 : 1) * ( ( float(rand()) / float(RAND_MAX) ) * 10 );
			
			if (randy - snowfall > -0.01 ) {

				glTranslatef(randx, randy - snowfall, randz);

			}
			else {
				glScalef(1, 0.5, 1);

			/*	if (randz > movetorix - 9 + 5 && randz < movetorix - 9 - 5 && randx > -1.6 && randx < 1.6) {
					glTranslatef(randx, 6.97, randz);
				}*/
				 glTranslatef(randx, -0.01, randz);
			}
			glColor3f(1, 1, 1);
			glutSolidSphere(0.2, 25, 25);
			glPopMatrix();

	}
	glEnable(GL_LIGHTING);

}

void keyPressedSp(int key, int xx, int yy) {
	float d = 0.2;
	switch (key) {
	case GLUT_KEY_LEFT: santaX -= d; santaTheta = -90; break;
	case GLUT_KEY_RIGHT: santaX += d; santaTheta = +90; break;
	case GLUT_KEY_UP:  santaZ -= d; santaTheta = 180;break;
	case GLUT_KEY_DOWN: santaZ += d;santaTheta = 0; break;


	}
}
void keyReleasedSp(int key, int x, int y) {

	//switch (key) {

	//case GLUT_KEY_LEFT:
	//case GLUT_KEY_RIGHT: deltax = 0; deltay = 0; deltaz = 0;break;

	//case GLUT_KEY_UP:
	//case GLUT_KEY_DOWN: deltax = 0; deltay = 0; deltaz = 0;break;
	//}
}
void Keyboard(unsigned char key, int x, int y) {
	float d = 0.5;

	switch (key) {
	case 'w':
		if (!gameend) camera.moveY(d);
		break;
	case 's':
		if (camera.eye.y >= 1.99167)
			if (!gameend) camera.moveY(-d);
		break;
	case 'a':
		if (!gameend) camera.moveX(d);
		break;
	case 'd':
		if (!gameend) camera.moveX(-d);
		break;
	case 'q':
		if (!gameend) camera.moveZ(d);
		break;
	case 'e':
		if (!gameend) camera.moveZ(-d);
		break;
	case 'f':
		camera.eye.x = 0.0f;
		camera.eye.y = 3.0f;
		camera.eye.z = 30.0f;
		break;

	case 't':
		if (!gameend) {
			camera.eye.x = 0.0f;
			camera.eye.y = 40.0f;
			camera.eye.z = 6.0f;
		}
		break;
	case 'h':
		if (!gameend) {
			camera.eye.x = 30.0f;
			camera.eye.y = 2.0f;
			camera.eye.z = -0.3;
		}
		break;
	case 'z':
		treeRotate = !treeRotate;
		//cout << treeRotate << endl;
		break;

	case 'x':
		animgift = !animgift;
		break;

	case 'v':
		animtori = !animtori;
		break;

	case 'b':
		snowmanjump = !snowmanjump;
		break;


	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
	}

	glutPostRedisplay();
}

void Timer(int value) {

	timeElapsed += 60 / 1000.0;
//	cout << timeElapsed << endl;
	glutTimerFunc(60, Timer, 0);
	glutPostRedisplay();
}
void Anim() {
	fenceG += dcolor;
	fenceB += dcolor;

	if (fenceG >= 0.6) {
		dcolor = -dcolor;
	}
	if (fenceG <= 0) {
		dcolor = -dcolor;
	}


	if (treeRotate) {
		if (treeTheta == 360) treeTheta = 0;
		treeTheta += 0.1;
	}


	bool startmoving = false;
	if (animgift) {
		if (elfscale <= 1.01) elfscale += 0.01;
		else {
			startmoving = true;
		}
		if (startmoving) {
			elfangle = 90;
			elfmovex += 0.08;
		}
	}
	else {
		elfscale = 0.2;
		elfmovex = 0;
	}



	if (animtori) {
		if (movetorix < 19)
			movetorix += 0.07;
	}
	else {

		if (movetorix != 0) {
			if (movetorix > 0) movetorix -= 0.07;
		}

	}



	if (snowmanjump) {


		if (snowmanup) {
			if (jumpsy < 1)
			{
				jumpsy += 0.05;
			}
			else {
				snowmanup = false;
			}
		}
		else {


			if (jumpsy > 0) {
				jumpsy += -0.06;
			}
			else {
				snowmanup = true;
			}
		}
	}
	else {
	
		jumpsy = 0.0;
		snowmanup = true;
	}




	if (!int(gametime - timeElapsed)) {

		//lift gifts up

		gameend = true;
		if(!gamewin) gameover = true;
		liftgiftup += 0.1;
	}

	snowfall += 0.1;



	glutPostRedisplay();

}
void renderScene(void) {

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1, 0.1, 0.2, 0.0f);


	//2D Text
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, 800, 0.0, 800);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(10, 10);
	timeString = "Time left : " + to_string(int(gametime - timeElapsed)) + " s";
	string overstg = "Game Over";

	if (gameend) {
		if (gameover) {
			drawBitmapText(overstg, 350, 750, 0);
		}

		if (gamewin) {
			drawBitmapText("You Won!", 350, 750, 0);
		}
	}
	else {
	
		drawBitmapText(timeString, 5, 10, 0);
	}

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();


	setupCamera();


	glPushMatrix();
	glPopMatrix();

	glPushMatrix();
	drawScene();
	glPopMatrix();

	gifts();

	glPushMatrix();
	glScalef(1.2, 1.2, 1.2);
	Trees();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(8.0f, 0.0f, -5.0f);
	glTranslatef(0, jumpsy, 0);
	glRotatef(-30, 0, 1.0f, 0);
	glScalef(1.2, 1.2, 1.2);
	drawSnowMan();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-8.0f, 0.0f, 6.0f);
	glTranslatef(0, jumpsy, 0);
	glRotatef(30, 0, 1.0f, 0);
	glScalef(1.2, 1.2, 1.2);
	drawSnowMan();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(santaX, santaY, santaZ);
	glRotatef(santaTheta, 0, 1, 0);
	glScalef(1.2, 1.2, 1.2);
	drawSanta();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, movetorix-9);

	glScalef(1.2, 1.2, 1.2);
	drawTorii();
	glPopMatrix();

	collisionhandling();
	snow();
	glFlush();
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(800, 800);
	changeSize(800, 800);
	//glutInitWindowPosition(200, 200);
	glutCreateWindow("Christmas");

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	//changeSize(600, 600);
	glutIdleFunc(Anim);
	glutTimerFunc(0, Timer, 0);

	enablelighting();

	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(keyPressedSp);
	glutSpecialUpFunc(keyReleasedSp);

	glEnable(GL_DEPTH_TEST);

	glutMainLoop();

}