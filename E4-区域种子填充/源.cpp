//DFS ϵͳ�ݹ�ջ

//#include<iostream>
//#include<vector>
//#include<GL/glut.h>
//using namespace std;
//int window_width = 800, window_height = 600;
//
//struct point
//{
//	int x, y;
//	point()
//	{}
//	point(int xx, int yy)
//		:x(xx), y(yy) {}
//};
//vector<point> vertice;
//
//float newcolor[3] = { 1, 0, 0 };
//float boundarycolor[3] = { 0, 0, 1 };
//
//void draw_a_point(int x, int y)
//{
//	glBegin(GL_POINTS);
//	glColor3fv(newcolor);
//	glVertex2f(x, y);
//	glEnd();
//	glFlush();
//}
//
//bool is_equal(float* a, float* b)
//{
//	return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
//}
//
//
//void BoundaryFill4(int x, int y)
//{
//	float color[3];
//	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, color);
//	cout << x << "," << y << ":" << color[0] << color[1] << color[2] << endl;
//	if (!is_equal(color, newcolor) && !is_equal(color, boundarycolor))
//	{
//		draw_a_point(x, y);
//		BoundaryFill4(x, y + 1);
//		BoundaryFill4(x, y - 1);
//		BoundaryFill4(x + 1, y);
//		BoundaryFill4(x - 1, y);
//	}
//}
//
//
//void mymouse(int button, int state, int x, int y)
//{
//	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
//	{
//		draw_a_point(x, window_height - y);
//
//		point p(x, window_height - y);
//		vertice.push_back(p);
//		cout << "����" << vertice.size() << ": (" << x << ", " << y << ")" << endl;
//	}
//
//	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
//	{
//		glClearColor(1, 1, 1, 1);//���û��ƴ�����ɫΪ��ɫ
//		glColor3fv(boundarycolor);
//
//		glBegin(GL_LINES);
//		for (int i = 0; i < vertice.size(); i++)
//		{
//			if (i == vertice.size() - 1)//�������һ���㣬ʹ��պ�
//			{
//				glVertex2f(vertice[0].x, vertice[0].y);
//				glVertex2f(vertice[i].x, vertice[i].y);
//			}
//			else
//			{
//				glVertex2f(vertice[i].x, vertice[i].y);
//				glVertex2f(vertice[i + 1].x, vertice[i + 1].y);
//			}
//		}
//		glEnd();
//		glFlush();
//		vertice.clear();
//	}
//
//	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
//	{
//		cout << "center: (" << x << ", " << y << ")" << endl;
//		BoundaryFill4(x, window_height - y);
//	}
//}
//void KeyBoards(unsigned char key, int x, int y)
//{
//	if (key == 32)
//	{
//		BoundaryFill4(x, window_height - y);
//		glFlush();
//	}
//}
//
//
//int main(int argc, char* argv[])
//{
//	cout << "������㣬�Ҽ���������γɶ����" << endl;
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
//	glutInitWindowPosition(50, 50);
//	glutInitWindowSize(window_width, window_height);
//	glutCreateWindow("PolyFill");
//	glMatrixMode(GL_PROJECTION);/*����ΪͶӰ����ģʽ�������۲����*/
//	//glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();/*����ΪͶӰ����ģʽ�������۲����*/
//	//gluOrtho2D(0, window_width, window_height, 0);/*����ΪͶӰ����ģʽ�������۲�������۲촰�ڵĴ�СҪ�뻭����Сһ�£�����ֱ������ȫ�ֱ�������*/
//	gluOrtho2D(0, window_width, 0, window_height);
//
//	glClearColor(1, 1, 1, 1);//���û��ƴ�����ɫΪ��ɫ
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	glutMouseFunc(&mymouse);
//	//glutKeyboardFunc(&KeyBoards);
//	glutMainLoop();
//	return 0;
//}


//BFS
#include<iostream>
#include<vector>
#include<GL/glut.h>
#include<queue>
#include<stack>
using namespace std;
int window_width = 800, window_height = 600;

struct point
{
	int x, y;
	point()
	{}
	point(int xx, int yy)
		:x(xx), y(yy) {}
};
vector<point> vertice;
float newcolor[3] = { 1, 0, 0 };
float boundarycolor[3] = { 0, 0, 1 };

void draw_a_point(int x, int y);
bool is_equal(float* a, float* b);
void BoundaryFill4(int x, int y);
void BoundaryFill4_Stack(int x, int y);
void mymouse(int button, int state, int x, int y);
void KeyBoards(unsigned char key, int x, int y);

int main(int argc, char* argv[])
{
	cout << "������������㣻" << endl << "�������Ҽ��������㣬�γɶ���Σ�" << endl << "�������м�ȷ������������ӵ�λ�á�" << endl;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("�����������");
	glMatrixMode(GL_PROJECTION);/*����ΪͶӰ����ģʽ�������۲����*/
	//glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();/*����ΪͶӰ����ģʽ�������۲����*/
	//gluOrtho2D(0, window_width, window_height, 0);/*����ΪͶӰ����ģʽ�������۲�������۲촰�ڵĴ�СҪ�뻭����Сһ�£�����ֱ������ȫ�ֱ�������*/
	gluOrtho2D(0, window_width, 0, window_height);//����Ĳ����ԣ�����

	glClearColor(1, 1, 1, 1);//���û��ƴ�����ɫΪ��ɫ
	glClear(GL_COLOR_BUFFER_BIT);

	glutMouseFunc(&mymouse);
	//glutKeyboardFunc(&KeyBoards);
	glutMainLoop();
	return 0;
}


void draw_a_point(int x, int y)
{
	glBegin(GL_POINTS);
	glColor3fv(newcolor);
	glVertex2f(x, y);
	glEnd();
	glFlush();
}
bool is_equal(float* a, float* b)
{
	return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
}
void BoundaryFill4(int x, int y)
{
	queue<point> q;
	q.push(point(x, y));
	while (!q.empty())
	{
		point now = q.front();
		q.pop();
		int nowx = now.x, nowy = now.y;
		float color[3];
		glReadPixels(nowx, nowy, 1, 1, GL_RGB, GL_FLOAT, color);
		if (!is_equal(color, newcolor) && !is_equal(color, boundarycolor))
		{
			draw_a_point(nowx, nowy);
			q.push(point(nowx, nowy + 1));
			q.push(point(nowx, nowy - 1));
			q.push(point(nowx + 1, nowy));
			q.push(point(nowx - 1, nowy));
		}
	}
}
void BoundaryFill4_Stack(int x, int y)
{
	//queue<point> q;
	stack < point> q;
	q.push(point(x, y));
	while (!q.empty())
	{
		point now = q.top();
		q.pop();
		int nowx = now.x, nowy = now.y;
		float color[3];
		glReadPixels(nowx, nowy, 1, 1, GL_RGB, GL_FLOAT, color);
		if (!is_equal(color, newcolor) && !is_equal(color, boundarycolor))
		{
			draw_a_point(nowx, nowy);
			q.push(point(nowx, nowy + 1));
			q.push(point(nowx, nowy - 1));
			q.push(point(nowx + 1, nowy));
			q.push(point(nowx - 1, nowy));
		}
	}
}
void mymouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		draw_a_point(x, window_height - y);

		point p(x, window_height - y);
		vertice.push_back(p);
		cout << "����" << vertice.size() << ": (" << x << ", " << y << ")" << endl;
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		glClearColor(1, 1, 1, 1);//���û��ƴ�����ɫΪ��ɫ
		glColor3fv(boundarycolor);

		glBegin(GL_LINES);
		for (int i = 0; i < vertice.size(); i++)
		{
			if (i == vertice.size() - 1)//�������һ���㣬ʹ��պ�
			{
				glVertex2f(vertice[0].x, vertice[0].y);
				glVertex2f(vertice[i].x, vertice[i].y);
			}
			else
			{
				glVertex2f(vertice[i].x, vertice[i].y);
				glVertex2f(vertice[i + 1].x, vertice[i + 1].y);
			}
		}
		glEnd();
		glFlush();
		vertice.clear();
	}

	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		cout << "center: (" << x << ", " << y << ")" << endl;
		//BoundaryFill4(x, window_height - y);
		BoundaryFill4_Stack(x, window_height - y);
	}
}
void KeyBoards(unsigned char key, int x, int y)
{
	if (key == 32)
	{
		BoundaryFill4(x, window_height - y);
		glFlush();
	}
}
