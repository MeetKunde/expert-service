#ifndef ATOM_PRODUCT

#ifdef FORWARD_DECLARATION
#ifndef PRODUCT_FORWARD
#define PRODUCT_FORWARD

namespace symbolicAlgebra::implementation {
class Product;
}

#endif  // PRODUCT_FORWARD
#endif  // FORWARD_DECLARATION

#ifdef DECLARATION
#define ATOM_PRODUCT
#ifndef PRODUCT_DECLARE
#define PRODUCT_DECLARE

namespace symbolicAlgebra::implementation {
class Product : public Atom {
 public:
  Product();
  explicit Product(std::unique_ptr<Atom> factor);
  explicit Product(std::unique_ptr<Atom> factor1, std::unique_ptr<Atom> factor2);
  explicit Product(std::vector<std::unique_ptr<Atom>> factors);

  Product(const Product& product) = default;
  Product(Product&& product) = default;

  Product& operator=(const Product& product) = default;
  Product& operator=(Product&& product) = default;

  ~Product() override = default;

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

 private:
  void preprocessConstructorArgs();
};
}  // namespace symbolicAlgebra::implementation

#endif  // PRODUCT_DECLARE
#endif  // DECLARATION

#endif  // ATOM_PRODUCT
