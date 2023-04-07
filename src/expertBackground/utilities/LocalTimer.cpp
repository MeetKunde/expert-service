#include "LocalTimer.h"

namespace expertBackground {
LocalTimer::LocalTimer() : lastTimeStamp(std::chrono::steady_clock::now()) {}

void LocalTimer::startMeasurement() {
  LocalTimer::lastTimeStamp = std::chrono::steady_clock::now();
}

void LocalTimer::takeMeasurement(const std::string& measurementTitle, bool startNextMeasurement, std::ostream& stream) {
  LocalTimer::currentTimeStamp = std::chrono::steady_clock::now();

  const std::chrono::duration<double> time_span =
      std::chrono::duration_cast<std::chrono::duration<double>>(LocalTimer::currentTimeStamp - LocalTimer::lastTimeStamp);

  if (startNextMeasurement) {
    LocalTimer::lastTimeStamp = LocalTimer::currentTimeStamp;
  }

  stream << measurementTitle << " done!    Measurement tooks " << time_span.count() << " seconds.\n";
}
}  // namespace expertBackground