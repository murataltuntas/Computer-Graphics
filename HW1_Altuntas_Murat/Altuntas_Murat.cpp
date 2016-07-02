/*
*	Murat ALTUNTAS
*	111044043
*	
*/
#include <iostream>
#include <math.h>
#include <windows.h>
#include <GL/glut.h>  // GLUT, includes glu.h and gl.h

using namespace std;

#define HIGHT 800   // windows size

/* Function that returns -1,0,1 depending on whether x */
/* is <0, =0, >0 respectively */
#define sign(x) ((x>0)?1:((x<0)?-1:0))

/* timer icin */
double PCFreq = 0.0;
__int64 CounterStart = 0;

/* noktalari tutmak icin Point struct'i */
struct Point {
	GLint x;
	GLint y;
};

Point p1, p2;
int flag = -1;
static int timeFlag = 1;
double ddaR, bresenR;

/* zamani tutmak icin kullandigim fonksiyonlar */
void StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		cout << "QueryPerformanceFrequency failed!\n";

	PCFreq = double(li.QuadPart) / 1000.0;

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
}
double GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - CounterStart) / PCFreq;
}

/* mouse eventi icin kullandigim fonksiyon */
void myMouseFunc(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		p1.x = (x);
		p1.y = (HIGHT - y);
	}
	else if (button == GLUT_RIGHT_BUTTON) {
		p2.x = (x);
		p2.y = (HIGHT - y);
	}
}

/* initial function */
void init()
{
	// Set display window color to as glClearColor(R,G,B,Alpha)
	glClearColor(255, 255, 255, 255);
	// Set projection parameters.
	glMatrixMode(GL_PROJECTION);
	// Set 2D Transformation as gluOrtho2D(Min Width, Max Width, Min Height, Max Height)
	gluOrtho2D(0.0, HIGHT, 0.0, HIGHT);
}

/* Hucrelerin icini dolduran fonksiyon */
void drawQuad(GLint x, GLint y) {
	
	x = x - ((int)x % 4);
	y = y - ((int)y % 4);
	glBegin(GL_QUADS);

	glVertex2i(x, y + 4);
	glVertex2i(x, y);
	glVertex2i(x + 4, y);
	glVertex2i(x + 4, y + 4);

	glEnd();
	glFlush();
}

/* DDA Algorithm */
void lineDDA(Point p1, Point p2) {
	double dx = p2.x - p1.x;
	double dy = p2.y - p1.y;	
	float x = p1.x;
	float y = p1.y;
	float x1 = p1.x;
	float y1 = p1.y;
	double step = 0;

	/* Find out whether to increment x or y */
	step = (abs(dx)>abs(dy)) ? (abs(dx)) : (abs(dy));

	float xInc = dx / (float)step;
	float yInc = dy / (float)step;

	for (float i = 1; i <= step; i++) {
		drawQuad(x1,y1);
		x1 = x1 + xInc;
		y1 = y1 + yInc;
	}
}

/* Bresenham's Algorithm */
void lineBresenham(int x0, int y0, int x1, int y1) {
	int dy = y1 - y0;
	int dx = x1 - x0;
	int stepx, stepy;
	if (dy < 0) { dy = -dy; stepy = -1; }
	else { stepy = 1; }
	if (dx < 0) { dx = -dx; stepx = -1; }
	else { stepx = 1; }
	dy <<= 1; // dy is now 2*dy
	dx <<= 1; // dx is now 2*dx
	drawQuad(x0, y0);
	if (dx > dy) {
		int fraction = dy - (dx >> 1); // same as 2*dy - dx
		while (x0 != x1) {
			if (fraction >= 0) {
				y0 += stepy;
				fraction -= dx; // same as fraction -=
			}
			x0 += stepx;
			fraction += dy; // same as fraction -=
			
				drawQuad(x0, y0);
		}
	} else {
		int fraction = dx - (dy >> 1);
		while (y0 != y1) {
			if (fraction >= 0) {
				x0 += stepx;
				fraction -= dy;
			}
			y0 += stepy;
			fraction += dx;
			drawQuad(x0, y0);
		}
	}
}

/* ekrana bir tasarim cizen fonksiyon */
void drawPattern(){
	Point pt1[1000];
	Point pt2[1000];

	for (int i = 0; i < 50; i++)
	{
		pt1[i].x = ((i + 125) * (i + 11)) % 800;
		pt1[i].y = (abs(i - 17) * (i + 13)) % 800;
	}

	for (int i = 0; i < 50; i++)
	{
		pt2[i].x = (abs((50 - i) - 17) * (i + 13)) % 800;
		pt2[i].y = ((i + 125) * ((100 - i) + 11)) % 800;
	}

	for (int i = 50; i < 100; i++)
	{
		pt1[i].x = (((i * 13) + 35) * (i + 11)) % 800;
		pt1[i].y = (abs((100 - i) * 15 - 17) * (i + 13)) % 800;
	}

	for (int i = 50; i < 100; i++)
	{
		pt2[i].x = (abs((250 - i)*14 - 17) * (i + 13)) % 800;
		pt2[i].y = ((i + 125) * ((100 - i)*8 + 11)) % 800;
	}

	for (int j = 1; j < 10; j++)
	{
		for (int i = 0; i < 100; i++)
		{
			pt1[i*j].x = (((i * 13) + 35) * (i + 11)) % 800;
			pt1[i*j].y = (abs((100 - i) * 15 - 17) * (i + 13)) % 800;
		}

		for (int i = 0; i < 100; i++)
		{
			pt2[i*j].x = (abs((250 - i) * 14 - 17) * (i + 13)) % 800;
			pt2[i*j].y = ((i + 125) * ((100 - i) * 8 + 11)) % 800;
		}
	}
	
	if (timeFlag){
		StartCounter();
		for (int i = 0; i < 1000; i++)
		{
			lineDDA(pt1[i], pt2[i]);
		}
		ddaR = GetCounter();
		cout << "DDA\t\t" << ddaR << " ms \n";		
		timeFlag = 0;
	}

	if (timeFlag == 0){
		StartCounter();
		for (int i = 0; i < 1000; i++)
		{
			lineBresenham(pt1[i].x, pt1[i].y, pt2[i].x, pt2[i].y);
		}
		bresenR = GetCounter();
		cout << "Bresenham\t" << bresenR << " ms \n";
		timeFlag = 2;
	}

	if (timeFlag == 2){
		
		(ddaR < bresenR) ? 
			cout << "DDA Algorithm faster than Bresenham Algorithm.\n"
			: cout << "Bresenham Algorithm faster than DDA Algorithm.\n";
		timeFlag = 3;
	}
}

/* ekrana gridleri , lineleri ve desenleri cizen fonksiyon */
void lines()
{
	glClear(GL_COLOR_BUFFER_BIT); // Clear display window

	// Calculate color

	glLineWidth(1);
	glColor3f(0,0,0);
	glBegin(GL_LINES);

	for (GLint i = 0; i < HIGHT; i += 4)
	{
		glVertex2i(i, HIGHT);
		glVertex2i(i, 0);
	}

	for (GLint i = 0; i < HIGHT; i += 4)
	{
		glVertex2i(0, i);
		glVertex2i(HIGHT, i);
	}

	glEnd();
	glFlush();

	if (flag == 0)
	{
		lineDDA(p1, p2);
	}
	if (flag == 1)
	{
		lineBresenham(p1.x, p1.y, p2.x, p2.y);
	}
	if (flag == 2)
	{
		cout << "\nnumber of lines: 1000\n";
		drawPattern();
	}

	glutSwapBuffers();
}

int main(int argc, char ** argv)
{
	printf("1. Use DDA Algorithm\n");
	printf("2. Use Bresenham Algorithm\n");
	printf("3. Start Benchmark\n");
	printf("4. Exit\n");
	printf("...\n");
	int option;
	scanf_s("%d", &option);

	if(option == 1)
	{
		flag = 0;
	}

	if(option == 2)
	{
		flag = 1;
	}

	if(option == 3)
	{
		flag = 2;
	}

	if (option == 4)
	{
		return 0;
	}

	// Initialize GLUT
	glutInit(&argc, argv);
	// Set display mode
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	// Set top - left display window position.
	glutInitWindowPosition(0, 0);
	// Set display window width and height
	glutInitWindowSize(HIGHT, HIGHT);
	// Create display window with the given title
	glutCreateWindow("Computer Graphics Hw1 Murat ALTUNTAS");
	glutMouseFunc(myMouseFunc);
	// Execute initialization procedure
	init();
	// Send graphics to display window
	glutDisplayFunc(lines);

	if (option != 3)
	{
		glutIdleFunc(lines);
	}
	// Display everything and wait.
	glutMainLoop();
}