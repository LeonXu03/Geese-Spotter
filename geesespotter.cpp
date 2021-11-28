#include "geesespotter_lib.h"

//g++ -o main geesespotter.cpp geesespotter_lib.cpp -std=c++11

//test for early submission: 
//createBoard()
//hideBoard()
//cleanBoard()
//printBoard()
//mark()

//Implement the following functions:
char *createBoard(std::size_t xdim, std::size_t ydim);
void computeNeighbors(char *board, std::size_t xdim, std::size_t ydim);
void hideBoard(char *board, std::size_t xdim, std::size_t ydim);
void cleanBoard(char *board);
void printBoard(char *board, std::size_t xdim, std::size_t ydim);
int reveal(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc);
int mark(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc);
bool isGameWon(char *board, std::size_t xdim, std::size_t ydim);


// //Provided functions (8)
// //#ifndef MARMOSET_TESTING
// int main();
// //#endif

// bool game();
// void spreadGeese(char *board, std::size_t xdim, std::size_t ydim, unsigned int numgeese);
// std::size_t readSizeT();
// std::size_t xdim_max();
// std::size_t ydim_max();
// char markedBit();
// char hiddenBit();
// char valueMask();

char *createBoard(std::size_t xdim, std::size_t ydim) {
    
    char *board {new char [xdim*ydim]{}}; 
    for(int i{0}; i<xdim*ydim; i++) {
        board[i] = 0;
    }

    return board;
}

void computeNeighbors(char *board, std::size_t xdim, std::size_t ydim) {

    int xdimension {(int)xdim};
    int ydimension {(int)ydim};
    
    int n_gooses{0};

    for(int i {0}; i<xdim*ydim; i++) {
        n_gooses = 0;
        int currentrownum {i/xdimension};

        if((board[i] & 0b00001111) != 9) {

            for(int j{-1}; j<=1; j++) { //top values
            int newindex {i-xdimension+j};
            if(((board[newindex] & 0b00001111) == 9) && (newindex/xdimension==(currentrownum-1)) && newindex>=0) {
                n_gooses++;
            }    
        }

        for(int j{-1}; j<=1; j++) { //bottom values
            int newindex {i+xdimension+j};
            if(((board[newindex] & 0b00001111) == 9) && (newindex/xdimension == (currentrownum + 1)) && newindex<xdim*ydim) {
                n_gooses++;
            }
        }

        //left side
        if(((board[i-1] & 0b00001111) == 9) && ((i-1)/xdim == currentrownum) && ((i-1)>=0)) {
            n_gooses++; 
        }

        //right side
        if(((board[i+1] & 0b00001111) == 9) && ((i+1)/xdim == currentrownum) && (i+1)<xdim*ydim) {
            n_gooses++;
        }

        board[i] = (board[i] & 0b11110000) | n_gooses;

        }

    }

}

void hideBoard(char *board, std::size_t xdim, std::size_t ydim) {
    for(int i{0}; i<xdim*ydim; i++) {
        board[i] = board[i] | 0b00100000;
    }
}

void cleanBoard(char *board) {
    delete [] board;
    board = nullptr;
}

void printBoard(char *board, std::size_t xdim, std::size_t ydim) {
    unsigned short goosecounter{};
    unsigned short unveiled{};

    for(int rows{0}; rows<ydim; rows++) {
        
        for(int cols{0}; cols < xdim; cols++) {

            if((board[rows*xdim + cols]) == 57 || (board[rows*xdim + cols]) == 41) {
                goosecounter++;
            }
            if((board[rows*xdim + cols])>=0 && board[rows*xdim + cols]<=8) {
                unveiled++;
            }
        }
    }

    for(int rows{0}; rows<ydim; rows++) {
        for(int cols{0}; cols < xdim; cols++) {

            //std::cout << std::showbase  << std::hex << int(board[rows*xdim+cols]) << "\t";


            
            if(((board[rows*xdim + cols] == 57  || board[rows*xdim + cols] ==41)  && ((unveiled+goosecounter) == xdim*ydim)) || board[rows*xdim + cols]==9) {
                  std::cout << "9";
              }
            else if(board[rows*xdim + cols] & 0b00010000) {     //marked bit is 0x10 and hidden bit is 0x20
                  std::cout << "M";
              }
              else if(board[rows*xdim + cols] & 0b00100000) {
                  std::cout << "*";
              }
              else if(board[rows*xdim + cols] >= 0 && board[rows*xdim + cols] <=8) {
                  std::cout << (int)board[rows*xdim+cols];
              }

        }

        std::cout << std::endl;
    }
}

int reveal(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc) {

    int xdimension {(int)xdim};
    int ydimension {(int)ydim};
    int xlocation {(int)xloc};
    int ylocation {(int)yloc};

    int currentindex {xdimension*ylocation + xlocation};

    if(board[currentindex] & 0b00010000) {
        return 1;
    }
    else if((board[currentindex] >= 0) && (board[currentindex] <=8)) {
        return 2;
    }
    else if((board[currentindex] & 0b00001111) == 9) {
        board[currentindex] = board[currentindex] ^ 0b00100000;
        return 9;
    }  
    
    if(((board[currentindex] & 0b00001111) == 0) && ((board[currentindex] & 0b00100000))>=1) { //for a 0 field

        int currentrownum {currentindex/xdimension};

        for(int j{-1}; j<=1; j++) { //top values
            int newindex {currentindex-xdimension+j};
            if((newindex>=0 && (board[newindex] & 0b00100000)) && (newindex/xdim==(currentrownum-1))) {
                board[newindex] = board[newindex] ^ 0b00100000;
            }    
        }

        for(int j{-1}; j<=1; j++) { //bottom values
            int newindex {currentindex+xdimension+j};
            if((newindex<xdim*ydim && (board[newindex] & 0b00100000)) && (newindex/xdim == (currentrownum+1))) {
                board[newindex] = board[newindex] ^ 0b00100000;
            }
        }

        //left side
        if(((board[currentindex-1] & 0b00100000)) && (((currentindex-1)/xdim) == currentrownum) && (currentindex-1)>=0) {
            board[currentindex-1] = board[currentindex-1] ^ 0b00100000;
        }

        //right side
        if((board[currentindex+1] & 0b00100000) && ((currentindex+1)/xdim == currentrownum) && (currentindex+1)<xdim*ydim) {
            board[currentindex+1] = board[currentindex+1] ^ 0b00100000;
        }

    }

    board[currentindex] = board[currentindex] ^ 0b00100000;

    
    return 0;
}


int mark(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc) {

    //xloc = 2;
    //yloc= 2;

    //board[xdim*yloc + xloc] = 0x9;
    
    if (board[xdim*yloc + xloc]>=0 && board[xdim*yloc + xloc]<=9) {
        return 2;
    }
    else if (board[xdim*yloc + xloc] & 0b00010000) { //already marked
        board[xdim*yloc+xloc] = board[xdim*yloc+xloc] ^ 0b00010000;
    }
    else  { //unmarked
        board[xdim*yloc+xloc] = board[xdim*yloc+xloc] | 0b00010000;
    }

    //std::cout << std::showbase  << std::hex << int(board[xdim*yloc + xloc]) << std::endl;
    
    return 0;
}

bool isGameWon(char *board, std::size_t xdim, std::size_t ydim) {

    bool gamecontinue {false};

    for(int i{0}; i<xdim*ydim; i++) {
        if((board[i] & 0b00100000 || board[i] & 0b00010000) && (((board[i] & 0b00001111) >=0) && ((board[i] & 0b00001111) <=8))) {
            gamecontinue = true;
        }

        //std::cout << ((board[i] & 0b00001111) >=0) << std::endl;
    }
    
    if(gamecontinue==true) {
        return false;
    }

    return true;
}

