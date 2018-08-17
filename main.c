#include "header.h"

int main(int argc,char* argv[]) {

    /*Index used to loop through the input files*/
    FILE *fp = NULL;
    int IC = 100;
    int DC = 0;

    if(argc < 2){
        printf("File not found, please check it and try again");
        exit(1);
    } else if(argc > 2){
        main(argc - 1 , argv);
    }

    fp = openFile(argv[argc-1],"r",AS_EXTENSION);
    if(!fp){
        printf("File opening error, please check it and try again");
    } else {
        /*first pass*/
        firstPass(fp, &IC, &DC);


        /*update symbols*/
        updateSymbols(IC);


        secondPass();
        /*Update words list addresses*/
        updateAddresses();
        /*update data list addresses*/
        updateDataAddresses(IC);
        /*Second pass*/


        secondPass();

        /*If there were no errors, export the files*/
        if (!isError())
        {
            thirdStage(argv[argc-1]);
        }
        /*If there were errors, export the error file*/
        else
        {
            printErrors(argv[argc-1]);
            /*Free the errors list*/
            freeErrors();
        }
        /*Cleanup*/
         freeData();
        freeSymbols();
        freeEntries();
        freeWords();
    }




    return 0;
}