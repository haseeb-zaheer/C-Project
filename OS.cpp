#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <queue>
#include <vector>
#include <algorithm>
#include <climits>
#include <unordered_map>
#include <time.h>
#include <cmath>
#include <utility>

using namespace std;

int Num_Of_Philosophers;
sem_t* forks;
sem_t limit; // Its value will be Number of philosophers - 1;

struct process
{
  int pid;
  int Queue_priority;
  int exec_priority;
  int burst_time;
  int arrival_time;
  int remaining_time;
  int time_executed;
  int feedback_ratio;
  int waiting_time;
};

bool compare_priority(const process&p1, const process&p2)
{
  return p1.exec_priority < p2.exec_priority;
}

bool compare_Arrival_time(const process&p1, const process&p2)
{
  return p1.arrival_time < p2.arrival_time;
}

bool compare_Remaining_time(const process&p1, const process&p2)
{
  return p1.remaining_time < p2.remaining_time;
}

bool compare_Remaining_time_2(const process&p1, const process&p2)
{
  return p1.remaining_time > p2.remaining_time;
}

bool compare_Feedback_ratio(const process&p1, const process&p2)
{
  return p1.feedback_ratio < p2.feedback_ratio;
}

void MQS()
{
  int num_of_process, time_quantum = 1;
  cout << "Enter the Number of processes: ";
  cin >> num_of_process;
  int current_time = 0;
  
  vector<process> pvec;
  
  for(int i = 0; i < num_of_process; i++)
  {
    process p;
    p.pid = i + 1;
    cout << "Enter the Queue Priority of the process P" << i + 1<< " : ";
    cin >> p.Queue_priority;
    cout << "Enter the Execution Priority of the process P" << i + 1<< " : ";
    cin >> p.exec_priority;
    cout << "Enter the Burst time of the process P" << i + 1<< " : ";
    cin >> p.burst_time;
    p.remaining_time = p.burst_time;
    cout << "Enter the Arrival time of the process P" << i + 1<< " : ";
    cin >> p.arrival_time;
    
    pvec.push_back(p);
  }
  
  queue<process> Priority_queue;
  queue<process> RR_queue;
  queue<process> SRJF_queue;
  vector<process> pqvec;
  vector<process> rrvec;
  vector<process> svec;
  
  for(const auto& i: pvec)
  {
    if(i.Queue_priority == 0)
    {
      pqvec.push_back(i);
    }
    else if(i.Queue_priority == 1)
    {
      rrvec.push_back(i); 
    }
    else if(i.Queue_priority == 2)
    {
      svec.push_back(i);
    }
  }
  
  //Sorting the processes based on arrival time
  sort(pqvec.begin(), pqvec.end(), compare_Arrival_time);
  sort(rrvec.begin(), rrvec.end(), compare_Arrival_time);
  sort(svec.begin(), svec.end(), compare_Arrival_time);
  
  while(pqvec.size() != 0 || !Priority_queue.empty() || rrvec.size() != 0 || !RR_queue.empty() || svec.size()  != 0 || !SRJF_queue.empty())
  {
    if(!Priority_queue.empty() || (pqvec.size() != 0 && pqvec[0].arrival_time <= current_time))
    {
      while(pqvec.size() != 0 && pqvec[0].arrival_time <= current_time)
      {
        Priority_queue.push(pqvec[0]);
        pqvec.erase(pqvec.begin() + 0);
      }
      
      
      vector<process> sortingvec;
      while(!Priority_queue.empty())
      {
        sortingvec.push_back(Priority_queue.front());
        Priority_queue.pop();
      }
    
      sort(sortingvec.begin(), sortingvec.end(), compare_priority);
    
      for(const auto& i: sortingvec)
      {
        Priority_queue.push(i);
      }
      
      process& p = Priority_queue.front();
      
      
      if(p.remaining_time == 1)
      {
        cout << "Process P" << p.pid << " executed successfully at ";
        current_time ++;
        cout << current_time << " seconds" << " with priority " << p.exec_priority << " in the Priority Queue." << endl;
        cout << "Current time: " << current_time;
        Priority_queue.pop();
      }
      else
      {
        current_time++;
        p.remaining_time -= 1;
        cout << "Process P" << p.pid << " executed at " << current_time << " seconds in Priority Queue. Remaining time is " << p.remaining_time << " seconds." << endl;
        cout << "Current time: " << current_time;
        
      }
    }
    
    else if(!RR_queue.empty() || (rrvec.size() != 0 && rrvec[0].arrival_time <= current_time))
    {
      
      while(rrvec.size() != 0 && rrvec[0].arrival_time <= current_time)
      {
        RR_queue.push(rrvec[0]);
        rrvec.erase(rrvec.begin() + 0);
      }
      
      process p = RR_queue.front();
      RR_queue.pop();
      
      if(p.remaining_time > 0)
      {
        if(p.remaining_time <= time_quantum)
        {
          current_time += p.remaining_time;
          p.remaining_time = 0;
          cout << "Process P" << p.pid << " executed successfully in the RR Queue" << endl;
          cout << "Current time: " << current_time;
        }
        else
        {
          p.remaining_time -= time_quantum;
          current_time += time_quantum;
          cout << "Process P" << p.pid << " executed in RR Queue for " << time_quantum << " remaining time is " << p.remaining_time << endl;
          cout << "Current time: " << current_time;
          
          while(rrvec.size() != 0 && rrvec[0].arrival_time <= current_time)
          {
            RR_queue.push(rrvec[0]);
            rrvec.erase(rrvec.begin() + 0);
          }         
          RR_queue.push(p);
        }
     }
    }
    
    else if(!SRJF_queue.empty() || (svec.size() != 0 && svec[0].arrival_time <= current_time))
    {
      while(svec.size() != 0 && svec[0].arrival_time <= current_time)
      {
        SRJF_queue.push(svec[0]);
        svec.erase(svec.begin() + 0);
      }
      
      vector<process> sortingvec;
      while(!SRJF_queue.empty())
      {
        sortingvec.push_back(SRJF_queue.front());
        SRJF_queue.pop();
      }
    
      sort(sortingvec.begin(), sortingvec.end(), compare_Remaining_time);
    
      for(const auto& i: sortingvec)
      {
        SRJF_queue.push(i);
      }
      
      process p = SRJF_queue.front();
      SRJF_queue.pop();
      
      if(p.remaining_time == 1)
      {
        cout << "Process P" << p.pid << " executed successfully at ";
        current_time ++;
        cout << current_time << " seconds in the SRJF Queue." << endl;
        cout << "Current time: " << current_time;
      }
      
      else
      {
        current_time++;
        p.remaining_time -= 1;
        cout << "Process P" << p.pid << " executed in the SRJF Queue at " << current_time << " seconds. Remaining time is " << p.remaining_time << " seconds." << endl;
        cout << "Current time: " << current_time;
        
        
        while(svec.size() != 0 && svec[0].arrival_time <= current_time)
        {
          SRJF_queue.push(svec[0]);
          svec.erase(svec.begin() + 0);
        }
        
        SRJF_queue.push(p);
      }
    }
    
    else
    {
      cout << "No process to execute yet " << endl;
      cout << "Current time: " << current_time;
      current_time++;
    }
  }
  
  return;
}

void MLFQ()
{
  int num_of_process, time_quantum = 0;
  cout << "Enter the Number of processes: ";
  cin >> num_of_process;
  cout << "Enter the Quantum time for RR: ";
  cin >> time_quantum;
  int current_time = 0;
  
  vector<process> pvec;
  
  for(int i = 0; i < num_of_process; i++)
  {
    process p;
    p.pid = i + 1;
    cout << "Enter the Queue Priority of the process P" << i + 1<< " : ";
    cin >> p.Queue_priority;
    cout << "Enter the Burst time of the process P" << i + 1<< " : ";
    cin >> p.burst_time;
    p.remaining_time = p.burst_time;
    cout << "Enter the Arrival time of the process P" << i + 1<< " : ";
    cin >> p.arrival_time;
    p.waiting_time = 0;
    cout << endl;
    pvec.push_back(p);
  }
  
  queue<process> FCFS_queue;
  queue<process> SJF_queue;
  queue<process> RR_queue;
  vector<process> fvec;
  vector<process> svec;
  vector<process> rrvec;
  
  for(const auto& i: pvec)
  {
    if(i.Queue_priority == 0)
    {
      fvec.push_back(i);
    }
    else if(i.Queue_priority == 1)
    {
      svec.push_back(i); 
    }
    else if(i.Queue_priority == 2)
    {
      rrvec.push_back(i);
    }
  }
  
  //Sorting the processes based on arrival time
  sort(fvec.begin(), fvec.end(), compare_Arrival_time);
  sort(svec.begin(), svec.end(), compare_Arrival_time);
  sort(rrvec.begin(), rrvec.end(), compare_Arrival_time);
  
  while(fvec.size() != 0 || !FCFS_queue.empty() || svec.size() != 0 || !SJF_queue.empty() || rrvec.size()  != 0 || !RR_queue.empty())
  {
  
    while(fvec.size() != 0 && fvec[0].arrival_time <= current_time)
    {
      FCFS_queue.push(fvec[0]);
      fvec.erase(fvec.begin() + 0);
    }
    
    while(svec.size() != 0 && svec[0].arrival_time <= current_time)
    {
      SJF_queue.push(svec[0]);
      svec.erase(svec.begin() + 0);
    }
    
    while(rrvec.size() != 0 && rrvec[0].arrival_time <= current_time)
    {
      RR_queue.push(rrvec[0]);
      rrvec.erase(rrvec.begin() + 0);
    }
    
    if(!FCFS_queue.empty())
    {
      //demotion check
      int i = FCFS_queue.size();
      
      for(; i > 0; i--)
      {
        process &p1 = FCFS_queue.front();
        if(p1.remaining_time > 10)
        {
          cout << "Process P" << p1.pid << " has too much burst time so it is being demoted to the SJF queue.\n" << "current time: " << current_time << endl;
          FCFS_queue.pop();
          SJF_queue.push(p1);
        }
        else
        {
          FCFS_queue.pop();
          FCFS_queue.push(p1);
        }
      }
      if(FCFS_queue.empty())
      {
        continue;
      }
      
      process p1 = FCFS_queue.front();
      FCFS_queue.pop();
      current_time += p1.burst_time;
      cout << "Process P" << p1.pid << " executed in the FCFS queue with a burst time of " << p1.burst_time << " seconds.\n" << "current time: " << current_time << endl; 
      
      //upgrade check
      int s1 = SJF_queue.size();
      int s2 = RR_queue.size();
      for(; s1 > 0; s1--)
      {
        process& p2 = SJF_queue.front();
        p2.waiting_time = current_time - p2.arrival_time;
        
        if(p2.waiting_time > 20)
        {
          cout << "Process P" << p2.pid << " has waited too much in the SJF queue so it is being upgraded to the FCFS queue.\n" << "current time: " << current_time << endl;
          SJF_queue.pop();
          FCFS_queue.push(p2);
        }
        else
        {
          SJF_queue.pop();
          SJF_queue.push(p2);
        }
      }
      
      for(; s2 > 0; s2--)
      {
        process& p2 = RR_queue.front();
        p2.waiting_time = current_time - p2.arrival_time;
        
        if(p2.waiting_time > 20)
        {
          cout << "Process P" << p2.pid << " has waited too much in the RR queue so it is being upgraded to the SJF queue.\n" << "current time: " << current_time << endl;
          RR_queue.pop();
          SJF_queue.push(p2);
        }
        else
        {
          RR_queue.pop();
          RR_queue.push(p2);
        }
      }
    }
    
    else if(!SJF_queue.empty())
    {
      vector<process> sortingvec;
      while(!SJF_queue.empty())
      {
        sortingvec.push_back(SJF_queue.front());
        SJF_queue.pop();
      }
    
      sort(sortingvec.begin(), sortingvec.end(), compare_Remaining_time);
    
      for(const auto& i: sortingvec)
      {
        SJF_queue.push(i);
      }
      
      
      //demotion check
      int i = SJF_queue.size();
      
      for(; i > 0; i--)
      {
        process &p1 = SJF_queue.front();
        if(p1.remaining_time > 10)
        {
          cout << "Process P" << p1.pid << " has too much burst time so it is being demoted to the RR queue.\n" << "current time: " << current_time << endl;
          SJF_queue.pop();
          RR_queue.push(p1);
        }
        else
        {
          SJF_queue.pop();
          SJF_queue.push(p1);
        }
      }
      
      if(SJF_queue.empty())
      {
        continue;
      }
      
      process p1 = SJF_queue.front();
      SJF_queue.pop();
      current_time += p1.burst_time;
      cout << "Process P" << p1.pid << " executed in the SJF queue with a burst time of " << p1.burst_time << " seconds.\n" << "current time: " << current_time << endl;
      
      //upgrade check
      int s1 = SJF_queue.size();
      int s2 = RR_queue.size();
      for(; s1 > 0; s1--)
      {
        process& p2 = SJF_queue.front();
        p2.waiting_time = current_time - p2.arrival_time;
        
        if(p2.waiting_time > 20)
        {
          cout << "Process P" << p2.pid << " has waited too much in the SJF queue so it is being upgraded to the FCFS queue.\n" << "current time: " << current_time << endl;
          SJF_queue.pop();
          FCFS_queue.push(p2);
        }
        else
        {
          SJF_queue.pop();
          SJF_queue.push(p2);
        }
      }
      
      for(; s2 > 0; s2--)
      {
        process& p2 = RR_queue.front();
        p2.waiting_time = current_time - p2.arrival_time;
        
        if(p2.waiting_time > 20)
        {
          cout << "Process P" << p2.pid << " has waited too much in the RR queue so it is being upgraded to the SJF queue.\n" << "current time: " << current_time << endl;
          RR_queue.pop();
          SJF_queue.push(p2);
        }
        else
        {
          RR_queue.pop();
          RR_queue.push(p2);
        }
      }
    }
    
    else if(!RR_queue.empty())
    {
       
      process p1 = RR_queue.front();
      RR_queue.pop();
      
      if(p1.remaining_time > 0)
      {
        if(p1.remaining_time <= time_quantum)
        {
          current_time += p1.remaining_time;
          p1.remaining_time = 0;
          cout << "Process P" << p1.pid << " executed successfully in the RR Queue with a burst time of " << p1.burst_time <<" seconds." << endl << "current time: " << current_time << endl;
          
          //upgrade check
          int s1 = RR_queue.size();
          for(; s1 > 0; s1--)
          {
            process& p2 = RR_queue.front();
            p2.waiting_time = current_time - p2.arrival_time;
            
            if(p2.waiting_time > 20)
            {
             cout << "Process P" << p2.pid << " has waited too much in the RR queue so it is being upgraded to the SJF queue.\n" << "current time: " << current_time << endl;
             RR_queue.pop();
             SJF_queue.push(p2);
            }
            else
            {
              RR_queue.pop();
              RR_queue.push(p2);
            }
          }
        }
        
        else
        {
          p1.remaining_time -= time_quantum;
          current_time += time_quantum;
          cout << "Process P" << p1.pid << " executed for " << time_quantum << " remaining time is " << p1.remaining_time << endl << "current time: " << current_time << endl; 
          while(rrvec.size() != 0 && rrvec[0].arrival_time <= current_time)
          {
            RR_queue.push(rrvec[0]);
            rrvec.erase(rrvec.begin() + 0);
          }         
          RR_queue.push(p1);
          
          //upgrade check
          int s1 = RR_queue.size();
          for(; s1 > 0; s1--)
          {
            process& p2 = RR_queue.front();
            p2.waiting_time = current_time - p2.arrival_time;
            
            if(p2.waiting_time > 20)
            {
             cout << "Process P" << p2.pid << " has waited too much in the RR queue so it is being upgraded to the SJF queue.\n" << "current time: " << current_time << endl;
             RR_queue.pop();
             SJF_queue.push(p2);
            }
            else
            {
              RR_queue.pop();
              RR_queue.push(p2);
            }
          }
        }
      }
    }
    
    else
    {
      cout << "No process to execute.\n";
      current_time++;
    }
      
  }  
  
  
}

void LJF()
{
  int num_of_process;
  cout << "Enter the Number of processes: ";
  cin >> num_of_process;
  int current_time = 0;
  
  vector<process> pvec;
  
  for(int i = 0; i < num_of_process; i++)
  {
    process p;
    p.pid = i + 1;
    p.Queue_priority = -1;
    p.exec_priority = -1;
    cout << "Enter the Burst time of the process P" << i + 1<< " : ";
    cin >> p.burst_time;
    p.remaining_time = p.burst_time;
    cout << "Enter the Arrival time of the process P" << i + 1<< " : ";
    cin >> p.arrival_time;
    p.time_executed = 0;
    pvec.push_back(p);
  }
  
  queue<process> q1;
  
  sort(pvec.begin(), pvec.end(), compare_Arrival_time);
  
  while(pvec.size() != 0 || !q1.empty())
  {
    while(pvec.size() != 0 && pvec[0].arrival_time <= current_time)
    {
      q1.push(pvec[0]);
      pvec.erase(pvec.begin() + 0);
    }
    
    if(q1.empty())
    {
      cout << "No process available to execute" << endl;
      current_time++;
    }
    
    else if(!q1.empty())
    {
      vector<process> sortingvec;
      while(!q1.empty())
      {
        sortingvec.push_back(q1.front());
        q1.pop();
      }
    
      sort(sortingvec.begin(), sortingvec.end(), compare_Remaining_time_2);
    
      for(const auto& i: sortingvec)
      {
        q1.push(i);
      }
      
      process &p = q1.front();
      q1.pop();
      current_time += p.burst_time;
      p.time_executed += p.burst_time;
      p.remaining_time = 0;
      cout << "Process P" << p.pid << " executed successfully for "<< p.time_executed << " seconds. Current time is " << current_time << endl;
    }
  }
  
  return;
}

void LFRN()
{
  int num_of_process;
  cout << "Enter the Number of processes: ";
  cin >> num_of_process;
  int current_time = 0;
  
  vector<process> pvec;
  
  for(int i = 0; i < num_of_process; i++)
  {
    process p;
    p.pid = i + 1;
    p.Queue_priority = -1;
    p.exec_priority = -1;
    p.waiting_time = 0;
    cout << "Enter the Burst time of the process P" << i + 1<< " : ";
    cin >> p.burst_time;
    p.remaining_time = p.burst_time;
    cout << "Enter the Arrival time of the process P" << i + 1<< " : ";
    cin >> p.arrival_time;
    p.time_executed = 0;
    pvec.push_back(p);
    p.feedback_ratio = (p.waiting_time + p.burst_time) / p.burst_time;
  }
  
  queue<process> q1;
  
  sort(pvec.begin(), pvec.end(), compare_Arrival_time);
  
  while(pvec.size() != 0 || !q1.empty())
  {
    while(pvec.size() != 0 && pvec[0].arrival_time <= current_time)
    {
      q1.push(pvec[0]);
      pvec.erase(pvec.begin() + 0);
    }
    
    if(q1.empty())
    {
      cout << "No process available to execute yet" << endl;
      current_time++;
    }
    
    else if(!q1.empty())
    {
      vector<process> sortingvec;
      while(!q1.empty())
      {
        sortingvec.push_back(q1.front());
        q1.pop();
      }
    
      sort(sortingvec.begin(), sortingvec.end(), compare_Feedback_ratio);
    
      for(const auto& i: sortingvec)
      {
        q1.push(i);
      }
      
      process p = q1.front();
      q1.pop();
      
      current_time += p.burst_time;
      p.remaining_time = 0;
      
      cout << "The Process P" << p.pid << " executed successfully at " << current_time << endl; 
      
      int i = q1.size();
      
      for(; i > 0; i--)
      {
        process& p1 = q1.front();
        q1.pop();
        p1.waiting_time = current_time - p1.arrival_time;
        p1.feedback_ratio = (p1.waiting_time + p1.burst_time) / p1.burst_time; // RECHECK
        q1.push(p1);
      }
    }
  }
}

void *Dining_Philosopher_problem(void* arg)
{
  int id = *((int*)arg);
  while(true)
  {
    cout << "Philosopher " << id << " is thinking." << endl;
    sleep(1);
    
    sem_wait(&limit);
    sem_wait(&forks[id]);
    cout << "Philosopher " << id << " picked up left fork" << endl;
    sem_wait(&forks[(id + 1) % Num_Of_Philosophers]);
    cout << "Philosopher " << id << " picked up right fork" << endl;
    
    cout << "Philosopher " << id << " is eating" << endl;
    
    sem_post(&forks[(id + 1) % Num_Of_Philosophers]);
    cout << "Philosopher " << id << " released right fork" << endl;
    sem_post(&forks[id]);
    cout << "Philosopher " << id << " released left fork" << endl;
    sem_post(&limit);
  }
  
  pthread_exit(NULL);
}

void Dining_func()
{
  cout << "Enter the number of philosophers: ";
  cin >> Num_Of_Philosophers;
        
  sem_init(&limit, 0 , Num_Of_Philosophers - 1);
  forks = new sem_t[Num_Of_Philosophers];
        
  pthread_t tid[Num_Of_Philosophers];
  int ids[Num_Of_Philosophers];
  for(int i = 0; i < Num_Of_Philosophers; i++)
  {
    sem_init(&forks[i], 0, 1);
  }
        
  for(int i = 0; i < Num_Of_Philosophers; i++)
  {
    ids[i] = i;
    pthread_create(&tid[i], NULL, Dining_Philosopher_problem, (void*)&ids[i]);
  }
  
  for(int i = 0 ; i < Num_Of_Philosophers; i++)
  {
    pthread_join(tid[i], NULL);
  }
  
  for(int i = 0 ; i < Num_Of_Philosophers; i++)
  {
    sem_destroy(&forks[i]);
  }
  
  sem_destroy(&limit);
  
  delete[] forks;
  
  return;
}

void Safty_Algo(int** arr1, int** arr2, int* arr3, int nop, int nor)
{
  int* Safe_seq = new int[nop];
  int index = 0;
  
  bool executed;
  bool loop_check = true;
  while(loop_check)
  {
    loop_check = false;
    
    for(int i = 0; i < nop; i++)
    {
      executed = false;
    
      for(int j = 0; j < index; j++)
      {
        if(Safe_seq[j] == i)
        {
          executed = true;
        }
      }
    
      if(!executed)
      {
        bool check = true;
        int* arr0 = arr1[i];
      
        for(int j = 0; j < nor; j++)
        {
          if(arr0[j] > arr3[j])
          {
            check = false;
          }
        }
      
        if(check)
        {
          Safe_seq[index] = i;
          index++;
        
          for(int j = 0; j < nor; j++)
          {
            arr3[j] += arr2[i][j];
          }
          
          loop_check = true;
        }
      }
    }
  }
  
  if(index == nop)
  {
    cout << "The system is in safe state and following is the Safe Sequence" << endl;
    for(int i = 0; i < index; i++)
    {
      cout << "P" << Safe_seq[i] << " ";
    }
    cout << endl;
  }
  else
  {
    cout << "Safe Sequence not generated therefore System is in DEADLOCK !!!" << endl;
    cout << index << endl;
  }
}

void Resource_req_algo(int ** N, int * Av, int ** Al, int nop, int nor)
{
  int proc;
  cout << "Enter the process for which you want to request resource : ";
  cin >> proc;
  
  int * req = new int[nor];
  for(int i = 0; i < nor; i++)
  {
    cout << "Enter the requested instances of resource " << i + 1 << " for process " << proc << " : ";
    cin >> req[i];
  }
  
  for(int i = 0; i < nor; i++)// Step 1
  {
    if(req[i] > N[proc][i])
    {
      cout << "Error!!! request cannot be granted" << endl;
      return;
    }
  }
  
  for(int i = 0; i < nor; i++)// Step 2
  {
    if(req[i] > Av[i])
    {
      cout << "waiting...." << endl;
      Safty_Algo(N, Al, Av, nop, nor);
      if(req[i] > Av[i])
      {
        cout << "Error!!! request cannot be granted" << endl;
      }
    }
  }
  
  for(int i = 0; i < nor; i++)
  {
    Av[i] = Av[i] - req[i];
    Al[proc][i] = Al[proc][i] + req[i];
    N[proc][i] = N[proc][i] - req[i];
  }
  
  Safty_Algo(N, Al, Av, nop, nor);
  cout << "Request can be granted immediately" << endl;
  
  return;
}

void bankers_func()
{
  int num_of_res;
  int num_of_process;
  
  cout << "Enter the Number of process: ";
  cin >> num_of_process;
  
  cout << "Enter number of resources: ";
  cin >> num_of_res;
  
  int *resources = new int[num_of_res];
  for(int i = 0; i < num_of_res; i++)
  {
    cout << "Enter total no. of instances of resource " << i + 1 << " : ";
    cin >> resources[i];
  }
  
  //Now creating the Max Matrix
  
  int ** Max = new int*[num_of_process];
  for(int i = 0; i < num_of_process; i++)
  {
    Max[i] = new int[num_of_res];
  }
  
  for(int i = 0; i < num_of_process; i++)
  {
    for(int j = 0; j < num_of_res; j++)
    {
      cout << "Enter the Max instances needed by process " << i + 1<< " of resource " << j + 1 << " : " ;
      cin >> Max[i][j];
    }
  }
  
  //Now creating the Allocated Matrix
  
  int ** Alloc = new int*[num_of_process];
  for(int i = 0; i < num_of_process; i++)
  {
    Alloc[i] = new int[num_of_res];
  }
  
  for(int i = 0; i < num_of_process; i++)
  {
    for(int j = 0; j < num_of_res; j++)
    {
      cout << "Enter the instances allocated to process " << i + 1<< " of resource " << j + 1 << " :" ;
      cin >> Alloc[i][j];
    }
  }
  
  //Now calculating the Available Matrix
  
  int* Avail = new int[num_of_res];
  int* sum = new int[num_of_res];
  
  for(int i = 0; i < num_of_res; i++)
  {
    sum[i] = 0;
  }
  
  for(int i = 0; i < num_of_process; i++)
  {
    for(int j = 0; j < num_of_res; j++)
    {
      sum[j] += Alloc[i][j];
    }
  }
  
  for(int i = 0; i < num_of_res; i++)
  {
    Avail[i] = resources[i] - sum[i];
  }
  
  //Now calculating the Need Matrix
  
  int ** Need = new int*[num_of_process];
  for(int i = 0; i < num_of_process; i++)
  {
    Need[i] = new int[num_of_res];
  }
  
  for(int i = 0; i < num_of_process; i++)
  {
    for(int j = 0; j < num_of_res; j++)
    {
      Need[i][j] = Max[i][j] - Alloc[i][j];
    }
  }
  
  int choice;
  cout << "To perform safety algorithm press 1\n To perform Resource Request Algorithm press 2/n";
  cin >> choice;
  
  if(choice == 1)
  {
    Safty_Algo(Need, Alloc, Avail, num_of_process, num_of_res);
  }
  else
  {
    Resource_req_algo(Need, Avail, Alloc, num_of_process, num_of_res);
  }
}

int virtual_address_gen(int n)
{
  return rand() % n;
}

void HashedPageTable_With_TLB()
{
  int num_of_pages;
  int num_of_frames;
  int* frame_no;
  int* offset;
  int* freq;
  int RAM[1000];
  unordered_map<int, int> HashPagetable;
  unordered_map<int, int> TLB;
  
  cout << "Enter the Number of pages: ";
  cin >> num_of_pages;
  cout << "Enter the number of frames in ram: ";
  cin >> num_of_frames;
  
  frame_no = new int[num_of_pages];
  offset = new int[num_of_pages];
  freq = new int[num_of_pages];
  
  for(int i = 0 ; i < num_of_pages; i++)
  {
    frame_no[i] = virtual_address_gen(num_of_frames);
    offset[i] = virtual_address_gen(num_of_frames);
    freq[i] = 0;
    HashPagetable.insert({frame_no[i], frame_no[i] * (1000 / num_of_frames)});
  }
  
  for(int i = 0; i < num_of_pages; i++)
  {
    RAM[HashPagetable[frame_no[i]] + offset[i]] = i;
  }
  
  int number1;
  while(true)
  {
    cout << "Enter the page number that you want to find: ";
    cin >> number1;
    if(number1 == -1)
    {
      break;
    }
    
    auto it = TLB.find(frame_no[number1]);
  
    if(it != TLB.end())
    {
      cout << "Page P" << number1 << " found in TLB at frame " << it->first << " with offset " << offset[number1] << endl;
      cout << "RAM : " <<RAM[TLB[frame_no[number1]] + offset[number1]] << endl;
    }
    else
    {
      cout << "Page not found in TLB now searching in the Hashed Page Table " << endl;
      auto it2 = HashPagetable.find(frame_no[number1]);
      
      if(it2 != HashPagetable.end())
      {
        cout << "Page P" << number1 << " found in Hashed page table at frame " << it2->first << " with offset " << offset[number1] << endl;
        cout << "RAM : " << RAM[HashPagetable[frame_no[number1]] + offset[number1]] << endl;
        freq[number1] ++;
        if(freq[number1] > 2)
        {
          TLB.insert({frame_no[number1], frame_no[number1] * num_of_frames});
        }
      }
      else
      {
        cout << "The page number that you entered does not exist." << endl;
      }
    }
  }
}

class node
{
  public:
  int page_no;
  int used_bit;
  node* next;
  
  node(int p, int u)
  {
    page_no = p;
    used_bit = u;
    next = nullptr;
  }
};

class circularlist
{
  public:
  node* head;
  node* tail;
  node* pointer;
  
  circularlist()
  {
    head = tail = pointer = nullptr;
  }
  
  void insert(int p, int u)
  {
    node* newnode = new node(p, u);
    
    if(head == nullptr)
    {
      head = newnode;
      pointer = head;
      tail = newnode;
      newnode->next = head;
    }
    else
    {
      newnode->next = head;
      tail->next = newnode;
      tail = newnode;
    }
  }
  
  bool empty()
  {
    if(head == nullptr)
    {
      return true;
    }
    return false;
  }
  
  bool findpage(int p)
  {
    node* curr = head;
    if(curr == nullptr)
    {
      return false;
    }
    while(curr != nullptr && curr->page_no != p)
    {
      curr = curr->next;
      if(curr == head)
      {
        return false;
      }
    }
    return true;
  }
  
  void printlist()
  {
    if(head == nullptr)
    {
      cout << "The list is empty";
    }
    else
    {
      node* curr1 = head;
      cout << "List: ";
      do
      {
        cout << "(" << curr1->page_no << ", " << curr1->used_bit << ") ";
        curr1 = curr1->next;
      }while(curr1 != head);
      cout << endl;
    }
  }
  
};

void SCA()
{
  float num_of_page_faults = 0;
  int no_of_ele = 0;
  
  circularlist c;
  
  int choice;
  float ram_size = 0.000;
  cout << "Enter the size of the ram: ";
  cin >> ram_size;
  int temp = ram_size;
  while(choice != -1)
  {
    if(temp > 0)
    {
      cout << "Press 1 for inserting a page into the list\n";
    }
    if(temp == 0)
    {
      cout << "RAM FULL!!!\n";
      cout << "Press 2 to perform page replacement\n";
    }
    cout << "Press 3 to reference a page\n";
    cout << "Press 4 to print Number of page faults and page fault percentage\n";
    cin >> choice;
    if(choice == 1 && temp > 0)
    {
      int number;
      cout << "Enter the page number that you want to Enter in the list: ";
      cin >> number;
      if(c.findpage(number))
      {
        cout << "Page already exists in the List\n";
      }
      else
      {
        c.insert(number, 1);
        no_of_ele++;
        temp--;
      }
      cout << "The List is as follows: \n";
      c.printlist();
    }
    else if(choice == 2)
    {
      if(c.empty())
      {
        cout << "The List is empty\n";
        continue;
      }
      int number;
      cout << "Enter the page number: ";
      cin >> number;
      
      if(c.findpage(number))
      {
        cout << "Page already exists in the list\n"; 
      }
      else if(!c.findpage(number))
      {
        if(c.empty())
        {
          cout << "The list is empty\n";
        }
        else
        {
          num_of_page_faults++;
          cout << "List before replacement: \n";
          c.printlist();
          
          node* curr = c.pointer;

          while(curr != nullptr && curr->used_bit == 1)
          {
            curr->used_bit = 0;
            curr = curr->next;
          }
          
          curr->page_no = number;
          curr->used_bit = 1;
          c.pointer = curr->next;
          
          cout << "List After replacement: \n";
          c.printlist();
        }
      }
    }
    else if(choice == 3)
    {
      if(c.empty())
      {
        cout << "The List is empty\n";
        continue;
      }
      int number;
      cout << "Enter the Page number: ";
      cin >> number;
      cout << "The List Before referencing: \n";
      c.printlist();
      node* curr = c.head;
      while(curr != nullptr && curr->page_no != number)
      {
        curr = curr->next;
        if(curr == c.head)
        {
          cout << "The page does not exist in the list\n";
          break;
        }
      }
      
      curr->used_bit = 1;
      cout << "The List After referencing: \n";
      c.printlist();
    }
    else if(choice == 4)
    {
      
      cout << "Number of page faults: " << num_of_page_faults << endl;
      if(no_of_ele == 0)
      {
        cout << "Page fault percentage: " << 0 << endl;
      }
      else
      {
        float res1 = num_of_page_faults/ram_size * 100.000;
        cout << "Page fault percentage: " <<  res1 << endl;
      }
    }
  }
}

int randgen(int n)
{
  return rand() % n;
}

void TWO_LEVEL_PAGE_TABLE()
{
  int PAS, LAS, Fsize, PTE;
  
  cout << "Enter the Physical Address Space bits: ";
  cin >> PAS;
  cout << "Enter the Logical Address Space bits: ";
  cin >> LAS;
  cout << "Enter the Offset size bits: "; //also known as offset. also the page size is the same as frame size so I will also use it as page size
  cin >> Fsize;
  cout << "Enter the size of a single page table entry: ";
  cin >> PTE;
  
  int num_of_frames = pow(2, PAS) / pow(2, Fsize);
  int num_of_pages = pow(2, LAS) / pow(2, Fsize);
  int main_page_table_size = num_of_pages * PTE;
  
  cout << "Page table size: " << main_page_table_size << endl;
  
  if(main_page_table_size < pow(2, Fsize))
  {
    cout << "Page table can be fit into a single frame so no need to divide it.\n";
    vector<bool> check(num_of_frames, false);
    vector<int> page_table(num_of_pages);
    
    for(int i = 0; i < num_of_pages; i++)
    {
      int n = randgen(num_of_frames);
      
      while(check[n] == true)
      {
        n = randgen(num_of_frames);
      }
      check[n] = true;
      page_table[i] = n;
    }
    int logical_address;
    while(true)
    {
      cout << "Enter the Logical Address which you want to search: ";
      cin >> logical_address;
      if(logical_address == -1)
      {
        break;
      }
      while(logical_address > pow(2, LAS))
      {
        cout << "Value out of bound.\nPlease enter a value smaller than " << pow(2, LAS) << endl;
        cin >> logical_address;
      }
      int page_no = logical_address / pow(2, Fsize);
      int offset = logical_address % (int)pow(2, Fsize);
      cout << "Page number: " << page_no << " found at frame number: " << page_table[page_no] << " with offset of " << offset << endl;
    }
    
  }
  
    else
  {
    cout << "Page table can not be fit into a single frame so it needs to be divided into two tables.\n";
    
    int outer_page_table_entries = main_page_table_size / pow(2, Fsize);
    int inner_page_table_entries = main_page_table_size % (int)pow(2, Fsize);
    
    vector< pair<int, vector<int>> > outer_page_table(outer_page_table_entries);
    vector< vector<int> > inner_page_tables(outer_page_table_entries);
    vector<bool> check(num_of_frames, false);     
         
    for(int i = 0; i < outer_page_table_entries - 1; i++)
    {
      vector<int> inner_page_table(pow(2, Fsize));
      
      for(int j = 0; j < pow(2, Fsize); j++)
      {
        int n = randgen(num_of_frames);
      
        while(check[n] == true)
        {
          n = randgen(num_of_frames);
        }
        check[n] = true;
        inner_page_table[j] = n;
      }
      
      inner_page_tables[i] = move(inner_page_table);
      
      int n = randgen(num_of_frames);
      
      while(check[n] == true)
      {
        n = randgen(num_of_frames);
      }
      check[n] = true;
      
      outer_page_table[i] = make_pair(n, inner_page_tables[i]);
    }
    
    vector<int> inner_page_table(inner_page_table_entries);
      
    for(int j = 0; j < inner_page_table_entries; j++)
    {
      int n = randgen(num_of_frames);
      
      while(check[n] == true)
      {
        n = randgen(num_of_frames);
      }
      check[n] = true;
      inner_page_table[j] = n;
    }
      
    inner_page_tables[outer_page_table_entries - 1] = move(inner_page_table);
      
    int n = randgen(num_of_frames);
      
    while(check[n] == true)
    {
      n = randgen(num_of_frames);
    }
    check[n] = true;
      
    outer_page_table[outer_page_table_entries - 1] = make_pair(n, inner_page_tables[outer_page_table_entries - 1]);
    
    int logical_address;
    while(true)
    {
      cout << "Enter the Logical Address which you want to search: ";
      cin >> logical_address;
      if(logical_address == -1)
      {
        break;
      }
      while(logical_address > pow(2, LAS))
      {
        cout << "Value out of bound.\nPlease enter a value smaller than " << pow(2, LAS) << endl;
        cin >> logical_address;
      }
      int page_no = logical_address / pow(2, Fsize);
      int offset = logical_address % (int)pow(2, Fsize);
      int outer_table_index = page_no / pow(2, Fsize);
      int offset_inner_page_table = page_no % (int)pow(2, Fsize);
      
      cout << "Page number: " << page_no << " found at inner_table_index: " << outer_table_index << " with inner Page table offset of: " << offset_inner_page_table << endl;
      cout << "Which is located at frame number: " << outer_page_table[outer_table_index].second[offset_inner_page_table] << " with offset of: " << offset << endl;
      
    }
    
  }
}

int main(int argc, char* argv[])
{
  int choice, subchoice;
  
  srand(time(NULL));
  
  do 
  {
    //system("clear");
    cout << "1) CPU Scheduling" << endl;
    cout << "2) Dining Philosophers Problem" << endl;
    cout << "3) Bankers Algorithm" << endl;
    cout << "4) Memory Management" << endl;
    cout << "5) Page Replacement" << endl;
    cout << "6) Clear Screen" << endl;
    cout<< "0) Exit the program" << endl;
    
    cin >> choice;
    
    switch (choice)
    {
      case 1:
        do
        {
          //system("clear");
          cout << "1) Multilevel Queue Scheduling" <<endl;
          cout << "2) Multilevel Feedback Queue Scheduling" <<endl;
          cout << "3) Longest Job First" <<endl;
          cout << "4) Lowest Feedback Ratio Next Scheduling" <<endl;
          cout << "5) Clear Screen" << endl;
          cout << "0) Return to previous screen" << endl;
          
          cin >> subchoice;
          
          switch (subchoice)
          {
            case 1:
              cout << "Now performing Multilevel Queue Scheduling" << endl;
              MQS();
              break;
              
            case 2:
              cout << "Now performing Multilevel Feedback Queue Scheduling" << endl;
              MLFQ();
              break;
            
            case 3:
              cout << "Now performing Longest Job First" <<endl;
              LJF();
              break;
            
            case 4:
              cout << "Now performing Lowest Feedback Ratio Next Scheduling" <<endl;
              LFRN();
              break;
              
            case 5:
              system("clear");
              break;
            
            case 0:
              break;
            
            default:
              cout << "Invalid choice" << endl;
              break;
          }
        }while(subchoice != 0);
        break;
        
      case 2:
        cout << "Now performing Dining Philosophers Problem" << endl;
        Dining_func();
        break;
        
      case 3:
        cout << "Now performing Bankers algorithm" << endl;
        bankers_func();
        break;
        
      case 4:
        cout << "Now performing Memory Management" << endl;
        int choice2;
        do
        {
          cout << "1) Two level Page Table Scheme" <<endl;
          cout << "2) Hashed Page Table with TLB" <<endl;
          cout << "3) Clear Screen" << endl;
          cout << "0) Return to previous screen" << endl;
          cin >> choice2;
          switch(choice2)
          {
          
            case 1:
              cout << "Now performing Two level Page Table Scheme." << endl;
              TWO_LEVEL_PAGE_TABLE();
              break;
              
            case 2:
              cout << "Now performing Hashed Page Table with TLB." << endl;
              HashedPageTable_With_TLB();
              break;
              
            case 3:
              system("clear");
              break;
              
            case 0:
              break;
            
            default:
              cout << "Invalid choice" << endl;
              break;
          }
        }while(choice2 != 0);
        
        break;
        
      case 5:
        cout << "Now performing Page Replacement using Second Chance algorithm" << endl;
        SCA();
        break;
        
      case 6:
        system("clear");
        break;
        
      case 0:
        system("clear");
        cout << "Program Terminated successfully" << endl;
        break;
        
      default:
        cout << "Invalid Choice" << endl;
        break;
    }
  } while(choice != 0);
  
  return 0;
}
