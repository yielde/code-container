CXX = g++
CXXFLAGS = -Wall -g -std=c++17

MUDUO_LIB_PATH = /usr/local/lib/muduo
MUDUO_LIBS = -L$(MUDUO_LIB_PATH) -lmuduo_base -lmuduo_net -lpthread

TARGETS = client server
all : $(TARGETS)

client: client.cc
	$(CXX) $(CXXFLAGS) -o client client.cc $(MUDUO_LIBS)

server: server.cc
	$(CXX) $(CXXFLAGS) -o server server.cc $(MUDUO_LIBS)

clean:
	rm -f $(TARGETS)

.PHONY: all clean