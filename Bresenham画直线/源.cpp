#include<iostream>
#include<GL/glut.h>
using namespace std;

int bx, by, ex, ey;//划线的起止点坐标
int window_width = 800, window_height = 600;

void swap(GLint& a, GLint& b);
void draw_a_point(GLint x, GLint y);
void bresenham1(GLint x1, GLint y1, GLint x2, GLint y2);
void bresenham2(GLint x1, GLint y1, GLint x2, GLint y2);
void drag_mouse(int x, int y);
void listen_mouse(int button, int state, int x, int y);
void display1();
void display2();

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Bresenham算法画直线");

	glClearColor(1, 1, 1, 1);//设置绘制窗口颜色为白色

	glMatrixMode(GL_PROJECTION);/*设置为投影类型模式和其他观察参数*/
	glLoadIdentity();/*设置为投影类型模式和其他观察参数*/
	gluOrtho2D(0, window_width, window_height, 0);/*设置为投影类型模式和其他观察参数，观察窗口的大小要与画布大小一致，所以直接设置全局变量即可*/

	glPointSize(3.0f);//设置点的大小
	glColor3f(10, -10, -10);//设置点的颜色


	glutDisplayFunc(&display1);
	//glutDisplayFunc(&display2);

	glutMouseFunc(listen_mouse);
	glutMotionFunc(drag_mouse);

	glutMainLoop();

	return 0;
}


void swap(GLint& a, GLint& b)
{
	GLint t = a;
	a = b;
	b = t;
}
void draw_a_point(GLint x, GLint y)
{
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}

void bresenham1(GLint x1, GLint y1, GLint x2, GLint y2)//感觉这个方法更好，不过需要推导，推导过程笔记在书里
{
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_LINE_SMOOTH);                           //设置反走样
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);              //设置反走样

	draw_a_point(x1, y1);
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
			draw_a_point(y, x);
		else
			draw_a_point(x, y);

		x += tx;//x增加步长
	}
}

void bresenham2(GLint x1, GLint y1, GLint x2, GLint y2)//分情况讨论
{
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_LINE_SMOOTH);                           //设置反走样
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);              //设置反走样


	draw_a_point(x1, y1);

	double dx = x2 - x1;
	double dy = y2 - y1;

	int x = round(x1);
	int y = round(y1);

	if (fabs(x - x1) < 1e-6)//平行x
	{
		if (y1 > y2)
		{
			swap(y1, y2);
			swap(x1, x2);
		}
		for (y = round(y1); y < round(y2); y++)
			draw_a_point(x, y);
	}
	else if (fabs(y - y1) < 1e-6)//平行y
	{
		if (x1 > x2)
		{
			swap(y1, y2);
			swap(x1, x2);
		}
		for (x = round(x1); x < round(x2); x++)
			draw_a_point(x, y);
	}
	else
	{
		double k = dy / dx;//斜率
		double d;//d为中点误差

		//0<k<1
		if (k >= 0 && k < 1)
		{
			if (x1 > x2) {
				swap(x1, x2);
				swap(y1, y2);
			}
			d = 0.5 - k;
			for (x = round(x1); x < round(x2); x++)
			{
				draw_a_point(round(x), round(y));
				if (d < 0)
				{
					y++;
					d += 1 - k;
				}
				else
				{
					d -= k;
				}
			}
		}
		//k>1
		if (k > 1)
		{
			if (y1 > y2) {
				swap(x1, x2);
				swap(y1, y2);
			}
			d = 1 - 0.5 * k;
			for (y = round(y1); y < round(y2); y++) {
				draw_a_point(round(x), round(y));
				if (d >= 0)
				{
					x++;
					d += 1 - k;
				}
				else
				{
					d += 1;
				}
			}
		}
		//-1<=k<=0
		if (k >= -1.0 && k <= 0.0) {

			if (x1 > x2) {
				swap(x1, x2);
				swap(y1, y2);
			}
			d = -0.5 - k;
			for (x = round(x1); x < round(x2); x++)
			{
				draw_a_point(round(x), round(y));
				if (d > 0) {
					y--;
					d -= 1 + k;
				}
				else {
					d -= k;
				}
			}

		}
		//k<-1
		if (k < -1.0) {

			if (y1 < y2) {
				swap(x1, x2);
				swap(y1, y2);
			}
			d = -1 - 0.5 * k;
			for (y = round(y1); y > round(y2); y--)
			{
				draw_a_point(round(x), round(y));
				if (d < 0)
				{
					x++;
					d -= 1 + k;
				}
				else
				{
					d -= 1;
				}
			}
		}
	}
}

//拖动鼠标
void drag_mouse(int x, int y)
{
	ex = x;
	ey = y;
	printf("待定坐标：起点(%d, %d) 终点(%d, %d)\n", bx, by, ex, ey);//把两个坐标打印出来

	bresenham1(bx, by, ex, ey);
	//bresenham2(bx, by, ex, ey);
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

		bresenham1(bx, by, ex, ey);//画线
		//bresenham2(bx, by, ex, ey);
		glFlush();

	}
	//点击鼠标的时候 是起点
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		bx = x;
		by = y;
	}
}

void display1()
{
	bresenham1(bx, by, ex, ey);
}
void display2()
{
	bresenham2(bx, by, ex, ey);
}


//做法2
//#include<iostream>
//#include<GL/glut.h>
//using namespace std;
//
//int bx, by, ex, ey;//划线的起止点坐标
//int window_width = 800, window_height = 600;
//
//void swap(GLint& a, GLint& b)
//{
//	GLint t = a;
//	a = b;
//	b = t;
//}
//void draw_a_point(GLint x, GLint y)
//{
//	glBegin(GL_POINTS);
//	glVertex2i(x, y);
//	glEnd();
//}
//
//void bresenham1(GLint x1, GLint y1, GLint x2, GLint y2)
//{
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	glEnable(GL_LINE_SMOOTH);                           //设置反走样
//	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);              //设置反走样
//
//	////swap
//	//if (x2 < x1)
//	//{
//	//	swap(x1, x2);
//	//	swap(y1, y2);
//	//}
//	//int x = x1;
//	//int y = y1;
//	//draw_a_point(x, y);
//	////与y轴平行
//	//if (x1 == x2)
//	//{
//	//	while (y < y2)
//	//	{
//	//		y++;
//	//		draw_a_point(x, y);
//	//	}
//	//	return;
//	//}
//	////与x轴平行
//	//if (y1 == y2)
//	//{
//	//	while (x < x2)
//	//	{
//	//		x++;
//	//		draw_a_point(x, y);
//	//	}
//	//	return;
//	//}
//	//int dx = abs(x1 - x2);
//	//int dy = abs(y1 - y2);
//	//double k = double(dy) / dx;/////////////////////////
//
//	draw_a_point(x1, y1);
//	int dx = abs(x1 - x2);
//	int dy = abs(y1 - y2);
//	int flag = 0;//表示当前斜率k的绝对值是否大于1
//	if (dx == 0 && dy == 0)
//		return;
//
//	if (dy > dx)//斜率绝对值大于1
//	{
//		flag = 1;
//		//横纵坐标轴互换，将x视作y，将y视作x，所有坐标都需要互换
//		swap(x1, y1);
//		swap(x2, y2);
//		swap(dx, dy);
//	}
//
//	//确定步长tx，ty，斜率为正或为负，且只考虑左/右上or左/右下，左/右边的情况不加步长即可
//	int tx = x2 > x1 ? 1 : -1;
//	int ty = y2 > y1 ? 1 : -1;
//
//	//确定下一个点的坐标x，y
//	int x = x1 + 1;
//	int y = y1;
//
//	//dt和ds由迭代公式推出。dt是右上的点，ds是右边的点
//	int dt = 2 * (dy - dx);
//	int ds = 2 * dy;
//
//	//判别式的值
//	int d = ds - dx;
//
//	while (x != x2)
//	{
//		if (d >= 0)//选T点（右上的点
//		{
//			d += dt;
//			y += ty;
//		}
//		else
//		{
//			d += ds;
//		}
//
//		if (flag)//斜率大于1
//			draw_a_point(y, x);
//		else
//			draw_a_point(x, y);
//
//		x += tx;
//	}
//}
//
//void bresenham2(GLint x1, GLint y1, GLint x2, GLint y2)
//{
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	glEnable(GL_LINE_SMOOTH);                           //设置反走样
//	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);              //设置反走样
//
//	draw_a_point(x1, y1);
//
//	double dx = x2 - x1;
//	double dy = y2 - y1;
//
//	int x = round(x1);
//	int y = round(y1);
//
//	if (fabs(x - x2) < 1e-6)//平行x
//	{
//		if (y1 > y2)
//		{
//			swap(y1, y2);
//			swap(x1, x2);
//		}
//		for (y = round(y1); y < round(y2); y++)
//			draw_a_point(x, y);
//	}
//	else if (fabs(y - y2) < 1e-6)//平行y
//	{
//		if (x1 > x2)
//		{
//			swap(y1, y2);
//			swap(x1, x2);
//		}
//		for (x = round(x1); x < round(x2); x++)
//			draw_a_point(x, y);
//	}
//	else
//	{
//		double k = dy / dx;//斜率
//		double d;//d为中点误差
//
//		//0<k<1
//		if (k >= 0.0 && k <= 1.0)
//		{
//			if (x1 > x2) {
//				swap(x1, x2);
//				swap(y1, y2);
//			}
//			d = 0.5 - k;
//			for (x = round(x1); x < round(x2); x++)
//			{
//				draw_a_point(round(x), round(y));
//				if (d < 0)
//				{
//					y++;
//					d += 1 - k;
//				}
//				else
//				{
//					d -= k;
//				}
//			}
//		}
//		//k>1
//		if (k > 1)
//		{
//			if (y1 > y2) {
//				swap(x1, x2);
//				swap(y1, y2);
//			}
//			d = 1 - 0.5 * k;
//			for (y = round(y1); y < round(y2); y++) {
//				draw_a_point(round(x), round(y));
//				if (d >= 0)
//				{
//					x++;
//					d += 1 - k;
//				}
//				else
//				{
//					d += 1;
//				}
//			}
//		}
//		//-1<=k<=0
//		if (k >= -1.0 && k <= 0.0) {
//
//			if (x1 > x2) {
//				swap(x1, x2);
//				swap(y1, y2);
//			}
//			d = -0.5 - k;
//			for (x = round(x1); x < round(x2); x++)
//			{
//				draw_a_point(round(x), round(y));
//				if (d > 0)
//				{
//					y--;
//					d -= 1 + k;
//				}
//				else {
//					d -= k;
//				}
//			}
//
//		}
//		//k<-1
//		if (k < -1.0) {
//
//			if (y1 < y2) {
//				swap(x1, x2);
//				swap(y1, y2);
//			}
//			d = -1 - 0.5 * k;
//			for (y = round(y1); y > round(y2); y--)
//			{
//				draw_a_point(round(x), round(y));
//				if (d < 0)
//				{
//					x++;
//					d -= 1 + k;
//				}
//				else
//				{
//					d -= 1;
//				}
//			}
//		}
//	}
//}
//
////拖动鼠标
//void drag_mouse(int x, int y)
//{
//	ex = x;
//	ey = y;
//	printf("待定坐标：起点(%d, %d) 终点(%d, %d)\n", bx, by, ex, ey);//把两个坐标打印出来
//
//	//bresenham1(bx, by, ex, ey);
//	bresenham2(bx, by, ex, ey);
//	glFlush();
//}
//
////监听鼠标位置，并且画点
//void listen_mouse(int button, int state, int x, int y)
//{
//	//抬起鼠标的时候是最终的 终点
//	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
//	{
//		ex = x;
//		ey = y;
//		printf("最终坐标：起点(%d, %d) 终点(%d, %d)\n", bx, by, ex, ey);//把两个坐标打印出来
//
//		//bresenham1(bx, by, ex, ey);//画线
//		bresenham2(bx, by, ex, ey);
//		glFlush();
//
//	}
//	//点击鼠标的时候 是起点
//	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
//	{
//		bx = x;
//		by = y;
//	}
//}
//
//void display1()
//{
//	bresenham1(bx, by, ex, ey);
//}
//void display2()
//{
//	bresenham2(bx, by, ex, ey);
//}
//
//int main(int argc, char* argv[])
//{
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
//	glutInitWindowPosition(100, 100);
//	glutInitWindowSize(window_width, window_height);
//	glutCreateWindow("Bresenham算法画直线");
//
//	glClearColor(1, 1, 1, 1);//设置绘制窗口颜色为白色
//	//glClear(GL_COLOR_BUFFER_BIT);//清除窗口内容
//
//	glMatrixMode(GL_PROJECTION);/*设置为投影类型模式和其他观察参数*/
//	glLoadIdentity();/*设置为投影类型模式和其他观察参数*/
//	gluOrtho2D(0, window_width, window_height, 0);/*设置为投影类型模式和其他观察参数，观察窗口的大小要与画布大小一致，所以直接设置全局变量即可*/
//
//	glPointSize(3.0f);//设置点的大小
//	glColor3f(10, -10, -10);//设置点的颜色
//
//
//	//glutDisplayFunc(&display1);
//	glutDisplayFunc(&display2);
//
//	glutMouseFunc(listen_mouse);
//	glutMotionFunc(drag_mouse);
//
//	glutMainLoop();
//
//	return 0;
//}