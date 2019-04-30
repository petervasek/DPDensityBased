#ifndef DP_MAIN_H
#define DP_MAIN_H

//-----INPUT FILE PATH
//#define INPUT_FILE "C:\\Users\\User1\\CLionProjects\\DP\\Data\Sources\fcs8_unlabeled.txt"
#define INPUT_FILE "C:\\Users\\User1\\CLionProjects\\DP\\Data\\Sources\\t4.8k.dat"
//#define INPUT_FILE "C:\\Users\\User1\\CLionProjects\\DP\\Data\\Sources\\t4.8k first.dat"
//#define INPUT_FILE "C:\\Users\\User1\\CLionProjects\\DP\\Data\\Sources\\t4.8k second.dat"
//#define LOAD_FILE "C:\\Users\\User1\\CLionProjects\\DP\\Data\\Results\\t4.8k_clustering_state_25_12.txt"

//-----SET TO READ INPUT FILE
#define LOAD_NEW_DATASET 1

//-----SET TO LOAD ONE OF PREVIOUS CLUSTERING RESULTS FROM FILE
//#define LOAD_CLUSTERING_RESULT 1
//#define LOAD_PATH "C:\\Users\\User1\\CLionProjects\\DP\\Data\\Results\\t4.8k_clustering_state_25_10.txt"

//-----SET TO DELETE POINTS FROM INPUT FILE FROM SAVED CLUSTERING RESULT
//#define DELETE_INPUTTED 1

//-----SET TO SAVE CLUSTERING RESULT TO FILE FOR FUTURE PROCESSING
//#define SAVE_CLUSTERING_RESULT 1

//-----ALGORITHM PARAMETERS
#define EPS 10
#define MIN_PTS 25

//-----PARALLELIZATION
//#define OPENMP 1

#endif