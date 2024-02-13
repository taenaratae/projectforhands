#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include <Windows.h>
#define V 150
#define INF 1000
#define HOSA 24
#define HOSB 68
#define HOSC 106
#define HT_A 75
#define HT_B 80
#define HT_C 85
#define SAT_A 80
#define SAT_B 90
#define SAT_C 70
#define TR_A 90
#define TR_B 70
#define TR_C 80

int now[1] = { 0 };
int shortest[1] = { HOSA };
int patent[1] = { 56 };
char type[1];
int emergency[1];
int up[1] = { 0 };

int busy[V];
int quality[V];
int adj[V][V];
float d[V];
int s[V + 1];
int parents[V];
int path[V];
float cost[V];

void update(int adj[V][V])
{
	for (int i = 1; i <= V; ++i)
	{
		for (int j = 1; j <= V; ++j)
		{
			scanf("%d", &adj[i - 1][j - 1]);
		}
	}
}
void initiate(int patent[1], int adj[V][V], float d[V], int parents[V], int path[V], int s[V + 1])
{
	s[0] = 1;
	for (int i = 1; i <= V; ++i)
	{
		d[i - 1] = adj[patent[0] - 1][i - 1];
		if (i != patent[0])
		{
			parents[i - 1] = patent[0];
			s[i] = NULL;
			path[i - 1] = NULL;
		}
		else
		{
			parents[i - 1] = NULL;
			s[i] = 1;
			path[i - 1] = NULL;
		}
	}

}
void dijkstra(int patent[1], float d[V], int adj[V][V], int s[V + 1], int parents[V])
{
	for (;;)
	{
		int smallest;
		if (s[0] != V)
		{
			for (int x = 1; x <= V; ++x)
			{
				if (s[x] != 1)
				{
					smallest = x; break;
				}
			}
			for (int k = 1; k <= V; ++k)
			{
				if (s[k] != 1)
				{
					if (d[smallest - 1] > d[k - 1])
					{
						smallest = k;
					}
				}
			}
			s[smallest] = 1; ++s[0];

			for (int j = 1; j <= V; ++j)
			{
				if (s[j] != 1)
				{
					if (d[j - 1] > d[smallest - 1] + adj[smallest - 1][j - 1])
					{
						d[j - 1] = d[smallest - 1] + adj[smallest - 1][j - 1];
						parents[j - 1] = smallest;
					}
				}
			}
		}
		else { break; }
	}
}
void findpath(int shortest[1], int path[V], int parents[V])
{
	int cur = shortest[0]; ++path[0]; path[path[0]] = cur;
	while (parents[cur - 1] != NULL)
	{
		++path[0];
		path[path[0]] = parents[cur - 1];
		cur = parents[cur - 1];
	}
}
void showpath(int shortest[1], float d[V], int parents[V], int path[V])
{
	printf("[Time To Arrive : %d min]", (int)d[shortest[0] - 1]);
	printf("\n[Path : ");
	findpath(shortest, path, parents);
	for (int i = path[0] + 1; i >= 3; --i)
	{
		printf("%d -> ", path[i - 1]);
	}
	printf("%d]\n", shortest[0]);
}
void recommend(int shortest[1], int emergency[1], float d[V], float cost[V])
{
	shortest[0] = HOSA;
	if (emergency[0] == 1)
	{
		cost[HOSA - 1] = (100 - 0.99 * d[HOSA - 1]) * (1 * HT_A);
		cost[HOSB - 1] = (100 - 0.99 * d[HOSB - 1]) * (1 * HT_B);
		cost[HOSC - 1] = (100 - 0.99 * d[HOSC - 1]) * (1 * HT_C);
	}
	else if (emergency[0] == 2)
	{
		cost[HOSA - 1] = (100 - 0.99 * d[HOSA - 1]) * (0.75 * HT_A + 0.148 * SAT_A + 0.102 * TR_A);
		cost[HOSB - 1] = (100 - 0.99 * d[HOSB - 1]) * (0.75 * HT_B + 0.148 * SAT_B + 0.102 * TR_B);
		cost[HOSC - 1] = (100 - 0.99 * d[HOSC - 1]) * (0.75 * HT_C + 0.148 * SAT_C + 0.102 * TR_C);
	}
	else if (emergency[0] == 3)
	{
		cost[HOSA - 1] = (100 - 0.75 * d[HOSA - 1]) * (0.50 * HT_A + 0.296 * SAT_A + 0.204 * TR_A);
		cost[HOSB - 1] = (100 - 0.75 * d[HOSB - 1]) * (0.50 * HT_B + 0.296 * SAT_B + 0.204 * TR_B);
		cost[HOSC - 1] = (100 - 0.75 * d[HOSC - 1]) * (0.50 * HT_C + 0.296 * SAT_C + 0.204 * TR_C);
	}
	else if (emergency[0] == 4)
	{
		cost[HOSA - 1] = (100 - 0.25 * d[HOSA - 1]) * (0.25 * HT_A + 0.444 * SAT_A + 0.306 * TR_A);
		cost[HOSB - 1] = (100 - 0.25 * d[HOSB - 1]) * (0.25 * HT_B + 0.444 * SAT_B + 0.306 * TR_B);
		cost[HOSC - 1] = (100 - 0.25 * d[HOSC - 1]) * (0.25 * HT_C + 0.444 * SAT_C + 0.306 * TR_C);
	}
	else if (emergency[0] == 5)
	{
		cost[HOSA - 1] = (0.592 * SAT_A + 0.408 * TR_A);
		cost[HOSB - 1] = (0.592 * SAT_B + 0.408 * TR_B);
		cost[HOSC - 1] = (0.592 * SAT_C + 0.408 * TR_C);
	}

	if (cost[shortest[0] - 1] < cost[HOSB - 1]) { shortest[0] = HOSB; }
	if (cost[shortest[0] - 1] < cost[HOSC - 1]) { shortest[0] = HOSC; }
	if (cost[shortest[0] - 1] >= 10000)
	{
		shortest[0] = 999;	// 어떤 병원으로도 갈 수 없는
	}
	printf("\t\t\t[Best Hospital : %d]\n", shortest[0]);
	//printf("\t\t\t[Hospital A: %d]\n", d[HOSA - 1]);
	//printf("\t\t\t[Hospital B: %d]\n", d[HOSB - 1]);
	//printf("\t\t\t[Hospital C: %d]\n", d[HOSC - 1]);
}
void navigation(int now[1], int shortest[1], int patent[1], int up[1], int golden[1], int adj[V][V], float d[V], int s[V + 1], int parents[V], int path[V])
{
	printf("-----------------------| Navigation |------------------------\n"); ++up[0];
	printf("\t\t[Time : %d min],\t[Patent : %d node]\n\n", now[0], patent[0]);
	update(adj);
	printf("-------------------------------------------------------------\n");
	initiate(patent, adj, d, parents, path, s);
	dijkstra(patent, d, adj, s, parents);
	recommend(shortest, emergency, d, cost);
	if (shortest[0] != 999)
	{
		showpath(shortest, d, parents, path);
		printf("-------------------------------------------------------------\n");
		printf("//////////////////////////////////////////////////////////\n\n\n\n\n\n");
		now[0] = now[0] + adj[patent[0] - 1][path[path[0] - 1] - 1];
		patent[0] = path[path[0] - 1];
	}
}
void vitalcheck(char type[1], int emergency[1])
{
	printf("----------------------| Vital Check |------------------------\n");
	printf("\t\t\t@ Type : "); scanf_s("%c", &type[0]);
	printf("\t\t\t@ KTAS : "); scanf_s("%d", &emergency[0]);
	printf("-------------------------------------------------------------\n");
	printf("//////////////////////////////////////////////////////////\n\n\n\n\n\n");
}
void report(char type[1], int emergency[1], int patent[1], int up[1], int busy[V], int quality[V])
{
	char hos;  if (patent[0] == HOSA) { hos = 'A'; } if (patent[0] == HOSB) { hos = 'B'; } if (patent[0] == HOSC) { hos = 'C'; }
	printf("-----------------------| Report |------------------------\n");
	printf("\t\t@ Time : %d min\n\t\t@ Patent : %c Hospital\n", now[0], hos);
	printf("\t\t@ Type : %c\n\t\t@ KTAS : %d \n", type[0], emergency[0]);
	printf("\t\t@ Update : %d \n", up[0]);
	if (patent[0] == HOSA) { printf("\t\t@ Qualtiy of Hospital : %d \n\t\t@ Free of Hospital : %d \n\t\t@ Reliablility of Hospital : %d\n", HT_A, SAT_A, TR_A); }
	else if (patent[0] == HOSB) { printf("\t\t@ Qualtiy of Hospital : %d \n\t\t@ Free of Hospital : %d \n\t\t@ Reliablility of Hospital : %d\n", HT_B, SAT_B, TR_B); }
	else if (patent[0] == HOSC) { printf("\t\t@ Qualtiy of Hospital : %d \n\t\t@ Free of Hospital : %d \n\t\t@ Reliablility of Hospital : %d\n", HT_C, SAT_C, TR_C); }
	printf("----------------------------------------------------------\n");
	printf("//////////////////////////////////////////////////////////\n\n\n\n\n\n");
}

int main() {

	LARGE_INTEGER ticksPerSec;
	LARGE_INTEGER c_start, c_end, diff;
	QueryPerformanceFrequency(&ticksPerSec);
	QueryPerformanceCounter(&c_start); QueryPerformanceCounter(&c_end); diff.QuadPart = c_end.QuadPart - c_start.QuadPart;
	vitalcheck(type, emergency);

	for (;;)
	{
		if (shortest[0] == 999) { printf("\t\t\t[No Possible Hospital !!!]\n\n\n\n\n"); break; }
		else if (patent[0] == shortest[0])
		{
			QueryPerformanceCounter(&c_start);

			printf("-----------------------| Navigation |------------------------\n");
			printf("\t\t[Time : %d min],\t[Patent : %d node]\n", now[0], patent[0]);
			printf("\t\t[We arrived at %d Hospital !!!]\n", patent[0]);
			printf("----------------------------------------------------------\n");
			printf("//////////////////////////////////////////////////////////\n\n\n\n\n\n");

			QueryPerformanceCounter(&c_end);
			diff.QuadPart += c_end.QuadPart - c_start.QuadPart;
			break;
		}
		else
		{
			QueryPerformanceCounter(&c_start);

			printf("-----------------------| Navigation |------------------------\n"); ++up[0];
			printf("\t\t[Time : %d min],\t[Patent : %d node]\n\n", now[0], patent[0]);

			QueryPerformanceCounter(&c_end);
			diff.QuadPart += c_end.QuadPart - c_start.QuadPart;

			update(adj);		// SCAN

			QueryPerformanceCounter(&c_start);

			printf("-------------------------------------------------------------\n");
			initiate(patent, adj, d, parents, path, s);
			dijkstra(patent, d, adj, s, parents);
			recommend(shortest, emergency, d, cost);
			if (shortest[0] != 999)
			{
				showpath(shortest, d, parents, path);
				printf("-------------------------------------------------------------\n");
				printf("//////////////////////////////////////////////////////////\n\n\n\n\n\n");
				now[0] = now[0] + adj[patent[0] - 1][path[path[0] - 1] - 1];
				patent[0] = path[path[0] - 1];
			}

			QueryPerformanceCounter(&c_end);
			diff.QuadPart += c_end.QuadPart - c_start.QuadPart;
		}
	}

	report(type, emergency, patent, up, busy, quality);

	printf("time: %.12f sec\n\n", ((double)diff.QuadPart / (double)ticksPerSec.QuadPart));

	return 0;
}
