#include <iostream>
#include <ctime>
#include <cmath>
#include "base.h"
using namespace std;
#define block 750
#define LAMBDA 0.0333 //Lambda = 2cars/min = 2/60 cars/sec
#define E 5
#define T -110
#define Pt -50
double arrivalModel (int t) {
    double probability;
    probability = LAMBDA * exp(LAMBDA * -t);
    return probability;
}

//to compare which base station has the larger power 
void cmp (double p1, double p2, double p3, double p4, double& pcur, int& hearing) {
    double sec1 = p1;
    double sec2 = p3;
    double Final;
    if (p2>p1)sec1 = p2;
    if (p4>p3)sec2 = p4;
    if(sec1>sec2)Final = sec1;
    else Final = sec2;
    cout << "Final is: " <<Final <<endl;
    pcur = Final;
    if (pcur == p1) hearing = 1;
    else if (pcur == p2) hearing = 2;
    else if (pcur == p3) hearing = 3;
    else hearing = 4;
}

//generate the car as the probability
bool genCar (double prob) {
    int random;
    int cmp = prob *1000;
    random = rand()%1000;
    return (random<=cmp? true: false);
}

class car {
    public:
    int velocity = 10; //10m/s
    int direct = 0; // straight = 0, right = 1, left = 2
    int face;// face 0 = face right, 1 = face down ,2 = face left, 3 = face up 
    int x, y; // position of car
    //Record the power of basestation
    double P1 = -130, P2=-130, P3=-130, P4=-130;
    double Plisten;
    //Record heraing basestation
    int hearing;
    int old_h1 = hearing;
    int old_h2 = hearing;
    int old_h3 = hearing;
    int old_h4 = hearing;
    double Pold_1 = -10000;
    double Pold_2 = -10000;
    double Pold_3 = -10000;
    double Pold_4;
    double Pentro, Pbest, Pthresh, Pself;
    double Pcur;
    int handoff_best = 0;
    int handoff_threshold = 0;
    int handoff_entropy=0;

    //record the position of a car
    void move () {
        int random;
        if (x%block==0 && y%block==0) {//if meet the intersection choose the direction
            random = rand()%6;
            if (random <=2 ) {direct = 0;}
            else if (random<= 4 && random > 2) {direct = 1;}
            else{ direct = 2;}
        }
        switch (face) {
            case 0:
            if (direct == 0 ){
                x+=velocity;
            }
            else if (direct == 1){
                y+=velocity;
            }
            else{
                x-=velocity;
            }

            break;
            case 1:
            if (direct == 0 ){
                y+=velocity;
            }
            else if (direct == 1){
                x-=velocity;
            }
            else{
                x+=velocity;
            }
            break;
            case 2:
            if (direct == 0 ){
                x-=velocity;
            }
            else if (direct == 1){
                y-=velocity;
            }
            else{
                y+=velocity;
            }
            break;
            case 3:
            if (direct == 0 ){
                y-=velocity;
            }
            else if (direct == 1){
                x+=velocity;
            }
            else{
                x-=velocity;
            }
            break;
        }
        
    }

    void show_pos () {
        cout << "x: "<<x<<"y: "<<y<<endl;
    }

    //calculate the power receiving from 4 base station
    void power_cal(base b1, base b2, base b3, base b4) {
        double dist1 = 0;
        int xd, yd;
        xd = (x-b1.x)*(x-b1.x);
        yd = (y-b1.y)*(y-b1.y);
        dist1 = sqrt(xd +yd);
        double chk = 0;
        if (dist1 > 0 )chk = log10(dist1);
        P1 = -60 -20 * chk; 

        chk =0;
        xd = (x-b2.x)*(x-b2.x);
        yd = (y-b2.y)*(y-b2.y);
        double dist2 = sqrt(xd +yd);
        if (dist2 > 0 )chk = log10(dist2);
        P2 = -60 -20 * chk;

        chk =0;
        xd = (x-b3.x)*(x-b3.x);
        yd = (y-b3.y)*(y-b3.y);
        double dist3 = sqrt(xd +yd);
        if (dist3 > 0 )chk = log10(dist3);
        P3 = -60 -20*chk; 

        chk =0;
        xd = (x-b4.x)*(x-b4.x);
        yd = (y-b4.y)*(y-b4.y);
        double dist4 = sqrt(xd +yd);;
        if (dist4 > 0 )chk = log10(dist4);
        P4 = -60 -20*chk;
        cmp(P1, P2, P3, P4, Pcur, hearing);
        cout << "Pcur is "<<hearing<<endl;
    }

    void best_policy (int& a) {
        if (Pcur > Pold_1 && hearing!=old_h1){ 
        cout<<"best count"<<endl;
        cout<<"current best is : "<<a<<endl;
        cout<<"Pold_1 : "<<Pold_1<<endl;
        handoff_best++;
        a++;
        Plisten  = Pcur;
        cout<<"Plisten_1 : "<<Plisten<<endl;
        Pold_1 = Plisten;
        Pbest = Plisten;
        old_h1 = hearing;
        }
    }
    void threshold (int& b) {
        if ((Pcur > Pold_2) && (Pold_2 < T) && (hearing!=old_h2)){
        cout<<"threshold count"<<endl;
        cout<<"current threshold is : "<<b<<endl;
        cout<<"Pold_2 : "<<Pold_2<<endl;
        handoff_threshold++;
        b++;
        Plisten  = Pcur;
        cout<<"Plisten_2 : "<<Plisten<<endl;
        Pold_2 = Plisten;
        Pthresh = Plisten;
        old_h2 = hearing;
        }
    }
    void entropy (int&c) {
        if (Pcur > (Pold_3+E)&& (hearing!=old_h3)) {
            cout<<"entropy count"<<endl;
            handoff_entropy++;
            c++;
            Plisten  = Pcur;
            cout<<"Plisten_3 : "<<Plisten<<endl;
            Pold_3 = Plisten;
            Pentro = Plisten;
            old_h3 = hearing;
        } 
    }

    void self (int&d) {
        if (hearing!=old_h4) {
            if((x==1500 && y<=1500) || (y==1500 && x<=1500)||(y>1500 && x==1500)||(y==1500 && x>1500)){
            Pold_4 = Plisten;
            Pself = Plisten;
            hearing = old_h4;
            }
        else {
            Plisten = Pcur;
            Pself = Plisten;
            old_h4 = hearing;
            d++;
        }
        }
        
     }

};

class car* carList;


