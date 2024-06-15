# the build target executable:
TARGET = udp_server

CXXFLAGS = -std=c++11

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(TARGET).cpp

clean:
	rm $(TARGET)