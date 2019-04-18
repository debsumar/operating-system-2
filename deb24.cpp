#include <iostream>
#include <bits/stdc++.h>
#include <queue>
#include <vector>

using namespace std;

struct ProcessData
{
	int Num;
	int Pid;  //Process Id
	int Arrival_time; //Process Arrival Time
	int Burst_time; //Process Bruest Time
	int Priority; //Process Priority
	int Finish_time; //Process Finish Time
	int Time_to_Finsh; //Process Remaining  Time During Execution
	int Waiting_Time; //Waiting Time
	int Start_Time; //Process start Time
	int Response_Time;

};
struct ProcessData currentProcess;


class ProcessInfo
{
public:

	vector< ProcessData > input;
	vector<ProcessData> input_copy;
	int numberOfProcesses;
	int total_exection_time;
	int* Ghant;
public:
	ProcessInfo();
	~ProcessInfo();
	void takeInput();
	int FindTotalExecutionTime(ProcessInfo&);
	static bool SortOnID(const ProcessData& x, const ProcessData& y);
	static bool SortOnArrivalTime(const ProcessData& x, const ProcessData& y);
	static bool Numsort(const ProcessData& x, const ProcessData& y);
	void StartScheduling(ProcessInfo&);
	void DisplayResult(ProcessInfo&);

};

struct comPare
{
	bool operator()(const ProcessData& x, const ProcessData& y)
	{
		if (x.Priority > y.Priority)
			return true;
		else if (x.Priority < y.Priority)
			return false;
		if (x.Pid > y.Pid)
			return true;

		return false;

	}

};

ProcessInfo::ProcessInfo() // Constructor
{
	numberOfProcesses = 0;
	total_exection_time = 0;
	Ghant = NULL;
}

ProcessInfo::~ProcessInfo()
{
	numberOfProcesses = 0;
	total_exection_time = 0;
	delete[] Ghant;
}

void ProcessInfo::takeInput()
{
	ProcessData temp;

	int Arrival_time;
	int Burst_time;
	int Pid;
	int Priority;

	for (int i = 0; i<numberOfProcesses; i++)
	{	cout<<"Enter process ID:";
	{
		cin >> Pid;
		cout<<"Enter Arrival Time:";
		cin>>Arrival_time;
		cout<<"Enter Burst_time:";
		cin>>Burst_time;
		cout<<"Priority:";
		cin>>Priority;
		temp.Num = i + 1;
		temp.Arrival_time = Arrival_time;
		temp.Burst_time = Burst_time;
		temp.Time_to_Finsh = Burst_time;
		temp.Pid = Pid;
		temp.Priority = Priority;
		this->input.push_back(temp);
	}
	this->input_copy = this->input;
}
}

int ProcessInfo::FindTotalExecutionTime(ProcessInfo& myProcess)
{
	int t = 0;
	t = t + myProcess.input[0].Arrival_time;
	for (int i = 0; i<numberOfProcesses; i++)
	{
		if (t >= myProcess.input[i].Arrival_time)
		{
			t += myProcess.input[i].Burst_time;
		}
		else
		{
			int diff = (myProcess.input[i].Arrival_time - t);
			t += diff + myProcess.input[i].Burst_time;

		}
	}
	return t;
}
static bool SortOnID(const ProcessData& x, const ProcessData& y)
{
	return x.Pid < y.Pid;
}

static bool SortOnArrivalTime(const ProcessData& x, const ProcessData& y)
{

	if (x.Arrival_time < y.Arrival_time)
		return true;
	else if (x.Arrival_time > y.Arrival_time)
		return false;
	if (x.Priority < y.Priority)
		return true;
	else if (x.Priority > y.Priority)
		return false;
	if (x.Pid < y.Pid)
		return true;

	return false;
}

static bool Numsort(const ProcessData& x, const ProcessData& y)
{
	return x.Num < y.Num;
}

void ProcessInfo::StartScheduling(ProcessInfo& myProcess)
{
	Ghant = new int[total_exection_time];
	int clock;
	//memset(Ghant, 0, total_exection_time);
	int PreemptivePriorityQueueProcess = 0; // for PriorityQueue process
	int RoundRobinQueueProcess = 0; // for RoundRobinQueue process
	int i;
	for (i = 0; i< total_exection_time; i++)
	{
		Ghant[i] = -1;
	}
	//cout<<"total_exection_time : "<<total_exection_time<<endl;

	priority_queue < ProcessData, vector<ProcessData>, comPare> PriorityQueue; //Priority Queue PriorityQueue

	queue< ProcessData > RoundRobinQueue; //Round Robin Queue RoundRobinQueue
	int cpu_state = 0; //idle if 0 then Idle if 1 the Busy
	int quantum = 4; //Time Quantum
	currentProcess.Pid = -2;
	currentProcess.Priority = 999999;

	for (clock = 0; clock< total_exection_time; clock++)
	{
		/**Insert the process with same Arrival time in Priority Queue**/
		for (int j = 0; j<myProcess.numberOfProcesses; j++)
		{
			if (clock == myProcess.input[j].Arrival_time)
			{
				PriorityQueue.push(myProcess.input[j]);
			}
		}


		if (cpu_state == 0) //If CPU idle
		{
			if (!PriorityQueue.empty())
			{
				currentProcess = PriorityQueue.top();
				cpu_state = 1;
				PreemptivePriorityQueueProcess = 1;
				PriorityQueue.pop();
				quantum = 4;
			}
			else if (!RoundRobinQueue.empty())
			{
				currentProcess = RoundRobinQueue.front();
				cpu_state = 1;
				RoundRobinQueueProcess = 1;
				RoundRobinQueue.pop();
				quantum = 4;
			}
		}
		else if (cpu_state == 1) //If cpu has any procss
		{
			if (PreemptivePriorityQueueProcess == 1 && (!PriorityQueue.empty()))
			{
				if (PriorityQueue.top().Priority < currentProcess.Priority) //IfnumberOfProcessesew process has high priority
				{
					RoundRobinQueue.push(currentProcess); //push currentProcess in RoundRobinQueue
					currentProcess = PriorityQueue.top();
					PriorityQueue.pop();
					quantum = 4;
				}
			}
			else if (RoundRobinQueueProcess == 1 && (!PriorityQueue.empty())) //If process is from RoundRobinQueue andnumberOfProcessesew process come  in PriorityQueue
			{
				RoundRobinQueue.push(currentProcess);
				currentProcess = PriorityQueue.top();
				PriorityQueue.pop();
				RoundRobinQueueProcess = 0;
				PreemptivePriorityQueueProcess = 1;
				quantum = 4;
			}


		}


		if (currentProcess.Pid != -2) // Process Execution
		{
			currentProcess.Time_to_Finsh--;
			quantum--;
			Ghant[clock] = currentProcess.Pid;
			if (currentProcess.Time_to_Finsh == 0) //If process Finish
			{
				cpu_state = 0;
				quantum = 4;
				currentProcess.Pid = -2;
				currentProcess.Priority = 999999;
				RoundRobinQueueProcess = 0;
				PreemptivePriorityQueueProcess = 0;
			}
			else if (quantum == 0) //If time Qunatum of a currentProcess running process Finish
			{
				RoundRobinQueue.push(currentProcess);
				currentProcess.Pid = -2;
				currentProcess.Priority = 999999;
				RoundRobinQueueProcess = 0;
				PreemptivePriorityQueueProcess = 0;
				cpu_state = 0;

			}

		}

	}
}

void ProcessInfo::DisplayResult(ProcessInfo& myProcess)
{



	for (int i = 0; i<myProcess.numberOfProcesses; i++)
	{
		cout << "PID Response Time Finish Time Wait Time\n";
		cout << myProcess.input[i].Pid << " " << myProcess.input[i].Response_Time << " " << myProcess.input[i].Finish_time << " " << myProcess.input[i].Waiting_Time << endl;

	}
}

int main()
{
	ProcessInfo myProcess;
	int i;
	cout<<"Enter number of processes:";
	cin >> myProcess.numberOfProcesses;
	myProcess.takeInput();

	sort(myProcess.input.begin(), myProcess.input.end(), SortOnArrivalTime);

	myProcess.total_exection_time = myProcess.FindTotalExecutionTime(myProcess);
	myProcess.StartScheduling(myProcess);
	sort(myProcess.input.begin(), myProcess.input.end(), SortOnID);
	for (int i = 0; i<myProcess.numberOfProcesses; i++)
	{
		for (int k = myProcess.total_exection_time; k >= 0; k--)
		{
			if (myProcess.Ghant[k] == myProcess.input[i].Pid)
			{
				myProcess.input[i].Finish_time = k;
				break;

			}
		}
	}
	for (int i = 0; i<myProcess.numberOfProcesses; i++)
	{
		for (int k = 0; k<myProcess.total_exection_time; k++)
		{

			if (myProcess.Ghant[k] == myProcess.input[i].Pid)
			{
				myProcess.input[i].Start_Time = k;
				break;
			}
		}
	}

	sort(myProcess.input.begin(), myProcess.input.end(), Numsort);

	for (int i = 0; i<myProcess.numberOfProcesses; i++)
	{
		myProcess.input[i].Response_Time = myProcess.input[i].Start_Time - myProcess.input[i].Arrival_time + 1;
		myProcess.input[i].Waiting_Time = (myProcess.input[i].Finish_time - myProcess.input[i].Arrival_time) - myProcess.input[i].Burst_time + 1;

	}
	myProcess.DisplayResult(myProcess);


	return 0;
}
