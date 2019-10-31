#include<gl/glut.h>
#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;
const float window_width = 800, window_height = 600;
const int d = 8;
const int maxn = 99999;

struct point
{
	float x, y;
	point() {}
	point(float xx, float yy)
		:x(xx), y(yy) {}
};
vector<point> input_vertice;
vector<point> control_point;

char OP;
int k = 3;
float controlpoint_color[3] = { 1,0,0 };
float straightline_color[3] = { 0,1,0 };
float Bspline_color[3] = { 0,0,1 };

float N[5];


void draw_a_point(float x, float y, float color[]);
void deBoor();
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
	glutCreateWindow("de Boor ����B��������");
	cout << "�����������2-4�л�����" << endl;
	cout << "����������Ĭ��k=3���������������������밴k���������" << endl;
	cout << "���̵��p�󣬵�����������ƿ��Ƶ�" << endl;
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

			/*glBegin(GL_LINE_STRIP);
			glColor3fv(straightline_color);
			for (int i = 0; i < input_vertice.size(); i++)
				glVertex2f(input_vertice[i].x, input_vertice[i].y);
			glEnd();
			glFlush();*/
			control_point = input_vertice;
			deBoor();
		}
		
	}
	if (OP == 'i')//������Ƶ�
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			index1 = getdis(x, window_height - y);
			cout << "�������µĿ��Ƶ㵽 ѡ�еĿ��Ƶ�" << index1 << "ǰ" << endl;
		}
		if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && index1 != -1)
		{
			input_vertice.insert(input_vertice.begin() + index1, point(x, window_height - y));//���±�posǰ����һ��Ԫ��
			//deBoor();
		}
		control_point = input_vertice;
		deBoor();
	}
	if (OP == 'e')
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)//ɾ�����Ƶ�
		{
			int index = getdis(x, window_height - y);
			cout << "ɾ����" << index << " ";
			if (index != -1)//��귶Χ���п��Ƶ��ˣ�����ɾ��������������
			{
				vector<point>::iterator it = input_vertice.begin();
				cout << (*(it + index)).x << " " << (*(it + index)).y << endl;
				input_vertice.erase(it + index);
			}
			else
				cout << "û�п���ɾ���ĵ�" << endl;

			control_point = input_vertice;
			deBoor();
		}
	}
}

void dragmouse(int x, int y)
{
	int index = getdis(x, window_height - y);
	if (OP == 'd' && index != -1)//��귶Χ���п��Ƶ��ˣ������϶�
	{
		//cout << "�޸Ŀ��Ƶ�" << input_vertice.size() << ":(" << input_vertice[index].x << ", " << input_vertice[index].y << ")" << endl;
		input_vertice[index].x = x;
		input_vertice[index].y = window_height - y;

		control_point = input_vertice;

		//deBoor();
		glClear(GL_COLOR_BUFFER_BIT);
		//���ƿ��Ƶ�
		for (int i = 0; i < input_vertice.size(); i++)
			draw_a_point(input_vertice[i].x, input_vertice[i].y, controlpoint_color);
		//���ƶ����
		glLineWidth(2.0f);
		glBegin(GL_LINE_STRIP);
		glColor3fv(straightline_color);
		for (int i = 0; i < input_vertice.size(); i++)
			glVertex2f(input_vertice[i].x, input_vertice[i].y);
		glEnd();
		glFlush();

		deBoor();
	}
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);
	if (key == 'p')//����
	{
		OP = 'p';
		cout << "��ǰ����Ϊ" << k << endl;
		cout << "��������������ʼ���ƿ��Ƶ��Լ�B��������" << endl;
	}
	if (key == 'd')//�϶��˵�
	{
		OP = 'd';
		cout << "�밴ס�˵��϶�" << endl;
	}
	if (key == 'i')//������Ƶ�
	{
		OP = 'i';
		cout << "����һ�����еĿ��Ƶ㰴ס���ţ��ɿ�λ��Ϊ�²��붥���λ�á��²���Ķ���λ�ڵ���Ķ���֮ǰ�� " << endl;
	}
	if (key == 'e')
	{
		OP = 'e';
		cout << "���������Ҫɾ���ĵ�" << endl;
	}
	if (key == '2')
	{
		k = 2;
		cout << "�޸Ľ���Ϊ2�����»���B��������" << endl;
		deBoor();
	}
	if (key == '3')
	{
		k = 3;
		cout << "�޸Ľ���Ϊ3�����»���B��������" << endl;
		deBoor();
	}
	if (key == '4')
	{
		k = 4;
		cout << "�޸Ľ���Ϊ4�����»���B��������" << endl;
		deBoor();
	}
	if (key == 'k')
	{
		cout << "�޸Ľ���Ϊ��" << endl;
		cin >> k;
		cout << "���»���B��������" << endl;
		deBoor();
	}
}

void deBoor()
{
	vector<point> b_spline;
	int n = control_point.size() - 1;
	//cout <<"control_point_size-1=" << n << endl;

	float t[maxn];
	//����B����
	/*for (int i = 0; i <= input_vertice.size() + k; i++)
		t[i] = i + 1;*/
	//׼����B����
	t[0] = 0; 
	for (int i = 0; i <= k - 1; i++)
		t[i] = 0;
	for (int i = k; i < n + 1; i++)
		t[i] = t[i - 1] + 1.0 / (n + 1 - k + 1);
	for (int i = n + 1; i <= n + k; i++)
		t[i] = 1;
	
	for (int j = k - 1; j <= n; j++)//jΪ�����ĩ�˵�
	{
		for (double u = t[j]; u <= t[j + 1]; u += 0.001 / n)
		{
			for (int r = 1; r <= k - 1; r++)//����k-1��
			{
				for (int i = j; i >= j - k + r + 1; i--)//�ɵ��ƹ�ʽ�ã���Ҫ��������i��i-1����i�У���bezierΪj��j+1����j��
				{
					float x1 = u - t[i];
					float x2 = t[i + k - r] - t[i];
					float y1 = t[i + k - r] - u;
					float y2 = t[i + k - r] - t[i];

					float coefficient1, coefficient2;
					if (x1 == 0.0 && x2 == 0.0)
						coefficient1 = 0;
					else
						coefficient1 = x1 / x2;
					if (y1 == 0.0 && y2 == 0.0)
						coefficient2 = 0;
					else
						coefficient2 = y1 / y2;

					if (r == 1)//��һ�ֱ�����������Ǽ������Ƶ�
					{
						control_point[i].x = input_vertice[i].x * coefficient1 + input_vertice[i - 1].x * coefficient2;
						control_point[i].y = input_vertice[i].y * coefficient1 + input_vertice[i - 1].y * coefficient2;
						continue;
					}
					else
					{
						control_point[i].x = control_point[i].x * coefficient1 + control_point[i - 1].x * coefficient2;
						control_point[i].y = control_point[i].y * coefficient1 + control_point[i - 1].y * coefficient2;
					}
				}
			}
			b_spline.push_back(control_point[j]);//���Ƶ����һ��ĵ㣬��Ϊ��õĵ�
		}
	}
	//cout << "BSpline size = :" << b_spline.size() << endl;

	glClear(GL_COLOR_BUFFER_BIT);//�����Ļ����һ�ε�����
	//���ƿ��Ƶ�
	for (int i = 0; i < input_vertice.size(); i++)
		draw_a_point(input_vertice[i].x, input_vertice[i].y, controlpoint_color);
	//���ƿ��ƶ����
	glBegin(GL_LINE_STRIP);
	glColor3fv(straightline_color);
	for (int i = 0; i < input_vertice.size(); i++)
		glVertex2f(input_vertice[i].x, input_vertice[i].y);
	glEnd();
	glFlush();
	//����b_spline����
	glLineWidth(3.0f);
	glBegin(GL_LINE_STRIP);
	glColor3fv(Bspline_color);
	for (int i = 0; i < b_spline.size(); i++)
		glVertex2f(b_spline[i].x, b_spline[i].y);
	glEnd();
	glFlush();
}