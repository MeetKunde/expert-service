#include "Utilities.h"

namespace symbolicAlgebra::implementation {
std::pair<int, int> Utilities::addFractions(const std::vector<std::pair<int, int>>& fractions) {
  int nominator = 0;
  int denominator = 1;

  for (const std::pair<int, int>& frac : fractions) {
    const int base = std::lcm(denominator, frac.second);
    nominator *= (base / denominator);
    nominator += frac.first * (base / frac.second);
    denominator = base;
  }

  const int gcd = std::gcd(nominator, denominator);

  return {nominator / gcd, denominator / gcd};
}

std::pair<int, int> Utilities::mulFractions(const std::vector<std::pair<int, int>>& fractions) {
  int nominator = 1;
  int denominator = 1;

  for (const std::pair<int, int>& frac : fractions) {
    nominator *= frac.first;
    denominator *= frac.second;
  }

  const int gcd = std::gcd(nominator, denominator);

  return {nominator / gcd, denominator / gcd};
}
}  // namespace symbolicAlgebra::implementation