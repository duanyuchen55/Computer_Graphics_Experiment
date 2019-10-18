#include<iostream>
#include<gl/glut.h>
#include<algorithm>
#include<vector>
#include<stack>
#include<queue>
using namespace std;
const int window_width = 800, window_height = 500;
const int maxn = 99999;

struct point
{
	float x, y;
	point()
	{}
	point(int xx, int yy)
		:x(xx), y(yy) {}
};
vector<point> vertice;
typedef struct XET
{
	float x;
	float dx;
	float ymax;//float
	XET* next;
}AET, NET;//AET 活性边表； NET新边表

void draw_a_point(int x, int y);
void PolyScan();
void mymouse(int button, int state, int x, int y);
void display();


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 50);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("扫描线填充");
	cout << "左键画点\n右键连接成多边形\n中键开始扫描线填充" << endl;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, window_width,  0, window_height);

	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glutMouseFunc(&mymouse); 

	glutMainLoop();
	return 0;
}

void draw_a_point(int x, int y)
{
	glBegin(GL_POINTS);
	glColor3f(1, 0, 0);
	glVertex2f(x, y);
	glEnd();
	glFlush();
}
void PolyScan()
{
	//得到最高点的y坐标
	int Max_Y = 0;
	for (int i = 0; i < vertice.size(); i++)
		/*Max_Y = max(Max_Y, vertice[i].y);*/
		if (vertice[i].y > Max_Y)
			Max_Y = vertice[i].y; 
	//cout << "MAX_Y=" << Max_Y << endl;


	//初始化AET表
	AET* pAET = new AET;
	pAET->next = NULL;
	//初始化NET表
	NET* pNET[800];
	for (int i = 0; i <= Max_Y; i++)
	{
		pNET[i] = new NET;
		//pNET[i]->x = NULL;//写的时候我是脑子瓦特了嘛，辣鸡bug，毁我青春！！
		pNET[i]->next = NULL;;
	}

	//扫描并且建立NET表
	int len = vertice.size();
	for (int i = 0; i <= Max_Y; i++)
	{
		//cout << "i=" << i << " ";
		for (int j = 0; j < len; j++)
		{
			//cout << "j=" << j << endl;
			if (i == vertice[j].y)
			{
				//cout <<"y="<< i << endl;
				//如果一个点和前一个点有一条边相连，则该点和后面一个点也相连
				if (vertice[(j - 1 + len) % len].y > vertice[j].y)
				{
					NET* p = new NET;
					p->x = vertice[j].x;
					p->ymax = vertice[(j - 1 + len) % len].y;//与当前扫描线相交的活性边 的 最高点即为相邻顶点的y
					float DX = vertice[(j - 1 + len) % len].x - vertice[j].x;
					float DY = vertice[(j - 1 + len) % len].y - vertice[j].y;
					p->dx = DX / DY;//dx为直线斜率的倒数
					p->next = pNET[i]->next;
					pNET[i]->next = p;
				}
				if (vertice[(j + 1) % len].y > vertice[j].y)
				{
					NET* p = new NET;
					p->x = vertice[j].x;
					p->ymax = vertice[(j + 1) % len].y;
					float DX = vertice[(j + 1) % len].x - vertice[j].x;
					float DY = vertice[(j + 1) % len].y - vertice[j].y;
					p->dx = DX / DY;//dx为直线斜率的倒数
					p->next = pNET[i]->next;
					pNET[i]->next = p;
				}
			}
		}
	}

	//建立并且更新活性边表AET
	for (int i = 0; i <= Max_Y; i++)
	{
		//计算新的交点 更新AET
		NET* p = pAET->next;
		while (p)
		{
			p->x = p->x + p->dx;
			p = p->next;
		}
		  
		//排序
		AET* tq = pAET;
		p = pAET->next;
		tq->next = NULL;

		while (p != NULL)//顺着链表往下走
		{
			//找到第一个比它大的数字tq->next->x，则从p->next到tq->next都是比p->x小的
			while (tq->next != NULL && tq->next->x <= p->x)
				tq = tq->next;
			//把这一段小的整体向前移动
			NET* t = p->next;
			p->next = tq->next;
			tq->next = p;
			p = t;

			tq = pAET;//回到头
		}

		//(改进算法)先从AET表中删除ymax==i的结点****************************************/
		AET* q = pAET;
		p = q->next;
		while (p)
		{
			if (p->ymax == i)
			{
				q->next = p->next;
				delete p;
				p = q->next;
			}
			else
			{
				q = q->next;
				p = q->next;
			}
		}

		//将NET中的新点用插入法插入AET，按x递增的顺序排列
		p = pNET[i]->next;
		q = pAET;
		while (p)
		{
			while (q->next != NULL && p->x >= q->next->x)
				q = q->next;

			NET* t = p->next;
			p->next = q->next;
			q->next = p;
			p = t;

			q = pAET;
		}

		//配对后填充颜色
		p = pAET->next;
		while (p != NULL && p->next != NULL)
		{
			for (float j = p->x; j <= p->next->x; j++)
			{
				draw_a_point(j, i);
				//cout << "(" << j << ", " << i << ")" << endl;
			}
			p = p->next->next;//考虑端点情况
		}
	}
	glFlush();
}
void mymouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		draw_a_point(x, window_height - y);

		point p(x, window_height - y);
		vertice.push_back(p);
		cout << "顶点" << vertice.size() << ": (" << x << ", " << window_height - y << ")" << endl;
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		glClearColor(1, 1, 1, 1);//设置绘制窗口颜色为白色
		glColor3f(1, 0, 0);

		glBegin(GL_LINES);
		for (int i = 0; i < vertice.size(); i++)
		{
			if (i == vertice.size() - 1)//画完最后一个点，使其闭合
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
		//cout << "center: (" << x << ", " << y << ")" << endl;
		//BoundaryFill4(x, window_height - y);
		//BoundaryFill4_Stack(x, window_height - y);

		cout << "多边形顶点个数为" << vertice.size() << "。 " << "开始扫描线填充。" << endl;
		PolyScan();
	}
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.4, 0.2);
	glPointSize(1);
	glBegin(GL_POINTS);
	PolyScan();
	glEnd();
	glFlush();
}