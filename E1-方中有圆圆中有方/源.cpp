#include <GL/glut.h>
#include <math.h>
#include<stdio.h>

const GLfloat Pi = 3.1415926536f;

//GLfloat R = 1.0f;
void display1();//无穷尽焉-导致死机
void display2();//有穷

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);//对GLUT进行初始化，这个函数必须在其它的GLUT使用之前调用一次
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE); //设置显示方式
	glutInitWindowPosition(500, 100);
	glutInitWindowSize(700, 700);
	glutCreateWindow("图形学"); //根据前面设置的信息创建窗口。参数将被作为窗口的标题。

	glutDisplayFunc(&display1);//无穷
	//glutDisplayFunc(&display2);//有穷
	glutMainLoop();

	return 0;
}

void display1()//无穷尽焉
{
	GLfloat R = 1.0f;
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_LINE_SMOOTH);                           //设置反走样
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);              //设置反走样

	int k = 0;
	for (/*int k = 0; k < 100; k++*/;;)
	{
		//方
		glBegin(GL_LINE_LOOP);
		if (k % 2 == 0)
			for (int i = 1; i <= 4; ++i)
				glVertex2f(R * cos(Pi * i / 2), R * sin(Pi * i / 2));
		else
			for (int i = 1; i <= 4; i++)
				glVertex2f(R * cos(Pi * i / 2 - Pi / 4), R * sin(Pi * i / 2 - Pi / 4));
		R = sqrt(2) / 2 * R;
		glEnd();

		//圆
		glBegin(GL_LINE_LOOP);
		for (int i = 1; i <= 150; ++i)
			glVertex2f(R * cos(2 * Pi / 150 * i), R * sin(2 * Pi / 150 * i));
		glEnd();
		//无穷尽焉的glFlush();就写在这里，及时输出
		glFlush();
		k++;
	}
	//glFlush();
}

void display2()//有穷
{
	GLfloat R = 1.0f;
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_LINE_SMOOTH);                           //设置反走样
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);              //设置反走样

	//int k = 0;
	for (int k = 0; k < 100; k++)
	{
		//方
		glBegin(GL_LINE_LOOP);
		if (k % 2 == 0)
			for (int i = 1; i <= 4; ++i)
				glVertex2f(R * cos(Pi * i / 2), R * sin(Pi * i / 2));
		else
			for (int i = 1; i <= 4; i++)
				glVertex2f(R * cos(Pi * i / 2 - Pi / 4), R * sin(Pi * i / 2 - Pi / 4));
		R = sqrt(2) / 2 * R;
		glEnd();

		//圆
		glBegin(GL_LINE_LOOP);
		for (int i = 1; i <= 150; ++i)
			glVertex2f(R * cos(2 * Pi / 150 * i), R * sin(2 * Pi / 150 * i));
		glEnd();
	}
	glFlush();
}

//#include<GL/glut.h>
//#include<cmath>
//using namespace std;
//
//const int n = 100;
//const GLfloat PI = 3.1415926;
//
//
//void display()
//{
//	GLfloat r = 1.0f;
//	glClear(GL_COLOR_BUFFER_BIT);
//	for (int k = 0; k < 10; k++)
//	{
//
//		glBegin(GL_LINE_LOOP);
//		if (k % 2 == 0)
//		{
//			for (int i = 1; i <= 4; i++)
//			{
//				glVertex2f(r * cos(2 * PI / 4 * i), r * sin(2 * PI / 4 * i));
//			}
//		}
//		else
//		{
//			for (int i = 1; i <= 4; i++)
//			{
//				glVertex2f(r * cos((i - 1) * PI / 2 + PI / 4), r * sin((i - 1) * PI / 2 + PI / 4));  //45 135 225 315
//			}
//		}
//		glEnd();
//		r /= 1.414;
//		glBegin(GL_LINE_LOOP);
//		for (int i = 1; i <= 100; i++)
//		{
//			glVertex2f(r * cos(2 * PI / 100 * i), r * sin(2 * PI / 100 * i));
//
//		}
//		glEnd();
//
//
//	}
//	glFlush();
//}
//
//int main(int argc, char** argv)
//{
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
//	glutInitWindowPosition(300, 300);
//	glutInitWindowSize(800, 800);
//	glutCreateWindow("opengl");
//	glutDisplayFunc(display);
//	glutMainLoop();
//	return 0;
//}