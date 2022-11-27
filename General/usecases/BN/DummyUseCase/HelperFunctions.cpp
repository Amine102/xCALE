#include "HelperFunctions.h"
#include "learnDummy.h"
#include <algorithm>

using namespace std;
using namespace PILGRIM;

void splitData(string data_file, int numberOfParts)
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


void mergeFiles(string data_file1, string data_file2){
    
}

int GetNumberOfSplitsFromUser()
{
    int numberOfParts;
    cout << "How many parts do you want your data to be divided ? Please Enter an integer number" << endl;
    cin >> numberOfParts;
    //Checking that the entered value is an int number
    if (numberOfParts != int(numberOfParts))
    {
        cout << "Error, number is not in" << endl;
        return -1;
    }
    return numberOfParts;
}

bool AskUserIfChangingForgetFactor()
{
    cout << "Do you want the value of the forgetting factor to increment while learning, Please Enter yes or no" << endl;
    string decrementingForgettingFactor;
    bool changingForgettingFactor;
    cin >> decrementingForgettingFactor;
    if (decrementingForgettingFactor == "yes")
    {
        return true;
    }
    return false;
}

double getForgettingFactorFromUser()
{
    double ff;
    cout << "Please enter the value of the forgetting factor that you wish. Please enter a value between 0 and 1" << endl;
    cin >> ff;
    if (ff < 0)
    {
        return 0;
    }
    else if (ff > 1)
    {
        return 1;
    }
    else
    {
        return ff;
    }
}

plComputableObject getComputableObject(string headSignature, plComputableObjectList compObject_List){
    for (int i = 0; i < compObject_List.size(); i++)
    {
      if (compObject_List[i].get_head_string() == headSignature)
      {
          cout<<"FOUND!!"<<endl;
        return compObject_List[i];
      }
    }
    cout<<"finished for loop"<<endl;
}

void exportVectorOfProba(plComputableObjectList co_list, string headSignature, int id)
{   
    plComputableObject co = getComputableObject(headSignature, co_list);
    vector<plProbValue> vect;
    co.tabulate(vect);
    std::ofstream outFile(to_string(id) + ".txt");
    // the important part
    for (const auto &e : vect)
        {
            outFile << e << " ";
        }
}