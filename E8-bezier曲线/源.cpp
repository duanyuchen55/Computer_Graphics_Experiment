#include<gl/glut.h>
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
vector<point> input_vertice;
vector<point> controlpoint;

char OP;
float controlpoint_color[3] = { 1,0,0 };
float straightline_color[3] = { 0,1,0 };
float bezierline_color[3] = { 0,0,1 };

void draw_a_point(float x, float y, float color[]);
void deCasteljau();
void mymouse(int button, int state, int x, int y);
void dragmouse(int x, int y);
void keyboard(unsigned char key, int x, int y);
int getdis(int x, int y);//��ȡ�������ĵ�

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("de Casteljau ����bezier����");
	cout << "���̵��p�󣬵��������ƿ��Ƶ�" << endl;
	cout << "���̵��i�󣬲�����Ƶ�" << endl;
	cout << "���̵��d���ƶ����Ƶ�" << endl;
	cout << "���̵��e�󣬵��������ɾ�����Ƶ�" << endl;
	cout << "��ESC�˳�" << endl << endl;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, window_width, 0, window_height);

	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glutMouseFunc(&mymouse);
	glutMotionFunc(&dragmouse);
	glutKeyboardFunc(&keyboard);

	glutMainLoop();
	return 0;
}

void draw_a_point(float x, float y, float color[])
{
	glPointSize(3.0f);
	glBegin(GL_POINTS);
	glColor3fv(color);
	glVertex2f(x, y);
	glEnd();
	glFlush();
}

int getdis(int x, int y)
{
	int ans = -1;
	float shortdis = 99999999;
	for (int i = 0; i < input_vertice.size(); i++)
	{
		float dis = sqrt(pow(x - input_vertice[i].x, 2) + pow(y - input_vertice[i].y, 2));
		if (dis < shortdis && dis <= d)
		{
			shortdis = dis;
			ans = i;
		}
	}
	return ans;
}

int index1 = -1;
void mymouse(int button, int state, int x, int y)
{
	if (OP == 'p')//���ƿ��Ƶ�
	{
		int drag_point_index = -1;
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			int index = getdis(x, window_height - y);
			if (index == -1)//��귶Χ��û�п��Ƶ㣬�¼Ӳ�����
			{
				draw_a_point(x, window_height - y, controlpoint_color);
				point p(x, window_height - y);
				input_vertice.push_back(p);
				cout << "������Ŀ��Ƶ�" << input_vertice.size() << ":(" << x << ", " << window_height - y << ")" << endl;
			}

			//���µ�����Ŀ��Ƶ㸳ֵ�����Ƶ�����
			controlpoint = input_vertice;

			if (input_vertice.size() >= 2)//���ڵ����������ʱ�򣬻�bezier���ߣ����Ƶ�����bezier������
				deCasteljau();
			else				//С���������ʱ��ֻ�������Ƶ�
			{
				glBegin(GL_LINE_STRIP);
				glColor3fv(straightline_color);
				for (int i = 0; i < input_vertice.size(); i++)
					glVertex2f(input_vertice[i].x, input_vertice[i].y);
				glEnd();
				glFlush();
			}
			//glutPostRedisplay();//
		}
	}
	if (OP == 'i')//������Ƶ�
	{
		//int index1 = -1;//�����Է�������������Ϊ����ˢ�µ����ж�upʱ��index�����-1
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			index1 = getdis(x, window_height - y);
			cout << "�������µĿ��Ƶ㵽 ѡ�еĿ��Ƶ�" << index1 << "ǰ" << endl;
		}
		if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && index1 != -1)
		{
			input_vertice.insert(input_vertice.begin() + index1, point(x, window_height - y));//���±�posǰ����һ��Ԫ��
			//cout << input_vertice.size() << endl;
		}

		//���µ�����Ŀ��Ƶ㸳ֵ�����Ƶ�����
		controlpoint = input_vertice;
		
		if (input_vertice.size() >= 2)//���ڵ����������ʱ�򣬻�bezier���ߣ����Ƶ�����bezier������
			deCasteljau();
		else				//С���������ʱ��ֻ�������Ƶ�
		{
			glBegin(GL_LINE_STRIP);
			glColor3fv(straightline_color);
			for (int i = 0; i < input_vertice.size(); i++)
				glVertex2f(input_vertice[i].x, input_vertice[i].y);
			glEnd();
			glFlush();
		}
	}

	if (OP == 'e')
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)//ɾ�����Ƶ�
		{
			int index = getdis(x, window_height - y);
			cout << "ɾ����" << index << endl;
			if (index != -1)//��귶Χ���п��Ƶ��ˣ�����ɾ��������������
			{
				vector<point>::iterator it = input_vertice.begin();
				cout << (*(it + index)).x << " " << (*(it + index)).y << endl;
				input_vertice.erase(it + index);
			}
			else
				cout << "û�п���ɾ���ĵ�" << endl;

			glClear(GL_COLOR_BUFFER_BIT);

			//���µ�����Ŀ��Ƶ㸳ֵ�����Ƶ�����
			controlpoint = input_vertice;
			//bezier����
			deCasteljau();
		}
	}
}

void dragmouse(int x, int y)
{
	int index = getdis(x, window_height - y);
	if (OP == 'd' && index != -1)//��귶Χ���п��Ƶ��ˣ������϶�
	{
		cout << "�޸Ŀ��Ƶ�" << input_vertice.size() << ":(" << input_vertice[index].x << ", " << input_vertice[index].y << ")" << endl;
		input_vertice[index].x = x;
		input_vertice[index].y = window_height - y;

		//���µ�����Ŀ��Ƶ㸳ֵ�����Ƶ�����
		controlpoint = input_vertice;

		if (input_vertice.size() >= 2)//���ڵ����������ʱ�򣬻�bezier���ߣ����Ƶ�����bezier������
			deCasteljau();
		else				//С���������ʱ��ֻ�������Ƶ�
		{
			glBegin(GL_LINE_STRIP);
			glColor3fv(straightline_color);
			for (int i = 0; i < input_vertice.size(); i++)
				glVertex2f(input_vertice[i].x, input_vertice[i].y);
			glEnd();
			glFlush();
		}
	}
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);
	if (key == 'p')//����
	{
		OP = 'p';
		cout << "��������������ʼ���ƿ��Ƶ��Լ�bezier����" << endl;
	}
	if (key == 'd')//�϶��˵�
	{
		OP = 'd';
		cout << "�밴ס�˵��϶�" << endl;
	}
	if (key == 'i')//������Ƶ�
	{
		OP = 'i';
		cout << "����һ�����еĿ��Ƶ㰴ס���ţ��ɿ�λ��Ϊ�²��붥���λ�á��²���Ķ���λ�ڵ���Ķ���֮ǰ�� "<< endl;
	}
	if (key == 'e')
	{
		OP = 'e';
		cout << "���������Ҫɾ���ĵ�" << endl;
	}
}

void deCasteljau()
{
	vector<point> bezierpoint;
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

	glClear(GL_COLOR_BUFFER_BIT);//�����Ļ����һ�ε�bezier����
	//����µ����еĿ��Ƶ�
	glBegin(GL_LINE_STRIP);
	glColor3fv(straightline_color);
	for (int i = 0; i < input_vertice.size(); i++)
		glVertex2f(input_vertice[i].x, input_vertice[i].y);
	glEnd();
	glFlush();

	//����bezier����
	glLineWidth(3.0f);
	glBegin(GL_LINE_STRIP);
	glColor3fv(bezierline_color);
	for (int i = 0; i < bezierpoint.size(); i++)
		glVertex2f(bezierpoint[i].x, bezierpoint[i].y);
	glEnd();
	glFlush();
}