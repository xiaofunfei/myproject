# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -I./include
LDFLAGS := -L/usr/local/lib
LIBS := -lmysqlclient -lcrypt -llog4cpp -lavformat -lavcodec -lavutil -lpthread -lcrypto -lcurl

# OpenSSL 3.0 compatibility (for MD5 deprecation warnings)
ifeq ($(shell pkg-config --modversion openssl | cut -d. -f1),3)
    CXXFLAGS += -DOPENSSL_API_COMPAT=10100
endif

# Directories
SRC_DIR := src
INC_DIR := include
BUILD_DIR := build
BIN_DIR := .
TARGET := $(BIN_DIR)/SmartHomeServer

# Source files (explicitly list main files first for better dependency management)
MAIN_SRCS := $(SRC_DIR)/SmartHomeMonitorServer.cpp $(SRC_DIR)/TestServer.cpp
OTHER_SRCS := $(filter-out $(MAIN_SRCS),$(wildcard $(SRC_DIR)/*.cpp))
SRCS := $(MAIN_SRCS) $(OTHER_SRCS)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Main target
all: $(BUILD_DIR) $(BIN_DIR) $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) $(LIBS)

# Object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create directories
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Clean
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean