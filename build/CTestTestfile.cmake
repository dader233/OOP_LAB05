# CMake generated Testfile for 
# Source directory: /workspaces/LR_005
# Build directory: /workspaces/LR_005/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(Lab05Tests "/workspaces/LR_005/build/tests")
set_tests_properties(Lab05Tests PROPERTIES  _BACKTRACE_TRIPLES "/workspaces/LR_005/CMakeLists.txt;24;add_test;/workspaces/LR_005/CMakeLists.txt;0;")
subdirs("_deps/googletest-build")
