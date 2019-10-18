#include<gl/glut.h>
#include<iostream>
#include<algorithm>
#include<vector>
#include<map>
#include<time.h>
using namespace std;
const int window_width = 800, window_height = 600;
struct point
{
	float x, y;
	point() {}
	point(float xx, float yy)
		:x(xx), y(yy) {}

	bool operator < (const point& a)const
	{
		return x < a.x;
	}
};
struct EDGE//Edge
{
	float bx, by, ex, ey;
	EDGE() {}
	EDGE(float bxx, float byy, float exx, float eyy)
		:bx(bxx), by(byy), ex(exx), ey(eyy) {}
};
map<float, map<float, int> > vis;
vector<point> input_vertice;
vector<point> output_vertice;
float intersect_point_color[3] = { 1,0,0 };
float polygon_point_color[3] = { 0,0,1 };
EDGE left(200, 450, 200, 200);
EDGE bottom(200, 200, 600, 200);
EDGE right(600, 200, 600, 450);
EDGE top(600, 450, 200, 450);

void draw_a_point(float x, float y, float color[]);
bool inside(point& pt, EDGE ClipBoundary);//判断点是否可见
void intersect(point& s, point& p, EDGE ClipBoundary, point& intersect_pt);//直线段SP和边界求交，返回交点
void SutherlandHodgmanClip(EDGE ClipBoundary, int in_len, int& out_len);
void mymouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);
void draw_line_point(float x, float y);
void bresenham1(GLint x1, GLint y1, GLint x2, GLint y2);

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RED);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("固定矩形窗口裁剪交互式多边形");
	cout << "左键画点\n右键连接成窗口\n按空格裁剪\n" << endl;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, window_width, 0, window_height);

	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	//画出边界
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
	glutKeyboardFunc(&keyboard);

	glutMainLoop();
	return 0;
}

bool inside(point& pt, EDGE ClipBoundary)//判断点是否可见
{
	if (ClipBoundary.ex > ClipBoundary.bx)
	{
		if (pt.y >= ClipBoundary.by)//裁剪边为窗口下边沿
			return true;
	}
	else if (ClipBoundary.ex < ClipBoundary.bx)
	{
		if (pt.y <= ClipBoundary.by)//裁剪边为窗口上边沿
			return true;
	}
	else if (ClipBoundary.ey > ClipBoundary.by)//裁剪边为窗口右边沿
	{
		if (pt.x <= ClipBoundary.bx)
			return true;
	}
	else if (ClipBoundary.ey < ClipBoundary.by)//裁剪边为窗口左边沿
	{
		if (pt.x >= ClipBoundary.bx)
			return true;
	}
	return false;
}

void intersect(point& s, point& p, EDGE ClipBoundary, point& intersect_pt)//直线段SP和边界求交，返回交点
{
	if (ClipBoundary.by == ClipBoundary.ey)//水平裁剪边界
	{
		intersect_pt.y = ClipBoundary.by;
		//x=起点的横坐标+dy/sp斜率
		intersect_pt.x = s.x + (ClipBoundary.by - s.y) * (p.x - s.x) / (p.y - s.y);
	}
	else//垂直裁剪边界
	{
		intersect_pt.x = ClipBoundary.bx;
		intersect_pt.y = s.y + (ClipBoundary.bx - s.x) * (p.y - s.y) / (p.x - s.x);
	}
}

void bresenham1(GLint x1, GLint y1, GLint x2, GLint y2)
{
	//glClear(GL_COLOR_BUFFER_BIT);

	draw_line_point(x1, y1);

	int dx = abs(x1 - x2);
	int dy = abs(y1 - y2);
	int flag = 0;//表示当前斜率k的绝对值是否大于1
	if (dx == 0 && dy == 0)
		return;

	if (dy > dx)//斜率绝对值大于1
	{
		flag = 1;
		//横纵坐标轴互换，将x视作y，将y视作x，所有坐标都需要互换
		swap(x1, y1);
		swap(x2, y2);
		swap(dx, dy);
	}

	//确定步长tx，ty，斜率为正或为负，且只考虑左/右上or左/右下，左/右边的情况不加步长即可
	int tx = x2 > x1 ? 1 : -1;
	int ty = y2 > y1 ? 1 : -1;

	//确定下一个点的坐标x，y
	int x = x1 + 1;
	int y = y1;

	//dt和ds由迭代公式推出。dt是右上的点，ds是右边的点
	int dt = 2 * (dy - dx);
	int ds = 2 * dy;

	//判别式的值d=2k*dx-dx = 2*dy-dx
	int d = 2 * dy - dx;

	while (x != x2)
	{
		if (d >= 0)//选T点（右上的点
		{
			//d(i+1) = d(i) + 2(dy - dx)
			d += dt;
			y += ty;
		}
		else//选S点，y方向不加步长
		{
			//d(i+1) = d(i) + 2*dy
			d += ds;
		}

		if (flag)//斜率大于1
		{
			draw_line_point(y, x);
		}
		else
		{
			draw_line_point(x, y);
		}
		x += tx;//x增加步长
	}
}

void SutherlandHodgmanClip(EDGE ClipBoundary)
{
	point s, p, ip;
	output_vertice.clear();
	s = input_vertice[input_vertice.size() - 1];//首尾

	for (int j = 0; j < input_vertice.size(); j++)
	{
		p = input_vertice[j];
		if (inside(p, ClipBoundary))//p在内
		{
			if (inside(s, ClipBoundary))//sp都在窗口内
			{
				//output(p);
				output_vertice.push_back(p);
			}
			else//p在里面 s不在
			{
				intersect(s, p, ClipBoundary, ip);
				//output(ip);
				//output(p); 
				output_vertice.push_back(ip);
				output_vertice.push_back(p);

			}
		}
		else//s在外面
		{
			if (inside(s, ClipBoundary))//s在窗口内p在窗口外
			{
				intersect(s, p, ClipBoundary, ip);

				//output(ip);
				output_vertice.push_back(ip);
			}
			//sp都在外面则无输出
		}
		s = p;
	}
	input_vertice = output_vertice;//这次的输出作为下一次的输入，input_vertice和output_vertice是全局变量
}

void draw_a_point(float x, float y, float color[])
{
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glColor3fv(color);
	glVertex2f(x, y);
	glEnd();
	glFlush();
}

void draw_line_point(float x, float y)
{
	vis[x][y]++;
	glPointSize(1.5f);
	glBegin(GL_POINTS);
	if (vis[x][y] % 2 == 0)
	{
		glColor3f(1, 1, 1);
		glVertex2f(x, y);
	}
	else
	{
		glColor3f(1, 0, 0);
		glVertex2f(x, y);
	}

	glEnd();
	glFlush();
}

void mymouse(int button, int state, int x, int y)
{
	glClearColor(1, 1, 1, 1);
	//bool flag = 0;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		draw_a_point(x, window_height - y, polygon_point_color);
		point p(x, window_height - y);
		input_vertice.push_back(p);
		cout << "多边形顶点" << input_vertice.size() << ":(" << x << ", " << window_height - y << ")" << endl;
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		glLineWidth(2.0f);
		glBegin(GL_LINES);
		glColor3fv(polygon_point_color);
		for (int i = 0; i < input_vertice.size(); i++)
		{
			if (i == input_vertice.size() - 1)
			{
				glVertex2f(input_vertice[0].x, input_vertice[0].y);
				glVertex2f(input_vertice[i].x, input_vertice[i].y);
			}
			else
			{
				glVertex2f(input_vertice[i].x, input_vertice[i].y);
				glVertex2f(input_vertice[i + 1].x, input_vertice[i + 1].y);
			}
		}
		glEnd();
		glFlush();
	}
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == 32)
	{
		SutherlandHodgmanClip(::left);
		SutherlandHodgmanClip(bottom);
		SutherlandHodgmanClip(::right);
		SutherlandHodgmanClip(top);

		glLineWidth(4.0f);
		glBegin(GL_LINE_LOOP);
		glColor3fv(intersect_point_color);
		for (int i = 0; i < output_vertice.size() - 1; i++)
		{
			bresenham1(output_vertice[i].x, output_vertice[i].y, output_vertice[i + 1].x, output_vertice[i + 1].y);
		}
		bresenham1(output_vertice[0].x, output_vertice[0].y, output_vertice[output_vertice.size() - 1].x, output_vertice[output_vertice.size() - 1].y);

		glEnd();
		glFlush();

	}
}