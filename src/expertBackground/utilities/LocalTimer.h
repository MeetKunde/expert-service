#ifndef EXPERT_SERVICE_LOCAL_TIMER_H
#define EXPERT_SERVICE_LOCAL_TIMER_H

#include <chrono>
#include <ostream>

namespace expertBackground {

class LocalTimer {
 private:
  std::chrono::steady_clock::time_point lastTimeStamp;
  std::chrono::steady_clock::time_point currentTimeStamp;

 public:
  LocalTimer();

  void startMeasurement();
  void takeMeasurement(const std::string& measurementTitle, bool startNextMeasurement, std::ostream& stream);
};

}  // namespace expertBackground

#endif  //EXPERT_SERVICE_LOCAL_TIMER_H
