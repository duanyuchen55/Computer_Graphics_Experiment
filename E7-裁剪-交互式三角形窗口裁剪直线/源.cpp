#include<gl/glut.h>
#include<iostream>
#include<algorithm>
#include<vector>
#define LEFT 1
#define RIGHT 2
#define BOTTOM 4
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
struct straightLine
{
	float bx, by, ex, ey;
	straightLine(float bxx, float byy, float exx, float eyy)
		:bx(bxx), by(byy), ex(exx), ey(eyy) {}
};
vector<point> vertice;
vector<straightLine> line;//����ֱ��
vector<straightLine> edge;//�����εı�
float bx, by, ex, ey;
float triangle_color[3] = { 1,0,0 };
float straightLine_color[3] = { 0,0,1 };
float remainingLine_color[3] = { 0,1,0 };

float judge(float x, float y, int c);
int encode(float x, float y);
point get_point(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);
void draw_triangle_point(float x, float y, float color[]);
void draw_straightLine_point(float x, float y, float color[]);
void draw_remaining_line(float x1, float y1, float x2, float y2, float color[]);
void CS_LineClip(float x1, float y1, float x2, float y2);

int getdis(int x, int y)
{
	int ans = -1;
	float shortdis = 99999999;
	for (int i = 0; i < line.size(); i++)
	{
		float dis = sqrt(pow(x - line[i].bx, 2) + pow(y - line[i].by, 2));
		if (dis < shortdis && dis <= d)
		{
			shortdis = dis;
			ans = i;
		}

		dis = sqrt(pow(x - line[i].ex, 2) + pow(y - line[i].ey, 2));
		if (dis < shortdis && dis <= d)
		{
			shortdis = dis;
			ans = i;
		}
	}
	return ans;
}

void drag_mouse(int x, int y);
void mymouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);




int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("����ʽ�����βü�ֱ��");
	cout << "�������\n�Ҽ����ӳɴ���\n����м��϶�����\n���ո�ü�\n" << endl;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, window_width, 0, window_height);

	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glutMouseFunc(&mymouse);
	glutMotionFunc(&drag_mouse);
	glutKeyboardFunc(&keyboard);

	glutMainLoop();
	return 0;
}

//������ѧ���������У���������Ϊ����������ӣ�Ȩ�طֱ�Ϊa��b
//��a+b=1����������ڶԱ���
//��a+b<1������������������ڲ�
//��a+b>1������������������ⲿ
float judge(float x, float y, int c)//�жϵ�(x,y)�� c�Ŷ���ĶԱ� ��λ�ù�ϵ
{
	float x1 = vertice[(c + 1) % 3].x - vertice[c].x;
	float y1 = vertice[(c + 1) % 3].y - vertice[c].y;
	float x2 = vertice[(c + 2) % 3].x - vertice[c].x;
	float y2 = vertice[(c + 2) % 3].y - vertice[c].y;
	float a = ((x - vertice[c].x) * y2 - (y - vertice[c].y) * x2) * 1.0 / (x1 * y2 - x2 * y1);
	float b = ((x - vertice[c].x) * y1 - (y - vertice[c].y) * x1) * 1.0 / (x2 * y1 - x1 * y2);
	return a + b;
}
int encode(float x, float y)
{
	int c = 0;
	/*if (judge(x, y, 0) > 1.0000005)c |= RIGHT;
	if (judge(x, y, 1) > 1.0000005)c |= LEFT;
	if (judge(x, y, 2) > 1.0000005)c |= BOTTOM;*/
	//���������0�Ŷ��㣬���������1�Ŷ��㣬���������2�Ŷ���
	if (judge(x, y, 0) > 1.0000005)c |= BOTTOM;
	if (judge(x, y, 1) > 1.0000005)c |= RIGHT;
	if (judge(x, y, 2) > 1.0000005)c |= LEFT;
	return c;
}

//�ⷽ�� ����ֱ��a��ֱ��b�Ľ���
point get_point(float a_bx, float a_by, float a_ex, float a_ey, float b_bx, float b_by, float b_ex, float b_ey)
{
	float a, b, c, d, e, f;
	a = a_ey - a_by;
	b = a_ex * a_by - a_bx * a_ey;
	c = a_ex - a_bx;
	d = b_ey - b_by;
	e = b_ex * b_by - b_bx * b_ey;
	f = b_ex - b_bx;
	float x, y;
	y = (a * e - b * d) * 1.0 / (a * f - c * d);
	x = (y * c - b) * 1.0 / a;
	point p = point(x, y);
	return p;
}

void draw_triangle_point(float x, float y, float color[])
{
	glPointSize(3.0f);
	glBegin(GL_POINTS);
	glColor3fv(color);
	glVertex2f(x, y);
	glEnd();
	glFlush();
}
void draw_straightLine_point(float x, float y, float color[])
{
	glPointSize(0.5f);
	glBegin(GL_POINTS);
	glColor3fv(color);
	glVertex2f(x, y);
	glEnd();
	glFlush();
}
void draw_remaining_line(float x1, float y1, float x2, float y2, float color[])
{
	glLineWidth(3.0f);
	glBegin(GL_LINES);
	glColor3fv(color);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
	glFlush();
}

void CS_LineClip(float x1, float y1, float x2, float y2)
{
	int code1, code2, code;
	code1 = encode(x1, y1);
	code2 = encode(x2, y2);
	while (code1 != 0 || code2 != 0)//ѭ��ֱ�� ֱ�ߵ������㶼ǡ�����ڲ�
	{
		if ((code1 & code2) != 0)//�����㶼������
			return;
		if (code1 != 0)
			code = code1;
		else 
			code = code2;
		point pt;
		if ((LEFT & code) != 0)//������ߵ���ߣ��󽻵�
		{
			pt = get_point(vertice[0].x, vertice[0].y, vertice[1].x, vertice[1].y, x1, y1, x2, y2);
		}
		else if ((RIGHT & code) != 0)//���ұ��ߵ���ߣ��󽻵�
		{
			pt = get_point(vertice[0].x, vertice[0].y, vertice[2].x, vertice[2].y, x1, y1, x2, y2);
		}
		else if ((BOTTOM & code) != 0)//���±��ߵ���ߣ��󽻵�
		{
			pt = get_point(vertice[1].x, vertice[1].y, vertice[2].x, vertice[2].y, x1, y1, x2, y2);
		}

		if (code == code1)
		{
			x1 = pt.x;
			y1 = pt.y;
			code1 = encode(x1, y1);
		}
		else
		{
			x2 = pt.x; 
			y2 = pt.y;
			code2 = encode(x2, y2);
		}
		cout << "code1=" << code1 << " code2=" << code2 << endl;
	}
	cout << "x1=" << x1 << " y1=" << y1 << " x2=" << x2 << " y2=" << y2 << endl;
	draw_remaining_line(x1, y1, x2, y2, remainingLine_color);
}
void drag_mouse(int x, int y)
{
	//int index = getdis(x, y);
	//if (index == -1)//û�е㣬������ͨ��ֱ��
	//{
		if (bx && by)//������ǰ���㣬���¶໭һ��ֱ��
		{
			ex = x;
			ey = window_height - y;
			//printf("ֱ�ߴ������꣺���(%d, %d) �յ�(%d, %d)\n", bx, by, ex, ey);//�����������ӡ����
			glClear(GL_COLOR_BUFFER_BIT);//�����������
			glBegin(GL_LINES);
			glColor3fv(straightLine_color);
			glVertex2f(bx, by);
			glVertex2f(ex, ey);
			glEnd();

			//����յ��������ٻ�һ��
			glLineWidth(2.0f);
			glBegin(GL_LINES);
			glColor3fv(triangle_color);
			for (int i = 0; i < vertice.size(); i++)
			{
				if (i == vertice.size() - 1)
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
			//��֮ǰ��ֱ���ٻ�һ��
			glLineWidth(1.0f);
			glBegin(GL_LINES);
			glColor3fv(straightLine_color);
			for (int i = 0; i < line.size(); i++)
			{
				glVertex2f(line[i].bx, line[i].by);
				glVertex2f(line[i].ex, line[i].ey);
			}
			glEnd();
			glFlush();
		}
	//}
	//else 
	//{
	//	//cout << "�޸Ŀ��Ƶ�" << ":(" << line[index].x << ", " << input_vertice[index].y << ")" << endl;
	//	line[index].x = x;
	//	input_vertice[index].y = window_height - y;
	//}
}
void mymouse(int button, int state, int x, int y)
{
	//glClearColor(1, 1, 1, 1);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		draw_triangle_point(x, window_height - y, triangle_color);
		point p(x, window_height - y);
		vertice.push_back(p);
		cout << "�����ζ���" << vertice.size() << ":(" << x << ", " << window_height - y << ")" << endl;
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		glLineWidth(2.0f);
		glBegin(GL_LINES);
		glColor3fv(triangle_color);
		for (int i = 0; i < vertice.size(); i++)
		{
			if (i == vertice.size() - 1)
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
	}

	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		bx = x;
		by = window_height - y;
		draw_straightLine_point(bx, by, straightLine_color);
	}
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP)
	{
		ex = x;
		ey = window_height - y;
		straightLine p(bx, by, ex, ey);
		line.push_back(p);

		glLineWidth(0.5f);
		glBegin(GL_LINES);
		glColor3fv(straightLine_color);
		glVertex2f(bx, by);
		glVertex2f(ex, ey);
		printf("ֱ��%d �������꣺���(%d, %d) �յ�(%d, %d)\n", line.size(), bx, by, ex, ey);
		glEnd();
		glFlush();
	}
}
void keyboard(unsigned char key, int x, int y)
{
	if (key == 32)
	{
		cout <<"linesize="<< line.size() << endl;
		for (int i = 0; i < line.size(); i++)
			CS_LineClip(line[i].bx, line[i].by, line[i].ex, line[i].ey);
	}
}
