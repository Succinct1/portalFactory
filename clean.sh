#!/bin/bash
# Remove all build artifacts and generated files

rm -rf build
find . -name 'CMakeFiles' -type d -exec rm -rf {} +
find . -name 'CMakeCache.txt' -delete
find . -name 'Makefile' -delete
find . -name '*.o' -delete
find . -name '*.obj' -delete
find . -name '*.so' -delete
find . -name '*.a' -delete
find . -name '*.dll' -delete
find . -name '*.exe' -delete
find . -name '*.log' -delete
