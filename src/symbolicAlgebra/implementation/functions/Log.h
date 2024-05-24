#ifndef ATOM_LOG

#ifdef FORWARD_DECLARATION
#ifndef LOG_FORWARD
#define LOG_FORWARD

namespace symbolicAlgebra::implementation {
class Log;
}

#endif  // LOG_FORWARD
#endif  // FORWARD_DECLARATION

#ifdef DECLARATION
#define ATOM_LOG
#ifndef LOG_DECLARE
#define LOG_DECLARE

namespace symbolicAlgebra::implementation {
class Log : public Atom {
 public:
  Log() = delete;
  explicit Log(std::unique_ptr<Atom> base, std::unique_ptr<Atom> argument);

  Log(const Log& log) = default;
  Log(Log&& log) = default;

  Log& operator=(const Log& log) = default;
  Log& operator=(Log&& log) = default;

  ~Log() override = default;

  std::unique_ptr<Atom> copy() const override;
  void print(std::ostream& stream) const override;
  bool canBeEvaluated() const override;
  double evaluate() const override;
  std::unique_ptr<Atom> substitute(const std::string& variableName, const std::unique_ptr<Atom>& node) const override;
  void getVariablesNames(std::set<std::string>& names) const override;
  std::unique_ptr<Atom> differentiate(const std::string& variable) const override;
  std::unique_ptr<Atom> simplify() const override;
  std::unique_ptr<Atom> expand() const override;
  bool compare(const std::unique_ptr<Atom>& other) const override;
  std::unique_ptr<Atom> coefficient(const std::unique_ptr<Atom>& node) const override;
};
}  // namespace symbolicAlgebra::implementation

#endif  // LOG_DECLARE
#endif  // DECLARATION

#endif  // ATOM_LOG