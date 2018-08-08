cmake_minimum_required(VERSION 3.10)
project(projectVer1_0 C)

set(CMAKE_C_STANDARD 90)

add_executable(projectVer1_0 main.c header.h dataList.c validationErrors.c operandAnalysis.c defines.h utils.c firstStage.c entryList.c secondStage.c symbolList.c structs.h wordList.c thirdStage.c)