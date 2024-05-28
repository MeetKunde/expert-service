#ifndef DEPENDENCY_CONCEPTS_H
#define DEPENDENCY_CONCEPTS_H

#include <nlohmann/json.hpp>

namespace expertBackground {
/**
 * @brief Concept to check if a type is streamable to std::ostream
*/
template<typename T>
concept streamable = requires(std::ostream& os, T t) {
    { os << t } -> std::convertible_to<std::ostream&>;
};

/**
 * @brief Concept to check if a type is streamable to json
*/
template<typename T>
concept jSONStreamable = requires(nlohmann::json& j, T t) {
    { j << t } -> std::convertible_to<nlohmann::json&>;
};

/**
 * @brief Concept to check if a type is comparable
*/
template<typename T>
concept comparable = requires(T a, T b) {
  { a == b } -> std::convertible_to<bool>;
  { a != b } -> std::convertible_to<bool>;
  { a < b }  -> std::convertible_to<bool>;
  { a <= b } -> std::convertible_to<bool>;
  { a > b }  -> std::convertible_to<bool>;
  { a >= b } -> std::convertible_to<bool>;
};

/**
 * @brief Concept to check if all types in a pack are streamable to std::ostream
*/
template<typename... Args>
concept AllStreamable = (streamable<Args> && ...);

/**
 * @brief Concept to check if all types in a pack are streamable to json
*/
template<typename... Args>
concept AllJSONStreamable = (jSONStreamable<Args> && ...);

/**
 * @brief Concept to check if all types in a pack are comparable
*/
template<typename... Args>
concept AllComparable = (comparable<Args> && ...);


template<typename T, typename U>
struct is_same_type : std::false_type {};

template<typename T>
struct is_same_type<T, T> : std::true_type {};

template<typename T, typename... Args>
struct are_all_of_type;

template<typename T>
struct are_all_of_type<T> : std::true_type {};

template<typename T, typename First, typename... Rest>
struct are_all_of_type<T, First, Rest...>
    : std::conditional_t<is_same_type<T, First>::value, are_all_of_type<T, Rest...>, std::false_type> {};

/**
 * @brief Check if all types in a pack are of the same type
 * @tparam T target type
 * @tparam Args types to check
 */
template<typename T, typename... Args>
constexpr bool AllAreOfType = are_all_of_type<T, Args...>::value;
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