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
bool inside(point& pt, EDGE ClipBoundary);//�жϵ��Ƿ�ɼ�
void intersect(point& s, point& p, EDGE ClipBoundary, point& intersect_pt);//ֱ�߶�SP�ͱ߽��󽻣����ؽ���
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
	glutCreateWindow("�̶����δ��ڲü�����ʽ�����");
	cout << "�������\n�Ҽ����ӳɴ���\n���ո�ü�\n" << endl;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, window_width, 0, window_height);

	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	//�����߽�
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

bool inside(point& pt, EDGE ClipBoundary)//�жϵ��Ƿ�ɼ�
{
	if (ClipBoundary.ex > ClipBoundary.bx)
	{
		if (pt.y >= ClipBoundary.by)//�ü���Ϊ�����±���
			return true;
	}
	else if (ClipBoundary.ex < ClipBoundary.bx)
	{
		if (pt.y <= ClipBoundary.by)//�ü���Ϊ�����ϱ���
			return true;
	}
	else if (ClipBoundary.ey > ClipBoundary.by)//�ü���Ϊ�����ұ���
	{
		if (pt.x <= ClipBoundary.bx)
			return true;
	}
	else if (ClipBoundary.ey < ClipBoundary.by)//�ü���Ϊ���������
	{
		if (pt.x >= ClipBoundary.bx)
			return true;
	}
	return false;
}

void intersect(point& s, point& p, EDGE ClipBoundary, point& intersect_pt)//ֱ�߶�SP�ͱ߽��󽻣����ؽ���
{
	if (ClipBoundary.by == ClipBoundary.ey)//ˮƽ�ü��߽�
	{
		intersect_pt.y = ClipBoundary.by;
		//x=���ĺ�����+dy/spб��
		intersect_pt.x = s.x + (ClipBoundary.by - s.y) * (p.x - s.x) / (p.y - s.y);
	}
	else//��ֱ�ü��߽�
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
	int flag = 0;//��ʾ��ǰб��k�ľ���ֵ�Ƿ����1
	if (dx == 0 && dy == 0)
		return;

	if (dy > dx)//б�ʾ���ֵ����1
	{
		flag = 1;
		//���������ụ������x����y����y����x���������궼��Ҫ����
		swap(x1, y1);
		swap(x2, y2);
		swap(dx, dy);
	}

	//ȷ������tx��ty��б��Ϊ����Ϊ������ֻ������/����or��/���£���/�ұߵ�������Ӳ�������
	int tx = x2 > x1 ? 1 : -1;
	int ty = y2 > y1 ? 1 : -1;

	//ȷ����һ���������x��y
	int x = x1 + 1;
	int y = y1;

	//dt��ds�ɵ�����ʽ�Ƴ���dt�����ϵĵ㣬ds���ұߵĵ�
	int dt = 2 * (dy - dx);
	int ds = 2 * dy;

	//�б�ʽ��ֵd=2k*dx-dx = 2*dy-dx
	int d = 2 * dy - dx;

	while (x != x2)
	{
		if (d >= 0)//ѡT�㣨���ϵĵ�
		{
			//d(i+1) = d(i) + 2(dy - dx)
			d += dt;
			y += ty;
		}
		else//ѡS�㣬y���򲻼Ӳ���
		{
			//d(i+1) = d(i) + 2*dy
			d += ds;
		}

		if (flag)//б�ʴ���1
		{
			draw_line_point(y, x);
		}
		else
		{
			draw_line_point(x, y);
		}
		x += tx;//x���Ӳ���
	}
}

void SutherlandHodgmanClip(EDGE ClipBoundary)
{
	point s, p, ip;
	output_vertice.clear();
	s = input_vertice[input_vertice.size() - 1];//��β

	for (int j = 0; j < input_vertice.size(); j++)
	{
		p = input_vertice[j];
		if (inside(p, ClipBoundary))//p����
		{
			if (inside(s, ClipBoundary))//sp���ڴ�����
			{
				//output(p);
				output_vertice.push_back(p);
			}
			else//p������ s����
			{
				intersect(s, p, ClipBoundary, ip);
				//output(ip);
				//output(p); 
				output_vertice.push_back(ip);
				output_vertice.push_back(p);

			}
		}
		else//s������
		{
			if (inside(s, ClipBoundary))//s�ڴ�����p�ڴ�����
			{
				intersect(s, p, ClipBoundary, ip);

				//output(ip);
				output_vertice.push_back(ip);
			}
			//sp���������������
		}
		s = p;
	}
	input_vertice = output_vertice;//��ε������Ϊ��һ�ε����룬input_vertice��output_vertice��ȫ�ֱ���
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
		cout << "����ζ���" << input_vertice.size() << ":(" << x << ", " << window_height - y << ")" << endl;
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