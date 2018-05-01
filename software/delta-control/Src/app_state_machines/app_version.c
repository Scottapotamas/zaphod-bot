#ifdef	__cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

#include "global.h"
#include "app_version.h"

#define USE_GIT_BUILD_INFO

#ifdef USE_GIT_BUILD_INFO
#include "build_info.h"
#endif // USE_GIT_BUILD

/* -------------------------------------------------------------------------- */

PUBLIC const char *   ProgramName        = "OV2460-STM32-CAMERA";

#ifdef __DEBUG
    PUBLIC const char *   ProgramBuildType   = "DEBUG";
#else
PUBLIC const char *   ProgramBuildType   = "RELEASE";
#endif

#ifdef USE_GIT_BUILD_INFO

    PUBLIC const char *   ProgramBuildBranch = BUILD_BRANCH;
    PUBLIC const char *   ProgramBuildInfo   = BUILD_INFO;
    PUBLIC const char *   ProgramBuildDate   = BUILD_DATE;
    PUBLIC const char *   ProgramBuildTime   = BUILD_TIME;

#else

    PUBLIC const char *   ProgramBuildBranch = "develop";

    #define VERSION   "1.0"

    #ifdef __DEBUG
        PUBLIC const char *   ProgramBuildInfo   = VERSION " " "D";
    #else
        PUBLIC const char *   ProgramBuildInfo   = VERSION;
    #endif

    PUBLIC const char *   ProgramBuildDate   = __DATE__;
    PUBLIC const char *   ProgramBuildTime   = __TIME__;

#endif

/* ----- End ---------------------------------------------------------------- */
