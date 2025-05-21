# TODO

- avoid second loop in depfile processing for CXX_IMPORTS
- investigate "too new for inode cache" messages
- rename CCACHE_CXX20_MODULES_FEATURE to something else
  - maybe define CCACHE_HAVE_JSON and define CCACHE_HAVE_CXX20_MODULES_FEATURE based on that
- remove storage of .bmi file name for GCC compilation
  - instead just reparse the stored .ddi file
  - this may be slightly less efficient but requires less of a change to manifest format
- add tests for the invocations CMake makes during module compilation
- add support for clang-scan-deps
- add support for storing .ddi files when compiler is run in preprocessor mode
  - needed to support clang-scan-deps
  - needed to support /sourceDependencies
