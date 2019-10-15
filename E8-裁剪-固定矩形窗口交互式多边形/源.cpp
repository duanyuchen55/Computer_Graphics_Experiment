#include<gl/glut.h>
#include<iostream>
#include<algorithm>
#include<vector>
#include<time.h>
using namespace std;
const int window_width = 800, window_height = 600;
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
//void output(point pt)
//{
//	//draw_a_point(pt.x, pt.y, intersect_point_color);
//	output_vertice.push_back(pt);
//	//cout << "output_vertice" << output_vertice.size() << ": (" << pt.x << ", " << pt.y << ")" << endl;
//}
void mymouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);

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

void SutherlandHodgmanClip(EDGE ClipBoundary)
{
	point s, p, ip;
	output_vertice.clear();
	s = input_vertice[input_vertice.size() - 1];//首尾

	for (int j = 0; j < input_vertice.size(); j++)
	{
		p = input_vertice[j];
		if (inside(p, ClipBoundary))//s在内
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
		for (int i = 0; i < output_vertice.size(); i++)
			//draw_a_point(output_vertice[i].x, output_vertice[i].y, intersect_point_color);
			glVertex2f(output_vertice[i].x, output_vertice[i].y);

		glEnd();
		glFlush();
	}
}
