CXXFLAGS ?= -I. -I ../src -std=c++11
LDFLAGS ?= -l gtest -l pthread

.PHONY: test
test: MCP342x.test
	./$^

MCP342x.test: MCP342x.test.cpp ../src/MCP342x.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)
