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
int getdis(int x, int y);//获取离得最近的点


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("de Boor 绘制B样条曲线");
	cout << "点击键盘数字2-4切换阶数" << endl;
	cout << "不做操作则默认k=3，如需输入其他阶数，请按k后输入阶数" << endl;
	cout << "键盘点击p后，点击鼠标左键绘制控制点" << endl;
	cout << "键盘点击i后，插入控制点" << endl;
	cout << "键盘点击d后，移动控制点" << endl;
	cout << "键盘点击e后，点击鼠标左键删除控制点" << endl;
	cout << "按ESC退出" << endl << endl;

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
	if (OP == 'p')//绘制控制点
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			int index = getdis(x, window_height - y);
			if (index == -1)//鼠标范围内没有控制点，新加操作点
			{
				draw_a_point(x, window_height - y, controlpoint_color);
				point p(x, window_height - y);
				input_vertice.push_back(p);
				cout << "新输入的控制点" << input_vertice.size() << ":(" << x << ", " << window_height - y << ")" << endl;
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
	if (OP == 'i')//插入控制点
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			index1 = getdis(x, window_height - y);
			cout << "将插入新的控制点到 选中的控制点" << index1 << "前" << endl;
		}
		if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && index1 != -1)
		{
			input_vertice.insert(input_vertice.begin() + index1, point(x, window_height - y));//在下标pos前插入一个元素
			//deBoor();
		}
		control_point = input_vertice;
		deBoor();
	}
	if (OP == 'e')
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)//删除控制点
		{
			int index = getdis(x, window_height - y);
			cout << "删除点" << index << " ";
			if (index != -1)//鼠标范围内有控制点了，可以删除，否则不做操作
			{
				vector<point>::iterator it = input_vertice.begin();
				cout << (*(it + index)).x << " " << (*(it + index)).y << endl;
				input_vertice.erase(it + index);
			}
			else
				cout << "没有可以删除的点" << endl;

			control_point = input_vertice;
			deBoor();
		}
	}
}

void dragmouse(int x, int y)
{
	int index = getdis(x, window_height - y);
	if (OP == 'd' && index != -1)//鼠标范围内有控制点了，可以拖动
	{
		//cout << "修改控制点" << input_vertice.size() << ":(" << input_vertice[index].x << ", " << input_vertice[index].y << ")" << endl;
		input_vertice[index].x = x;
		input_vertice[index].y = window_height - y;

		control_point = input_vertice;

		//deBoor();
		glClear(GL_COLOR_BUFFER_BIT);
		//绘制控制点
		for (int i = 0; i < input_vertice.size(); i++)
			draw_a_point(input_vertice[i].x, input_vertice[i].y, controlpoint_color);
		//绘制多边形
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
	if (key == 'p')//画点
	{
		OP = 'p';
		cout << "当前阶数为" << k << endl;
		cout << "请点击鼠标左键，开始绘制控制点以及B样条曲线" << endl;
	}
	if (key == 'd')//拖动端点
	{
		OP = 'd';
		cout << "请按住端点拖动" << endl;
	}
	if (key == 'i')//插入控制点
	{
		OP = 'i';
		cout << "请点击一个已有的控制点按住不放，松开位置为新插入顶点的位置。新插入的顶点位于点击的顶点之前。 " << endl;
	}
	if (key == 'e')
	{
		OP = 'e';
		cout << "鼠标左键点击要删除的点" << endl;
	}
	if (key == '2')
	{
		k = 2;
		cout << "修改阶数为2，重新绘制B样条曲线" << endl;
		deBoor();
	}
	if (key == '3')
	{
		k = 3;
		cout << "修改阶数为3，重新绘制B样条曲线" << endl;
		deBoor();
	}
	if (key == '4')
	{
		k = 4;
		cout << "修改阶数为4，重新绘制B样条曲线" << endl;
		deBoor();
	}
	if (key == 'k')
	{
		cout << "修改阶数为：" << endl;
		cin >> k;
		cout << "重新绘制B样条曲线" << endl;
		deBoor();
	}
}

void deBoor()
{
	vector<point> b_spline;
	int n = control_point.size() - 1;
	//cout <<"control_point_size-1=" << n << endl;

	float t[maxn];
	//均匀B样条
	/*for (int i = 0; i <= input_vertice.size() + k; i++)
		t[i] = i + 1;*/
	//准均匀B样条
	t[0] = 0; 
	for (int i = 0; i <= k - 1; i++)
		t[i] = 0;
	for (int i = k; i < n + 1; i++)
		t[i] = t[i - 1] + 1.0 / (n + 1 - k + 1);
	for (int i = n + 1; i <= n + k; i++)
		t[i] = 1;
	
	for (int j = k - 1; j <= n; j++)//j为区间的末端点
	{
		for (double u = t[j]; u <= t[j + 1]; u += 0.001 / n)
		{
			for (int r = 1; r <= k - 1; r++)//递推k-1层
			{
				for (int i = j; i >= j - k + r + 1; i--)//由递推公式得，需要倒着来，i和i-1存在i中；；bezier为j和j+1存在j中
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

					if (r == 1)//第一轮必须是输入的那几个控制点
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
			b_spline.push_back(control_point[j]);//递推的最后一层的点，即为求得的点
		}
	}
	//cout << "BSpline size = :" << b_spline.size() << endl;

	glClear(GL_COLOR_BUFFER_BIT);//清空屏幕上上一次的曲线
	//绘制控制点
	for (int i = 0; i < input_vertice.size(); i++)
		draw_a_point(input_vertice[i].x, input_vertice[i].y, controlpoint_color);
	//绘制控制多边形
	glBegin(GL_LINE_STRIP);
	glColor3fv(straightline_color);
	for (int i = 0; i < input_vertice.size(); i++)
		glVertex2f(input_vertice[i].x, input_vertice[i].y);
	glEnd();
	glFlush();
	//画出b_spline曲线
	glLineWidth(3.0f);
	glBegin(GL_LINE_STRIP);
	glColor3fv(Bspline_color);
	for (int i = 0; i < b_spline.size(); i++)
		glVertex2f(b_spline[i].x, b_spline[i].y);
	glEnd();
	glFlush();
}