#ifndef APP_VERSION_H
#define APP_VERSION_H

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

#define USE_GIT_BUILD_INFO

/* -------------------------------------------------------------------------- */

#include "global.h"

#ifdef USE_GIT_BUILD_INFO
#include "build_info.h"
#endif

/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */

PUBLIC const char *ProgramName = "Beeblebot V2";

#ifndef NDEBUG
    PUBLIC const char *ProgramBuildType = "DEBUG";
#else
    PUBLIC const char *ProgramBuildType = "RELEASE";
#endif

/* -------------------------------------------------------------------------- */

#ifdef USE_GIT_BUILD_INFO
    PUBLIC const char *ProgramBuildBranch = BUILD_BRANCH;
    PUBLIC const char *ProgramBuildInfo   = BUILD_INFO;
    PUBLIC const char *ProgramBuildDate   = BUILD_DATE;
    PUBLIC const char *ProgramBuildTime   = BUILD_TIME;
#else
    PUBLIC const char *ProgramBuildBranch = "develop";
    PUBLIC const char *ProgramBuildDate = __DATE__;
    PUBLIC const char *ProgramBuildTime = __TIME__;
    #define VERSION "2.0"

    #ifndef NDEBUG
        PUBLIC const char *ProgramBuildInfo   = VERSION " " "D";
    #else
        PUBLIC const char *ProgramBuildInfo = VERSION;
    #endif
#endif

/* -------------------------------------------------------------------------- */

extern const char *ProgramName;           ///< Application Name
extern const char *ProgramBuildType;      ///< Build Type e.g. DEBUG, RELEASE
extern const char *ProgramBuildBranch;    ///< Git branch in use
extern const char *ProgramBuildInfo;      ///< Git build info string
extern const char *ProgramBuildDate;      ///< Date string of last git commit
extern const char *ProgramBuildTime;      ///< Time string of last git commit

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* APP_VERSION_H */
