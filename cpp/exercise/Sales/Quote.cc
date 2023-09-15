#include "Quote.h"

using namespace std;
double print_total(ostream &os, const Quote &item, size_t n) {
  double ret = item.net_price(n);
  os << "ISBN: " << item.isbn() << " # sold: " << n << " total due: " << ret
     << endl;
  return ret;
}

double Bulk_quote::net_price(std::size_t cnt) const {
  if (cnt >= quantity)
    return cnt * (1 - discount) * price;
  else
    return cnt * price;
}

double Lim_quote::net_price(size_t cnt) const {
  size_t discounted = min(cnt, quantity);
  size_t undiscounted = cnt - discounted;
  return discounted * (1 - discount) * price + undiscounted * price;
}