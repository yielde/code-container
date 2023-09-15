#include <iostream>
#include <memory>
#include <string>

class Quote {
 public:
  Quote() = default;
  Quote(const std::string &book, double sales_price)
      : bookNo(book), price(sales_price) {}
  virtual ~Quote() = default;

  std::string isbn() const { return bookNo; }
  virtual double net_price(std::size_t n) const { return n * price; }
  // 引用限定
  virtual Quote *clone() const & { return new Quote(*this); }
  virtual Quote *clone() && { return new Quote(std::move(*this)); }

 private:
  std::string bookNo;

 protected:
  double price = 0.0;
};

class Disc_quote : public Quote {
 public:
  Disc_quote() = default;
  Disc_quote(const std::string &book, double price, std::size_t qty,
             double disc)
      : Quote(book, price), quantity(qty), discount(disc) {}

  double net_price(std::size_t) const = 0;
  std::pair<std::size_t, double> discount_policy() const {
    return {quantity, discount};
  }

 protected:
  std::size_t quantity = 0;
  double discount = 0.0;
};

class Bulk_quote : public Disc_quote {
 public:
  Bulk_quote() = default;
  Bulk_quote(const std::string &book, double p, std::size_t qty, double disc)
      : Disc_quote(book, p, qty, disc) {}

  double net_price(std::size_t) const override;

  Bulk_quote *clone() const & { return new Bulk_quote(*this); }
  Bulk_quote *clone() && { return new Bulk_quote(std::move(*this)); }
};

class Lim_quote : public Disc_quote {
 public:
  Lim_quote(const std::string &book = "", double sales_price = 0.0,
            std::size_t qty = 0, double disc_rate = 0.0)
      : Disc_quote(book, sales_price, qty, disc_rate) {}

  double net_price(std::size_t) const;

  Lim_quote *clone() const & { return new Lim_quote(*this); }
  Lim_quote *clone() && { return new Lim_quote(std::move(*this)); }
};

double print_total(std::ostream &, const Quote &, std::size_t);