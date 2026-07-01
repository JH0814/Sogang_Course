CXX ?= g++
CXXFLAGS ?= -O3 -std=c++17 -Wall -Wextra -pedantic
LDLIBS := -lrdkafka++ -lrdkafka -fopenmp
OPENMP := -fopenmp

.PHONY: all clean

all: producer/producer consumer/consumer

producer/producer: producer/producer.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDLIBS)

consumer/consumer: consumer/consumer.cpp
	$(CXX) $(CXXFLAGS) $(OPENMP) $< -o $@ $(LDLIBS)

clean:
	rm -f producer/producer consumer/consumer
