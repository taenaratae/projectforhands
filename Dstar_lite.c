#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <Windows.h>

#define HOSA 24
#define HOSB 68
#define HOSC 106
#define V 150

#define HT_A 85
#define HT_B 80
#define HT_C 75
#define SAT_A 75
#define SAT_B 85
#define SAT_C 80
#define TR_A 80
#define TR_B 75
#define TR_C 85

typedef struct node node;
typedef struct list_node list_node;
typedef struct queue queue;

struct list_node {
	int index;
	int cost;
	list_node* next;
};

struct node {
	int A_G;
	int A_RHS;
	int B_G;
	int B_RHS;
	int C_G;
	int C_RHS;
	int x;
	int y;
	list_node* adj;
};

struct queue {
	int index;
	double key;
};

void push_heap_A(int u);
void push_heap_B(int u);
void push_heap_C(int u);
int pop_heap_A();
int pop_heap_B();
int pop_heap_C();
double topkey(queue* openlist, int openlist_size);
void swap(queue* openlist, int a, int b);
void heapify(queue* openlist, int n, int i);
void minheapSort(queue* openlist, int n);
void insert_vertex(node* s, int i, int c);
int find_cost(int u, int v);
void update_A(int u);
void update_B(int u);
void update_C(int u);
double calculate_key_A(u);
double calculate_key_B(u);
double calculate_key_C(u);
void shortestpath_A();
void shortestpath_B();
void shortestpath_C();

int start;
int goal;
int last;

double km_A;
double km_B;
double km_C;

node s[V + 5];

char type[1];
int emergency[1];
int up[1] = { 0 };
int golden[1];
int quality[V + 1];
int busy[V + 1];
double v_cost[V + 1];
int shortest;

double h(int u, int v) {
	int x_distance = s[v].x - s[u].x;
	int y_distance = s[v].y - s[u].y;
	x_distance = x_distance * x_distance;
	y_distance = y_distance * y_distance;
	double x_dis = (double)(x_distance / 40000);
	double y_dis = (double)(y_distance / 40000);
	double result = sqrt(x_dis + y_dis);
	return result;
}

void vitalcheck(char type[1], int emergency[1])
{
	printf("----------------------| Vital Check |------------------------\n");
	printf("\t\t\t@ Type : "); scanf("%c", &type[0]);
	printf("\t\t\t@ KTAS : "); scanf("%d", &emergency[0]);
	printf("-------------------------------------------------------------\n");
	printf("//////////////////////////////////////////////////////////\n\n\n\n\n\n");
}

void report(char type[1], int emergency[1], int patent, int up[1], int busy[V], int quality[V], int now)
{
	char hos;  if (patent == HOSA) { hos = 'A'; } if (patent == HOSB) { hos = 'B'; } if (patent == HOSC) { hos = 'C'; }
	printf("-----------------------| Report |------------------------\n");
	printf("\t\t@ Time : %d min\n\t\t@ Patent : %c Hospital\n", now, hos);
	printf("\t\t@ Type : %c\n\t\t@ KTAS : %d \n", type[0], emergency[0]);
	printf("\t\t@ Update : %d \n", up[0]);
	if (patent == HOSA) { printf("\t\t@ Qualtiy of Hospital : %d \n\t\t@ Free of Hospital : %d \n\t\t@ Reliablility of Hospital : %d\n", HT_A, SAT_A, TR_A); }
	else if (patent == HOSB) { printf("\t\t@ Qualtiy of Hospital : %d \n\t\t@ Free of Hospital : %d \n\t\t@ Reliablility of Hospital : %d\n", HT_B, SAT_B, TR_B); }
	else if (patent == HOSC) { printf("\t\t@ Qualtiy of Hospital : %d \n\t\t@ Free of Hospital : %d \n\t\t@ Reliablility of Hospital : %d\n", HT_C, SAT_C, TR_C); }
	printf("----------------------------------------------------------\n");
	printf("//////////////////////////////////////////////////////////\n\n\n\n\n\n");
}
void recommend(int u)
{
	shortest = HOSA;
	if (emergency[0] == 1)
	{
		v_cost[HOSA] = (100 - 0.99 * (double)s[u].A_G) * (1 * HT_A);
		v_cost[HOSB] = (100 - 0.99 * (double)s[u].B_G) * (1 * HT_B);
		v_cost[HOSC] = (100 - 0.99 * (double)s[u].C_G) * (1 * HT_C);
	}
	else if (emergency[0] == 2)
	{
		v_cost[HOSA] = (100 - 0.99 * (double)s[u].A_G) * (0.75 * HT_A + 0.148 * SAT_A + 0.102 * TR_A);
		v_cost[HOSB] = (100 - 0.99 * (double)s[u].B_G) * (0.75 * HT_B + 0.148 * SAT_B + 0.102 * TR_B);
		v_cost[HOSC] = (100 - 0.99 * (double)s[u].C_G) * (0.75 * HT_C + 0.148 * SAT_C + 0.102 * TR_C);
	}
	else if (emergency[0] == 3)
	{
		v_cost[HOSA] = (100 - 0.75 * (double)s[u].A_G) * (0.50 * HT_A + 0.296 * SAT_A + 0.204 * TR_A);
		v_cost[HOSB] = (100 - 0.75 * (double)s[u].B_G) * (0.50 * HT_B + 0.296 * SAT_B + 0.204 * TR_B);
		v_cost[HOSC] = (100 - 0.75 * (double)s[u].C_G) * (0.50 * HT_C + 0.296 * SAT_C + 0.204 * TR_C);
	}
	else if (emergency[0] == 4)
	{
		v_cost[HOSA] = (100 - 0.25 * (double)s[u].A_G) * (0.25 * HT_A + 0.444 * SAT_A + 0.306 * TR_A);
		v_cost[HOSB] = (100 - 0.25 * (double)s[u].B_G) * (0.25 * HT_B + 0.444 * SAT_B + 0.306 * TR_B);
		v_cost[HOSC] = (100 - 0.25 * (double)s[u].C_G) * (0.25 * HT_C + 0.444 * SAT_C + 0.306 * TR_C);
	}
	else if (emergency[0] == 5)
	{
		v_cost[HOSA] = (0.592 * SAT_A + 0.408 * TR_A);
		v_cost[HOSB] = (0.592 * SAT_B + 0.408 * TR_B);
		v_cost[HOSC] = (0.592 * SAT_C + 0.408 * TR_C);
	}

	if (v_cost[shortest] < v_cost[HOSB]) { shortest = HOSB; }
	if (v_cost[shortest] < v_cost[HOSC]) { shortest = HOSC; }
	if (v_cost[shortest] >= 10000)
	{
		shortest = 999;
	}

	printf("HOSA cost: %f\n", v_cost[HOSA]);
	printf("HOSB cost: %f\n", v_cost[HOSB]);
	printf("HOSC cost: %f\n", v_cost[HOSC]);

	if (shortest == 1) { printf("\t\t\t[Best Hospital : %d]\n", shortest); }
	if (shortest == 11) { printf("\t\t\t[Best Hospital : %d]\n", shortest); }
	if (shortest == 25) { printf("\t\t\t[Best Hospital : %d]\n", shortest); }
}

queue openlist_A[V * 3];
queue openlist_B[V * 3];
queue openlist_C[V * 3];
int openlist_size_A = 0;
int openlist_size_B = 0;
int openlist_size_C = 0;


void push_heap_A(int u) {
	++openlist_size_A;
	int i = openlist_size_A;

	while (i != 1 && openlist_A[i / 2].key > calculate_key_A(u)) {
		openlist_A[i].key = openlist_A[i / 2].key;
		openlist_A[i].index = openlist_A[i / 2].index;
		i /= 2;
	}
	openlist_A[i].index = u;
	openlist_A[i].key = calculate_key_A(u);

	return;
}

void push_heap_B(int u) {
	++openlist_size_B;
	int i = openlist_size_B;

	while (i != 1 && openlist_B[i / 2].key > calculate_key_B(u)) {
		openlist_B[i].key = openlist_B[i / 2].key;
		openlist_B[i].index = openlist_B[i / 2].index;
		i /= 2;
	}
	openlist_B[i].index = u;
	openlist_B[i].key = calculate_key_B(u);

	return;
}

void push_heap_C(int u) {
	++openlist_size_C;
	int i = openlist_size_C;

	while (i != 1 && openlist_C[i / 2].key > calculate_key_C(u)) {
		openlist_C[i].key = openlist_C[i / 2].key;
		openlist_C[i].index = openlist_C[i / 2].index;
		i /= 2;
	}
	openlist_C[i].index = u;
	openlist_C[i].key = calculate_key_C(u);

	return;
}

int pop_heap_A() {
	if (openlist_size_A == 0) return 0;

	int value = openlist_A[1].index;
	openlist_A[1] = openlist_A[openlist_size_A];
	openlist_size_A--;

	int parent = 1;
	int child;

	while (1) {
		child = parent * 2;
		if (child + 1 <= openlist_size_A && openlist_A[child].key > openlist_A[child + 1].key) {
			child++;
		}

		if (child > openlist_size_A || openlist_A[child].key > openlist_A[parent].key) {
			break;
		}
		else {
			int temp_index = openlist_A[child].index;
			double temp_key = openlist_A[child].key;
			openlist_A[child].index = openlist_A[parent].index;
			openlist_A[child].key = openlist_A[parent].key;
			openlist_A[parent].index = temp_index;
			openlist_A[parent].key = temp_key;
			parent = child;
		}
	}
	return value;
}

int pop_heap_B() {
	if (openlist_size_B == 0) return -1;

	int value = openlist_B[1].index;
	openlist_B[1] = openlist_B[openlist_size_B--];

	int parent = 1;
	int child;

	while (1) {
		child = parent * 2;
		if (child + 1 <= openlist_size_B && openlist_B[child].key > openlist_B[child + 1].key) {
			child++;
		}

		if (child > openlist_size_B || openlist_B[child].key > openlist_B[parent].key) {
			break;
		}
		else {
			int temp_index = openlist_B[child].index;
			double temp_key = openlist_B[child].key;
			openlist_B[child].index = openlist_B[parent].index;
			openlist_B[child].key = openlist_B[parent].key;
			openlist_B[parent].index = temp_index;
			openlist_B[parent].key = temp_key;
			parent = child;
		}
	}
	return value;
}

int pop_heap_C() {
	if (openlist_size_C == 0) return 0;

	int value = openlist_C[1].index;
	openlist_C[1] = openlist_C[openlist_size_C--];

	int parent = 1;
	int child;

	while (1) {
		child = parent * 2;
		if (child + 1 <= openlist_size_C && openlist_C[child].key > openlist_C[child + 1].key) {
			child++;
		}

		if (child > openlist_size_C || openlist_C[child].key > openlist_C[parent].key) {
			break;
		}
		else {
			int temp_index = openlist_C[child].index;
			double temp_key = openlist_C[child].key;
			openlist_C[child].index = openlist_C[parent].index;
			openlist_C[child].key = openlist_C[parent].key;
			openlist_C[parent].index = temp_index;
			openlist_C[parent].key = temp_key;
			parent = child;
		}
	}
	return value;
}

double topkey(queue* openlist, int openlist_size) {
	if (openlist_size == 0)
		return 100000;
	else
		return openlist[1].key;
}


void swap(queue* openlist, int a, int b) {

	double temp_key = openlist[a].key;
	int temp_index = openlist[a].index;
	openlist[a].key = openlist[b].key;
	openlist[a].index = openlist[b].index;
	openlist[b].key = temp_key;
	openlist[b].index = temp_index;

	return;
}
void heapify(queue* openlist, int n, int i) {
	int smallest = i;
	int left = 2 * i;
	int right = 2 * i + 1;

	if (left <= n && openlist[left].key < openlist[smallest].key)
		smallest = left;

	if (right <= n && openlist[right].key < openlist[smallest].key)
		smallest = right;

	if (smallest != i) {
		swap(openlist, i, smallest);
		heapify(openlist, n, smallest);
	}
}

void build_heap(queue* openlist, int n) {
	for (int i = n / 2; i >= 1; i--)
		heapify(openlist, n, i);
}

void minheapSort(queue* openlist, int n) {
	build_heap(openlist, n);

	for (int j = n; j >= 2; j--) {
		swap(openlist, 1, j);
		heapify(openlist, j - 1, 1);
	}
	return;
}


void insert_vertex(node* s, int i, int c) {
	list_node* newNode = (list_node*)malloc(sizeof(list_node));    // newNode 할당 
	newNode->index = i;    // 데이터 할당 
	newNode->cost = c;
	newNode->next = NULL;    // next 포인터 초기화 

	if (s->adj == NULL) {    // empty
		s->adj = newNode;
	}
	else {
		newNode->next = s->adj;
		s->adj = newNode;
	}

}

void delete_vertex(node* s, int i) {
	list_node* ptr = s->adj;
	if (ptr->index == i) {
		s->adj = s->adj->next;
		free(ptr);
		return;
	}
	while (ptr->next->index != i)
		ptr = ptr->next;
	list_node* temp = ptr->next;

	if (ptr->next->next == NULL) {
		ptr->next = NULL;
	}
	else {
		ptr->next = ptr->next->next;
	}
	free(temp);
}

int find_cost(int u, int v) {

	list_node* ptr = s[u].adj;

	if (ptr == NULL)
		return 1000;
	else {
		while (v != ptr->index) {
			ptr = ptr->next;
			if (ptr == NULL)
				return 1000;
		}
		return ptr->cost;
	}

}

void initialize() {

	openlist_size_A = 0;
	openlist_size_B = 0;
	openlist_size_C = 0;

	km_A = 0;
	km_B = 0;
	km_C = 0;

	for (int i = 1; i <= V; i++) {
		s[i].A_RHS = 100000; s[i].A_G = 100000;
		s[i].B_RHS = 100000; s[i].B_G = 100000;
		s[i].C_RHS = 100000; s[i].C_G = 100000;
	}

	s[HOSA].A_RHS = 0;
	s[HOSB].B_RHS = 0;
	s[HOSC].C_RHS = 0;

	push_heap_A(HOSA);
	push_heap_B(HOSB);
	push_heap_C(HOSC);

}

double calculate_key_A(int u)
{
	if (s[u].A_G >= s[u].A_RHS)
		return (double)(s[u].A_RHS) + h(start, u) + km_A;
	else
		return (double)s[u].A_G + h(start, u) + km_A;
}

double calculate_key_B(int u)
{
	if (s[u].B_G >= s[u].B_RHS)
		return (double)(s[u].B_RHS) + h(start, u) + km_B;
	else
		return (double)(s[u].B_G) + h(start, u) + km_B;
}

double calculate_key_C(int u)
{
	if (s[u].C_G >= s[u].C_RHS)
		return (double)(s[u].C_RHS) + h(start, u) + km_C;
	else
		return (double)(s[u].C_G) + h(start, u) + km_C;
}

void update_A(int u) {

	list_node* ptr = s[u].adj;
	int min = 100000;

	if (u != HOSA) {
		while (ptr != NULL) {
			if (ptr->cost + s[ptr->index].A_G < min)
				min = ptr->cost + s[ptr->index].A_G;
			ptr = ptr->next;
		}
		if (min <= 100000) {
			s[u].A_RHS = min;
		}
	}

	for (int i = 1; i <= openlist_size_A; i++) {
		if (openlist_A[i].index == u) {
			openlist_A[i] = openlist_A[openlist_size_A--];
			minheapSort(openlist_A, openlist_size_A);
			break;
		}
	}

	if (s[u].A_G != s[u].A_RHS) {
		push_heap_A(u);
	}

}

void update_B(int u) {

	list_node* ptr = s[u].adj;
	int min = 100000;

	if (u != HOSB) {
		while (ptr != NULL) {
			if (ptr->cost + s[ptr->index].B_G < min)
				min = ptr->cost + s[ptr->index].B_G;
			ptr = ptr->next;
		}
		if (min <= 100000) {
			s[u].B_RHS = min;
		}
	}

	for (int i = 1; i <= openlist_size_B; i++) {
		if (openlist_B[i].index == u) {
			openlist_B[i] = openlist_B[openlist_size_B--];
			minheapSort(openlist_B, openlist_size_B);
			break;
		}
	}

	if (s[u].B_G != s[u].B_RHS) {
		push_heap_B(u);
	}

}

void update_C(int u) {

	list_node* ptr = s[u].adj;
	int min = 100000;

	if (u != HOSC) {
		while (ptr != NULL) {
			if (ptr->cost + s[ptr->index].C_G < min)
				min = ptr->cost + s[ptr->index].C_G;
			ptr = ptr->next;
		}

		if (min <= 100000) {
			s[u].C_RHS = min;
		}
	}

	for (int i = 1; i <= openlist_size_C; i++) {
		if (openlist_C[i].index == u) {
			openlist_C[i] = openlist_C[openlist_size_C--];
			minheapSort(openlist_C, openlist_size_C);
			break;
		}
	}

	if (s[u].C_G != s[u].C_RHS) {
		push_heap_C(u);
	}

}

void shortestpath_A() {

	int u;
	double k_old;
	list_node* cur;

	while (topkey(openlist_A, openlist_size_A) < calculate_key_A(start) || s[start].A_RHS != s[start].A_G) {
		k_old = topkey(openlist_A, openlist_size_A);
		u = pop_heap_A();
		if (k_old < calculate_key_A(u))
			push_heap_A(u);
		else if (s[u].A_G > s[u].A_RHS) {
			s[u].A_G = s[u].A_RHS;
			cur = s[u].adj;
			while (cur != NULL) {
				update_A(cur->index);
				cur = cur->next;
			}
		}
		else {
			s[u].A_G = 100000;
			update_A(u);
			cur = s[u].adj;
			while (cur != NULL) {
				update_A(cur->index);
				cur = cur->next;
			}
		}
	}

}

void shortestpath_B() {

	int u;
	double k_old;
	list_node* cur;

	while (topkey(openlist_B, openlist_size_B) < calculate_key_B(start) || s[start].B_RHS != s[start].B_G) {
		k_old = topkey(openlist_B, openlist_size_B);
		u = pop_heap_B();
		if (k_old < calculate_key_B(u))
			push_heap_B(u);
		else if (s[u].B_G > s[u].B_RHS) {
			s[u].B_G = s[u].B_RHS;
			cur = s[u].adj;
			while (cur != NULL) {
				update_B(cur->index);
				cur = cur->next;
			}
		}
		else {
			s[u].B_G = 100000;
			update_B(u);
			cur = s[u].adj;
			while (cur != NULL) {
				update_B(cur->index);
				cur = cur->next;
			}
		}
	}

}

void shortestpath_C() {

	int u;
	double k_old;
	list_node* cur;

	while (topkey(openlist_C, openlist_size_C) < calculate_key_C(start) || s[start].C_RHS != s[start].C_G) {
		k_old = topkey(openlist_C, openlist_size_C);
		u = pop_heap_C();
		if (k_old < calculate_key_C(u))
			push_heap_C(u);
		else if (s[u].C_G > s[u].C_RHS) {
			s[u].C_G = s[u].C_RHS;
			cur = s[u].adj;
			while (cur != NULL) {
				update_C(cur->index);
				cur = cur->next;
			}
		}
		else {
			s[u].C_G = 100000;
			update_C(u);
			cur = s[u].adj;
			while (cur != NULL) {
				update_C(cur->index);
				cur = cur->next;
			}
		}
	}

}

int find_argmin_A(int u) {

	list_node* ptr = s[u].adj;
	int min_value = 100000;
	int min_idx = 0;

	while (ptr != NULL) {
		if (ptr->cost + s[ptr->index].A_G < min_value) {
			min_value = ptr->cost + s[ptr->index].A_G;
			min_idx = ptr->index;
		}
		ptr = ptr->next;
	}

	return min_idx;
}

int find_argmin_B(int u) {

	list_node* ptr = s[u].adj;
	int min_value = 100000;
	int min_idx = 0;

	while (ptr != NULL) {
		if (ptr->cost + s[ptr->index].B_G < min_value) {
			min_value = ptr->cost + s[ptr->index].B_G;
			min_idx = ptr->index;
		}
		ptr = ptr->next;
	}

	return min_idx;
}

int find_argmin_C(int u) {

	list_node* ptr = s[u].adj;
	int min_value = 100000;
	int min_idx = 0;

	while (ptr != NULL) {
		if (ptr->cost + s[ptr->index].C_G < min_value) {
			min_value = ptr->cost + s[ptr->index].C_G;
			min_idx = ptr->index;
		}
		ptr = ptr->next;
	}

	return min_idx;
}

void update_cost(int u, int v, int c) {
	list_node* ptr = s[u].adj;

	while (ptr != NULL) {
		if (v == ptr->index) {
			ptr->cost = c;
			return;
		}
		else ptr = ptr->next;
	}

	return;
}

void main()
{

	LARGE_INTEGER ticksPerSec;
	LARGE_INTEGER c_start, c_end, diff;

	vitalcheck(type, emergency);

	printf("Enter the start point: ");
	scanf("%d", &start);

	last = start;

	int sum = 0;

	int cnt = 0;

	int c;

	int adjacent;
	int change = 0;

	int pos_A, pos_B, pos_C;

	//좌표 데이터 입력
	printf("좌표입력\n");
	for (int i = 1; i <= V; i++)
	{
		scanf("%d", &s[i].x);
		scanf("%d", &s[i].y);
	}

	//인접 행렬을 입력받으면 linked list 형태로 변환
	printf("현재 시간 0분. 인접 행렬 입력\n");
	for (int i = 1; i <= V; i++)
	{
		for (int j = 1; j <= V; j++)
		{
			scanf("%d", &adjacent);
			if (adjacent < 1000) {
				insert_vertex(&s[i], j, adjacent);
			}
		}
	}

	QueryPerformanceFrequency(&ticksPerSec);
	QueryPerformanceCounter(&c_start);

	initialize();

	shortestpath_A();

	shortestpath_B();

	shortestpath_C();

	recommend(start);

	pos_A = start;
	pos_B = start;
	pos_C = start;

	printf("-----------------------| Navigation |------------------------\n"); ++up[0];
	printf("\t\t[Time : %d min],\t[Patent : %d node]\n\n", sum, start);
	printf("To the hospital: A -> %d min, B -> %d min, C -> %d min \n\n", s[start].A_G, s[start].B_G, s[start].C_G);
	printf("-------------------------------------------------------------\n");
	printf("//////////////////////////////////////////////////////////\n\n\n\n\n\n");


	printf("To A: %d", start);
	while (pos_A != HOSA) {
		pos_A = find_argmin_A(pos_A);
		printf(" -> %d", pos_A);
		cnt++;
		if (cnt == 50) break;
	}
	printf("\n");
	cnt = 0;

	printf("To B: %d", start);
	while (pos_B != HOSB) {
		pos_B = find_argmin_B(pos_B);
		printf(" -> %d", pos_B);
		cnt++;
		if (cnt == 50) break;
	}
	printf("\n");
	cnt = 0;

	printf("To C: %d", start);
	while (pos_C != HOSC) {
		pos_C = find_argmin_C(pos_C);
		printf(" -> %d", pos_C);
		cnt++;
		if (cnt == 50) break;
	}
	printf("\n");

	goal = shortest;

	if (shortest == HOSA) {
		sum += find_cost(start, find_argmin_A(start));
		start = find_argmin_A(start);
	}
	else if (shortest == HOSB) {
		sum += find_cost(start, find_argmin_B(start));
		start = find_argmin_B(start);
	}
	else if (shortest == HOSC) {
		sum += find_cost(start, find_argmin_C(start));
		start = find_argmin_C(start);
	}
	else {
		printf("\t\t\t[No Possible Hospital !!!]\n\n\n\n\n");
		return;
	}

	printf("\n\nCurrent time: %d\n", sum);
	printf("moved to the node %d\n\n", start);

	QueryPerformanceCounter(&c_end);

	diff.QuadPart = c_end.QuadPart - c_start.QuadPart;

	while (start != goal) {

		printf("인접 행렬 입력\n");
		for (int i = 1; i <= V; i++)
		{
			for (int j = 1; j <= V; j++)
			{
				scanf("%d", &adjacent);

				QueryPerformanceCounter(&c_start);

				c = find_cost(i, j);
				if (adjacent != c) {
					change = 1;
					if (c == 1000) {
						insert_vertex(&s[i], j, adjacent);
						update_A(i);
						update_B(i);
						update_C(i);
					}
					else if (adjacent == 1000) {
						delete_vertex(&s[i], j);
						update_A(i);
						update_B(i);
						update_C(i);
					}
					else {
						update_cost(i, j, adjacent);
						update_A(i);
						update_B(i);
						update_C(i);
					}
				}

				QueryPerformanceCounter(&c_end);

				diff.QuadPart += c_end.QuadPart - c_start.QuadPart;
			}
		}

		QueryPerformanceCounter(&c_start);

		if (change == 1) {
			km_A = km_A + h(last, start);
			km_B = km_B + h(last, start);
			km_C = km_C + h(last, start);

			last = start;
			shortestpath_A();
			shortestpath_B();
			shortestpath_C();

		}

		if (s[start].A_G == 100000) {
			update_A(start);
			shortestpath_A();
		}
		if (s[start].B_G == 100000) {
			update_B(start);
			shortestpath_B();
		}
		if (s[start].C_G == 100000) {
			update_C(start);
			shortestpath_C();
		}

		recommend(start);
		if (shortest == 999) {
			printf("\t\t\t[No Possible Hospital !!!]\n\n\n\n\n"); break;
		}
		goal = shortest;

		pos_A = start;
		pos_B = start;
		pos_C = start;

		printf("-----------------------| Navigation |------------------------\n"); ++up[0];
		printf("\t\t[Time : %d min],\t[Patent : %d node]\n\n", sum, start);
		printf("To the hospital: A -> %d min, B -> %d min, C -> %d min \n\n", s[start].A_G, s[start].B_G, s[start].C_G);
		printf("-------------------------------------------------------------\n");
		printf("//////////////////////////////////////////////////////////\n\n\n\n\n\n");

		/*
		printf("\n");
		for (int i = 1; i <= 25; i++) {
			printf("NODE[%d]\tG:%d\tRHS:%d\t%f\n", i, s[i].A_G, s[i].A_RHS, calculate_key_A(i));
		}
		for (int i = 1; i <= 5; i++) {
			printf("(%d,%f)\t", openlist_A[i].index, openlist_A[i].key);
		}
		printf("%d\n", openlist_size_A);


		for (int i = 1; i <= 25; i++) {
			printf("NODE[%d]\tG:%d\tRHS:%d\t%f\n", i, s[i].B_G, s[i].B_RHS, calculate_key_B(i));
		}
		for (int i = 1; i <= 5; i++) {
			printf("(%d,%f)\t", openlist_B[i].index, openlist_B[i].key);
		}
		printf("%d\n", openlist_size_B);


		for (int i = 1; i <= 25; i++) {
			printf("NODE[%d]\tG:%d\tRHS:%d\t%f\n", i, s[i].C_G, s[i].C_RHS, calculate_key_C(i));
		}
		for (int i = 1; i <= 5; i++) {
			printf("(%d,%f)\t", openlist_C[i].index, openlist_C[i].key);
		}
		printf("%d\n", openlist_size_C);
		*/

		cnt = 0;

		printf("To A: %d", start);
		while (pos_A != HOSA) {
			pos_A = find_argmin_A(pos_A);
			printf("->%d", pos_A);
			cnt++;
			if (cnt == 50) break;
		}
		printf("\n");
		cnt = 0;

		printf("To B: %d", start);
		while (pos_B != HOSB) {
			pos_B = find_argmin_B(pos_B);
			printf("->%d", pos_B);
			cnt++;
			if (cnt == 50) break;
		}
		printf("\n");
		cnt = 0;

		printf("To C: %d", start);
		while (pos_C != HOSC) {
			pos_C = find_argmin_C(pos_C);
			printf("->%d", pos_C);
			cnt++;
			if (cnt == 50) break;
		}
		printf("\n");

		change = 0;

		if (start == goal) break;

		if (shortest == HOSA) {
			sum += find_cost(start, find_argmin_A(start));
			start = find_argmin_A(start);
		}
		else if (shortest == HOSB) {
			sum += find_cost(start, find_argmin_B(start));
			start = find_argmin_B(start);
		}
		else if (shortest == HOSC) {
			sum += find_cost(start, find_argmin_C(start));
			start = find_argmin_C(start);
		}

		printf("\n\nCurrent time: %d\n", sum);
		printf("moved to the node %d\n\n", start);

		QueryPerformanceCounter(&c_end);

		diff.QuadPart += c_end.QuadPart - c_start.QuadPart;
	}

	printf("\n");

	printf("-----------------------| Navigation |------------------------\n");
	printf("\t\t[Time : %d min],\t[Patent : %d node]\n", sum, start);
	printf("\t\t[We arrived at %d Hospital !!!]\n", start);
	printf("----------------------------------------------------------\n");
	printf("//////////////////////////////////////////////////////////\n\n\n\n\n\n");

	/*
	printf("\n");
	for (int i = 1; i <= 25; i++) {
		printf("NODE[%d]\tG:%d\tRHS:%d\t%f\n", i, s[i].A_G, s[i].A_RHS, calculate_key_A(i));
	}
	for (int i = 1; i <= 5; i++) {
		printf("(%d,%f)\t", openlist_A[i].index, openlist_A[i].key);
	}
	printf("%d\n", openlist_size_A);


	for (int i = 1; i <= 25; i++) {
		printf("NODE[%d]\tG:%d\tRHS:%d\t%f\n", i, s[i].B_G, s[i].B_RHS, calculate_key_B(i));
	}
	for (int i = 1; i <= 5; i++) {
		printf("(%d,%f)\t", openlist_B[i].index, openlist_B[i].key);
	}
	printf("%d\n", openlist_size_B);


	for (int i = 1; i <= 25; i++) {
		printf("NODE[%d]\tG:%d\tRHS:%d\t%f\n", i, s[i].C_G, s[i].C_RHS, calculate_key_C(i));
	}
	for (int i = 1; i <= 5; i++) {
		printf("(%d,%f)\t", openlist_C[i].index, openlist_C[i].key);
	}
	printf("%d\n", openlist_size_C);


	report(type, emergency, start, up, busy, quality, sum);
	*/

	printf("time: %.12f sec\n\n", ((double)diff.QuadPart / (double)ticksPerSec.QuadPart));



	//G, RHS값 확인용
	/*
	for (int i = 1; i <= 16; i++) {
		printf("NODE[%d]\tG:%d\tRHS:%d\tH:%f\t%f\n", i, s[i].G, s[i].RHS, s[i].H, calculate_key(s[i]));
	}
	*/
	return;
}
