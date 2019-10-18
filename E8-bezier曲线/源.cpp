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
	glutCreateWindow("bezier曲线");
	cout << "左键画控制点\n右键画bezier曲线" << endl;

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
		cout << "输入的控制点" << input_vertice.size() << ":(" << x << ", " << window_height - y << ")" << endl;

		//即时输出已有的控制点
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

	//画出bezier曲线
	glLineWidth(3.0f);
	glBegin(GL_LINE_STRIP);
	glColor3fv(bezierline_color);
	for (int i = 0; i < bezierpoint.size(); i++)
		glVertex2f(bezierpoint[i].x, bezierpoint[i].y);
	glEnd();
	glFlush();
}