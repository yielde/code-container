CXX = g++
CXXFLAGS = -g -Wall -std=c++17

MUDUO_LIB_PATH = /usr/local/lib/muduo
MUDUO_LIBS = -L$(MUDUO_LIB_PATH) -lmuduo_net -lmuduo_base
PTHREAD_LIB = -lpthread

TARGETS = fileTransferMuduo  fileTransferClient

all: $(TARGETS)

fileTransferMuduo:fileTransferMuduo.cc
	$(CXX) $(CXXFLAGS) -o fileTransferMuduo fileTransferMuduo.cc $(MUDUO_LIBS) $(PTHREAD_LIB)

fileTransferClient:fileTransferClient.cc
	$(CXX) $(CXXFLAGS) -o fileTransferClient fileTransferClient.cc $(MUDUO_LIBS) $(PTHREAD_LIB)

clean:
	rm -f $(TARGETS)

.PHONY: all clean