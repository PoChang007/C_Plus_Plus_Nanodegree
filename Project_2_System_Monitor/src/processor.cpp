#include "processor.h"

// Return the aggregate CPU utilization
float Processor::Utilization()
{
  int idle = idle_ + iowait_;
  int nonIdle = user_ + nice_ + system_ + irq_ + softirq_ + steal_;
  int total = idle + nonIdle;
  int totald = total;
  int idled = idle;

  return (float)(totald - idled) / (float)totald;
}