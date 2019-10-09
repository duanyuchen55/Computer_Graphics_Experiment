#include<iostream>
#include<GL/glut.h>
using namespace std;

float bx, by, ex, ey, r;//���ߵ���ֹ�����꣬�ɴ�ȷ��Բ��Բ��(bx, by)�Ͱ뾶

int window_width = 1000, window_height = 800;

void MidPointCircle(int x0, int y0, int r);
void display();
void drag_mouse(int x, int y);
void listen_mouse(int button, int state, int x, int y);

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("�е㻭Բ�㷨");

	glClearColor(1, 1, 1, 1);//���û��ƴ�����ɫΪ��ɫ\

	glMatrixMode(GL_PROJECTION);/*����ΪͶӰ����ģʽ�������۲����*/
	glLoadIdentity();/*����ΪͶӰ����ģʽ�������۲����*/
	gluOrtho2D(0, window_width, window_height, 0);/*����ΪͶӰ����ģʽ�������۲�������۲촰�ڵĴ�СҪ�뻭����Сһ�£�����ֱ������ȫ�ֱ�������*/

	glPointSize(3.0f);//���õ�Ĵ�С
	glColor3f(10, -10, -10);//���õ����ɫ


	glutDisplayFunc(&display);

	glutMouseFunc(listen_mouse);
	glutMotionFunc(drag_mouse);
	glutMainLoop();

	return 0;
}

void MidPointCircle(int x0, int y0, int r)
{
	glClear(GL_COLOR_BUFFER_BIT);

	int x = 0;
	int y = r;
	double d = 1.25 - r;
	glBegin(GL_POINTS);
	glVertex2f(x0, y0);
	glVertex2f(x + x0, y + y0);
	glEnd();
	while (x <= y)
	{
		if (d < 0)
			d += 2 * x + 3;
		else
		{
			d += 2 * (x - y) + 5;
			y--;
		}
		x++;

		glBegin(GL_POINTS);
		//���ð˶Գ��Ի�������Բ
		glVertex2f(x + x0, y + y0);
		glVertex2f(y + x0, x + y0);
		glVertex2f(-x + x0, y + y0);
		glVertex2f(y + x0, -x + y0);
		glVertex2f(x + x0, -y + y0);
		glVertex2f(-y + x0, x + y0);
		glVertex2f(-x + x0, -y + y0);
		glVertex2f(-y + x0, -x + y0);
		glEnd();
	}
	glFlush();
}
void display()
{
	MidPointCircle(bx, by, r);
}

//�϶����
void drag_mouse(int x, int y)
{
	ex = x;
	ey = y;
	printf("�������꣺���(%d, %d) �յ�(%d, %d)\n", bx, by, ex, ey);//�����������ӡ����

	r = sqrt((bx - ex) * (bx - ex) + (by - ey) * (by - ey));

	MidPointCircle(bx, by, r);
	glFlush();
}

//�������λ�ã����һ���
void listen_mouse(int button, int state, int x, int y)
{
	//̧������ʱ�������յ� �յ�
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		ex = x;
		ey = y;
		printf("�������꣺���(%d, %d) �յ�(%d, %d)\n", bx, by, ex, ey);//�����������ӡ����
		r = sqrt((bx - ex) * (bx - ex) + (by - ey) * (by - ey));
		printf("Բ�뾶Ϊ��%d\n", r);
		MidPointCircle(bx, by, r);//����
		glFlush();
	}
	//�������ʱ�� �����
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		bx = x;
		by = y;
	}
}