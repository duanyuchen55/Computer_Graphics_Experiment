#include<iostream>
#include<GL/glut.h>
using namespace std;

int bx, by, ex, ey;//���ߵ���ֹ������
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
	glutCreateWindow("Bresenham�㷨��ֱ��");

	glClearColor(1, 1, 1, 1);//���û��ƴ�����ɫΪ��ɫ

	glMatrixMode(GL_PROJECTION);/*����ΪͶӰ����ģʽ�������۲����*/
	glLoadIdentity();/*����ΪͶӰ����ģʽ�������۲����*/
	gluOrtho2D(0, window_width, window_height, 0);/*����ΪͶӰ����ģʽ�������۲�������۲촰�ڵĴ�СҪ�뻭����Сһ�£�����ֱ������ȫ�ֱ�������*/

	glPointSize(3.0f);//���õ�Ĵ�С
	glColor3f(10, -10, -10);//���õ����ɫ


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

void bresenham1(GLint x1, GLint y1, GLint x2, GLint y2)//�о�����������ã�������Ҫ�Ƶ����Ƶ����̱ʼ�������
{
	glClear(GL_COLOR_BUFFER_BIT);

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
			draw_a_point(y, x);
		else
			draw_a_point(x, y);

		x += tx;//x���Ӳ���
	}
}

void bresenham2(GLint x1, GLint y1, GLint x2, GLint y2)//���������
{
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_LINE_SMOOTH);                           //���÷�����
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);              //���÷�����


	draw_a_point(x1, y1);

	double dx = x2 - x1;
	double dy = y2 - y1;

	int x = round(x1);
	int y = round(y1);

	if (fabs(x - x1) < 1e-6)//ƽ��x
	{
		if (y1 > y2)
		{
			swap(y1, y2);
			swap(x1, x2);
		}
		for (y = round(y1); y < round(y2); y++)
			draw_a_point(x, y);
	}
	else if (fabs(y - y1) < 1e-6)//ƽ��y
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
		double k = dy / dx;//б��
		double d;//dΪ�е����

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

//�϶����
void drag_mouse(int x, int y)
{
	ex = x;
	ey = y;
	printf("�������꣺���(%d, %d) �յ�(%d, %d)\n", bx, by, ex, ey);//�����������ӡ����

	bresenham1(bx, by, ex, ey);
	//bresenham2(bx, by, ex, ey);
	glFlush();
}

//�������λ�ã����һ���
void listen_mouse(int button, int state, int x, int y)
{
	//̧������ʱ�������յ� �յ�
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		ex = x;
		ey = y;
		printf("�������꣺���(%d, %d) �յ�(%d, %d)\n", bx, by, ex, ey);//�����������ӡ����

		bresenham1(bx, by, ex, ey);//����
		//bresenham2(bx, by, ex, ey);
		glFlush();

	}
	//�������ʱ�� �����
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


//����2
//#include<iostream>
//#include<GL/glut.h>
//using namespace std;
//
//int bx, by, ex, ey;//���ߵ���ֹ������
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
//	glEnable(GL_LINE_SMOOTH);                           //���÷�����
//	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);              //���÷�����
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
//	////��y��ƽ��
//	//if (x1 == x2)
//	//{
//	//	while (y < y2)
//	//	{
//	//		y++;
//	//		draw_a_point(x, y);
//	//	}
//	//	return;
//	//}
//	////��x��ƽ��
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
//	int flag = 0;//��ʾ��ǰб��k�ľ���ֵ�Ƿ����1
//	if (dx == 0 && dy == 0)
//		return;
//
//	if (dy > dx)//б�ʾ���ֵ����1
//	{
//		flag = 1;
//		//���������ụ������x����y����y����x���������궼��Ҫ����
//		swap(x1, y1);
//		swap(x2, y2);
//		swap(dx, dy);
//	}
//
//	//ȷ������tx��ty��б��Ϊ����Ϊ������ֻ������/����or��/���£���/�ұߵ�������Ӳ�������
//	int tx = x2 > x1 ? 1 : -1;
//	int ty = y2 > y1 ? 1 : -1;
//
//	//ȷ����һ���������x��y
//	int x = x1 + 1;
//	int y = y1;
//
//	//dt��ds�ɵ�����ʽ�Ƴ���dt�����ϵĵ㣬ds���ұߵĵ�
//	int dt = 2 * (dy - dx);
//	int ds = 2 * dy;
//
//	//�б�ʽ��ֵ
//	int d = ds - dx;
//
//	while (x != x2)
//	{
//		if (d >= 0)//ѡT�㣨���ϵĵ�
//		{
//			d += dt;
//			y += ty;
//		}
//		else
//		{
//			d += ds;
//		}
//
//		if (flag)//б�ʴ���1
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
//	glEnable(GL_LINE_SMOOTH);                           //���÷�����
//	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);              //���÷�����
//
//	draw_a_point(x1, y1);
//
//	double dx = x2 - x1;
//	double dy = y2 - y1;
//
//	int x = round(x1);
//	int y = round(y1);
//
//	if (fabs(x - x2) < 1e-6)//ƽ��x
//	{
//		if (y1 > y2)
//		{
//			swap(y1, y2);
//			swap(x1, x2);
//		}
//		for (y = round(y1); y < round(y2); y++)
//			draw_a_point(x, y);
//	}
//	else if (fabs(y - y2) < 1e-6)//ƽ��y
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
//		double k = dy / dx;//б��
//		double d;//dΪ�е����
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
////�϶����
//void drag_mouse(int x, int y)
//{
//	ex = x;
//	ey = y;
//	printf("�������꣺���(%d, %d) �յ�(%d, %d)\n", bx, by, ex, ey);//�����������ӡ����
//
//	//bresenham1(bx, by, ex, ey);
//	bresenham2(bx, by, ex, ey);
//	glFlush();
//}
//
////�������λ�ã����һ���
//void listen_mouse(int button, int state, int x, int y)
//{
//	//̧������ʱ�������յ� �յ�
//	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
//	{
//		ex = x;
//		ey = y;
//		printf("�������꣺���(%d, %d) �յ�(%d, %d)\n", bx, by, ex, ey);//�����������ӡ����
//
//		//bresenham1(bx, by, ex, ey);//����
//		bresenham2(bx, by, ex, ey);
//		glFlush();
//
//	}
//	//�������ʱ�� �����
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
//	glutCreateWindow("Bresenham�㷨��ֱ��");
//
//	glClearColor(1, 1, 1, 1);//���û��ƴ�����ɫΪ��ɫ
//	//glClear(GL_COLOR_BUFFER_BIT);//�����������
//
//	glMatrixMode(GL_PROJECTION);/*����ΪͶӰ����ģʽ�������۲����*/
//	glLoadIdentity();/*����ΪͶӰ����ģʽ�������۲����*/
//	gluOrtho2D(0, window_width, window_height, 0);/*����ΪͶӰ����ģʽ�������۲�������۲촰�ڵĴ�СҪ�뻭����Сһ�£�����ֱ������ȫ�ֱ�������*/
//
//	glPointSize(3.0f);//���õ�Ĵ�С
//	glColor3f(10, -10, -10);//���õ����ɫ
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