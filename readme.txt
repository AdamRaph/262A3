Compile command:
1.cd to the parent directory of ids_code
2.CC -o IDS ids_code/src/ActivityEngine.cpp ids_code/src/AlertEngine.cpp ids_code/src/AnalysisEngine.cpp ids_code/src/DataHandler.cpp ids_code/src/IDSRunner.cpp

Usage:
./IDS <eventsFile> <username> <statsFile> <days>

Note:
To begin reading source code, main function is in the IDERunner.cpp.