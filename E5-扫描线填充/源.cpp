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
}AET, ET;//AET 活性边表； ET新边表

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


	//初始化AET表和ET表
	AET* pAET = new AET;
	pAET->next = NULL;

	ET* pET[1000];
	for (int i = 0; i <= Max_Y; i++)
	{
		pET[i] = new ET;
		//pET[i]->x = NULL;//写的时候我是脑子瓦特了嘛，辣鸡bug，毁我青春！！
		pET[i]->next = NULL;
	}

	//扫描并且建立ET表，遍历每一条扫描线，同时再遍历端点数组，寻找落在扫描线上的端点
	int len = vertice.size();
	for (int i = 0; i <= Max_Y; i++)
	{
		//cout << "i=" << i << " ";
		for (int j = 0; j < len; j++)
		{
			//cout << "j=" << j << endl;
			if (i == vertice[j].y)//找到了落在了扫描线上的一个端点，那么这个端点的和他的前一个和后一个端点都有一条边，这两条边的另外一个端点的y就是ymax
			{
				//cout <<"y="<< i << endl;
				if (vertice[(j - 1 + len) % len].y > vertice[j].y)
				{
					ET* p = new ET;
					p->x = vertice[j].x;
					p->ymax = vertice[(j - 1 + len) % len].y;//与当前扫描线相交的活性边 的 最高点即为相邻顶点的y
					float DX = vertice[(j - 1 + len) % len].x - vertice[j].x;
					float DY = vertice[(j - 1 + len) % len].y - vertice[j].y;
					p->dx = DX / DY;//dx为直线斜率的倒数
					p->next = pET[i]->next;
					pET[i]->next = p;
				}
				if (vertice[(j + 1) % len].y > vertice[j].y)
				{
					ET* p = new ET;
					p->x = vertice[j].x;
					p->ymax = vertice[(j + 1) % len].y;//与当前扫描线相交的活性边 的 最高点即为相邻顶点的y
					float DX = vertice[(j + 1) % len].x - vertice[j].x;
					float DY = vertice[(j + 1) % len].y - vertice[j].y;
					p->dx = DX / DY;//dx为直线斜率的倒数
					p->next = pET[i]->next;
					pET[i]->next = p;
				}
			}
		}
	}

	//建立并且更新活性边表AET
	for (int i = 0; i <= Max_Y; i++)
	{
		//计算新的交点更新AET
		//到活性边表的末尾
		ET* p = pAET->next;
		while (p)
		{
			p->x = p->x + p->dx;
			p = p->next;
		}
		  
		//排序，为了处理这种情况：上一次交点是一个点，在经过更新后，变成了两个点，两个点的x大小需要由他们的dx确定，导致大小顺序不确定，需要排序保证顺序
		AET* tq = pAET;
		p = pAET->next;
		tq->next = NULL;
		while (p != NULL)//顺着链表往下走
		{
			while (tq->next != NULL && tq->next->x <= p->x)
				tq = tq->next;
			ET* t = p->next;
			p->next = tq->next;
			tq->next = p;
			p = t;

			tq = pAET;
		}

		//从AET表中删除那些ymax==i的点
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

		//将ET中的新点用插入法插入AET，按x递增的顺序排列
		p = pET[i]->next;
		q = pAET;
		while (p)
		{
			while (q->next != NULL && p->x >= q->next->x)
				q = q->next;

			ET* t = p->next;
			p->next = q->next;
			q->next = p;
			p = t;

			q = pAET;
		}

		//两个交点配对，连线
		p = pAET->next;
		while (p != NULL && p->next != NULL)
		{
			for (float j = p->x; j <= p->next->x; j++)//活性边表 每两个点之间涂色
			{
				draw_a_point(j, i);
				//cout << "(" << j << ", " << i << ")" << endl;
			}
			p = p->next->next;
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