#include <iostream>
#include <stdexcept>


#include "Sales_data.h"

int main() {
  Sales_data data1, data2;
  double price;
  std::cin >> data1.bookNo >> data1.units_sold >> price;
  data1.revenue = data1.units_sold * price;

  std::cin >> data2.bookNo >> data2.units_sold >> price;
  data2.revenue = data2.units_sold * price;

  try {
    if (data1.bookNo == data2.bookNo) {
      unsigned int totalCnt = data1.units_sold + data2.units_sold;
      double totalRevenew = data1.revenue + data2.revenue;
      std::cout << data1.bookNo << " " << totalCnt << " " << totalRevenew
                << " ";

      if (totalCnt != 0)
        std::cout << totalRevenew / totalCnt << std::endl;
      else
        std::cout << "(no sales)" << std::endl;
      return 0;
    } else {
      throw std::runtime_error("Data must refer to the same ISBN");
    }
  } catch (std::runtime_error err) {
    std::cout << err.what() << std::endl;
  }
  return 0;
}