# mini-scheduler

## 1. Introduction
The project is to write a simplistic simulator of a process scheduler.

## 2. Workload
You are given a workload, that is a trace of an operating system activity which lists events that are to happen during a period of time, at discretized timesteps.

An event is a process, with a unique pid that is planned to start at time ts. It also has an estimated finish time tf. The finish time can be delay in case this process is interrupted during its lifetime. In case of interruption, we will record for how long in the idle field and increase tf.

The parent of a process is also recorded in the ppid field.

Most importantly for the scheduler, each process has a priority prio. We consider here priorities from 1 to 10. The higher the priority, the greater chance the process has to run without being interrupted.

## 3. Scheduling Rules
### 3.1. CPU capability
Each running process is given a share of the CPU: we decide that this share is proportional to its priority, to reflect the fact that a process with a high priority has less chance to be de-scheduled from running. Note: We consider in this subject a single CPU.
The CPU has a limited capability: it can simultaneously run a number of processes up to a maximum capability C
. We will set C=20
 in this project.
For instance, consider a set of n
 processes Pi
, each with a priority prioi
, all n
 processes can run simultaneously only if ∑ni=1prioi≤C
. If the sum of priorities exceed C
, some of them should be put asleep waiting for room on the CPU.
### 3.2. Queues
The scheduler maintains 2 queues, each containing pairs (prio,pid).

the running queue,
the pending queue.
At the end of a scheduling round at time t
, the processes that have been put in the running queue, are considered the ones being running at timestep t
. Those in the pending queue are either waiting for their start date to reach the current timestep (ts≥t
), or have been de-scheduled because higher-priority processes filled the CPU capacity.

### 3.3. Scheduling
Our simulation should enumerate all timesteps from 0 to N (We set N to 20 in our example).

At each timestep t
, we run a scheduling round.
During the scheduling round t
, the scheduler looks up all possible processes, which are those with a finish date greater than the the current timestep (t≤tf
). So, at t=0
 all processes are possible. Otherwise, having t>tf
 means the process is finished, and should be removed from all queues.
Among the possible processes, the scheduler considers at time t
 only those that can possibly run, i.e a process must satisfy: ts≤t≤tf
. We say such a process is current.
The scheduler has then to choose among the current processes those to run. Consider n
 current processes at time t
. Two cases can arise:
All processes can fit in the CPU because the sum of their priorities is less or equal than C
. In that case all are put in the running queue.
The sum of priorities is greater than C
. In that case, only the processes with greater priority should be put in running queue. The rest of the processes should join the waiting queue (with all the other possible processes). Note that this can apply to a process that just came as a new candidate (ts=t
) or to a process that was previously running (t>ts
). This latter situation corresponds to a process being de-scheduled.
When a process is de-scheduled in a given scheduling round, we increment by 1 its idle
 time, and its tf
 since its termination is delayed.
Here is a recap of the rules. At a scheduling round with n
 current processes, consider P1,...,Pk
 are put in running queue, and Pk+1,...,Pn
 are put in waiting queue. Two properties must hold:

processes in running queue must not exceed the CPU capability (as mentioned earlier) : ∑ki=1prioi≤C
.
the priority of processes in run queue must be greater or equal to any current process priority in waiting queue :
∀Pi,i∈{1,...,k}, ts(Pi)≤t≤tf(Pi)∀Pj,j∈{k+1,...,n}, ts(Pj)≤t≤tf(Pj)we have prio(Pi)≥prio(Pj)

## 4. Input
You must read the workload from a text file with the same structure as the one given in the C structure as an example in the beginning of the document. The equivalent workload text file is shown below. Its columns are blank-separated.

Your executable shoud be named sched and read such a text file from the stadard input (stdin).

## 5. Output
Your program must output a recap of the scheduling for the input workload as a chronogram. The horizontal line starting from first | (which is t=0
) represents the time. On the vertical axis are the processes' commands, followed by the characters X to represent the timeframe when the process was scheduled first, and then finished.

        |....|....|....|....
init    XXXXXXXXXXXXXXXXXX                       (tf=18,idle=0)
bash     XXXXXXXXXXXXXXXXXXX                     (tf=20,idle=4)
bash       XXXXXXXXXXXXXXXX                      (tf=19,idle=3)
find        XX                   (tf=6,idle=0)
gcc            XX                        (tf=9,idle=0)
ld              X                        (tf=9,idle=0)
ssh               XXXXXXX                        (tf=17,idle=4)
crypt              XX                    (tf=13,idle=0)
snake                 XXX                        (tf=17,idle=1)
cat                   X                  (tf=15,idle=0)
The chronogram must reflect the actual final date of the processes, and the total idle time suffered by the process. On the example below, you can see that the two bash processes have been de-scheduled several times due to their low priority: bash with pid=1
 finishes actually at t=20
 while it was initially planned to finish at 16
.

## 6. Trace
Finally, here is an exec of my scheduler on the example workload. Altough this is a bit lengthy, it might be informative when you have your's ready and runinng to compare.

The output should be sel-explanatory. Note that the states of the running and pending queues are displayed after each scheduling round. The elements of this queue are (prio,pid).

The CPU occupation is displayed each time it is tested for available room or modified.
