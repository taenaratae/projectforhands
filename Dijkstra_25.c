#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include <Windows.h>
#define V 25
#define INF 1000
#define HOSA 1
#define HOSB 11
#define HOSC 25
#define HT_A 75
#define HT_B 80
#define HT_C 85
#define SAT_A 80
#define SAT_B 85
#define SAT_C 75
#define TR_A 85
#define TR_B 75
#define TR_C 80

int shortest[1] = { HOSA };
int now[1] = { 0 };
char type[1];
int emergency[1];
int up[1] = { 0 };

typedef struct plusint {
	int node;
	int data;
}plusint;

typedef struct plusfloat {
	int node;
	float data;
}plusfloat;


int busy[V];
int quality[V];
int adj[V][V];
plusfloat d[V];
int s[V + 1];
int parents[V];
plusint path[V];
float cost[V];
plusint patent[V + 1];


void modify(plusfloat d[V], plusint path[V], plusint patent[V + 1])
{
	d[1 - 1].node = 24; 	 path[1 - 1].node = 24;	   patent[1 - 1].node = 24;
	d[2 - 1].node = 26; 	 path[2 - 1].node = 26;	   patent[2 - 1].node = 26;
	d[3 - 1].node = 28; 	 path[3 - 1].node = 28;	   patent[3 - 1].node = 28;
	d[4 - 1].node = 30; 	 path[4 - 1].node = 30;	   patent[4 - 1].node = 30;
	d[5 - 1].node = 35; 	 path[5 - 1].node = 35;	   patent[5 - 1].node = 35;
	d[6 - 1].node = 38; 	 path[6 - 1].node = 38;	   patent[6 - 1].node = 38;
	d[7 - 1].node = 46; 	 path[7 - 1].node = 46;	   patent[7 - 1].node = 46;
	d[8 - 1].node = 56;		 path[8 - 1].node = 56;	   patent[8 - 1].node = 56;
	d[9 - 1].node = 63; 	 path[9 - 1].node = 63;	   patent[9 - 1].node = 63;
	d[10 - 1].node = 64; 	 path[10 - 1].node = 64;	   patent[10 - 1].node = 64;
	d[11 - 1].node = 68; 	 path[11 - 1].node = 68;	   patent[11 - 1].node = 68;
	d[12 - 1].node = 69; 	 path[12 - 1].node = 69;	   patent[12 - 1].node = 69;
	d[13 - 1].node = 71; 	 path[13 - 1].node = 71;	   patent[13 - 1].node = 71;
	d[14 - 1].node = 72; 	 path[14 - 1].node = 72;	   patent[14 - 1].node = 72;
	d[15 - 1].node = 76; 	 path[15 - 1].node = 76;	   patent[15 - 1].node = 76;
	d[16 - 1].node = 77;	 path[16 - 1].node = 77;	   patent[16 - 1].node = 77;
	d[17 - 1].node = 86; 	 path[17 - 1].node = 86;	   patent[17 - 1].node = 86;
	d[18 - 1].node = 92; 	 path[18 - 1].node = 92;	   patent[18 - 1].node = 92;
	d[19 - 1].node = 94; 	 path[19 - 1].node = 94;	   patent[19 - 1].node = 94;
	d[20 - 1].node = 96; 	 path[20 - 1].node = 96;	   patent[20 - 1].node = 96;
	d[21 - 1].node = 100; 	 path[21 - 1].node = 100;	   patent[21 - 1].node = 100;
	d[22 - 1].node = 102; 	 path[22 - 1].node = 102;	   patent[22 - 1].node = 102;
	d[23 - 1].node = 104; 	 path[23 - 1].node = 104;	   patent[23 - 1].node = 104;
	d[24 - 1].node = 105; 	 path[24 - 1].node = 105;	   patent[24 - 1].node = 105;
	d[25 - 1].node = 106;	 path[25 - 1].node = 106;	   patent[25 - 1].node = 106;

	patent[26 - 1].data = 8;// 출발 위치
}

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
void initiate(plusint patent[V + 1], int adj[V][V], plusfloat d[V], int parents[V], plusint path[V], int s[V + 1])
{
	s[0] = 1;
	for (int i = 1; i <= V; ++i)
	{
		d[i - 1].data = adj[patent[25].data - 1][i - 1];
		if (i != patent[25].data)
		{
			parents[i - 1] = patent[25].data;
			s[i] = NULL;
			path[i - 1].data = NULL;
		}
		else
		{
			parents[i - 1] = NULL;
			s[i] = 1;
			path[i - 1].data = NULL;
		}
	}

}
void dijkstra(plusint patent[V + 1], plusfloat d[V], int adj[V][V], int s[V + 1], int parents[V])
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
					if (d[smallest - 1].data > d[k - 1].data)
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
					if (d[j - 1].data > d[smallest - 1].data + adj[smallest - 1][j - 1])
					{
						d[j - 1].data = d[smallest - 1].data + adj[smallest - 1][j - 1];
						parents[j - 1] = smallest;
					}
				}
			}
		}
		else { break; }
	}
}
void findpath(int shortest[1], plusint path[V], int parents[V])
{
	int cur = shortest[0]; ++path[0].data; path[path[0].data].data = cur;
	while (parents[cur - 1] != NULL)
	{
		++path[0].data;
		path[path[0].data].data = parents[cur - 1];
		cur = parents[cur - 1];
	}
}
void showpath(int shortest[1], plusfloat d[V], int parents[V], plusint path[V])
{
	printf("[Time To Arrive : %d min]", (int)d[shortest[0] - 1].data);
	printf("\n[Path : ");
	findpath(shortest, path, parents);
	for (int i = path[0].data + 1; i >= 3; --i)
	{
		printf("%d -> ", path[path[i - 1].data - 1].node);
	}
	printf("%d]\n", shortest[0]);
}
void recommend(int shortest[1], int emergency[1], plusfloat d[V], float cost[V])
{
	shortest[0] = HOSA;
	if (emergency[0] == 1)
	{
		cost[HOSA - 1] = (100 - 0.99 * d[HOSA - 1].data) * (1 * HT_A);
		cost[HOSB - 1] = (100 - 0.99 * d[HOSB - 1].data) * (1 * HT_B);
		cost[HOSC - 1] = (100 - 0.99 * d[HOSC - 1].data) * (1 * HT_C);
	}
	else if (emergency[0] == 2)
	{
		cost[HOSA - 1] = (100 - 0.99 * d[HOSA - 1].data) * (0.75 * HT_A + 0.148 * SAT_A + 0.102 * TR_A);
		cost[HOSB - 1] = (100 - 0.99 * d[HOSB - 1].data) * (0.75 * HT_B + 0.148 * SAT_B + 0.102 * TR_B);
		cost[HOSC - 1] = (100 - 0.99 * d[HOSC - 1].data) * (0.75 * HT_C + 0.148 * SAT_C + 0.102 * TR_C);
	}
	else if (emergency[0] == 3)
	{
		cost[HOSA - 1] = (100 - 0.75 * d[HOSA - 1].data) * (0.50 * HT_A + 0.296 * SAT_A + 0.204 * TR_A);
		cost[HOSB - 1] = (100 - 0.75 * d[HOSB - 1].data) * (0.50 * HT_B + 0.296 * SAT_B + 0.204 * TR_B);
		cost[HOSC - 1] = (100 - 0.75 * d[HOSC - 1].data) * (0.50 * HT_C + 0.296 * SAT_C + 0.204 * TR_C);
	}
	else if (emergency[0] == 4)
	{
		cost[HOSA - 1] = (100 - 0.25 * d[HOSA - 1].data) * (0.25 * HT_A + 0.444 * SAT_A + 0.306 * TR_A);
		cost[HOSB - 1] = (100 - 0.25 * d[HOSB - 1].data) * (0.25 * HT_B + 0.444 * SAT_B + 0.306 * TR_B);
		cost[HOSC - 1] = (100 - 0.25 * d[HOSC - 1].data) * (0.25 * HT_C + 0.444 * SAT_C + 0.306 * TR_C);
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
	if (shortest[0] == 1) { printf("\t\t\t[Best Hospital : %d]\n", shortest[0] + 23); }
	if (shortest[0] == 11) { printf("\t\t\t[Best Hospital : %d]\n", shortest[0] + 57); }
	if (shortest[0] == 25) { printf("\t\t\t[Best Hospital : %d]\n", shortest[0] + 81); }
	// printf("\t\t\t[Best Hospital : %d]\n", shortest[0]);
}

void vitalcheck(char type[1], int emergency[1])
{
	printf("----------------------| Vital Check |------------------------\n");
	printf("\t\t\t@ Type : "); scanf_s("%c", &type[0]);
	printf("\t\t\t@ KTAS : "); scanf_s("%d", &emergency[0]);
	printf("-------------------------------------------------------------\n");
	printf("//////////////////////////////////////////////////////////\n\n\n\n\n\n");
}
void report(char type[1], int emergency[1], plusint patent[V + 1], int up[1], int busy[V], int quality[V])
{
	char hos;  if (patent[25].data == HOSA) { hos = 'A'; } if (patent[25].data == HOSB) { hos = 'B'; } if (patent[25].data == HOSC) { hos = 'C'; }
	printf("-----------------------| Report |------------------------\n");
	printf("\t\t@ Time : %d min\n\t\t@ Patent : %c Hospital\n", now[0], hos);
	printf("\t\t@ Type : %c\n\t\t@ KTAS : %d \n", type[0], emergency[0]);
	printf("\t\t@ Update : %d \n", up[0]);
	if (patent[25].data == HOSA) { printf("\t\t@ Qualtiy of Hospital : %d \n\t\t@ Free of Hospital : %d \n\t\t@ Reliablility of Hospital : %d\n", HT_A, SAT_A, TR_A); }
	else if (patent[25].data == HOSB) { printf("\t\t@ Qualtiy of Hospital : %d \n\t\t@ Free of Hospital : %d \n\t\t@ Reliablility of Hospital : %d\n", HT_B, SAT_B, TR_B); }
	else if (patent[25].data == HOSC) { printf("\t\t@ Qualtiy of Hospital : %d \n\t\t@ Free of Hospital : %d \n\t\t@ Reliablility of Hospital : %d\n", HT_C, SAT_C, TR_C); }
	printf("----------------------------------------------------------\n");
	printf("//////////////////////////////////////////////////////////\n\n\n\n\n\n");
}

int main() {

	LARGE_INTEGER ticksPerSec;
	LARGE_INTEGER c_start, c_end, diff;
	QueryPerformanceFrequency(&ticksPerSec);
	QueryPerformanceCounter(&c_start); modify(d, path, patent); QueryPerformanceCounter(&c_end); diff.QuadPart = c_end.QuadPart - c_start.QuadPart;
	vitalcheck(type, emergency);

	for (;;)
	{
		if (shortest[0] == 999) { printf("\t\t\t[No Possible Hospital !!!]\n\n\n\n\n"); break; }
		else if (patent[25].data == shortest[0])
		{
			QueryPerformanceCounter(&c_start);

			printf("-----------------------| Navigation |------------------------\n");
			printf("\t\t[Time : %d min],\t[Patent : %d node]\n", now[0], patent[patent[25].data - 1].node);
			printf("\t\t[We arrived at %d Hospital !!!]\n", patent[patent[25].data - 1].node);
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
			printf("\t\t[Time : %d min],\t[Patent : %d node]\n\n", now[0], patent[patent[25].data - 1].node);

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
				now[0] = now[0] + adj[patent[25].data - 1][path[path[0].data - 1].data - 1];
				patent[25].data = path[path[0].data - 1].data;
			}

			QueryPerformanceCounter(&c_end);
			diff.QuadPart += c_end.QuadPart - c_start.QuadPart;
		}
	}

	report(type, emergency, patent, up, busy, quality);

	printf("time: %.12f sec\n\n", ((double)diff.QuadPart / (double)ticksPerSec.QuadPart));

	return 0;
}
