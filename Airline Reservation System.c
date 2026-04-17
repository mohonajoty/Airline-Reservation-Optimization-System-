#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define MAX 50
#define CITY 5
#define INF 9999

// ================= PASSENGER =================
typedef struct {
    char name[30];
    int age;
    int seat;
    char payment[30];
    char from[30];
    char to[30];
} Passenger;

// ================= FLIGHT =================
typedef struct {
    char from[30];
    char to[30];
    int price;
    int seats;
} Flight;

// ================= GLOBAL =================
Passenger passengers[MAX];
Flight flights[10];

int passengerCount = 0;
int revenue = 0;
int flightCount = 5;

// ================= CITY =================
char city[CITY][20] = {"Dhaka","Chittagong","Sylhet","CoxBazar","Rajshahi"};

int graph[CITY][CITY] = {
    {0, 5, 3, 0, 0},
    {5, 0, 4, 6, 0},
    {3, 4, 0, 2, 7},
    {0, 6, 2, 0, 3},
    {0, 0, 7, 3, 0}
};

// ================= LOGIN =================
int login() {
    char u[20], p[20];

    printf("\n===== Authentication & Access Control Module =====\n");
    printf("Username: ");
    scanf("%s", u);
    printf("Password: ");
    scanf("%s", p);

    if(strcmp(u,"admin")==0 && strcmp(p,"1234")==0) {
        printf("Login Success ✔\n");
        return 1;
    }

    printf("Login Failed ❌\n");
    return 0;
}

// ================= INIT FLIGHTS =================
void addFlights() {
    strcpy(flights[0].from,"Dhaka");
    strcpy(flights[0].to,"Chittagong");
    flights[0].price=3000; flights[0].seats=45;

    strcpy(flights[1].from,"Dhaka");
    strcpy(flights[1].to,"Sylhet");
    flights[1].price=2500; flights[1].seats=50;

    strcpy(flights[2].from,"Sylhet");
    strcpy(flights[2].to,"CoxBazar");
    flights[2].price=2000; flights[2].seats=40;

    strcpy(flights[3].from,"Dhaka");
    strcpy(flights[3].to,"Rajshahi");
    flights[3].price=2700; flights[3].seats=30;

    strcpy(flights[4].from,"Chittagong");
    strcpy(flights[4].to,"CoxBazar");
    flights[4].price=2200; flights[4].seats=32;
}

// ================= SHOW FLIGHTS =================
void showFlights() {
    printf("\n--- FLIGHTS ---\n");

    for(int i=0;i<flightCount;i++) {
        printf("%d. %s -> %s | Price: %d | Seats: %d\n",
            i+1,
            flights[i].from,
            flights[i].to,
            flights[i].price,
            flights[i].seats
        );
    }
}

// ================= PAYMENT =================
int paymentSystem(char *paymentType) {
    int op;

    printf("\n===== PAYMENT =====\n");
    printf("1.Card  2.Mobile  3.Cash\n");
    printf("Choose: ");
    scanf("%d",&op);

    if(op==1) {
        char c[20], p[10];
        printf("Card No: ");
        scanf("%s", c);
        printf("PIN: ");
        scanf("%s", p);

        if(strlen(c) < 8 || strlen(p) < 4) {
            strcpy(paymentType, "FAILED");
            printf("Payment Failed ❌\n");
            return 0;
        }
        strcpy(paymentType, "Card");
    }
    else if(op==2) {
        int otp;
        printf("Enter OTP: ");
        scanf("%d",&otp);

        if(otp != 1234) {
            strcpy(paymentType, "FAILED");
            printf("OTP Failed ❌\n");
            return 0;
        }
        strcpy(paymentType, "Mobile");
    }
    else {
        strcpy(paymentType, "Cash");
    }

    printf("Payment Success ✔\n");
    return 1;
}

// ================= BOOK TICKET =================
void bookTicket() {
    int ch;
    Passenger p;

    printf("\n========== BOOK TICKET ==========\n");

    showFlights();

    printf("\nEnter Flight Choice: ");
    scanf("%d",&ch);

    if(ch < 1 || ch > flightCount) {
        printf("Invalid Choice ❌\n");
        return;
    }

    Flight *f = &flights[ch-1];

    if(f->seats <= 0) {
        printf("No Seats Available ❌\n");
        return;
    }

    printf("\n--- PASSENGER DETAILS ---\n");

    printf("Name: ");
    scanf("%s", p.name);

    printf("Age: ");
    scanf("%d", &p.age);

    strcpy(p.from, f->from);
    strcpy(p.to, f->to);

    printf("Confirm Booking? (1/0): ");
    int c;
    scanf("%d",&c);

    if(!c) return;

    p.seat = f->seats;
    f->seats--;

    // ✅ FIXED PAYMENT LOGIC
    int pay = paymentSystem(p.payment);

    if(!pay) {
        printf("Booking Cancelled due to Payment Failure ❌\n");
        return;
    }

    passengers[passengerCount++] = p;
    revenue += f->price;

    printf("Booking Success ✔ Seat No: %d\n", p.seat);
}

// ================= CANCEL =================
void cancelTicket() {
    char name[30];

    printf("Enter Name: ");
    scanf("%s", name);

    for(int i=0;i<passengerCount;i++) {
        if(strcmp(passengers[i].name,name)==0) {

            for(int j=i;j<passengerCount-1;j++) {
                passengers[j] = passengers[j+1];
            }

            passengerCount--;
            printf("Cancelled ✔\n");
            return;
        }
    }

    printf("Not Found ❌\n");
}

// ================= VIEW =================
void viewPassengers() {
    printf("\n--- PASSENGERS ---\n");

    for(int i=0;i<passengerCount;i++) {
        printf("Name:%s From:%s To:%s Seat:%d\n",
            passengers[i].name,
            passengers[i].from,
            passengers[i].to,
            passengers[i].seat
        );
    }
}

// ================= REVENUE =================
void showRevenue() {
    printf("\nRevenue = %d\n", revenue);
}

// ================= DIJKSTRA =================
void dijkstra(int src) {
    int dist[CITY], vis[CITY]={0};

    for(int i=0;i<CITY;i++) dist[i]=INF;
    dist[src]=0;

    for(int i=0;i<CITY-1;i++) {
        int u=-1, min=INF;

        for(int j=0;j<CITY;j++)
            if(!vis[j] && dist[j]<min)
                min=dist[j], u=j;

        if(u==-1) break;

        vis[u]=1;

        for(int v=0;v<CITY;v++)
            if(graph[u][v] && dist[u]+graph[u][v]<dist[v])
                dist[v]=dist[u]+graph[u][v];
    }

    printf("\n--- Shortest Paths ---\n");

    for(int i=0;i<CITY;i++)
        printf("%s = %d\n", city[i], dist[i]);
}

// ================= PRIM =================
void prim() {
    int key[CITY], parent[CITY], mst[CITY]={0};

    for(int i=0;i<CITY;i++) key[i]=INF;

    key[0]=0;
    parent[0]=-1;

    for(int i=0;i<CITY-1;i++) {
        int u=-1, min=INF;

        for(int j=0;j<CITY;j++)
            if(!mst[j] && key[j]<min)
                min=key[j], u=j;

        mst[u]=1;

        for(int v=0;v<CITY;v++)
            if(graph[u][v] && !mst[v] && graph[u][v]<key[v]) {
                parent[v]=u;
                key[v]=graph[u][v];
            }
    }

    printf("\n--- Network Optimization ---\n");

    for(int i=1;i<CITY;i++)
        printf("%s - %s = %d\n",
            city[parent[i]],
            city[i],
            graph[i][parent[i]]
        );
}

// ================= MAX FLOW =================
int bfs(int rGraph[CITY][CITY], int s, int t, int parent[]) {
    int vis[CITY]={0}, q[CITY], f=0, r=0;

    q[r++]=s;
    vis[s]=1;
    parent[s]=-1;

    while(f<r) {
        int u=q[f++];

        for(int v=0;v<CITY;v++)
            if(!vis[v] && rGraph[u][v]>0) {
                q[r++]=v;
                parent[v]=u;
                vis[v]=1;
            }
    }

    return vis[t];
}

void maxFlow() {
    int rGraph[CITY][CITY];

    for(int i=0;i<CITY;i++)
        for(int j=0;j<CITY;j++)
            rGraph[i][j]=graph[i][j];

    int parent[CITY], flow=0;

    while(bfs(rGraph,0,3,parent)) {

        int path=INT_MAX;

        for(int v=3; v!=0; v=parent[v]) {
            int u=parent[v];
            if(rGraph[u][v]<path)
                path=rGraph[u][v];
        }

        for(int v=3; v!=0; v=parent[v]) {
            int u=parent[v];
            rGraph[u][v]-=path;
            rGraph[v][u]+=path;
        }

        flow += path;
    }

    printf("\nMax Flow = %d\n", flow);
}

// ================= MAIN =================
int main() {
    addFlights();

    if(!login()) return 0;

    int ch;
printf("\n==============================================\n");
    printf("   ✈ Airline Reservation & Optimization System ✈\n");
    printf("==============================================\n");
    while(1) {
        printf("\n========== MAIN MENU ==========\n");
        printf("1.Book Ticket\n2.Cancel Ticket\n3.View Passengers\n4.Show Flights\n5.Revenue\n6.Shortest Route Optimization\n7.Flight Network Cost Minimization System\n8.Capacity Optimization & Traffic Flow Analysis\n9.Exit\n");
        printf("Enter choice: ");

        scanf("%d",&ch);

        switch(ch) {
            case 1: bookTicket(); break;
            case 2: cancelTicket(); break;
            case 3: viewPassengers(); break;
            case 4: showFlights(); break;
            case 5: showRevenue(); break;
            case 6: dijkstra(0); break;
            case 7: prim(); break;
            case 8: maxFlow(); break;
            case 9:
                printf("Thank you ✈ Goodbye!\n");
                exit(0);
            default:
                printf("Invalid Choice!\n");
        }
    }
}
