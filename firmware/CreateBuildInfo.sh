#!/usr/bin/env bash

# This is the path where we will create a C header
BUILD_INFO_FILE=./build_info.h

echo "Getting metadata from last local commit on current branch"

BUILD_DATE=$(git log -1 --format="%at" | xargs -I{} date -d @{} +%Y/%m/%d)
BUILD_TIME=$(git log -1 --format="%at" | xargs -I{} date -d @{} +%H:%M:%S)
BUILD_INFO=$(git describe --tags --abbrev=4 --dirty --always)
BUILD_BRANCH=$(git symbolic-ref --short -q HEAD)

echo "Build Info: $BUILD_BRANCH $BUILD_INFO $BUILD_DATE $BUILD_TIME into $BUILD_INFO_FILE"

echo "/**                                           "	>  $BUILD_INFO_FILE
echo " * @file    build_info.h                      "	>> $BUILD_INFO_FILE
echo " * @brief   Git tag and commit information    "	>> $BUILD_INFO_FILE
echo " * @warning DO NOT EDIT!                      "	>> $BUILD_INFO_FILE
echo " *          Generated by CreateBuildInfo.sh   "	>> $BUILD_INFO_FILE
echo " */                                           "	>> $BUILD_INFO_FILE
echo "                                              "	>> $BUILD_INFO_FILE
echo "#ifndef BUILD_INFO_H                          "	>> $BUILD_INFO_FILE
echo "#define BUILD_INFO_H                          "	>> $BUILD_INFO_FILE
echo "                                              "	>> $BUILD_INFO_FILE
echo "#define BUILD_DATE    \"$BUILD_DATE\"          "	>> $BUILD_INFO_FILE
echo "                                              "	>> $BUILD_INFO_FILE
echo "#define BUILD_TIME    \"$BUILD_TIME\"          "	>> $BUILD_INFO_FILE
echo "                                              "	>> $BUILD_INFO_FILE
echo "#define BUILD_INFO    \"$BUILD_INFO\"          "	>> $BUILD_INFO_FILE
echo "                                              "	>> $BUILD_INFO_FILE
echo "#define BUILD_BRANCH  \"$BUILD_BRANCH\"        "	>> $BUILD_INFO_FILE
echo "                                              "	>> $BUILD_INFO_FILE
echo "#endif /* BUILD_INFO_H */                     "	>> $BUILD_INFO_FILE
echo "                                              " 	>> $BUILD_INFO_FILE
