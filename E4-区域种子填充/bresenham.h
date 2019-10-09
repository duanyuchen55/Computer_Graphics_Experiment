#pragma once
#include<GL/glut.h>

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

void bresenham(GLint x1, GLint y1, GLint x2, GLint y2)
{
	//glClear(GL_COLOR_BUFFER_BIT);

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

	//判别式的值
	int d = ds - dx;

	while (x != x2)
	{
		if (d >= 0)//选T点（右上的点
		{
			d += dt;
			y += ty;
		}
		else
		{
			d += ds;
		}

		if (flag)//斜率大于1
			draw_a_point(y, x);
		else
			draw_a_point(x, y);

		x += tx;
	}
}