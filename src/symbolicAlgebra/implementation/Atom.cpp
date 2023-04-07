#include "Atom.h"

namespace symbolicAlgebra::implementation {
Atom::Atom() : type(AtomType::ANY) {}

Atom::Atom(AtomType type) : type(type) {}

Atom::Atom(AtomType type, std::unique_ptr<Atom> arg) : type(type) {
  args.emplace_back(std::move(arg));
}

Atom::Atom(AtomType type, std::unique_ptr<Atom> arg1, std::unique_ptr<Atom> arg2) : type(type) {
  args.emplace_back(std::move(arg1));
  args.emplace_back(std::move(arg2));
}

Atom::Atom(AtomType type, std::vector<std::unique_ptr<Atom>> args) : args(std::move(args)), type(type) {}

Atom::Atom(const Atom& atom) : type(atom.type) {
  std::for_each(atom.args.begin(), atom.args.end(),
                [this](const std::unique_ptr<Atom>& arg) { this->args.push_back(arg->copy()); });
}

Atom& Atom::operator=(const Atom& atom) {
  this->type = atom.type;
  this->args = std::vector<std::unique_ptr<Atom>>();

  std::for_each(atom.args.begin(), atom.args.end(),
                [this](const std::unique_ptr<Atom>& arg) { this->args.push_back(arg->copy()); });

  return *this;
}

Atom::~Atom() = default;

std::ostream& operator<<(std::ostream& stream, const Atom& atom) {
  atom.print(stream);

  return stream;
}
}  // namespace symbolicAlgebra::implementation