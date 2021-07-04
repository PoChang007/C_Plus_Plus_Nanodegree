#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();                   
  std::vector<Process>& Processes();  
  float MemoryUtilization();          
  long UpTime();                      
  int TotalProcesses();               
  int RunningProcesses();             
  std::string Kernel();               
  std::string OperatingSystem();      
  
  void ProcessorClear() {
    cpu_ = {};
  }

  void ProcessesClear() {
    processes_.clear();
  }

 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
};

#endif