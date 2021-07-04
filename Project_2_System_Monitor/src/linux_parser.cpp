#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// Read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string memTotal, memFree;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (getline(stream, line)) {
      std::string currentKey, currentNum;
      std::istringstream linestream(line);
      linestream >> currentKey >> currentNum;
      if (currentKey == "MemTotal:") {
        memTotal = currentNum;
      }
      if (currentKey == "MemFree:") {
        memFree = currentNum;
      }
    }
  }
  float memUtilizationRate =
      (std::stof(memTotal) - std::stof(memFree)) / std::stof(memTotal);
  return memUtilizationRate;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  string upTime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    getline(stream, line);
    std::istringstream linestream(line);
    linestream >> upTime;
  }
  return std::stol(upTime);
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  std::string line;
  std::vector<std::string> processorInfo;
  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open()) {
    while (getline(stream, line)) {
      std::istringstream linestream(line);
      std::string token;
      linestream >> token;
      if (token == "cpu") {
        while (linestream >> token) processorInfo.push_back(token);
      }
    }
  }

  return processorInfo;
}

float LinuxParser::CpuUtilizationProcess(int pid) {
  std::vector<std::string> processCpuInfo;
  std::string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);

  if (stream.is_open()) {
    getline(stream, line);
    std::string currentToken;
    std::istringstream linestream(line);
    while (linestream >> currentToken) {
      processCpuInfo.push_back(currentToken);
    }
  }

  float utime = 0.0f;
  float stime = 0.0f;
  float cutime = 0.0f;
  float cstime = 0.0f;
  float starttime = 0.0f;

  try {
    utime = std::stof(processCpuInfo[13]);
  } catch (...) {
  }
  try {
    stime = std::stof(processCpuInfo[14]);
  } catch (...) {
  }
  try {
    cutime = std::stof(processCpuInfo[15]);
  } catch (...) {
  }
  try {
    cstime = std::stof(processCpuInfo[16]);
  } catch (...) {
  }
  try {
    starttime = std::stof(processCpuInfo[21]);
  } catch (...) {
  }

  float total_time = utime + stime + cutime + cstime;
  float seconds = (float)LinuxParser::UpTime() -
                  ((starttime + 0.0000001f) / (float)sysconf(_SC_CLK_TCK));
  float cpu_usage =
      ((total_time + 0.0000001f) / (float)sysconf(_SC_CLK_TCK)) / seconds;

  return cpu_usage;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::string processesKey, processNum;
  std::string line;
  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open()) {
    while (getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> processesKey >> processNum;
      if (processesKey == "processes") break;
    }
  }
  return std::stoi(processNum);
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::string procRunning, procRunningNum;
  std::string line;
  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open()) {
    while (getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> procRunning >> procRunningNum;
      if (procRunning == "procs_running") break;
    }
  }
  return std::stoi(procRunningNum);
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  std::string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);

  if (stream.is_open()) {
    getline(stream, line);
  }
  return line.substr(0, 60);
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line;
  string ramKey, ramValue = "0";
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);

  if (stream.is_open()) {
    while (getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> ramKey;
      if (ramKey == "VmSize") {
        linestream >> ramValue;
        return std::to_string(std::stoi(ramValue) / 1000);
      }
    }
  }
  return ramValue;
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  std::string firstToken, secondToken;
  std::string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);

  if (stream.is_open()) {
    while (getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> firstToken >> secondToken;
      if (firstToken == "Uid:") break;
    }
  }
  return secondToken;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  auto uid = LinuxParser::Uid(pid);
  std::string firstToken;
  std::string line;
  std::ifstream stream(kPasswordPath);

  if (stream.is_open()) {
    while (getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::string secondToken, thirdToken, fourthToken;
      std::istringstream linestream(line);
      linestream >> firstToken >> secondToken >> thirdToken >> fourthToken;
      if (thirdToken == uid && fourthToken == uid) break;
    }
  }
  return firstToken;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  std::string line;
  std::string uptimeToken;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  std::vector<string> token;

  if (stream.is_open()) {
    getline(stream, line);
    std::istringstream linestream(line);

    while (linestream >> uptimeToken) {
      token.push_back(uptimeToken);
    }
  }

  try {
    return (std::stol(token[21]) / sysconf(_SC_CLK_TCK));
  } catch (std::invalid_argument& error) {
    return std::stol("0");
  }
}
