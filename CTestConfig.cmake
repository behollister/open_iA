## This file should be placed in the root directory of your project.
## Then modify the CMakeLists.txt file in the root directory of your
## project to incorporate the testing dashboard.

## # The following are required to uses Dart and the Cdash dashboard
##   ENABLE_TESTING()
##   INCLUDE(CTest)

set(CTEST_PROJECT_NAME "openiA")
set(CTEST_NIGHTLY_START_TIME "00:01:00 CEST")

set(CTEST_DROP_METHOD "https")
set(CTEST_DROP_SITE "git.3dct.at")
set(CTEST_DROP_LOCATION "/CDash/submit.php?project=openiA")
set(CTEST_DROP_SITE_CDASH TRUE)
