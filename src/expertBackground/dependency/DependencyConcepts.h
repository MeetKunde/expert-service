#ifndef DEPENDENCY_CONCEPTS_H
#define DEPENDENCY_CONCEPTS_H

#include <nlohmann/json.hpp>

namespace expertBackground {
/**
 * @brief Concept to check if a type is streamable to std::ostream
*/
template<typename T>
concept Streamable = requires(std::ostream& os, T t) {
    { os << t } -> std::convertible_to<std::ostream&>;
};

/**
 * @brief Concept to check if a type is streamable to json
*/
template<typename T>
concept JSONStreamable = requires(nlohmann::json& j, T t) {
    { j << t } -> std::convertible_to<nlohmann::json&>;
};

/**
 * @brief Concept to check if all types in a pack are streamable to std::ostream
*/
template<typename... Args>
concept AllStreamable = (Streamable<Args> && ...);

/**
 * @brief Concept to check if all types in a pack are streamable to json
*/
template<typename... Args>
concept AllJSONStreamable = (JSONStreamable<Args> && ...);

/**
 * @brief Concept to check if a type is comparable
*/
template<typename T>
concept Comparable = requires(T a, T b) {
    { a == b } -> std::convertible_to<bool>;
    { a != b } -> std::convertible_to<bool>;
    { a < b }  -> std::convertible_to<bool>;
    { a <= b } -> std::convertible_to<bool>;
    { a > b }  -> std::convertible_to<bool>;
    { a >= b } -> std::convertible_to<bool>;
};

/**
 * @brief Concept to check if all types in a pack are comparable
*/
template<typename... Args>
concept AllComparable = (Comparable<Args> && ...);
}  // namespace expertBackground


namespace nlohmann {
/**
* @brief Streamable to json concept for nlohmann::json and std::string
*/
inline nlohmann::json& operator<<(nlohmann::json &j, const std::string& str) {
  j = str;
  return j;
}
}  // namespace nlohmann

#endif //DEPENDENCY_CONCEPTS_H