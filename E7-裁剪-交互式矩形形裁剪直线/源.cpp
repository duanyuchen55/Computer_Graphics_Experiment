#include<GL/glut.h>
#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;
const float window_width = 800, window_height = 600;
const int d = 8;
struct point
{
	float x, y;
	point() {}
	point(float xx, float yy)
		:x(xx), y(yy) {}
};
struct EDGE//Edge
{
	float bx, by, ex, ey;
	EDGE() {}
	EDGE(float bxx, float byy, float exx, float eyy)
		:bx(bxx), by(byy), ex(exx), ey(eyy) {}
};

char OP;
EDGE left(200, 450, 200, 200);
EDGE bottom(200, 200, 600, 200);
EDGE right(600, 200, 600, 450);
EDGE top(600, 450, 200, 450);
float straightLine_color[3] = { 0,0,1 };
float remainingLine_color[3] = { 1,0,0 };
vector<point> line;

void draw_a_point(int x, int y)
{
	glPointSize(3.0f);
	glBegin(GL_POINTS);
	glColor3f(0,1,0);
	glVertex2f(x, y);
	glEnd();
	glFlush();
}
/*
XL <= x1 + u*dx <= XR
YB <= y1 + u*dy <= YT

p<0 ���⵽��
p>0 ���ڵ���
 p!=0 ����u=q/p
https://blog.csdn.net/weixin_41894030/article/details/102817736

��1��Pk=0
���������qk<0�����߶���ȫ�ڱ߽��⣬Ӧ�� �����߶�
���qk��0�����һ���ж�
��2��Pk!=0
��pk < 0ʱ���߶δӲü��߽��ӳ��ߵ��ⲿ ���쵽�ڲ�������߽���
��pk > 0ʱ���߶δӲü��߽��ӳ��ߵ��ڲ� ���쵽�ⲿ���ǳ��߽���

�߶κʹ��ڱ߽�һ�����ĸ����㣬����pk�ķ��ţ���֪�� ���������뽻�㣬�������ǳ�����
��pk < 0ʱ����Ӧ��߽���
��pk > 0ʱ����Ӧ���߽���

һ���ĸ�uֵ���ټ���u=0��u=1�����˵�ֵ���ܹ�����ֵ
��pk��0������uֵ����ߵ��������㣩��0�Ƚ�ȥ�����ģ���pk��0������uֵ�����ߵ��������㣩��1�Ƚ�ȥ����С�ģ������͵õ������˵�Ĳ���ֵ

*/
bool ClipT(float p, float q, float* u1, float* u2) 
{
	float r;
	if (p < 0) 
	{
		r = q / p;
		if (r > * u2)
			return FALSE;
		if (r > * u1)
			*u1 = r;
	}
	else if (p > 0)
	{
		r = q / p;
		if (r < *u1)
			return FALSE;
		if (r < *u2)
			*u2 = r;
	}
	else
		return q >= 0;
	return TRUE;
}

void LB_LineClip(float x1, float y1, float x2, float y2, float XL, float XR, float YB, float YT) 
{
	float dx, dy, u1, u2;
	u1 = 0;
	u2 = 1;
	dx = x2 - x1;
	dy = y2 - y1;
	if (ClipT(-dx, x1 - XL, &u1, &u2))//p1=-dx q1=x1-XL
		if (ClipT(dx, XR - x1, &u1, &u2))//p2=dx q2=XR-x1
			if (ClipT(-dy, y1 - YB, &u1, &u2))//p3=-dy q3=y1-YB
				if (ClipT(dy, YT - y1, &u1, &u2))//p4=dy q4=YT-y1
				{
					glLineWidth(3);
					glColor3fv(remainingLine_color);
					glBegin(GL_LINES);

					glVertex2f(x1 + u1 * dx, y1 + u1 * dy);
					glVertex2f(x1 + u2 * dx, y1 + u2 * dy);
					glEnd();
					glFlush();
				}
}

int getdis(int x, int y)
{
	int ans = -1;
	float shortdis = 99999999;
	for (int i = 0; i < line.size(); i++)
	{
		float dis = sqrt(pow(x - line[i].x, 2) + pow(y - line[i].y, 2));
		if (dis < shortdis && dis <= d)
		{
			shortdis = dis;
			ans = i;
		}

		/*dis = sqrt(pow(x - lineend[i].x, 2) + pow(y - lineend[i].y, 2));
		if (dis < shortdis && dis <= d)
		{
			shortdis = dis;
			ans = i;
		}*/
	}
	return ans;
}

void mymouse(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		for (int i = 0; i < line.size() - 1; i += 2)
		{
			LB_LineClip(line[i].x, line[i].y, line[i + 1].x, line[i + 1].y, 200, 600, 200, 450);
		}
	}
	if (OP == 'p')
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			draw_a_point(x, window_height - y);
			line.push_back(point(x, window_height - y));
			cout << "ֱ��" << "(" << x << ", " << window_height - y << ") ";// << endl;
		}
		if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		{
			line.push_back(point(x, window_height - y));
			cout << "(" << x << ", " << window_height - y << ")" << endl;
		}
	}
}

void dragmouse(int x, int y)
{
	glClear(GL_COLOR_BUFFER_BIT);//�����������
	if (OP == 'p')//��ֱ��
	{
		glBegin(GL_LINES);
		glColor3fv(straightLine_color);
		glVertex2f(line[line.size() - 1].x, line[line.size() - 1].y);
		glVertex2f(x, window_height - y);
		glEnd();
	}
	if (OP == 'd')//�϶��˵�
	{
		int index = getdis(x, window_height - y);
		if (index != -1)
		{
			line[index].x = x;
			line[index].y = window_height - y;
		}
	}
	

	//�����߽�
	glColor3f(0, 1, 0);
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glVertex2f(::left.bx, ::left.by); glVertex2f(::left.ex, ::left.ey);
	glVertex2f(bottom.bx, bottom.by); glVertex2f(bottom.ex, bottom.ey);
	glVertex2f(::right.bx, ::right.by); glVertex2f(::right.ex, ::right.ey);
	glVertex2f(top.bx, top.by); glVertex2f(top.ex, top.ey);
	glEnd();

	//��֮ǰ���е�ֱ�߻�����
	glBegin(GL_LINES);
	glColor3fv(straightLine_color);
	for (int i = 0; i < line.size() - 1; i+=2)
	{
		glVertex2f(line[i].x, line[i].y);
		glVertex2f(line[i + 1].x, line[i + 1].y);
	}
	glEnd();
	glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);
	if (key == 'p')//��ֱ��
	{
		OP = 'p';
		cout << "�밴ס���������϶���껭ֱ��" << endl;
	}
	if (key == 'd')//�϶��˵�
	{
		OP = 'd';
		cout << "�밴סֱ�߶˵��϶�" << endl;
	}
}

//void processmenuevent(int op)
//{
//	switch (op)
//	{
//	case 'p':
//		OP = 'p';
//	case 'd':
//		OP = 'd';
//	case 'c':
//		OP = 'c';
//	default:
//		break;
//	}
//}
//void createMenu()
//{
//	int menu = glutCreateMenu(processmenuevent);
//	glutAddMenuEntry("����", 'p');
//	glutAddMenuEntry("�϶�", 'd');
//	glutAddMenuEntry("�ü�", 'c');
//
//	glutAttachMenu(GLUT_MIDDLE_BUTTON);
//}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("liang-Barskey����ʽ���α߿�ü�ֱ��");
	cout << "���̵��p��ʼ����\n���̵��d�϶��˵�\n�������Ҽ��ü�\n���ESC�˳�" << endl;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, window_width, 0, window_height);

	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0, 1, 0);
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glVertex2f(::left.bx, ::left.by); glVertex2f(::left.ex, ::left.ey);
	glVertex2f(bottom.bx, bottom.by); glVertex2f(bottom.ex, bottom.ey);
	glVertex2f(::right.bx, ::right.by); glVertex2f(::right.ex, ::right.ey);
	glVertex2f(top.bx, top.by); glVertex2f(top.ex, top.ey);
	glEnd();
	glFlush();

	glutMouseFunc(&mymouse);
	glutMotionFunc(&dragmouse);
	glutKeyboardFunc(&keyboard);

	glutMainLoop();
	return 0;
}