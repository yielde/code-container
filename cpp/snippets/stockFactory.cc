#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <memory>
#include <thread>
#include <netinet/in.h>
#include <string>
#include <functional>
#include <iostream>
#include <mutex>
#include <unistd.h>

using namespace std;

class Stock
{
public:
  string key()
  {
    return key_;
  }
  void setKey(string s)
  {
    key_ = s;
  }

private:
  string key_;
};

class StockFactory : public enable_shared_from_this<StockFactory>
{
public:
  shared_ptr<Stock> get(string s)
  {
    shared_ptr<Stock> pstock;
    lock_guard<mutex> lock(mutex_);
    weak_ptr<Stock> wpstock = stocks_[s];
    pstock = wpstock.lock();
    if (!pstock)
    {
      pstock.reset(new Stock, bind(&StockFactory::deleteCallback, weak_ptr<StockFactory>(shared_from_this()), placeholders::_1));
      pstock->setKey(s);
      wpstock = pstock;
      stocks_[s] = wpstock;
    }
    return pstock;
  }

  void print()
  {
    for (auto i : stocks_)
    {
      cout << "key: " << i.first << ": " << i.second.use_count() << endl;
    }
  }

private:
  static void deleteCallback(const weak_ptr<StockFactory> &wpFactory, Stock *stock)
  {
    shared_ptr<StockFactory> pstockfactory = wpFactory.lock();
    if (pstockfactory)
    {
      pstockfactory->removeStock(stock);
    }
    delete stock;
  }

  void removeStock(Stock *stock)
  {
    if (stock)
    {
      lock_guard<mutex> lock(mutex_);
      stocks_.erase(stock->key());
    }
  }

  mutex mutex_;
  unordered_map<string, weak_ptr<Stock>> stocks_;
};

void StockFirstDie()
{
  shared_ptr<StockFactory> factory(new StockFactory);
  {
    shared_ptr<Stock> stock = factory->get("hello1");
    shared_ptr<Stock> stock2 = factory->get("hello1");
    // stock destructs
  }
  factory->print();

  // factory destructs
}

void FactoryFirstDie()
{
  shared_ptr<Stock> stock;
  {
    shared_ptr<StockFactory> factory(new StockFactory);
    stock = factory->get("hello1");
    shared_ptr<Stock> stock2 = factory->get("hello1");
    factory->print();
    // factory destructs
  }
  // stock destructs
}

int main()
{
  StockFirstDie();
  FactoryFirstDie();
  return 0;
}
