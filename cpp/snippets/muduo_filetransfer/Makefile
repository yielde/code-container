CXX = g++
CXXFLAGS = -g -Wall -std=c++17

MUDUO_LIB_PATH = /usr/local/lib/muduo
MUDUO_LIBS = -L$(MUDUO_LIB_PATH) -lmuduo_net -lmuduo_base
PTHREAD_LIB = -lpthread

SRCS = fileTransferMuduo.cc stockFactory.cc fileTransferClient.cc
TARGETS = fileTransferMuduo stockFactory fileTransferClient

all: $(TARGETS)

fileTransferMuduo:fileTransferMuduo.cc
	$(CXX) $(CXXFLAGS) -o fileTransferMuduo fileTransferMuduo.cc $(MUDUO_LIBS) $(PTHREAD_LIB)

stockFactory:stockFactory.cc
	$(CXX) $(CXXFLAGS) -o stockFactory stockFactory.cc $(MUDUO_LIBS) $(PTHREAD_LIB)

fileTransferClient:fileTransferClient.cc
	$(CXX) $(CXXFLAGS) -o fileTransferClient fileTransferClient.cc $(MUDUO_LIBS) $(PTHREAD_LIB)

clean:
	rm -f $(TARGETS)

.PHONY: all clean