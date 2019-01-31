#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <bits/stdc++.h>
#include <utility> // std::pair
#include <stdio.h>
#include <algorithm>

using namespace std;

//Utility
int cast(string s);
void displayVector(vector<int> vec);
float sumVec(vector<int> vec);
vector<vector<int>> fileToVector(string file_path);

//main
void FCFS(vector<int> AT, vector<int> BT);
void SJF(vector<int> AT, vector<int> BT, vector<int> PID);
void SRJF(vector<int> AT, vector<int> BT, vector<int> PID);
void EDF(vector<int> AT, vector<int> BT, vector<int> DL, vector<int> PID);
void RR(vector<int> AT, vector<int> BT, vector<int> PID, int q);

main(int argc, char const *argv[])
{
    vector<vector<int>> AT_BT_DL = fileToVector("process.txt");

    vector<int> PID;
    vector<int> AT = AT_BT_DL[0];
    vector<int> BT = AT_BT_DL[1];
    vector<int> DL = AT_BT_DL[2];
    for (int i = 0; i < AT.size(); i++)
    {
        PID.push_back(i + 1);
    }

   // FCFS(AT, BT);
    //SJF(AT, BT, PID);*/
    RR(AT, BT, PID, 3);/*
    SRJF(AT, BT, PID); //needs attention
    EDF(AT, BT, DL, PID);
    //RR(AT, BT, PID, 4);*/

    return 0;
}

int cast(string s)
{
    stringstream cast(s);
    int x = 0;
    cast >> x;
    return x;
}

void displayVector(vector<int> vec)
{
    for (int i = 0; i < vec.size(); i++)
    {
        cout << vec[i] << endl;
    }
}

vector<vector<int>> fileToVector(string file_path) //create BT, AT, DL vector from the input file
{
    string s;
    vector<vector<string>> tempVector;

    ifstream myfile(file_path);

    while (getline(myfile, s))
    {

        std::stringstream ss(s);
        std::istream_iterator<std::string> begin(ss);
        std::istream_iterator<std::string> end;
        std::vector<std::string> vstrings(begin, end);
        tempVector.push_back(vstrings);
    }

    myfile.close();

    vector<int> AT;

    vector<int> BT;

    vector<int> DL;
    for (int j = 0; j < tempVector.size(); j++)
    {
        AT.push_back(cast(tempVector[j][1]));
        BT.push_back(cast(tempVector[j][2]));
        DL.push_back(cast(tempVector[j][3]));
    }

    vector<vector<int>> AT_BT_DL;

    AT_BT_DL.push_back(AT);
    AT_BT_DL.push_back(BT);
    AT_BT_DL.push_back(DL);
    return AT_BT_DL;
}

void FCFS(vector<int> AT, vector<int> BT)
{
    cout << "FCFS scheduler " << endl;
    int throughput_dem = sumVec(BT);
    vector<int> CT;
    CT.push_back(BT[0]);
    for (int i = 0; i < BT.size(); i++)
    {
        CT.push_back(CT[i - 1] + BT[i]);
        //		cout << CT[i] << "\n";
    }

    vector<int> TAT;
    for (int i = 0; i < AT.size(); i++)
    {
        TAT.push_back(CT[i] - AT[i]);
        //	cout << TAT[i] << "\n";
    }

    vector<int> WT;
    for (int i = 0; i < AT.size(); i++)
    {
        WT.push_back(TAT[i] - BT[i]);
    }
    vector<int> RT;

    int RT_val = 0;
    for (int i = 0; i < BT.size(); i++)
    {

        RT.push_back(BT[i] + RT_val);
    }

    float TAT_Avg = sumVec(TAT) / TAT.size();
    float WT_Avg = sumVec(WT) / WT.size();
    cout << "TAT_Avg: " << TAT_Avg << endl;
    cout << "WT_Avg: " << WT_Avg << endl;
    cout << "RT_Avg: " << (float)sumVec(RT) / (float)BT.size() << endl;
    cout << "Throughput: " << throughput_dem / (float)BT.size() << endl;
    cout << "utilization: " << (double)sumVec(BT) / sumVec(BT) << endl;
    int max_TAT = TAT[0];
    double Prop = 0;
    for (int i = 0; i < TAT.size(); i++)
    {
        if (max_TAT < TAT[i])
            max_TAT = TAT[i];
            Prop= max_TAT/BT[i];
    }

    cout << "Proportionality: " << Prop<< endl;

    vector<int> seq;
    cout << "EXEC_SEQ:  " << endl;
    for (int i = 0; i < AT.size(); i++)
    {
        seq.push_back(AT[i]);
        cout << "P" << AT[i] << endl;
    }
}

float sumVec(vector<int> vec)
{
    float sum = 0;
    for (int i = 0; i < vec.size(); i++)
    {
        sum += vec[i];
    }
    return sum;
}

void RR(vector<int> AT, vector<int> BT, vector<int> PID, int quantum)
{
    cout << "RR scheduler " << endl;

    vector<int> initial_BT = BT;
    vector<int> initial_AT = AT;
    vector<int> seq;

    int t = 0;
    vector<int> WT;
    vector<int> CT;

    while (true)
    {
        bool flag = true;
        for (int i = 0; i < PID.size(); i++)
        {
            if (initial_AT[i] <= t)
            {
                if (initial_AT[i] <= quantum)
                {
                    if (initial_BT[i] > 0)
                    {
                        flag = false;
                        if (initial_BT[i] > quantum)
                        {

                            t = t + quantum;
                            initial_BT[i] = initial_BT[i] - quantum;
                            initial_AT[i] = initial_AT[i] + quantum;
                            seq.push_back(PID[i]);
                        }
                        else
                        {

                            t = t + initial_BT[i];

                            CT.push_back(t - AT[i]);

                            WT.push_back(t - BT[i] - AT[i]);
                            initial_BT[i] = 0;

                            seq.push_back(PID[i]);
                        }
                    }
                }
                else if (initial_AT[i] > quantum)
                {

                    for (int j = 0; j < PID.size(); j++)
                    {

                        if (initial_AT[j] < initial_AT[i])
                        {
                            if (initial_BT[j] > 0)
                            {
                                flag = false;
                                if (initial_BT[j] > quantum)
                                {
                                    t = t + quantum;
                                    initial_BT[j] = initial_BT[j] - quantum;
                                    initial_AT[j] = initial_AT[j] + quantum;
                                    seq.push_back(PID[i]);
                                }
                                else
                                {
                                    t = t + initial_BT[j];
                                    CT.push_back(t - AT[j]);
                                    WT.push_back(t - BT[j] - AT[j]);
                                    initial_BT[j] = 0;
                                    seq.push_back(PID[i]);
                                }
                            }
                        }
                    }
                    if (initial_BT[i] > 0)
                    {
                        flag = false;

                        if (initial_BT[i] > quantum)
                        {
                            t = t + quantum;
                            initial_BT[i] = initial_BT[i] - quantum;
                            initial_AT[i] = initial_AT[i] + quantum;
                            seq.push_back(PID[i]);
                        }
                        else
                        {
                            t = t + initial_BT[i];
                            CT.push_back(t - AT[i]);
                            WT.push_back(t - BT[i] - AT[i]);
                            initial_BT[i] = 0;
                            seq.push_back(PID[i]);
                        }
                    }
                }
            }

            else if (initial_AT[i] > t)
            {
                t++;
                i--;
            }
        }

        if (flag)
        {
            break;
        }
    }
    vector<int> TAT;

    for (int i = 0; i < CT.size(); i++)
    {
        TAT.push_back(CT[i] - initial_AT[i]);
    }
    cout << "TAT_Avg: " << sumVec(TAT) / (float)TAT.size() << endl;
    cout << "WT_Avg:  " << sumVec(WT) / (float)WT.size() << endl;
    cout << "CT_Avg:  " << sumVec(CT) / (float)WT.size() << endl;
    cout << "Throughput:  " << sumVec(BT) / (float)WT.size() << endl;
    cout << "Utilization:  " << sumVec(BT)/ sumVec(initial_BT) << endl;


      int max_TAT = TAT[0];
    double Prop = 0;
    for (int i = 0; i < TAT.size(); i++)
    {
        if (max_TAT < TAT[i])
            max_TAT = TAT[i];
            Prop= max_TAT/BT[i];
    }

    cout << "Proportionality: " << Prop<< endl;

    cout << "EXEC_SEQ:" << endl;
    displayVector(seq);
}

void SJF(vector<int> AT, vector<int> BT, vector<int> PID)
{
    cout << "SJF scheduler " << endl;

    vector<pair<int, int>> indexed_PID;
    std::pair<int, int> tmp;
    vector<int> seq;

    for (int i = 0; i < BT.size(); i++)
    {
        tmp = std::make_pair(BT[i], PID[i]);
        indexed_PID.push_back(tmp);
        //  cout<<BT[i]<<" , " <<PID[i]<<endl;
    }
    sort(indexed_PID.begin(), indexed_PID.end());
    for (int i = 0; i < PID.size(); i++)
    {
        seq.push_back(indexed_PID[i].second);
    }

    /*  displayVector(seq);
   for (int i = 0; i < PID.size(); i++)
    {
        cout << indexed_PID[i].first << " "
             << indexed_PID[i].second << endl;
    }*/
    vector<int> new_BT, new_AT;
    for (int i = 0; i < PID.size(); i++)
    {
        new_BT.push_back(indexed_PID[i].first);
        new_AT.push_back(indexed_PID[i].second);
    }

    FCFS(new_AT, new_BT);
}

/*
void SRJF(vector<int> AT, vector<int> BT, vector<int> PID)
{
    vector<pair<int, int>> indexed_PID;
    std::pair<int, int> tmp;
    displayVector(BT);
    for (int i = 0; i < BT.size(); i++)
    {
        tmp = std::make_pair(BT[i], PID[i]);
        indexed_PID.push_back(tmp);
        //  cout<<BT[i]<<" , " <<PID[i]<<endl;
    }

    vector<pair<int, int>> readyQ;
    vector<int> seq;
    //execute first proc
    indexed_PID[0].first--;
    seq.push_back(indexed_PID[0].second);
    if (indexed_PID[0].first > 0)
    {
        readyQ.push_back(indexed_PID[0]);
    }
    //<bt, idx>

    int execution_counter = 0;

    vector<int> initial_BT = BT;

    cout << "SRTF seq:" << endl;
    displayVector(seq);
}*/

void SRJF(vector<int> AT, vector<int> BT, vector<int> PID)
{
    cout << "SRJF scheduler " << endl;

    struct proc
    {
        int AT, BT, PID, CT, WT, initial_BT, RT;
        int TAT = CT - AT;
        // int WT = TAT - initial_BT;
        int stamp = 0;
    };
    struct compare_BT
    {
        bool operator()(proc lhs, proc rhs) { return (lhs.BT < rhs.BT); };
    };

    vector<proc> proc_vec;
    int smallest = 0;

    for (int i = 0; i < PID.size(); i++)
    {
        proc x;
        x.AT = AT[i];
        x.BT = BT[i];
        x.PID = PID[i];
        x.initial_BT = BT[i];
        x.RT = 0;
        proc_vec.push_back(x);
    }
    vector<proc> seq;
    vector<proc> ready_Q;
    proc_vec[0].BT--;
    proc_vec[0].stamp++;

    seq.push_back(proc_vec[0]);
    if (proc_vec[0].BT > 0)
    {
        ready_Q.push_back(proc_vec[0]);
    }

    for (int i = 1; i < proc_vec.size(); i++)
    {
        sort(ready_Q.begin(), ready_Q.end(), compare_BT());
        //  cout << proc_vec[i].BT << "," << ready_Q[smallest].BT << endl;
        if (proc_vec[i].BT < ready_Q[smallest].BT)
        {
            proc_vec[i].BT--;
            proc_vec[i].stamp = seq.back().stamp + 1;

            seq.push_back(proc_vec[i]);

            if (proc_vec[i].BT > 0)
            {
                ready_Q.push_back(proc_vec[i]);
            }
        }
        else if (proc_vec[i].BT >= ready_Q[smallest].BT)
        {
            ready_Q[smallest].BT--;
            ready_Q[smallest].stamp = seq.back().stamp + 1;
            seq.push_back(ready_Q[smallest]);
            ready_Q.push_back(proc_vec[i]);
        }

        //remove completed proc from the q
        for (int i = 0; i < ready_Q.size(); i++)
        {
            if (ready_Q[i].BT == 0)
            {
                smallest++;
                // cout << "smallest: " << smallest << endl;
                //
                //  ready_Q.erase(ready_Q.begin()+i,ready_Q.begin()+1+i);
                // ready_Q.erase(std::remove(ready_Q.begin(), ready_Q.end(), ready_Q[i].BT), ready_Q.end());
            }
        }
    }

    sort(ready_Q.begin(), ready_Q.end(), compare_BT());

    for (int i = 0; i < ready_Q.size(); i++)
    {
        if (ready_Q[i].BT > 0)
        {
            ready_Q[i].stamp = seq.back().stamp + ready_Q[i].BT;
            seq.push_back(ready_Q[i]);
        }
    }
    for (int i = 0; i < seq.size(); i++)
    {
        cout << seq[i].PID << endl;
    }

    for (int i = 0; i < proc_vec.size(); i++)
    {
        for (int j = seq.size(); j >= 0; j--)
        {
            if (proc_vec[i].PID == seq[j].PID)
            {
                proc_vec[i].CT = seq[j].stamp;
                //  cout << "stamp" << seq[j].stamp << endl;
                break;
            }
        }
    }

    for (int i = 0; i < proc_vec.size(); i++)
    {

        proc_vec[i].TAT = proc_vec[i].CT - proc_vec[i].AT;
        proc_vec[i].WT = proc_vec[i].TAT - proc_vec[i].initial_BT;
        proc_vec[i].RT = proc_vec[i].CT - proc_vec[i].TAT;

        //    cout << proc_vec[i].TAT << " , " << proc_vec[i].WT << endl;
    }
    double TAT_Avg, WT_Avg, RT_Avg, throughput, utilization = 1.0;
    for (int i = 0; i < proc_vec.size(); i++)
    {
        TAT_Avg += proc_vec[i].TAT;
        WT_Avg += proc_vec[i].WT;
        //cout << "RT: " << proc_vec[i].RT << endl;
        RT_Avg += proc_vec[i].RT;
        //  utilization = (double)utilization *(1-(double) proc_vec[i].WT);
    }

    cout << "TAT_Avg: " << TAT_Avg / proc_vec.size() << endl;
    cout << "WT_Avg:  " << WT_Avg / proc_vec.size() << endl;
    cout << "RT_Avg:  " << RT_Avg / proc_vec.size() << endl;
    // cout << "CPU Utiliation:  " << utilization << endl;

    cout << "Throughput:  " << (float)seq.back().stamp / (float)proc_vec.size() << endl;

        cout << "Utilization:  " << (float)seq.back().stamp / (float)sumVec(BT)  << endl;

      int max_TAT = proc_vec[0].TAT;
    double Prop = 0;
    for (int i = 0; i < proc_vec.size(); i++)
    {
        if (max_TAT < proc_vec[i].TAT)
            max_TAT = proc_vec[i].TAT;
            Prop= max_TAT/proc_vec[i].initial_BT;
    }

    cout << "Proportionality: " << Prop<< endl;
}

void EDF(vector<int> AT, vector<int> BT, vector<int> DL, vector<int> PID)
{
    struct proc
    {
        int AT, BT, PID, CT, WT, initial_BT, RT;
        int TAT = CT - AT;
        int DL;
        int stamp = 0;
    };
    struct compare_DL
    {
        bool operator()(proc lhs, proc rhs) { return (lhs.DL < rhs.DL); };
    };
    struct compare_TAT
    {
        bool operator()(proc lhs, proc rhs) { return (lhs.TAT < rhs.TAT); };
    };

    vector<proc> proc_vec;

    for (int i = 0; i < PID.size(); i++)
    {
        proc x;
        x.AT = AT[i];
        x.BT = BT[i];
        x.PID = PID[i];
        x.initial_BT = BT[i];
        x.RT = 0;
        x.DL = DL[i];
        proc_vec.push_back(x);
    }
    vector<proc> seq;
    sort(proc_vec.begin(), proc_vec.end(), compare_DL());

    for (int i = 0; i < proc_vec.size(); i++)
    {
        //   proc_vec[i].stamp += seq.back().initial_BT;

        seq.push_back(proc_vec[i]);
    }
    cout << "EXEC_SEQ:  " << endl;
    seq[0].CT = seq[0].BT;
    for (int i = 1; i < seq.size() + 1; i++)
    {
        seq[i].CT = seq[i - 1].CT + seq[i].BT;

        cout << seq[i - 1].PID - 1 << " >> " << seq[i - 1].CT << endl;
    }
    proc_vec = seq;
    for (int i = 0; i < proc_vec.size(); i++)
    {

        proc_vec[i].TAT = proc_vec[i].CT - proc_vec[i].AT;
        proc_vec[i].WT = proc_vec[i].TAT - proc_vec[i].initial_BT;
        proc_vec[i].RT = proc_vec[i].CT - proc_vec[i].TAT;

        //    cout << proc_vec[i].TAT << " , " << proc_vec[i].WT << endl;
    }
    double TAT_Avg, WT_Avg, RT_Avg, throughput, utilization = 1.0;
    for (int i = 0; i < proc_vec.size(); i++)
    {
        TAT_Avg += proc_vec[i].TAT;
        WT_Avg += proc_vec[i].WT;
        //cout << "RT: " << proc_vec[i].RT << endl;
        RT_Avg += proc_vec[i].RT;
        //  utilization = (double)utilization *(1-(double) proc_vec[i].WT);
    }

    cout << "TAT_Avg: " << TAT_Avg / proc_vec.size() << endl;
    cout << "WT_Avg:  " << WT_Avg / proc_vec.size() << endl;
    cout << "RT_Avg:  " << RT_Avg / proc_vec.size() << endl;
    cout << "CPU Utiliation:  " << (float)seq.back().CT / (float)sumVec(BT) << endl;
    cout << "Throughput:  " << (float)seq.back().CT / (float)proc_vec.size() << endl;
    sort(proc_vec.begin(), proc_vec.end(), compare_TAT());
    cout << "proportionality:  " << (float)proc_vec.back().TAT / proc_vec.back().BT << endl;
}
