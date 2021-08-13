#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using namespace std;

bool compare(Process A, Process B)
{
    return A.GetCpuutilization() > B.GetCpuutilization();
}

// Return the system's CPU
Processor &System::Cpu()
{
    Processor processor(LinuxParser::CpuUtilization());
    cpu_ = processor;
    return cpu_;
}

// Return a container composed of the system's processes
vector<Process> &System::Processes()
{
    auto pidList = LinuxParser::Pids();

    for (size_t i = 0; i < pidList.size(); ++i)
    {
        Process process(pidList[i], LinuxParser::Ram(pidList[i]), LinuxParser::User(pidList[i]),
                        LinuxParser::UpTime(pidList[i]), LinuxParser::Command(pidList[i]),
                        LinuxParser::CpuUtilizationProcess(pidList[i]));
        processes_.emplace_back(process);
    }
    std::sort(processes_.begin(), processes_.end(), compare);
    return processes_;
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }