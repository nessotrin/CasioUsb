AUTO_CPPS = $(shell find src -type f -name "*.cpp")

AUTO_FOLDER = $(shell find src -type d)

AUTO_INCLUDE = $(shell find ./src -type d | sed 's;./src;-I ./src;')

CXX = g++
CXX_FLAGS = -fstack-protector-all -std=c++11 -Wfatal-errors -Wall -Wextra -Wpedantic -Wconversion -Wshadow -g 
BUILD_FLAGS = $(AUTO_INCLUDE) -I ../src/
LINK_FLAGS = -L ../build/ -lCasioUsb -lusb-1.0

# Final binary
BIN = CasioUsbUploader
# Put all auto generated stuff to this build dir.
BUILD_DIR = build/



# List of all .cpp source files.
CPPS =  $(AUTO_CPPS)

# All .o files go to build dir.
OBJ = $(CPPS:%.cpp=$(BUILD_DIR)/%.o)
# Gcc/Clang will create these .d files containing dependencies.
DEP = $(OBJ:%.o=%.d)


# Default target named after the binary.
$(BIN) : $(BUILD_DIR)/$(BIN)

# Actual target of the binary - depends on all .o files.
$(BUILD_DIR)/$(BIN) : $(OBJ)
	@# Create build directories - same structure as sources.
	@mkdir -p $(@D)
	@# Just link all the object files.
	$(CXX) $^  -o $@ $(CXX_FLAGS) $(LINK_FLAGS)

# Include all .d files
-include $(DEP)

# Build target for every single object file.
# The potential dependency on header files is covered
# by calling `-include $(DEP)`.
$(BUILD_DIR)/%.o : %.cpp
	@mkdir -p $(@D)
	@# The -MMD flags additionaly creates a .d file with
	@# the same name as the .o file.
	$(CXX) -c $< -o $@ $(CXX_FLAGS) -MMD $(BUILD_FLAGS)
	

.PHONY : clean
clean :
	# This should remove all generated files.
	-rm -r $(BUILD_DIR)
	done 
