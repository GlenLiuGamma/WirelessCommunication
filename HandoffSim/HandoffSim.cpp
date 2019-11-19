#include <iostream>
#include <cmath>
#include <ctime>
#include <fstream>
#include "car.h"
#define ARRAYAMT 3000
using namespace std;

int main () {
    //create base station
    base B1(750,750), B2(2250,750), B3(750,2250), B4(2250,2250);
    //for recording queue of cars
    int front = 0;
    int rear = 0;
    //count:
    int best[86401] = {0};
    int threshold[86401] = {0};
    int entropy[86401] = {0};
    int self[86401] = {0};
    srand(time(NULL));
    car* L = new car [50000];
    double prob = arrivalModel(1);
    for (int i = 0; i <= 86400; i++) {
        //left_side
        for (int k = 1; k <= 3; k ++) {
            if (genCar(prob)){
            rear = (rear + 1)%ARRAYAMT;
            L[rear].face = 0;
            L[rear].x = 0;
            L[rear].y = 750*k;
            if (k==1) L[rear].hearing = 1;
            else if (k==3) L[rear].hearing = 3;
            else L[rear].hearing = rand()%1+1;
            }
        }

        //up_side
        for (int k = 1; k <= 3; k ++) {
            if (genCar(prob)){
            rear = (rear + 1)%ARRAYAMT;
            L[rear].face = 1;
            L[rear].x = 750 * k;
            L[rear].y = 0;
            if (k==1) L[rear].hearing = 1;
            else if (k==3) L[rear].hearing = 2;
            else L[rear].hearing = rand()%1+1;
            }
        }

        //right_side
        for (int k = 1; k <= 3; k ++) {
            if (genCar(prob)){
            rear = (rear + 1)%ARRAYAMT;
            L[rear].face = 2;
            L[rear].x = 3000;
            L[rear].y = 750 * k;
            if (k==1) L[rear].hearing = 2;
            else if (k==3) L[rear].hearing = 4;
            else L[rear].hearing = rand()%1+1;
            }
        }

        //down_side
        for (int k = 1; k <= 3; k ++) {
            if (genCar(prob)){
            rear = (rear + 1)%ARRAYAMT;
            L[rear].face = 0;
            L[rear].x = 750*k;
            L[rear].y = 3000;
            if (k==1) L[rear].hearing = 3;
            else if (k==3) L[rear].hearing = 4;
            else L[rear].hearing = rand()%1+1;
            }
        }

        //cars move
        for (int j = front + 1; j <rear; j++) {
            L[j].move();
            //L[j].show_pos();
            L[j].power_cal(B1,B2,B3,B4);
            L[j].threshold(threshold[i]);
            L[j].best_policy(best[i]);
            L[j].entropy(entropy[i]);
            L[j].self(self[i]);
            cout <<endl;
            if ((L[j].face ==0 || L[j].face==2) && (L[j].y == 750 || L[j].y == 1500 || L[j].y == 2250)) {
                //cout << "car left" << endl;
                front = (front + 1)%ARRAYAMT;
            }
            else if ((L[j].face ==1 || L[j].face==3) && (L[j].x == 750 || L[j].x == 1500 || L[j].x == 2250)) {
                //cout << "car left" << endl;
                front = (front + 1)%ARRAYAMT;
            }
        }
    }
    //output the amount of handoff in different methods, 
    //representing the total handoffs according to each time
    for (int i = 1; i <= 86400; i++) {
        threshold[i] += threshold[i-1];
        best[i] += best[i-1];
        entropy[i] += entropy[i-1];
        self[i]+= self[i-1];
    }

    cout << "Generate " << rear << " cars" <<endl;
    cout << "Self: " <<self[86400]<<endl;
    cout << "Best: " <<best[86400]<<endl;
    cout << "threshold: " << threshold[86400]<<endl;
    cout<<"entropy: " <<entropy[86400]<<endl;

    double AvgEntro = 0;
    double AvgBest = 0;
    double AvgThresh = 0;
    double AvgSelf = 0;
    for (int i = 0; i<=rear; i++) {
        AvgEntro+=L[i].Pentro;
        AvgBest+=L[i].Pbest;
        AvgThresh+=L[i].Pthresh;
        AvgSelf+=L[i].Pself;
    }
    AvgBest/=rear;
    AvgEntro/=rear;
    AvgThresh/=rear;
    AvgSelf/=rear;
    cout << "AvgSelf: " << AvgSelf <<endl;
    cout << "AvgBest: " << AvgBest <<endl;
    cout << "AvgEntro: "<<AvgEntro <<endl;
    cout << "AvgThresh: "<<AvgThresh<<endl;
    ofstream fout("best.txt");
    ofstream fout2("threshold.txt");
    ofstream fout3("entropy.txt");
    ofstream fout4("self.txt");
    ofstream total("total.csv");
    total << "best,threshold,entropy"<<endl;
    for (int i = 0; i<= 86400; i++) {
        fout << best[i]<<endl;
        fout2 << threshold[i]<<endl;
        fout3 << entropy[i]<<endl;
        fout4<<self[i]<<endl;
        total<<best[i]<<","<<threshold[i]<<","<<entropy[i]<<","<<self[i]<<endl;
    }
}