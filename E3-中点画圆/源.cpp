#include<iostream>
#include<GL/glut.h>
using namespace std;

float bx, by, ex, ey, r;//划线的起止点坐标，由此确定圆的圆心(bx, by)和半径

int window_width = 1000, window_height = 800;

void MidPointCircle(int x0, int y0, int r);
void display();
void drag_mouse(int x, int y);
void listen_mouse(int button, int state, int x, int y);

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("中点画圆算法");

	glClearColor(1, 1, 1, 1);//设置绘制窗口颜色为白色\

	glMatrixMode(GL_PROJECTION);/*设置为投影类型模式和其他观察参数*/
	glLoadIdentity();/*设置为投影类型模式和其他观察参数*/
	gluOrtho2D(0, window_width, window_height, 0);/*设置为投影类型模式和其他观察参数，观察窗口的大小要与画布大小一致，所以直接设置全局变量即可*/

	glPointSize(3.0f);//设置点的大小
	glColor3f(10, -10, -10);//设置点的颜色


	glutDisplayFunc(&display);

	glutMouseFunc(listen_mouse);
	glutMotionFunc(drag_mouse);
	glutMainLoop();

	return 0;
}

void MidPointCircle(int x0, int y0, int r)
{
	glClear(GL_COLOR_BUFFER_BIT);

	int x = 0;
	int y = r;
	double d = 1.25 - r;
	glBegin(GL_POINTS);
	glVertex2f(x0, y0);
	glVertex2f(x + x0, y + y0);
	glEnd();
	while (x <= y)
	{
		if (d < 0)
			d += 2 * x + 3;
		else
		{
			d += 2 * (x - y) + 5;
			y--;
		}
		x++;

		glBegin(GL_POINTS);
		//利用八对称性画出整个圆
		glVertex2f(x + x0, y + y0);
		glVertex2f(y + x0, x + y0);
		glVertex2f(-x + x0, y + y0);
		glVertex2f(y + x0, -x + y0);
		glVertex2f(x + x0, -y + y0);
		glVertex2f(-y + x0, x + y0);
		glVertex2f(-x + x0, -y + y0);
		glVertex2f(-y + x0, -x + y0);
		glEnd();
	}
	glFlush();
}
void display()
{
	MidPointCircle(bx, by, r);
}

//拖动鼠标
void drag_mouse(int x, int y)
{
	ex = x;
	ey = y;
	printf("待定坐标：起点(%d, %d) 终点(%d, %d)\n", bx, by, ex, ey);//把两个坐标打印出来

	r = sqrt((bx - ex) * (bx - ex) + (by - ey) * (by - ey));

	MidPointCircle(bx, by, r);
	glFlush();
}

//监听鼠标位置，并且画点
void listen_mouse(int button, int state, int x, int y)
{
	//抬起鼠标的时候是最终的 终点
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		ex = x;
		ey = y;
		printf("最终坐标：起点(%d, %d) 终点(%d, %d)\n", bx, by, ex, ey);//把两个坐标打印出来
		r = sqrt((bx - ex) * (bx - ex) + (by - ey) * (by - ey));
		printf("圆半径为：%d\n", r);
		MidPointCircle(bx, by, r);//画线
		glFlush();
	}
	//点击鼠标的时候 是起点
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		bx = x;
		by = y;
	}
}