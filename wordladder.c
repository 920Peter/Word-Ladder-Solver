#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

//node struct to store words in a ladder 
typedef struct WordNode_struct {
    char* myWord;
    struct WordNode_struct* next; 
} WordNode;

//node struct to store multiple ladders 
typedef struct LadderNode_struct {
    WordNode* topWord;
    struct LadderNode_struct* next; 
} LadderNode;

//function prototype for test_getLadderHeight() to be valid 
void freeLadder(WordNode* ladder);

//Function to count how word in file to match lengths
int countWordsOfLength(char* filename, int wordSize) { 
    FILE* file = fopen(filename, "r");          //opens file to read 
    if (file == NULL) {
        return -1;                              //return -1 if file cant be found/opened
    }

    int count = 0;      //initialize count at 0 
    char word[99];      

    while (fscanf(file, "%s", word) == 1) {     //scans each word and checks if same length 
        if (strlen(word) == wordSize) {
            count++;            //increments count if so
        }
    }
    fclose(file);       //closes file after read to make sure no memory leaks 
    return count; 
}

//test function for countWordsOfLength()
bool test_countWordsOfLength() {
    int count = countWordsOfLength("ptest.txt",4);
    if (count != 4) {
        printf("test_countWordsOfLength(): FAILED, got a %d letter word instead of 4 letter word expected\n", count);
        return false;
    }
    else {
         printf("test_countWordsOfLength(): PASSED, 4 letter word found!\n");
        return true;
    }
}

//fills the array with words that are the correct length 
bool buildWordArray(char* filename, char** words, int numWords, int wordSize) { 
    FILE* file = fopen(filename, "r");      //opens file to read 
    if (file == NULL) {
        return false;               //returns false if file not found/cant open 
    }
    
    //word has max of 99, initialize new word at 0
    char word[99];
    int newWord = 0;
    
    while (fscanf(file, "%s", word) == 1 ) {        //reads file word by word
        if (strlen(word) == wordSize) { 
            strcpy(words[newWord], word);           //copies matching word to the array
            newWord++;
        }
    }
    fclose(file);                       //closes file so no memory leaks 
    return (newWord == numWords);       //checks that all new words were added 
}

//function to find a word in the array using binary search
int findWord(char** words, char* aWord, int loInd, int hiInd) { 
    //binary search between loInd and hiInd
    while (loInd <= hiInd) {
        int mid = (loInd + hiInd) / 2;
        int cmp = strcmp( aWord, words[mid]);       //compares words

        if (cmp == 0) {
            return mid;                 //if equal returns it.
        }
        else if (cmp < 0) {             //if not, searches left half 
            hiInd = mid - 1;
        }
        else {                          //searches right half 
            loInd = mid + 1;
        }
    }
    return -1;
}

//function to free all memory allocate(malloc) for word array 
void freeWords(char** words, int numWords) {
    for (int i = 0; i < numWords; i++) {            //frees each words individually  
        free(words[i]);
    }
    free(words);            //frees array 
}

//function to add new word to the front of the word ladder 
void insertWordAtFront(WordNode** ladder, char* newWord) {
    WordNode* newNode = (WordNode*)malloc(sizeof(WordNode));     //allocate memory for new node
    if (newNode == NULL) {
        return;
    }
    newNode->myWord = newWord;   //pointer to existing word 
    newNode->next = *ladder;     //pointer to current head of ladder
    *ladder = newNode;           //updates head to point to new node
}

//function to count how many words (height) are in a ladder 
int getLadderHeight(WordNode* ladder) {
    int height = 0;
    WordNode* current = ladder;

    while (current != NULL) {       //traverses ladder and counts nodes 
        height++;
        current = current->next;
    }
    return height; // modified to height 
}

//test function for getLadderHeight()
bool test_getLadderHeight() {
    WordNode* ladder = NULL;

    insertWordAtFront(&ladder, "cold");
    insertWordAtFront(&ladder, "cord");
    insertWordAtFront(&ladder, "card");
    insertWordAtFront(&ladder, "ward");

    int res = getLadderHeight(ladder);
    int expected = 4;

    if (res != expected) {
        printf("test_getLadderHeight() FAILED, did not get expected 4 valid 4 letter word, got %d instead.\n",res);
        freeLadder(ladder);
        return false;
    }
    else {
        printf("test_getLadderHeight() PASSED, got expected 4 valid 4 letter word.\n");
        freeLadder(ladder);
        return true;
    }
}


//function to make a copy of a word ladder 
WordNode* copyLadder(WordNode* ladder) {
    if (ladder == NULL) {
        return NULL;
    }

    WordNode* reverseLadder = NULL;
    WordNode** current = &reverseLadder;

    //copies each node from a word ladder to new ladder 
    while (ladder != NULL) {
        WordNode* newNode = (WordNode*)malloc(sizeof(WordNode));        //allocates memory for new node
        newNode->myWord = ladder->myWord;                               //copies the word pointer
        newNode->next = NULL;

        *current = newNode;                                             //links new node to copied ladder 
        current = &(newNode->next);

        ladder = ladder->next;      //moves to next node
    }
    return reverseLadder; //modiied to return reverseLadder
}

//frees memory allocated from a word ladder 
void freeLadder(WordNode* ladder) {
    WordNode* current = ladder;

    //traverses and free each node 
    while (current != NULL) {
        WordNode* temp = current;
        current = current->next;
        free(temp);
    }
}

//function to insert a new ladder at the end of another word ladder 
void insertLadderAtBack(LadderNode** list, WordNode* newLadder) {
    // allocates memory for new ladder node 
    LadderNode* newNode = (LadderNode*)malloc(sizeof(LadderNode));
    newNode->topWord = newLadder;
    newNode->next = NULL;
    if (newNode == NULL) {
        return;
    }

    //if list is null/empty, the list is the new node
    if (*list == NULL) {
        *list = newNode;
        return;
    }

    //loop to find end of list 
    LadderNode* current = *list;
    while(current->next != NULL) {
        current = current->next;
    }

    //appends new node to the end 
    current->next = newNode;
}

//Returns and removes first word ladder at the front from the list of ladders 
WordNode* popLadderFromFront(LadderNode** list) {
    //for empty list case 
    if (*list == NULL) {
        return NULL;
    }

    LadderNode* front = *list;                  //save node to free 
    WordNode* popLadder = front->topWord;       //save ladder to return

    *list = front->next;                        //update list head to next node
    free(front);                                //free popped ladder node

    return popLadder; //modified to popLadder
}

//frees memory allocated(malloc) from ladder list 
void freeLadderList(LadderNode* myList) {
    while (myList != NULL) {
        LadderNode* temp = myList;
        myList = myList->next;

        freeLadder(temp->topWord);      //frees the word ladder
        free(temp);                     //and then frees the ladder node
    }
}

//function to find shorest word ladder between startWord and finalWord using traversal
WordNode* findShortestWordLadder(   char** words, 
                                    bool* usedWord, 
                                    int numWords, 
                                    int wordSize, 
                                    char* startWord, 
                                    char* finalWord ) {
    //creates initial ladder with startWord
    WordNode* initialLadder = NULL;
    insertWordAtFront(&initialLadder, startWord);

    LadderNode* myList = NULL;    //initialize ladders list
    insertLadderAtBack(&myList, initialLadder);

    //marks start word 
    int startInd = findWord(words, startWord, 0, numWords-1);
    if (startInd >= 0) {
        usedWord[startInd] = true;
    }

    while (myList != NULL) {
        WordNode* currLadder = popLadderFromFront(&myList);
        char* topWord = currLadder->myWord;

        //checks if we have hit finalWord
        if (strcmp(topWord, finalWord) == 0) {
            freeLadderList(myList);             //cleans up remaining of ladder 
            return currLadder;                  //returns ladder
        }

        //loop through all variations of topword
        for (int i = 0; i < wordSize; i++) {
            char variant[wordSize + 1];
            strcpy(variant, topWord);

            //loops through every letter in alphabet in current position
            for (char c = 'a'; c <= 'z'; c++) {
                if (c == topWord[i]) continue;

                variant[i] = c;
                int varIndex = findWord(words, variant, 0, numWords - 1);

                //if variant is valid and not used
                if (varIndex >= 0 && !usedWord[varIndex]) {
                    usedWord[varIndex] = true;

                    //create new ladder, copying current and adding word to ladder 
                    WordNode* newLadder = copyLadder(currLadder);
                    insertWordAtFront(&newLadder, words[varIndex]);
                    insertLadderAtBack(&myList, newLadder);
                }
            }
        }
        freeLadder(currLadder);     //frees current ladder 
    }
    return NULL;
}

//interactive user-input to set a word; 
//ensures the word is in the dictionary word array
void setWord(char** words, int numWords, int wordSize, char* aWord) {
    bool valid = false;
    printf("  Enter a %d-letter word: ", wordSize);
    int count = 0;
    while (!valid) {
        scanf("%s",aWord);
        count++;
        valid = (strlen(aWord) == wordSize);
        if (valid) {
            int wordInd = findWord(words, aWord, 0, numWords-1);
            if (wordInd < 0) {
                valid = false;
                printf("    Entered word %s is not in the dictionary.\n",aWord);
                printf("  Enter a %d-letter word: ", wordSize); 
            }
        } else {
            printf("    Entered word %s is not a valid %d-letter word.\n",aWord,wordSize);
            printf("  Enter a %d-letter word: ", wordSize);
        }
        if (!valid && count >= 5) { //too many tries, picking random word
            printf("\n");
            printf("  Picking a random word for you...\n");
            strcpy(aWord,words[rand()%numWords]);
            printf("  Your word is: %s\n",aWord);
            valid = true;
        }
    }
}

// helpful debugging function to print a single Ladder
void printLadder(WordNode* ladder) {
    WordNode* currNode = ladder;
    while (currNode != NULL) {
        printf("\t\t\t%s\n",currNode->myWord);
        currNode = currNode->next;
    }
}

// helpful debugging function to print the entire list of Ladders
void printList(LadderNode* list) {
    printf("\n");
    printf("Printing the full list of ladders:\n");
    LadderNode* currList = list;
    while (currList != NULL) {
        printf("  Printing a ladder:\n");
        printLadder(currList->topWord);
        currList = currList->next;
    }
    printf("\n");
}

void runAllTestFuncs() {
    int PASSED = 0;
    int total = 0;
    
    total++;
    if (test_countWordsOfLength()) PASSED++;

    total++;
    if (test_getLadderHeight()) PASSED++;

    printf("A total of %d passed out of %d tests.\n",PASSED,total);
}


//-----------------------------------------------------
// The primary application is fully provided in main(); 
//  no changes should be made to main(), except for 
//  testing-related purposes (such as command-line 
//  arguments for "TESTING MODE" to call a test case 
//  master function, or something similar)
//-----------------------------------------------------
int main(int argc, char* argv[]) {
    //only change in main() for testing related purposes(command line arguments for testing mode)
    //run tests if test mode ON and exit after 
    if (argc > 1 && strcmp(argv[1], "TEST") ==0) {
        runAllTestFuncs();
        return 0; 
    }


    srand((int)time(0));
    
    printf("\nWelcome to the CS 211 Word Ladder Generator!\n\n");

    // set word length using interactive user-input
    int wordSize;
    printf("Enter the word size for the ladder: ");
    scanf("%d",&wordSize);
    printf("\n");

    printf("This program will make the shortest possible\n"); 
    printf("word ladder between two %d-letter words.\n\n",wordSize);
    
    // interactive user-input sets the dictionary file;
    //  check that file exists; if not, user enters another file
    //  if file exists, count #words of desired length [wordSize]
    char dict[100];
    printf("Enter filename for dictionary: ");
    scanf("%s", dict);
    printf("\n");
    int numWords = countWordsOfLength(dict,wordSize);
    while (numWords < 0) {
        printf("  Dictionary %s not found...\n",dict);
        printf("Enter filename for dictionary: ");
        scanf("%s", dict);
        printf("\n");
        numWords = countWordsOfLength(dict,wordSize);
    }
    
    // allocate heap memory for the word array; only words with desired length
    char** words = (char**)malloc(numWords*sizeof(char*));
    for (int i = 0; i < numWords; ++i) {
        words[i] = (char*)malloc((wordSize+1)*sizeof(char));
    }

    // end program if file does not have at least two words of desired length
    if (numWords < 2) {
        printf("  Dictionary %s contains insufficient %d-letter words...\n",dict,wordSize);
        printf("Terminating program...\n");
        return -1;
    }
    
    // [usedWord] bool array has same size as word array [words];
    //  all elements initialized to [false];
    //  later, usedWord[i] will be set to [true] whenever 
    //      words[i] is added to ANY partial word ladder;
    //      before adding words[i] to another word ladder,
    //      check for previous usage with usedWord[i] 
    bool* usedWord = (bool*)malloc(numWords*sizeof(bool));
    for (int i = 0; i < numWords; ++i) {
        usedWord[i] = false;
    }
    
    // build word array (only words with desired length) from dictionary file
    printf("Building array of %d-letter words... ", wordSize); 
    bool status = buildWordArray(dict,words,numWords,wordSize);
    if (!status) {
        printf("  ERROR in building word array.\n");
        printf("  File not found or incorrect number of %d-letter words.\n",wordSize);
        printf("Terminating program...\n");
        return -1;
    }
    printf("Done!\n"); 

    // set the two ends of the word ladder using interactive user-input
    //  make sure start and final words are in the word array, 
    //  have the correct length (implicit by checking word array), AND
    //  that the two words are not the same
    char startWord[30];
    char finalWord[30];
    printf("Setting the start %d-letter word... \n", wordSize);
    setWord(words, numWords, wordSize, startWord);
    printf("\n");
    printf("Setting the final %d-letter word... \n", wordSize);
    setWord(words, numWords, wordSize, finalWord);
    while (strcmp(finalWord,startWord) == 0) {
        printf("  The final word cannot be the same as the start word (%s).\n",startWord);
        printf("Setting the final %d-letter word... \n", wordSize);
        setWord(words, numWords, wordSize, finalWord);
    }
    printf("\n");

    // run the algorithm to find the shortest word ladder
    WordNode* myLadder = findShortestWordLadder(words, usedWord, numWords, wordSize, startWord, finalWord);

    // display word ladder and its height if one was found
    if (myLadder == NULL) {
        printf("There is no possible word ladder from %s to %s\n",startWord,finalWord);    
    } else {
        printf("Shortest Word Ladder found!\n");
        printLadder(myLadder);
    }
    printf("Word Ladder height = %d\n",getLadderHeight(myLadder));
    
    // free the heap-allocated memory for the shortest ladder
    freeLadder(myLadder);
    // free the heap-allocated memory for the words array
    freeWords(words,numWords);
    free(usedWord);
    
    return 0;
}
