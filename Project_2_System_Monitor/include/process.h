#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization();                  
  std::string Ram();                       
  long int UpTime();                       
  bool operator<(Process const& a) const;  

  Process(int pid, std::string ram, std::string usr, long int uptime, 
          std::string command, float processCpuInfo) {
    pid_ = pid;
    ram_ = ram;
    usr_ = usr;
    uptime_ = uptime;
    command_ = command;
    processCpuInfo_ = processCpuInfo;
  }

  int GetRam() { return std::stoi(ram_); }

  float GetCpuutilization() { return processCpuInfo_; }

 private:
  int pid_;
  std::string ram_;
  std::string usr_;
  long int uptime_;
  std::string command_;
  float processCpuInfo_;
};

#endif