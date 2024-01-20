# Operating System Simulator
C++ Project for Operating Systems Course of 4th Semester at FAST NUCES.  
In this project, I was tasked to develop an operating system simulator. The simulator was developed using concepts of operating systems which we had studied during the 
course.  
---
## How to Run:
* Project can be run using any C++ IDE.
* 1. First step is to install any C++ IDE or Compiler.
  2. After setting up your IDE, make a C++ file.
  3. Now, copy the code from the main OS.cpp file and compile it.
---
## Functionalities
The simulator performs the following functionalities:  
1. _**CPU scheduling:**_  
Implement the following CPU Scheduling algorithms and choose parameters (i.e 
waiting time burst time etc) for CPU Scheduling accordingly.  
    ● **Multilevel Queue Scheduling**:
     * I implemented 3 queues; each queue will implement a different scheduling 
        algorithm. First queue will implement priority scheduling, 2nd will implement RR and third 
        queue will be implemented using SRJF(shortest remaining job first).
         
    ● Multilevel Feedback Queue Scheduling:  
   * A process can move between the various queues; aging can be implemented this way.  
        It is defined by the following parameters:    
         ● number of queues(3)   
         ● scheduling algorithms for each queue  
         ● method used to determine when to upgrade a process  
         ● method used to determine when to demote a process  
         ● method used to determine which queue a process will enter when that process needs service
       
        Similarly for MLFQ I have again implemented 3 queues, each queue will implement 
        a different scheduling algorithm. First queue will implement FCFS, 2nd will implement SJF and third queue will be 
        implemented using RR (shortest remaining job first).  
  
    ● Longest Job First  
    ● Lowest Feedback Ratio Next Scheduling:  
   * Lowest Feedback Ratio Next is one of the scheduling algorithms. This is 
        a non-preemptive algorithm in which the scheduling is done on the basis of an 
        extra parameter called Feedback Ratio. A Feedback Ratio is calculated for each 
        of the available jobs and the Job with the lowest feedback ratio  is given priority 
        over the others.   
        Feedback Ratio is calculated by the given formula: Feedback Ratio = (W+S)/S   
            Where: W is Waiting Time, S is Service Time or Burst Time.  
3. _**Semaphore**_:  
  * Implemented Dining Philosopher Problem.   
  *   Designed and implemented a solution for the dining philosopher’s problem using semaphores.  
  In this problem, a group of philosophers sit at a round table and alternate between 
  thinking and eating. However, there are only a limited number of chopsticks available 
  for the philosophers to use as utensils. To eat, a philosopher must pick up both 
  chopsticks adjacent to them. If a philosopher cannot pick up both chopsticks, they 
  must wait until both are available. Use semaphores to ensure that each philosopher 
  is able to eat without creating a deadlock or starvation scenario.(for more information 
  see the slides). 
3. _**Deadlock:**_:   
● Bankers Algorithm: Banker’s algorithm is used to avoid deadlock and allocate resources safely to each process in 
the computer system.
● The 'S-State' examines all possible tests or activities before deciding 
whether the allocation should be allowed to each process.
● It also helps the operating system 
to successfully share the resources between all the processes.
● The banker's algorithm is 
named because it checks whether a person should be sanctioned a loan amount or not to help 
the bank system safely simulate allocation resources.  
● Suppose the number of account holders in a particular bank is 'n', and the total money in a 
bank is ‘T’. If an account holder applies for a loan; first, the bank subtracts the loan amount 
from full cash and then estimates the cash difference is greater than T to approve the loan 
amount. These steps are taken because if another person applies for a loan or withdraws 
some amount from the bank, it helps the bank manage and operate all things without any 
restriction in the functionality of the banking system.   
● Similarly, it works in an operating system. When a new process is created in a computer 
system, the process must provide all types of information to the operating system like 
upcoming processes, requests for their resources, counting them, and delays. Based on these 
criteria, the operating system decides which process sequence should be executed or waited 
so that no deadlock occurs in a system. Therefore, it is also known as deadlock avoidance 
algorithm or deadlock detection in the operating system.    
When working with a banker's algorithm, it requests to know about three things:     
  ● How much each process can request for each resource in the system.  
  ● How much each process is currently holding each resource in a system. It 
  represents the number of each resource currently available in the system. Following are the important data structures terms applied in the banker's algorithm as 
follows:  
● The Banker's Algorithm is the combination of the safety algorithm and the resource request 
algorithm to control the processes and avoid deadlock in a system:  
● _Safety Algorithm_:    
It is a safety algorithm used to check whether or not a system is in a safe state or follows the 
safe sequence in a banker's algorithm.  
● _Resource Request Algorithm_:   
A resource request algorithm checks how a system will behave when a process makes each 
type of resource request in a system as a request matrix.
  
4. _**Memory Management**_:  
This section implements the following concepts:  
● Two-Level Page-Table Scheme  
● Hashed Page Table with TLB   
5. _**Page Replacement**_:   
● Implement Second Chance Algorithm using circular link list.
● Also Using Second chance Algorithm calculate No. of Page faults, page fault probability 
and page fault percentage.
--- 

## Interface of Project   
* The Interface of the simulator is console based. Project has a complete menu in which user will select option and Simulator will perform functionality.
---
