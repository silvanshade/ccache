# cxx_modules

* fix `foo.cppm` including its own `foo.gcm` as a dependency in manifest
  - requires more sophisticated parsing of depfiles
* remove (or change) -sourceDependencies option for MSVC
* use util::noncopyable
* add driver for `clang-scan-deps`
  - requires two-passes
  - ensure to architect in such a way that we can use -fdeps-* options if available
