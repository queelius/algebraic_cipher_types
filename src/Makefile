CXX = g++
CXXFLAGS = -I. -std=c++2a -Wall -g

or: or.cpp
	$(CXX) $(CXXFLAGS) -o or or.cpp -lboost_program_options

and: and.cpp
	$(CXX) $(CXXFLAGS) -o and and.cpp -lboost_program_options

kvs: kvs.cpp
	$(CXX) $(CXXFLAGS) -o kvs kvs.cpp -lboost_program_options

clean:
	rm or and store