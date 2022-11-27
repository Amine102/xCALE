#include <ostream>
#include "pilgrim/general/pmCSVdynDataSet.h"
#include <plStructureLearner.h>



using namespace PILGRIM;
using namespace std;
//! To load data file of type csv or txt by specifying the number of the fields.
/*!
\return  Pointer variable of type pmCSVDataSet holding the current data.
*/


/*
=====CONSTRUCTORS=====
*/
//=============================================================================
pmCSVdynDataSet::pmCSVdynDataSet( char* _file_name) : file_name(_file_name) {
    split();

    string str(file_name);  
    size_t pos = str.find('.');
    string out_D0 = str.substr(0, pos) + "_D0.csv";
    string out_Dt = str.substr(0, pos) + "_Dt.csv";
  
    // Those two lines might crash one day, better to switch char* in string...
    char* file_D0 = strdup(out_D0.c_str());
    char* file_Dt = strdup(out_Dt.c_str());

    data_set_t0 = new pmCSVDataSet(file_D0);
    data_set_T = new pmCSVDataSet(file_Dt);
}

//=============================================================================
pmCSVdynDataSet::pmCSVdynDataSet(   char *_file_name,
                                    size_t &nb_header_lines,
                                    size_t &nb_columns,
                                    const plVariablesConjunction &variables_t0,
                                    const plVariablesConjunction &variables_T,
                                    char delimiter) : file_name(_file_name) {
    
    split();

    string str(file_name);  
    size_t pos = str.find('.');
    string out_D0 = str.substr(0, pos) + "_D0.csv";
    string out_Dt = str.substr(0, pos) + "_Dt.csv";
  

    // Those two lines crash one day, better to switch char* in string...
    char* file_D0 = strdup(out_D0.c_str());
    char* file_Dt = strdup(out_Dt.c_str());

    size_t nb_columns_dt = nb_columns * 2;
    size_t &pnbcolumn = nb_columns_dt;

    data_set_t0 = new pmCSVDataSet(file_D0, nb_header_lines, nb_columns, delimiter, variables_t0);
    data_set_T = new pmCSVDataSet(file_Dt, nb_header_lines, pnbcolumn, delimiter, variables_T);
}


/*
=====DESTRUCTOR=====
*/
//=============================================================================
pmCSVdynDataSet::~pmCSVdynDataSet(void) {
    delete[] data_set_t0;
    delete[] data_set_T;
}

/*
=====GETTERS=====
*/
//=============================================================================
plCSVFileDataDescriptor* pmCSVdynDataSet::get_data_descriptor_t0() {
	return data_set_t0->get_data_descriptor();
}

//=============================================================================
plCSVFileDataDescriptor* pmCSVdynDataSet::get_data_descriptor_T() {
	return data_set_T->get_data_descriptor();
}

//=============================================================================
pmCSVDataSet* pmCSVdynDataSet::get_data_set_t0() {
    return data_set_t0;
}

//=============================================================================
pmCSVDataSet* pmCSVdynDataSet::get_data_set_T() {
    return data_set_T;
}

//=============================================================================
bool pmCSVdynDataSet::get_data_record_t0(std::vector<std::string>& records) {
    return data_set_t0->get_data_record(records);
}

//=============================================================================
bool pmCSVdynDataSet::get_data_record_T(std::vector<std::string>& records) {
    return data_set_T->get_data_record(records);
}

//=============================================================================
size_t pmCSVdynDataSet::get_n_records_t0() {
    return data_set_t0->get_n_records();
}

//=============================================================================
size_t pmCSVdynDataSet::get_n_records_T() {
    return data_set_T->get_n_records();
}

//=============================================================================
unsigned int pmCSVdynDataSet::get_num_fields_t0() {
    return data_set_t0->get_num_fields();
}

//=============================================================================
unsigned int pmCSVdynDataSet::get_num_fields_T() {
    return data_set_T->get_num_fields();
}

//=============================================================================
plVariablesConjunction pmCSVdynDataSet::observed_variables_t0() {
    return data_set_t0->observed_variables();
}

//=============================================================================
plVariablesConjunction pmCSVdynDataSet::observed_variables_T() {
    return data_set_T->observed_variables();
}

/*
=====PRINTER=====
*/

//=============================================================================
void pmCSVdynDataSet::summary() {
    data_set_t0->summary();
    data_set_T->summary();
}

/*
=====METHOD=====
*/
//=============================================================================
void pmCSVdynDataSet::split()
{
    string file_name = this->file_name;
    char delim = ';';
    fstream myFile(file_name, ios::in);
    if (myFile)
    {
        unsigned int iterator(0);
        string tuple;
        size_t pos = file_name.find('.');
        const string out_D0 = file_name.substr(0, pos) + "_D0.csv";
        const string out_Dt = file_name.substr(0, pos) + "_Dt.csv";
        ofstream out(out_D0, ios::out);
        ofstream out_T(out_Dt, ios::out);
        string seq_value;
        string prev_tuple;
        string dt_tuple;
        while (getline(myFile, tuple))
        {
            unsigned int word_count = 0;
            string element;

            size_t pos = tuple.find_first_of(delim);
            string cut_tuple = tuple.substr(pos + 1);
            string first_element_of_tuple = tuple.substr(0, pos); // Keeping it just to know when I change index number
            if (iterator == 0)
            {
                istringstream stream_tuple(cut_tuple);
                while (getline(stream_tuple, element, delim))
                {
                    size_t found = element.find_last_of('/"');
                    if (stream_tuple.eof()) // If Last word, go next line
                    {
                        out << element.substr(0, found) + "_t0" + '\"' << endl;
                        out_T   << element.substr(0, found) + "" + '\"' << delim 
                                << element.substr(0, found) + "_t" + '\"' << endl;
                    }
                    else// Else we write a delim character
                    {
                        out << element.substr(0, found) + "_t0" + '\"' << delim;
                        out_T   << element.substr(0, found) + "" + '\"' << delim 
                                << element.substr(0, found) + "_t" + '\"' << delim;
                    }
                }
            }
            else
            {
                if (first_element_of_tuple != seq_value)
                {
                    seq_value = first_element_of_tuple;
                    out << cut_tuple << endl;
                    prev_tuple = cut_tuple;
                    dt_tuple.clear();
                }
                else
                {
                    //current_tuple = cut_tuple;
                    istringstream buff_1(prev_tuple);
                    istringstream buff_2(cut_tuple);
                    string alternative;
                    while (!buff_2.eof())
                    {
                        getline(buff_1, alternative, delim);
                        dt_tuple += (alternative + delim);
                        if (!buff_1.eof())
                        {
                            getline(buff_2, alternative, delim);
                            dt_tuple += (alternative + delim);
                        }
                        else
                        {
                            getline(buff_2, alternative, delim);
                            dt_tuple += (alternative);
                        }
                    }
                    out_T << dt_tuple << endl;
                    dt_tuple = prev_tuple;
                    prev_tuple = cut_tuple;
                    dt_tuple.clear();
                }
            }
            iterator++;
        }
        myFile.close();
    }
    else
        cerr << "ERROR: failing to open the file" << endl;
}