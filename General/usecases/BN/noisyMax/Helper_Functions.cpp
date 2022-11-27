#include "Helper_Functions.h"
#include <algorithm>

using namespace std;
using namespace PILGRIM;


plValues get_Label_Index(plVariable x, string s)
{
    return plValues().add(x, plLabelType(x.get_type()).label_to_index(s));
}

void split_Data(string data_file, int numberOfParts)
{
    string *arrayFiles = new string[numberOfParts]; // initialize an array with size numberOfParts

    /*
        =========================================================================================================
        get the number of lines to know the length of the initial data file  
        */
    ifstream file_temp(data_file);
    string l;
    int c = 0;
    while (getline(file_temp, l))
        c++;
    int file_length = c;
    /* ========================================================================================================= */

    int Split_length = file_length / numberOfParts; // the number of lines in each file after splitting
    int files_iterator = 0;                         // iterator for the arrayFiles array

    /*
        Getting the first line of the initial file which is the header line where there the name of the variables and add it to the first file
        */
    ifstream file(data_file);
    string value;
    getline(file, value);
    // This string will contain the first line of the data file, which is the names of the variables
    string header_VarsNames = (string(value) + "\n");
    arrayFiles[files_iterator] += header_VarsNames;

    // Loop on the initial file lines
    // file.good() --> Returns true if none of the stream's error state flags (eofbit, failbit and badbit) is set.
    while (file.good())
    {
        getline(file, value);
        /*
            When a file is finished and the number of lines added to it = Split_length, 
            we reinitialize the Split_length variable with file_length / numberOfParts
            */
        if (Split_length == 0)
        {
            Split_length = file_length / numberOfParts;
            files_iterator++;
            arrayFiles[files_iterator] += header_VarsNames;
            if (files_iterator > numberOfParts)
                break;
        }
        else
        {
            Split_length--;
            arrayFiles[files_iterator] += (string(value) + "\n");
        }
    }

    //After we finished filling the string variables of each file we create the file and save them.
    for (int i = 0; i < numberOfParts; i++)
    {
        ofstream f;
        f.open(data_file + to_string(i) + ".data");
        f << arrayFiles[i];
        f.close();
    }
};