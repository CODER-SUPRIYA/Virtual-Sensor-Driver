CXX=g++
CXXFLAGS=-std=c++17 -Wall -Wextra -O2

vsensor_app: main.cpp ../include/vsensor_ioctl.h
	$(CXX) $(CXXFLAGS) main.cpp -o vsensor_app

clean:
	rm -f vsensor_app vsensor.log
