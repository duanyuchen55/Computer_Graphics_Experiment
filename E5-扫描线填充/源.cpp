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
}AET, ET;//AET ���Ա߱� ET�±߱�

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


	//��ʼ��AET���ET��
	AET* pAET = new AET;
	pAET->next = NULL;

	ET* pET[1000];
	for (int i = 0; i <= Max_Y; i++)
	{
		pET[i] = new ET;
		//pET[i]->x = NULL;//д��ʱ���������������������bug�������ഺ����
		pET[i]->next = NULL;
	}

	//ɨ�貢�ҽ���ET������ÿһ��ɨ���ߣ�ͬʱ�ٱ����˵����飬Ѱ������ɨ�����ϵĶ˵�
	int len = vertice.size();
	for (int i = 0; i <= Max_Y; i++)
	{
		//cout << "i=" << i << " ";
		for (int j = 0; j < len; j++)
		{
			//cout << "j=" << j << endl;
			if (i == vertice[j].y)//�ҵ���������ɨ�����ϵ�һ���˵㣬��ô����˵�ĺ�����ǰһ���ͺ�һ���˵㶼��һ���ߣ��������ߵ�����һ���˵��y����ymax
			{
				//cout <<"y="<< i << endl;
				if (vertice[(j - 1 + len) % len].y > vertice[j].y)
				{
					ET* p = new ET;
					p->x = vertice[j].x;
					p->ymax = vertice[(j - 1 + len) % len].y;//�뵱ǰɨ�����ཻ�Ļ��Ա� �� ��ߵ㼴Ϊ���ڶ����y
					float DX = vertice[(j - 1 + len) % len].x - vertice[j].x;
					float DY = vertice[(j - 1 + len) % len].y - vertice[j].y;
					p->dx = DX / DY;//dxΪֱ��б�ʵĵ���
					p->next = pET[i]->next;
					pET[i]->next = p;
				}
				if (vertice[(j + 1) % len].y > vertice[j].y)
				{
					ET* p = new ET;
					p->x = vertice[j].x;
					p->ymax = vertice[(j + 1) % len].y;//�뵱ǰɨ�����ཻ�Ļ��Ա� �� ��ߵ㼴Ϊ���ڶ����y
					float DX = vertice[(j + 1) % len].x - vertice[j].x;
					float DY = vertice[(j + 1) % len].y - vertice[j].y;
					p->dx = DX / DY;//dxΪֱ��б�ʵĵ���
					p->next = pET[i]->next;
					pET[i]->next = p;
				}
			}
		}
	}

	//�������Ҹ��»��Ա߱�AET
	for (int i = 0; i <= Max_Y; i++)
	{
		//�����µĽ������AET
		//�����Ա߱��ĩβ
		ET* p = pAET->next;
		while (p)
		{
			p->x = p->x + p->dx;
			p = p->next;
		}
		  
		//����Ϊ�˴��������������һ�ν�����һ���㣬�ھ������º󣬱���������㣬�������x��С��Ҫ�����ǵ�dxȷ�������´�С˳��ȷ������Ҫ����֤˳��
		AET* tq = pAET;
		p = pAET->next;
		tq->next = NULL;
		while (p != NULL)//˳������������
		{
			while (tq->next != NULL && tq->next->x <= p->x)
				tq = tq->next;
			ET* t = p->next;
			p->next = tq->next;
			tq->next = p;
			p = t;

			tq = pAET;
		}

		//��AET����ɾ����Щymax==i�ĵ�
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

		//��ET�е��µ��ò��뷨����AET����x������˳������
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

		//����������ԣ�����
		p = pAET->next;
		while (p != NULL && p->next != NULL)
		{
			for (float j = p->x; j <= p->next->x; j++)//���Ա߱� ÿ������֮��Ϳɫ
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