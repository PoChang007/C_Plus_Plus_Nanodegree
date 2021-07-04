#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

class Processor {
 public:
  float Utilization();

  Processor() {}
  Processor(std::vector<std::string> processor) {
    user_ = std::stoi(processor[0]);
    nice_ = std::stoi(processor[1]);
    system_ = std::stoi(processor[2]);
    idle_ = std::stoi(processor[3]);
    iowait_ = std::stoi(processor[4]);
    irq_ = std::stoi(processor[5]);
    softirq_ = std::stoi(processor[6]);
    steal_ = std::stoi(processor[7]);
    guest_ = std::stoi(processor[8]);
    guest_nice_ = std::stoi(processor[9]);
  }

 private:
  int user_;
  int nice_;
  int system_;
  int idle_;
  int iowait_;
  int irq_;
  int softirq_;
  int steal_;
  int guest_;
  int guest_nice_;
};

#endif