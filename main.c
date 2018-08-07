#include "header.h"

int main(int argc,char* argv[]) {

    /*Index used to loop through the input files*/
    int index;
    FILE *fp = NULL;
    int IC = 100;
    int DC = 0;

    if(argc < 2){
        FILE_NOT_FOUND;
        exit(1);
    } else if(argc>2){
        main(argc-1,argv);
    }

    fp = fopen(strcat(argv[argc-1],FILE_EXTENTION),"r");
    if(!fp){
        FILE_ERROR;
    } else{
        /*first pass*/
               firstPass(f, &IC, &DC);
        /*update symbols*/
                updateSymbols(IC);
        /*Update words list addresses*/
                           updateAddresses();
        /*update data list addresses*/
                          updateDataAddresses(IC);
        /*Second pass*/
                secondPass(IC);
        /*If there were no errors, export the files*/
        if (!isError())
        {
            exportFiles(IC, DC, argv[index]);
        }
        /*If there were errors, export the error file*/
        else
        {
            printErrors(argv[index]);
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