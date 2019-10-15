#include <GL/glut.h>
#include <math.h>
#include<stdio.h>

const GLfloat Pi = 3.1415926536f;

//GLfloat R = 1.0f;
void display1();//�����-��������
void display2();//����

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);//��GLUT���г�ʼ�����������������������GLUTʹ��֮ǰ����һ��
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE); //������ʾ��ʽ
	glutInitWindowPosition(500, 100);
	glutInitWindowSize(700, 700);
	glutCreateWindow("ͼ��ѧ"); //����ǰ�����õ���Ϣ�������ڡ�����������Ϊ���ڵı��⡣

	glutDisplayFunc(&display1);//����
	//glutDisplayFunc(&display2);//����
	glutMainLoop();

	return 0;
}

void display1()//�����
{
	GLfloat R = 1.0f;
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_LINE_SMOOTH);                           //���÷�����
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);              //���÷�����

	int k = 0;
	for (/*int k = 0; k < 100; k++*/;;)
	{
		//��
		glBegin(GL_LINE_LOOP);
		if (k % 2 == 0)
			for (int i = 1; i <= 4; ++i)
				glVertex2f(R * cos(Pi * i / 2), R * sin(Pi * i / 2));
		else
			for (int i = 1; i <= 4; i++)
				glVertex2f(R * cos(Pi * i / 2 - Pi / 4), R * sin(Pi * i / 2 - Pi / 4));
		R = sqrt(2) / 2 * R;
		glEnd();

		//Բ
		glBegin(GL_LINE_LOOP);
		for (int i = 1; i <= 150; ++i)
			glVertex2f(R * cos(2 * Pi / 150 * i), R * sin(2 * Pi / 150 * i));
		glEnd();
		//����ɵ�glFlush();��д�������ʱ���
		glFlush();
		k++;
	}
	//glFlush();
}

void display2()//����
{
	GLfloat R = 1.0f;
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_LINE_SMOOTH);                           //���÷�����
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);              //���÷�����

	//int k = 0;
	for (int k = 0; k < 100; k++)
	{
		//��
		glBegin(GL_LINE_LOOP);
		if (k % 2 == 0)
			for (int i = 1; i <= 4; ++i)
				glVertex2f(R * cos(Pi * i / 2), R * sin(Pi * i / 2));
		else
			for (int i = 1; i <= 4; i++)
				glVertex2f(R * cos(Pi * i / 2 - Pi / 4), R * sin(Pi * i / 2 - Pi / 4));
		R = sqrt(2) / 2 * R;
		glEnd();

		//Բ
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