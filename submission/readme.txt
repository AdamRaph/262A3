Compile command:
1.cd to the parent directory of ids_code
2.CC -o IDS ActivityEngine.cpp AlertEngine.cpp AnalysisEngine.cpp DataHandler.cpp IDSRunner.cpp
or g++ -o IDS ActivityEngine.cpp AlertEngine.cpp AnalysisEngine.cpp DataHandler.cpp IDSRunner.cpp

Usage:
./IDS <eventsFile> <username> <statsFile> <days>

Note:
To begin reading source code, main function is in the IDERunner.cpp.