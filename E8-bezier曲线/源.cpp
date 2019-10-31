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
int getdis(int x, int y);//获取离得最近的点

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("de Casteljau 绘制bezier曲线");
	cout << "键盘点击p后，点击左键绘制控制点" << endl;
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
		int drag_point_index = -1;
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

			//把新的输入的控制点赋值给控制点数组
			controlpoint = input_vertice;

			if (input_vertice.size() >= 2)//大于等于两个点的时候，画bezier曲线，控制点留给bezier函数画
				deCasteljau();
			else				//小于两个点的时候，只画出控制点
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
	if (OP == 'i')//插入控制点
	{
		//int index1 = -1;//不可以放在这里，否则会因为不断刷新导致判断up时候index恒等于-1
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			index1 = getdis(x, window_height - y);
			cout << "将插入新的控制点到 选中的控制点" << index1 << "前" << endl;
		}
		if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && index1 != -1)
		{
			input_vertice.insert(input_vertice.begin() + index1, point(x, window_height - y));//在下标pos前插入一个元素
			//cout << input_vertice.size() << endl;
		}

		//把新的输入的控制点赋值给控制点数组
		controlpoint = input_vertice;
		
		if (input_vertice.size() >= 2)//大于等于两个点的时候，画bezier曲线，控制点留给bezier函数画
			deCasteljau();
		else				//小于两个点的时候，只画出控制点
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
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)//删除控制点
		{
			int index = getdis(x, window_height - y);
			cout << "删除点" << index << endl;
			if (index != -1)//鼠标范围内有控制点了，可以删除，否则不做操作
			{
				vector<point>::iterator it = input_vertice.begin();
				cout << (*(it + index)).x << " " << (*(it + index)).y << endl;
				input_vertice.erase(it + index);
			}
			else
				cout << "没有可以删除的点" << endl;

			glClear(GL_COLOR_BUFFER_BIT);

			//把新的输入的控制点赋值给控制点数组
			controlpoint = input_vertice;
			//bezier曲线
			deCasteljau();
		}
	}
}

void dragmouse(int x, int y)
{
	int index = getdis(x, window_height - y);
	if (OP == 'd' && index != -1)//鼠标范围内有控制点了，可以拖动
	{
		cout << "修改控制点" << input_vertice.size() << ":(" << input_vertice[index].x << ", " << input_vertice[index].y << ")" << endl;
		input_vertice[index].x = x;
		input_vertice[index].y = window_height - y;

		//把新的输入的控制点赋值给控制点数组
		controlpoint = input_vertice;

		if (input_vertice.size() >= 2)//大于等于两个点的时候，画bezier曲线，控制点留给bezier函数画
			deCasteljau();
		else				//小于两个点的时候，只画出控制点
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
	if (key == 'p')//画点
	{
		OP = 'p';
		cout << "请点击鼠标左键，开始绘制控制点以及bezier曲线" << endl;
	}
	if (key == 'd')//拖动端点
	{
		OP = 'd';
		cout << "请按住端点拖动" << endl;
	}
	if (key == 'i')//插入控制点
	{
		OP = 'i';
		cout << "请点击一个已有的控制点按住不放，松开位置为新插入顶点的位置。新插入的顶点位于点击的顶点之前。 "<< endl;
	}
	if (key == 'e')
	{
		OP = 'e';
		cout << "鼠标左键点击要删除的点" << endl;
	}
}

void deCasteljau()
{
	vector<point> bezierpoint;
	int n = controlpoint.size();

	for (double t = 0.0; t <= 1.0; t += 0.005 / n)//保证点的数量足够多，使曲线更加圆滑
	{
		for (int i = 1; i < n; i++)//控制每走一轮，控制点都会少 1
		{
			for (int j = 0; j < n - i; j++)//每走一轮，控制点都会少 1
			{
				if (i == 1)//迭代第一轮必须是输入的那几个控制点，so，，不可以只用一个controlpoint，input_vertice也有用。。。。
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
		//cout << "控制点数量=" << controlpoint.size() << endl;一直是点的数目，因为之前直接存在这个里面了，但是有用的只有下标为1的才有用
		bezierpoint.push_back(controlpoint[0]);//把最后一个在bezier曲线上的点保存起来，最后链接在一起即为bezier曲线
	}

	glClear(GL_COLOR_BUFFER_BIT);//清空屏幕上上一次的bezier曲线
	//输出新的所有的控制点
	glBegin(GL_LINE_STRIP);
	glColor3fv(straightline_color);
	for (int i = 0; i < input_vertice.size(); i++)
		glVertex2f(input_vertice[i].x, input_vertice[i].y);
	glEnd();
	glFlush();

	//画出bezier曲线
	glLineWidth(3.0f);
	glBegin(GL_LINE_STRIP);
	glColor3fv(bezierline_color);
	for (int i = 0; i < bezierpoint.size(); i++)
		glVertex2f(bezierpoint[i].x, bezierpoint[i].y);
	glEnd();
	glFlush();
}