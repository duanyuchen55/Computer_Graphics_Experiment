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

	glEnable(GL_LINE_SMOOTH);                           //���÷�����
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);              //���÷�����

	draw_a_point(x1, y1);
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

	//�б�ʽ��ֵ
	int d = ds - dx;

	while (x != x2)
	{
		if (d >= 0)//ѡT�㣨���ϵĵ�
		{
			d += dt;
			y += ty;
		}
		else
		{
			d += ds;
		}

		if (flag)//б�ʴ���1
			draw_a_point(y, x);
		else
			draw_a_point(x, y);

		x += tx;
	}
}