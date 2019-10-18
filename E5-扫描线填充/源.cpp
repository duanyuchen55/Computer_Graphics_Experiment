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
}AET, NET;//AET ���Ա߱� NET�±߱�

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
	glutCreateWindow("ɨ�������");
	cout << "�������\n�Ҽ����ӳɶ����\n�м���ʼɨ�������" << endl;

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
	//�õ���ߵ��y����
	int Max_Y = 0;
	for (int i = 0; i < vertice.size(); i++)
		/*Max_Y = max(Max_Y, vertice[i].y);*/
		if (vertice[i].y > Max_Y)
			Max_Y = vertice[i].y; 
	//cout << "MAX_Y=" << Max_Y << endl;


	//��ʼ��AET��
	AET* pAET = new AET;
	pAET->next = NULL;
	//��ʼ��NET��
	NET* pNET[800];
	for (int i = 0; i <= Max_Y; i++)
	{
		pNET[i] = new NET;
		//pNET[i]->x = NULL;//д��ʱ���������������������bug�������ഺ����
		pNET[i]->next = NULL;;
	}

	//ɨ�貢�ҽ���NET��
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
				//���һ�����ǰһ������һ������������õ�ͺ���һ����Ҳ����
				if (vertice[(j - 1 + len) % len].y > vertice[j].y)
				{
					NET* p = new NET;
					p->x = vertice[j].x;
					p->ymax = vertice[(j - 1 + len) % len].y;//�뵱ǰɨ�����ཻ�Ļ��Ա� �� ��ߵ㼴Ϊ���ڶ����y
					float DX = vertice[(j - 1 + len) % len].x - vertice[j].x;
					float DY = vertice[(j - 1 + len) % len].y - vertice[j].y;
					p->dx = DX / DY;//dxΪֱ��б�ʵĵ���
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
					p->dx = DX / DY;//dxΪֱ��б�ʵĵ���
					p->next = pNET[i]->next;
					pNET[i]->next = p;
				}
			}
		}
	}

	//�������Ҹ��»��Ա߱�AET
	for (int i = 0; i <= Max_Y; i++)
	{
		//�����µĽ��� ����AET
		NET* p = pAET->next;
		while (p)
		{
			p->x = p->x + p->dx;
			p = p->next;
		}
		  
		//����
		AET* tq = pAET;
		p = pAET->next;
		tq->next = NULL;

		while (p != NULL)//˳������������
		{
			//�ҵ���һ�������������tq->next->x�����p->next��tq->next���Ǳ�p->xС��
			while (tq->next != NULL && tq->next->x <= p->x)
				tq = tq->next;
			//����һ��С��������ǰ�ƶ�
			NET* t = p->next;
			p->next = tq->next;
			tq->next = p;
			p = t;

			tq = pAET;//�ص�ͷ
		}

		//(�Ľ��㷨)�ȴ�AET����ɾ��ymax==i�Ľ��****************************************/
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

		//��NET�е��µ��ò��뷨����AET����x������˳������
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

		//��Ժ������ɫ
		p = pAET->next;
		while (p != NULL && p->next != NULL)
		{
			for (float j = p->x; j <= p->next->x; j++)
			{
				draw_a_point(j, i);
				//cout << "(" << j << ", " << i << ")" << endl;
			}
			p = p->next->next;//���Ƕ˵����
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
		cout << "����" << vertice.size() << ": (" << x << ", " << window_height - y << ")" << endl;
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		glClearColor(1, 1, 1, 1);//���û��ƴ�����ɫΪ��ɫ
		glColor3f(1, 0, 0);

		glBegin(GL_LINES);
		for (int i = 0; i < vertice.size(); i++)
		{
			if (i == vertice.size() - 1)//�������һ���㣬ʹ��պ�
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

		cout << "����ζ������Ϊ" << vertice.size() << "�� " << "��ʼɨ������䡣" << endl;
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