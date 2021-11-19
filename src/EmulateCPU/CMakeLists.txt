cmake_minimum_required(VERSION 3.17)
project(EmulateCPU)

set(CMAKE_CXX_STANDARD 14)



include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(EmulateCPU m6502.cpp m6502.h Tests/M6502LoadRegisterTests.cpp main_6502.cpp Tests/M6502StoreRegisterTests.cpp Tests/M6502JumpsAndCallsTests.cpp Tests/M6502StackOperationsTests.cpp Tests/M6502LogicalTests.cpp)
target_link_libraries(EmulateCPU gtest_main)
add_test(NAME example_test COMMAND example)