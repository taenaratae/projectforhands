#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <Windows.h>

#define V 150
#define INFI 1000
#define HOSA 24
#define HOSB 68
#define HOSC 106

#define HT_A 80
#define HT_B 75
#define HT_C 85
#define SAT_A 75
#define SAT_B 85
#define SAT_C 80
#define TR_A 85
#define TR_B 80
#define TR_C 75

typedef struct {
    int index; //노드 숫자는 1부터 시작
    int order_index;
    int x, y; //좌표는 따로 지정
    int gCost;
    int hCost;
    int fCost; //g+h
} Node;

typedef struct {
    Node* pqnodes[V]; 
    int size;
} PriorityQueue;

Node nodes[V]; //배열로 나타냄
PriorityQueue openSet;
int closedSet[V];
int adj[V][V]; //adjacency matrix

char type;
int emergency=0;
float cost_HOSA = 0;
float cost_HOSB = 0;
float cost_HOSC = 0;
int busy[V];
int quality[V];

int shortest = HOSA; // 처음에 가정하고 시작?
int patient_position = 56; // 환자의 현재 위치?
int accumulated_time = 0; //누적 시간?
int update_number = 0; //업데이트 횟수
int parent[V+1];
int route_HOSA[20];
int route_HOSB[20];
int route_HOSC[20]; //바꿔야.
int route_node_number_HOSA;
int route_node_number_HOSB;
int route_node_number_HOSC;
int accumulated_gscore = 0;
int accumulated_gscore_HOSA=0;
int accumulated_gscore_HOSB=0;
int accumulated_gscore_HOSC=0;

void update() {
    for (int i = 1; i <= V; ++i) {
        for (int j = 1; j <= V; ++j) {
            scanf("%d", &adj[i - 1][j - 1]);
        }
    }
} //adjacency matrix update해주는 함수

int heuristic(Node current, Node goal) {
    int x_distance = current.x - goal.x;
    int y_distance = current.y - goal.y;
    x_distance = x_distance * x_distance;
    y_distance = y_distance * y_distance;
    double x_dis = (double)(x_distance / 40000);
    double y_dis = (double)(y_distance / 40000);
    double result = sqrt(x_dis + y_dis);
    return (int)result;
} //피타고라스

void initiate() {
    openSet.size = 0;
    for (int i = 0; i < V; ++i) {
        closedSet[i] = 0;
    }
    for (int i = 0; i < V; ++i) {
        nodes[i].gCost = 1000;
    }
}

void pqInsert(PriorityQueue* pq, Node* node) {
    if (pq->size >= V) return;

    pq->pqnodes[pq->size] = node;
    int current = pq->size;
    pq->size = pq->size + 1;
 
    //printf("pq->size: %d ", current);

    while (current >= 1) {
        int binaryparent = (current - 1) / 2;
        if (pq->pqnodes[current]->fCost >= pq->pqnodes[binaryparent]->fCost) break;

        Node* temp = pq->pqnodes[current];
        pq->pqnodes[current] = pq->pqnodes[binaryparent];
        pq->pqnodes[binaryparent] = temp;
        current = binaryparent;
    } //heapify 느낌
    return;
}

Node* pqPop(PriorityQueue* pq) {
    if (pq->size <= 0) return NULL; //이 부분 문제 생길 수도!!

    Node* result = pq->pqnodes[0];
    pq->pqnodes[0] = pq->pqnodes[pq->size-1]; //새로운 친구를 맨 앞으로.
    pq->size = pq->size - 1; //size 줄이고.
    //printf("POPed: %d\n", result->index);

    int current = 0;
    while (true) {
        int child = current * 2 + 1; //왼쪽 child
        if (child >= pq->size) break;

        if (child + 1 < pq->size && pq->pqnodes[child + 1]->fCost < pq->pqnodes[child]->fCost) {
            child++;
        }

        if (pq->pqnodes[current]->fCost <= pq->pqnodes[child]->fCost) break;

        Node* temp = pq->pqnodes[current];
        pq->pqnodes[current] = pq->pqnodes[child];
        pq->pqnodes[child] = temp;
        current = child;
    }
    return result;
}

int pqIsEmpty(PriorityQueue* pq) {

    return pq->size == 0;
}

int a_star_HOSA(int goal) {

    nodes[patient_position - 1].gCost = 0;
    nodes[patient_position - 1].hCost = 0;
    nodes[patient_position - 1].fCost = 0;
    pqInsert(&openSet, &nodes[patient_position - 1]); //heapify까지 해결

    while (openSet.size>0) {
        int current = pqPop(&openSet)->order_index; //pop당한 node의 index를 알려줌

        //printf("%d ", current);
        if (current == nodes[goal - 1].order_index) {
            // 경로 찾음
            // 경로 복원 로직
            route_HOSA[0] = goal;
            int temp = 0;
            while (current != patient_position) {
                temp = temp + 1;
                route_HOSA[temp] = parent[current];
                current = parent[current];
            }
            route_node_number_HOSA = temp+1;
            accumulated_gscore = nodes[route_HOSA[0] - 1].gCost;
            return accumulated_gscore;
        }

        closedSet[current - 1] = 1;

        // 인접 노드 순회 및 비용 계산
        for (int i = 0; i < V; i++) {
            if (closedSet[i] == 0) { //closedSet=0 인 노드들만을 위해, 근데 인접한 애들만을 위해
                int tentativeGCost = nodes[current - 1].gCost + adj[current - 1][i];
                //printf("tentativeGCost: %d\n", tentativeGCost);
                if (tentativeGCost < nodes[i].gCost) {
                    //printf("here ");
                    parent[i+1] = current;
                    nodes[i].gCost = tentativeGCost;
                    nodes[i].hCost = heuristic(nodes[i], nodes[goal - 1]);
                    //printf("nodes[i].hCost: %d \n", nodes[i].hCost);
                    nodes[i].fCost = nodes[i].gCost + nodes[i].hCost;

                    pqInsert(&openSet, &nodes[i]);
                }
            }
        }
    }
    printf("경로를 찾을 수 없습니다.\n");
}

int a_star_HOSB(int goal) {

    nodes[patient_position - 1].gCost = 0;
    nodes[patient_position - 1].hCost = 0;
    nodes[patient_position - 1].fCost = 0;
    pqInsert(&openSet, &nodes[patient_position - 1]); //heapify까지 해결

    while (openSet.size > 0) {
        int current = pqPop(&openSet)->order_index; //pop당한 node의 index를 알려줌

        //printf("%d ", current);
        if (current == nodes[goal - 1].order_index) {
            // 경로 찾음
            // 경로 복원 로직
            route_HOSB[0] = goal;
            int temp = 0;
            while (current != patient_position) {
                temp = temp + 1;
                route_HOSB[temp] = parent[current];
                current = parent[current];
            }
            route_node_number_HOSB = temp + 1;
            accumulated_gscore = nodes[route_HOSB[0] - 1].gCost;
            return accumulated_gscore;
        }

        closedSet[current - 1] = 1;

        // 인접 노드 순회 및 비용 계산
        for (int i = 0; i < V; i++) {
            if (closedSet[i] == 0) { //closedSet=0 인 노드들만을 위해, 근데 인접한 애들만을 위해
                int tentativeGCost = nodes[current - 1].gCost + adj[current - 1][i];
                //printf("tentativeGCost: %d\n", tentativeGCost);
                if (tentativeGCost < nodes[i].gCost) {
                    //printf("here ");
                    parent[i + 1] = current;
                    nodes[i].gCost = tentativeGCost;
                    nodes[i].hCost = heuristic(nodes[i], nodes[goal - 1]);
                    //printf("nodes[i].hCost: %d  ", nodes[i].hCost);
                    nodes[i].fCost = nodes[i].gCost + nodes[i].hCost;

                    pqInsert(&openSet, &nodes[i]);
                }
            }
        }
    }
    printf("경로를 찾을 수 없습니다.\n");
}

int a_star_HOSC(int goal) {

    nodes[patient_position - 1].gCost = 0;
    nodes[patient_position - 1].hCost = 0;
    nodes[patient_position - 1].fCost = 0;
    pqInsert(&openSet, &nodes[patient_position - 1]); //heapify까지 해결

    while (openSet.size > 0) {
        int current = pqPop(&openSet)->order_index; //pop당한 node의 index를 알려줌

        //printf("%d ", current);
        if (current == nodes[goal - 1].order_index) {
            // 경로 찾음
            // 경로 복원 로직
            route_HOSC[0] = goal;
            int temp = 0;
            while (current != patient_position) {
                temp = temp + 1;
                route_HOSC[temp] = parent[current];
                current = parent[current];
            }
            route_node_number_HOSC = temp + 1;
            accumulated_gscore = nodes[route_HOSC[0] - 1].gCost;
            return accumulated_gscore;
        }

        closedSet[current - 1] = 1;

        // 인접 노드 순회 및 비용 계산
        for (int i = 0; i < V; i++) {
            if (closedSet[i] == 0) { //closedSet=0 인 노드들만을 위해, 근데 인접한 애들만을 위해
                int tentativeGCost = nodes[current - 1].gCost + adj[current - 1][i];
                //printf("tentativeGCost: %d\n", tentativeGCost);
                if (tentativeGCost < nodes[i].gCost) {
                    //printf("here ");
                    parent[i + 1] = current;
                    nodes[i].gCost = tentativeGCost;
                    nodes[i].hCost = heuristic(nodes[i], nodes[goal - 1]);
                    //printf("nodes[i].hCost: %d  ", nodes[i].hCost);
                    nodes[i].fCost = nodes[i].gCost + nodes[i].hCost;

                    pqInsert(&openSet, &nodes[i]);
                }
            }
        }
    }
    printf("경로를 찾을 수 없습니다.\n");
}

void vitalcheck() {
    printf("----------------------| Vital Check |------------------------\n");
    printf("\t\t\t@ Type : "); scanf("%c", &type);
    printf("\t\t\t@ KTAS : "); scanf("%d", &emergency);
    printf("----------------------| Position Check |------------------------\n");
    printf("\t\t@ Base and Hospital Coordinate Info :\n");
    for (int i = 1; i <= V; ++i) {
        scanf("%d", &nodes[i - 1].x);
        scanf("%d", &nodes[i - 1].y);
    }
    for (int i = 1; i <= V; ++i) {
        nodes[i - 1].index = i;
        nodes[i - 1].order_index = i;
        route_HOSA[i - 1] = 1000;
        route_HOSB[i - 1] = 1000;
        route_HOSC[i - 1] = 1000;
    }
    printf("//////////////////////////////////////////////////////////\n\n\n\n\n\n");
} //좌표를 입력하는 부분에서는, 동일한 minicity를 설계할 거면 미리 저장해둬도 될 듯.

void recommend()
{
    shortest = HOSA;
    if (emergency == 1) {
        cost_HOSA = (100 - 0.99 * (float)accumulated_gscore_HOSA) * (1 * HT_A);
        cost_HOSB = (100 - 0.99 * (float)accumulated_gscore_HOSB) * (1 * HT_B);
        cost_HOSC = (100 - 0.99 * (float)accumulated_gscore_HOSC) * (1 * HT_C);
    }
    else if (emergency == 2) {
        cost_HOSA = (100 - 0.99 * (float)accumulated_gscore_HOSA) * (0.75 * HT_A + 0.148 * SAT_A + 0.102 * TR_A);
        cost_HOSB = (100 - 0.99 * (float)accumulated_gscore_HOSB) * (0.75 * HT_B + 0.148 * SAT_B + 0.102 * TR_B);
        cost_HOSC = (100 - 0.99 * (float)accumulated_gscore_HOSC) * (0.75 * HT_C + 0.148 * SAT_C + 0.102 * TR_C);
    }
    else if (emergency == 3)
    {
        cost_HOSA = (100 - 0.75 * (float)accumulated_gscore_HOSA) * (0.50 * HT_A + 0.296 * SAT_A + 0.204 * TR_A);
        cost_HOSB = (100 - 0.75 * (float)accumulated_gscore_HOSB) * (0.50 * HT_B + 0.296 * SAT_B + 0.204 * TR_B);
        cost_HOSC = (100 - 0.75 * (float)accumulated_gscore_HOSC) * (0.50 * HT_C + 0.296 * SAT_C + 0.204 * TR_C);
    }
    else if (emergency == 4)
    {
        cost_HOSA = (100 - 0.25 * (float)accumulated_gscore_HOSA) * (0.25 * HT_A + 0.444 * SAT_A + 0.306 * TR_A);
        cost_HOSB = (100 - 0.25 * (float)accumulated_gscore_HOSB) * (0.25 * HT_B + 0.444 * SAT_B + 0.306 * TR_B);
        cost_HOSC = (100 - 0.25 * (float)accumulated_gscore_HOSC) * (0.25 * HT_C + 0.444 * SAT_C + 0.306 * TR_C);
    }
    else if (emergency == 5)
    {
        cost_HOSA = (0.592 * SAT_A + 0.408 * TR_A);
        cost_HOSB = (0.592 * SAT_B + 0.408 * TR_B);
        cost_HOSC = (0.592 * SAT_C + 0.408 * TR_C);
    }

    if (cost_HOSB >= cost_HOSA) { shortest = HOSB; }
    if (shortest == HOSA) {
        if (cost_HOSC >= cost_HOSA) {
            shortest = HOSC;
        }
    }
    if (shortest == HOSB) {
        if (cost_HOSC >= cost_HOSB) {
            shortest = HOSC;
        }
    }
    if (shortest == HOSA) {
        if (cost_HOSA >= 10000) {
            shortest = 999;
        }
    }
    if (shortest == HOSB) {
        if (cost_HOSB >= 10000) {
            shortest = 999;
        }
    }
    if (shortest == HOSC) {
        if (cost_HOSC >= 10000) {
            shortest = 999;
        }
    }

    printf("cost_HOSA: %f\n", cost_HOSA);
    printf("cost_HOSB: %f\n", cost_HOSB);
    printf("cost_HOSC: %f\n", cost_HOSC);
    printf("shortest: %d\n", shortest);

    if (shortest == 24) { printf("\t\t\t[Best Hospital : %d]\n", shortest); }
    if (shortest == 68) { printf("\t\t\t[Best Hospital : %d]\n", shortest); }
    if (shortest == 106) { printf("\t\t\t[Best Hospital : %d]\n", shortest); }
}

void showpath()
{
    if (shortest == 24) { printf("[Time To Arrive : %d min]", accumulated_gscore_HOSA); }
    if (shortest == 68) { printf("[Time To Arrive : %d min]", accumulated_gscore_HOSB); }
    if (shortest == 106) { printf("[Time To Arrive : %d min]", accumulated_gscore_HOSC); }
    printf("\n[Path : ");
    if(shortest == HOSA) {
        for (int i = route_node_number_HOSA-1; i>0; i--)
        {
            printf("%d -> ", nodes[route_HOSA[i]-1].index);
        }
        printf("24]\n");
    }
    else if (shortest == HOSB) {
        for (int i = route_node_number_HOSB - 1; i > 0; i--)
        {
            printf("%d -> ", nodes[route_HOSB[i] - 1].index);
        }
        printf("68]\n");
    }
    else if (shortest == HOSC) {
        for (int i = route_node_number_HOSC - 1; i > 0; i--)
        {
            printf("%d -> ", nodes[route_HOSC[i] - 1].index);
        }
        printf("106]\n");
    }
}

void report()
{
    char hos;
    if (patient_position == HOSA) { hos = 'A'; }
    if (patient_position == HOSB) { hos = 'B'; }
    if (patient_position == HOSC) { hos = 'C'; }

    printf("-----------------------| Report |------------------------\n");
    printf("\t\t@ Time : %d min\n\t\t@ Patient : %c Hospital\n", accumulated_time, hos);
    printf("\t\t@ Type : %c\n\t\t@ KTAS : %d / 5\n", type, emergency);
    printf("\t\t@ Update : %d \n", update_number);
    if (patient_position == HOSA) { printf("\t\t@ Qualtiy of Hospital : %d \n\t\t@ Free of Hospital : %d \n\t\t@ Reliablility of Hospital : %d\n", HT_A, SAT_A, TR_A); }
    else if (patient_position == HOSB) { printf("\t\t@ Qualtiy of Hospital : %d \n\t\t@ Free of Hospital : %d \n\t\t@ Reliablility of Hospital : %d\n", HT_B, SAT_B, TR_B); }
    else if (patient_position == HOSC) { printf("\t\t@ Qualtiy of Hospital : %d \n\t\t@ Free of Hospital : %d \n\t\t@ Reliablility of Hospital : %d\n", HT_C, SAT_C, TR_C); }
    printf("----------------------------------------------------------\n");
    printf("//////////////////////////////////////////////////////////\n\n\n\n\n\n");
}


int main() {

    LARGE_INTEGER ticksPerSec;
    LARGE_INTEGER c_start, c_end, diff = { 0 };
    QueryPerformanceFrequency(&ticksPerSec);

    vitalcheck();
    /*printf("%c\n", type);*/
    printf("%d\n", emergency);
    /*for (int i = 1; i <= V; ++i) {
         printf("%d: ", nodes[i-1].index);
         printf("%d ", nodes[i - 1].x);
         printf("%d ", nodes[i - 1].y);
         printf("%d ", cost[i - 1]);
         printf("%d ", busy[i - 1]);
         printf("%d\n", quality[i - 1]);
    }*/

    for (;;)
    {
        if (shortest == 999) { printf("\t\t\t[No Possible Hospital !!!]\n\n\n\n\n"); break; }
        else if (patient_position == shortest) //도착했다는 의미
        {
            QueryPerformanceCounter(&c_start);

            printf("-----------------------| Navigation |------------------------\n");
            printf("\t\t[Time : %d min],\t[Patient : %d node]\n", accumulated_time, patient_position);
            printf("\t\t[We arrived at %d Hospital !!!]\n", patient_position);
            printf("----------------------------------------------------------\n");
            printf("//////////////////////////////////////////////////////////\n\n\n\n\n\n");

            QueryPerformanceCounter(&c_end);
            diff.QuadPart += c_end.QuadPart - c_start.QuadPart;
            break;
        }
        else
        {
            QueryPerformanceCounter(&c_start);

            printf("-----------------------| Navigation |------------------------\n"); ++update_number;
            printf("\t[Time : %d min],\t[Patient Position : Node %d]\n\n", accumulated_time, nodes[patient_position-1].index);

            QueryPerformanceCounter(&c_end);
            diff.QuadPart += c_end.QuadPart - c_start.QuadPart;

            update(); //추가적으로 업데이트..

            QueryPerformanceCounter(&c_start);

            printf("-------------------------------------------------------------\n");

            initiate(); //다시 새로운 노드에 대해 a star를 돌릴 수 있는 준비
            accumulated_gscore_HOSA = a_star_HOSA(HOSA);
            printf("HOSA gscore: %d\n", accumulated_gscore_HOSA);
            initiate(); //다시 새로운 노드에 대해 a star를 돌릴 수 있는 준비
            accumulated_gscore_HOSB = a_star_HOSB(HOSB);
            printf("HOSB gscore: %d\n", accumulated_gscore_HOSB);
            initiate(); //다시 새로운 노드에 대해 a star를 돌릴 수 있는 준비
            accumulated_gscore_HOSC = a_star_HOSC(HOSC);
            printf("HOSC gscore: %d\n", accumulated_gscore_HOSC);


            recommend();

            if (shortest != 999)
            {
                showpath();
                printf("-------------------------------------------------------------\n");
                printf("//////////////////////////////////////////////////////////\n\n\n\n\n\n");
                if (shortest == HOSA) {
                    //if (route_HOSA[route_node_number_HOSA - 2] != HOSA) {
                    accumulated_time = accumulated_time + adj[patient_position - 1][route_HOSA[route_node_number_HOSA - 2] - 1];
                    //}
                    patient_position = route_HOSA[route_node_number_HOSA - 2];
                }
                else if (shortest == HOSB) {
                    //if (route_HOSB[route_node_number_HOSB - 2] != HOSB) {
                    accumulated_time = accumulated_time + adj[patient_position - 1][route_HOSB[route_node_number_HOSB - 2] - 1];
                    //}
                    patient_position = route_HOSB[route_node_number_HOSB - 2];
                }
                else if (shortest == HOSC) {
                    accumulated_time = accumulated_time + adj[patient_position - 1][route_HOSC[route_node_number_HOSC - 2] - 1];
                    patient_position = route_HOSC[route_node_number_HOSC - 2];
                }

                QueryPerformanceCounter(&c_end);
                diff.QuadPart += c_end.QuadPart - c_start.QuadPart;

            } //확인 필요
        }
    }

    report();

    printf("time: %.12f sec\n\n", ((double)diff.QuadPart / (double)ticksPerSec.QuadPart));

    return 0;
}
