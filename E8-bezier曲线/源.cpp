#include<gl/glut.h>
#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;
const float window_width = 800, window_height = 600;


struct point
{
	float x, y;
	point() {}
	point(float xx, float yy)
		:x(xx), y(yy) {}
};
vector<point> input_vertice;
vector<point> controlpoint;
vector<point> bezierpoint;

float controlpoint_color[3] = { 1,0,0 };
float straightline_color[3] = { 0,1,0 };
float bezierline_color[3] = { 0,0,1 };

void draw_a_point(float x, float y, float color[])
{
	glPointSize(3.0f);
	glBegin(GL_POINTS);
	glColor3fv(color);
	glVertex2f(x, y);
	glEnd();
	glFlush();
}
void deCasteljau();
void mymouse(int button, int state, int x, int y);

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("bezier����");
	cout << "��������Ƶ�\n�Ҽ���bezier����" << endl;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, window_width, 0, window_height);

	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glutMouseFunc(&mymouse);

	glutMainLoop();
	return 0;
}

void mymouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		draw_a_point(x, window_height - y, controlpoint_color);
		point p(x, window_height - y);
		input_vertice.push_back(p);
		cout << "����Ŀ��Ƶ�" << input_vertice.size() << ":(" << x << ", " << window_height - y << ")" << endl;

		//��ʱ������еĿ��Ƶ�
		glBegin(GL_LINE_STRIP);
		glColor3fv(straightline_color);
		for (int i = 0; i < input_vertice.size(); i++)
			glVertex2f(input_vertice[i].x, input_vertice[i].y);
		glEnd();
		glFlush();
		
		controlpoint = input_vertice;
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		deCasteljau();
	}
}

void deCasteljau()
{
	int n = controlpoint.size();

	for (double t = 0.0; t <= 1.0; t += 0.005 / n)//��֤��������㹻�࣬ʹ���߸���Բ��
	{
		for (int i = 1; i < n; i++)//����ÿ��һ�֣����Ƶ㶼���� 1
		{
			for (int j = 0; j < n - i; j++)//ÿ��һ�֣����Ƶ㶼���� 1
			{
				if (i == 1)//������һ�ֱ�����������Ǽ������Ƶ㣬so����������ֻ��һ��controlpoint��input_verticeҲ���á�������
				{
					controlpoint[j].x = (1 - t) * input_vertice[j].x + t * input_vertice[j + 1].x;
					controlpoint[j].y = (1 - t) * input_vertice[j].y + t * input_vertice[j + 1].y;
					continue;
				}
				else
				{
					controlpoint[j].x = (1 - t) * controlpoint[j].x + t * controlpoint[j + 1].x;
					controlpoint[j].y = (1 - t) * controlpoint[j].y + t * controlpoint[j + 1].y;
				}
			}
		}
		//cout << "���Ƶ�����=" << controlpoint.size() << endl;һֱ�ǵ����Ŀ����Ϊ֮ǰֱ�Ӵ�����������ˣ��������õ�ֻ���±�Ϊ1�Ĳ�����
		bezierpoint.push_back(controlpoint[0]);//�����һ����bezier�����ϵĵ㱣�����������������һ��Ϊbezier����
	}

	//����bezier����
	glLineWidth(3.0f);
	glBegin(GL_LINE_STRIP);
	glColor3fv(bezierline_color);
	for (int i = 0; i < bezierpoint.size(); i++)
		glVertex2f(bezierpoint[i].x, bezierpoint[i].y);
	glEnd();
	glFlush();
}