#include<iostream>
#include<algorithm>
using namespace std;
const int maxn = 1010;

struct node//状态结构体
{
	int a, b;
	int last;
	int step;
};

int vis[maxn][maxn];
int A, B, C;
node q[maxn];
int front, rear;

int bfs(int x, int y)
{
	node t;
	t.a = x; t.b = y; t.step = 0; t.last = -1;
	//q.push(t);
	q[front] = t;
	rear++;

	while (front < rear)
	{
		//node now = q.front(); q.pop();
		node now = q[front]; //front++;

		//cout << "front=" << front << " rear=" << rear << endl;
		//cout << now.a << " " << now.b << endl;

		if (now.a == C || now.b == C)
		{
			q[rear] = now;//记得保存最后这一次，否则输出的路径是错误结果的路径
			return now.step;
		}

		if (!vis[now.a][now.b])
		{
			vis[now.a][now.b] = 1;

			node temp;
			temp.a = 0; temp.b = 0; temp.last = -1; temp.step = 0;
			for (int i = 1; i <= 6; i++)
			{
				//1、a有水，倒空
				if (i == 1 && now.a > 0)
				{
					temp.a = 0;
					temp.b = now.b;
					temp.step = now.step + 1;
					temp.last = front;
					//q.push(temp);
					/*if (!vis[temp.a][temp.b])
					{
						q[rear] = temp; rear++;
					}*/
				}
				//2、b有水，倒空
				if (i == 2 && now.b > 0)
				{
					temp.a = now.a;
					temp.b = 0;
					temp.step = now.step + 1;
					temp.last = front;
					//q.push(temp);
					/*if (!vis[temp.a][temp.b])
					{
						q[rear] = temp; rear++;
					}*/
				}
				//3、a未满 a灌满水
				if (i == 3 && now.a < A)
				{
					temp.a = A;
					temp.b = now.b;
					temp.step = now.step + 1;
					temp.last = front;
					//q.push(temp);
					/*if (!vis[temp.a][temp.b])
					{
						q[rear] = temp; rear++;
					}*/
				}
				//4、b未满 b灌满水
				if (i == 4 && now.b < B)
				{
					temp.a = now.a;
					temp.b = B;
					temp.step = now.step + 1;
					temp.last = front;
					//q.push(temp);
					/*if (!vis[temp.a][temp.b])
					{
						q[rear] = temp; rear++;
					}*/
				}
				//5、a有水，b没满，a倒入b
				if (i == 5 && now.a > 0 && now.b < B)
				{
					if (now.a > B - now.b)
					{
						temp.a = now.a - (B - now.b);
						temp.b = B;
						temp.step = now.step + 1;
						temp.last = front;
						//q.push(temp);
						/*if (!vis[temp.a][temp.b])
						{
							q[rear] = temp; rear++;
						}*/
					}
					else
					{
						temp.a = 0;
						temp.b = now.a + now.b;
						temp.step = now.step + 1;
						temp.last = front;
						//q.push(temp);
						/*if (!vis[temp.a][temp.b])
						{
							q[rear] = temp; rear++;
						}*/
					}
				}
				//6、b有水，a没满，b倒入a
				if (i == 6 && now.a < A && now.b>0)
				{
					if (now.b > A - now.a)
					{
						temp.a = A;
						temp.b = now.b - (A - now.a);
						temp.step = now.step + 1;
						temp.last = front;
						//q.push(temp);
						/*if (!vis[temp.a][temp.b])
						{
							q[rear] = temp; rear++;
						}*/
					}
					else
					{
						temp.b = 0;
						temp.a = now.a + now.b;
						temp.step = now.step + 1;
						temp.last = front;
						//q.push(temp);
						/*if (!vis[temp.a][temp.b])
						{
							q[rear] = temp; rear++;
						}*/
					}
				}

				if (!vis[temp.a][temp.b])
				{
					q[rear] = temp; rear++;
				}
			}
		}
		front++;
	}
	return -1;
}

void displayPath(int i)
{
	if (q[i].last == -1)
	{
		cout << "(" << q[i].a << ", " << q[i].b << ") -> ";
		return;
	}
	displayPath(q[i].last);
	cout << "(" << q[i].a << ", " << q[i].b << ") ->" ;
}
//void displayPath(node x)
//{
//	if (x.last == -1)
//	{
//		cout << "(" << x.a << ", " << x.b << ") ->";
//		return;
//	}
//	displayPath(q[x.last]);
//	cout << "(" << x.a << ", " << x.b << ") -> ";
//}

int main()
{
	cin >> A >> B >> C;
	int t = bfs(0, 0);
	if (t != -1)
	{
		cout << "Yes" << endl;
		cout << "操作次数为：" << t << endl;
		//displayPath(q[rear]);
		displayPath(rear);
	}
	else
		cout << "Impossible" << endl;
	return 0;
}