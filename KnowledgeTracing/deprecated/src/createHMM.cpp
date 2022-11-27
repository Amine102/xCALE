#include "../header/createHMM.h"
#include <ctime>

#include <pl.h>
#include <stdio.h>

using namespace boost;
using namespace std;
using namespace PILGRIM;

/**
 *\brief create a DBN_HMM for knowledge tracing 
 */
void split(string file_name)
{
   char delim = ';';
   fstream myFile(file_name, ios::in);
   if (myFile)
   {
       unsigned int iterator(0);
       string tuple;
       size_t pos = file_name.find('.');
       const string out_D0 = file_name.substr(0, pos) + "_t0.csv";
       const string out_Dt = file_name.substr(0, pos) + "_t.csv";
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


void rewrite(string file_name) {
	char delim = ';';
	fstream myFile(file_name.c_str(), ios::in);
	unsigned int iterator(0);
	string line;
	string word;
	std::vector<pair<string, string>> col_label;
	if (myFile) {
		size_t pos = file_name.find('.');
		const string out_ = file_name.substr(0, pos) + "_corrected.csv";
		ofstream out(out_, ios::out);
		unsigned int pos_column(0);

		while (getline(myFile, line)) {
			if (iterator == 0) {
				istringstream token(line);
				pair<string, string> notResult;
				pair<string, string> result;
				notResult = make_pair("no", "yes");
				result = make_pair("failed", "passed");
				out << line << endl;
				while(getline(token, word, delim)) {
					// CHECK IF VARIABLE NAME START WITH R
					if (word.find("\"R") == 0) {
						col_label.push_back(result);
					} else col_label.push_back(notResult);
				}
			}
			else {
				pos_column = 0;
				istringstream token(line);

				while(getline(token, word, delim)) {
					if (word == "0") {
						if (token.eof()) {
							out << col_label[pos_column].first << endl;
						}
						else {
							
							out << col_label[pos_column].first << delim;
						}
					}
					else {
						if (token.eof()) {
							out << col_label[pos_column].second << endl;
						}
						else {
							out << col_label[pos_column].second << delim;
						}
					}
					pos_column++;
				}
			}
			iterator++;
		}
	}
	else cerr << "File not found" << endl;
}

void vectWrite( std::string file_name,
               	ObservationSequence_t& observationSequence_T
                ) {
  char delim = ';';
  fstream myFile(file_name, ios::in);	
	if (myFile)
	{
    string line; 										// temporary stocking my line
    string memory_sequence = "1";		// stocking in which sequence number I am
    string observation;							// stocking my string data to convert it in float
		string first_word;							// temporary the sequence number of the line I am in

		bool iAmFirstLine(true);				// Boolean saying if i'm in the first line
		unsigned int pos_(0);						// Position of the variable I want
		unsigned int counter(0);				// temporary incrementing in which variable I am

		Observation_t observation_T;		// My vector of value in one sequence, need to be cleared when I change of sequence

    while (getline(myFile, line)) {	// Getting my line from my file (boolean)
			counter = 0;
			if (iAmFirstLine) {
				istringstream token(line);	// stream object with my line content
				string word;								
				while (getline(token, word, delim)) {
					if (word == "\"ResA\"") break;
					else pos_++;
				}
      	iAmFirstLine = false;
			}
      else {
        //stateSequence_T.push_back(stoul(sequence_number, nullptr, 0));
        istringstream token(line);
        while (getline(token, observation, delim)) {
					// cout << observation << endl;
					if (counter == 0) {
						first_word = observation;
					}
					if (counter == pos_) {
						if (first_word != memory_sequence) {
							memory_sequence = first_word;
							observationSequence_T.push_back(observation_T);
        			observation_T.clear();
						}
						// cout << typeid(stof(observation)).name() << endl;
						observation_T.push_back(stof(observation));
					}
					counter++;
				}
			}
    }
		observationSequence_T.push_back(observation_T);
    observation_T.clear();
  }
  else cerr << "No file found" << endl;
}

void vectWriteBis( 	std::string file_name,
										ObservationSequenceArray_t& observation_sequences
										) {
  char delim = ';';
  fstream myFile(file_name, ios::in);	
	if (myFile)
	{
    string line; 										// temporary stocking my line
    string memory_sequence = "1";		// stocking in which sequence number I am
    string observation;							// stocking my string data to convert it in float
		string first_word;							// temporary the sequence number of the line I am in

		bool iAmFirstLine(true);				// Boolean saying if i'm in the first line
		unsigned int pos_(0);						// Position of the variable I want
		unsigned int counter(0);				// temporary incrementing in which variable I am

		Observation_t observation_T;		// My vector of value in one sequence, need to be cleared when I change of sequence
		ObservationSequence_t observationSequence_T; // My vector of vector


    while (getline(myFile, line)) {	// Getting my line from my file (boolean)
			counter = 0;
			if (iAmFirstLine) {
				istringstream token(line);	// stream object with my line content
				string word;								
				while (getline(token, word, delim)) {
					if (word == "\"ResA\"") break;
					else pos_++;
				}
      	iAmFirstLine = false;
			}
      else {
        //stateSequence_T.push_back(stoul(sequence_number, nullptr, 0));
        istringstream token(line);
        while (getline(token, observation, delim)) {
					// cout << observation << endl;
					if (counter == 0) {
						first_word = observation;
					}
					if (counter == pos_) {
						if (first_word != memory_sequence) {
							memory_sequence = first_word;
							observation_sequences.push_back(observationSequence_T);
        			observationSequence_T.clear();
						}
						// cout << typeid(stof(observation)).name() << endl;
						observation_T.push_back(stof(observation));
						observationSequence_T.push_back(observation_T);
						observation_T.clear();
					}
					counter++;
				}
			}
    }
		observation_sequences.push_back(observationSequence_T);
		observationSequence_T.clear();
  }
  else cerr << "No file found" << endl;
}

void displayVect( ObservationSequence_t& observationSequence_T) {
	cout << observationSequence_T.size() << endl;
  for (unsigned int i = 0; i < observationSequence_T.size(); i++) {
    cout << "seq: " << i + 1 << "\t";
    for (unsigned int j = 0; j < observationSequence_T[i].size(); j++) {
      cout << observationSequence_T[i][j] << ";";
    }
    cout << endl;
  }
}

void displayVectBis( ObservationSequenceArray_t& observation_sequences) {
	cout << observation_sequences.size() << endl;
  for (unsigned int i = 0; i < observation_sequences.size(); i++) {
    cout << "seq: " << i + 1 << "\t";
    for (unsigned int j = 0; j < observation_sequences[i].size(); j++) {
      cout << observation_sequences[i][j][0] << ";";
    }
    cout << endl;
  }
}



void createHMM() {

	plError::ignore_this_message(138, true);

	/*
	========== GENERAL DATA FOR BOTH BAYESIAN NETWORK SETTINGS ==========
	*/
	vector<plLearnObject*> learnObjectList_t0 ;
	vector<plLearnObject*> learnObjectList_t ;

	//Type definitions
	std::vector<std::string> State_SKILL;
	State_SKILL.push_back("no");
	State_SKILL.push_back("yes");
	plLabelType SKILL_TYPE(State_SKILL); // Label for skill

	std::vector<std::string> State_RESULT;
	State_RESULT.push_back("failed");
	State_RESULT.push_back("passed");
	plLabelType RESULT_TYPE(State_RESULT); // Label for exam result

	//User-defined parameters
	double P_INITIAL = 0;
	double PA_GUESS = 0.1;
	double PB_GUESS = 0.3;
	double PC_GUESS = 0.2;
	double PA_SLIP = 0.15;
	double PB_SLIP = 0.3;
	double PC_SLIP = 0.2;
	double PA_LEARN = 0.5;
	double PB_LEARN = 0.8;
	double PC_LEARN = 0.7;
	double P_FORGET = 0;
	double P1 = 0.4;
	double P2 = 0.5;
	double P_LR = 0.6;
	double P_L2= 0.7;

	/*
	========== MANUALLY SETTING OF INITIAL BAYESIAN NETWORK ==========
	*/

	// Variable definitions
	plVariable SkillA_t0("SkillA_t0", SKILL_TYPE);
	plVariable SkillB_t0("SkillB_t0", SKILL_TYPE);  
	plVariable SkillC_t0("SkillC_t0", SKILL_TYPE);
	plVariable ResA_t0("ResA_t0", RESULT_TYPE);
	plVariable ResB_t0("ResB_t0", RESULT_TYPE);
	plVariable ResC_t0("ResC_t0", RESULT_TYPE);
	plVariable ZA_t0("ZA_t0", SKILL_TYPE);
	plVariable ZB_t0("ZB_t0", SKILL_TYPE);
	plVariable PRC_t0("PRC_t0", SKILL_TYPE);
	plVariable ZPRC_t0("ZPRC_t0", SKILL_TYPE);
	plVariable ZSRC_t0("ZSRC_t0", SKILL_TYPE);  

	// Probability table of SkillA_t0
	plProbValue tableSkillA_t0[] = {
		1-P_INITIAL, 
		P_INITIAL
	};
	plProbTable P_SkillA_t0(SkillA_t0, tableSkillA_t0);
	learnObjectList_t0.push_back(new plLearnLaplace(SkillA_t0));
	
	// Probability table of SkillB_t0
	plProbValue tableSkillB_t0[] = {
		1-P_INITIAL, 
		P_INITIAL
	};
	plProbTable P_SkillB_t0(SkillB_t0, tableSkillB_t0);
	learnObjectList_t0.push_back(new plLearnLaplace(SkillB_t0));

	// Probability table of ResA_t0, which depends on SkillA_t0
	plProbValue tableResA_t0_knowingSkillA_t0[] = {
		1-PA_GUESS, PA_GUESS,
		PA_SLIP, 1-PA_SLIP
	};
	plCndDistribution P_ResA_t0(ResA_t0,SkillA_t0, tableResA_t0_knowingSkillA_t0);
	plLearnDistributionTable PL_ResA_t0(ResA_t0,SkillA_t0);
	PL_ResA_t0.push(plLearnLaplace(ResA_t0) ,0);
	PL_ResA_t0.push(plLearnLaplace(ResA_t0) ,1);
	learnObjectList_t0.push_back(&PL_ResA_t0);


	// Probability table of ResB_t0, which depends on SkillB_t0
	plProbValue tableResB_t0_knowingSkillB_t0[] = {
		1-PB_GUESS, PB_GUESS,
		PB_SLIP, 1-PB_SLIP
	};
	plCndDistribution P_ResB_t0(ResB_t0,SkillB_t0, tableResB_t0_knowingSkillB_t0);
	plLearnDistributionTable PL_ResB_t0(ResB_t0,SkillB_t0);
	PL_ResB_t0.push(plLearnLaplace(ResB_t0),0);
	PL_ResB_t0.push(plLearnLaplace(ResB_t0),1);
	learnObjectList_t0.push_back(&PL_ResB_t0);

	// Probability table of ZA_t0, which depends on SkillA_t0
	plProbValue tableZA_t0_knowingSkillA_t0[] = {
		1, 0,
		1-P1, P1
	};
	plProbValue frozen1_0[] = {1, 0};
	plCndDistribution P_ZA_t0(ZA_t0, SkillA_t0, tableZA_t0_knowingSkillA_t0);
	plLearnDistributionTable PL_ZA_t0(ZA_t0,SkillA_t0);
	//plProbTable PLZA(ZA_t0,frozen1_0);
	//plLearnFrozenDistribution PLZFA(PLZA);
	//PL_ZA_t0.push(plLearnFrozenDistribution(PLZA),0); // <============ BUG
	std::vector<plFloat> alphaVect;
	alphaVect.push_back(100000000000000000000000000000000000000000000000000000.0);
	alphaVect.push_back(0);
	PL_ZA_t0.push(plBayesLearnProbTable(ZA_t0,alphaVect),0); // <== before solving the "frozen10 bug", let's learn the column
	PL_ZA_t0.push(plLearnLaplace(ZA_t0),1);
	learnObjectList_t0.push_back(&PL_ZA_t0);

	std::vector<plFloat> alphaVect2;
	alphaVect2.push_back(0);
	alphaVect2.push_back(100000000000000000000000000000000000000000000000000000.0);

	// Probability table of ZB_t0, which depends on SkillB_t0
	plProbValue tableZB_t0_knowingSkillB_t0[] = {
	1, 0,
	1-P2, P2
	};
	plCndDistribution P_ZB_t0(ZB_t0, SkillB_t0, tableZB_t0_knowingSkillB_t0);
	plLearnDistributionTable PL_ZB_t0(ZB_t0,SkillB_t0);
	//plProbTable PLZB(ZB_t0,frozen1_0);
	//plLearnFrozenDistribution PLZFB(PLZB);
	//PL_ZB_t0.push(PLZFB,0); // <========================================= BUG
	PL_ZB_t0.push(plBayesLearnProbTable(ZB_t0,alphaVect),0); // <== before solving the "frozen10 bug", let's learn the column
	PL_ZB_t0.push(plLearnLaplace(ZB_t0),1);
	learnObjectList_t0.push_back(&PL_ZB_t0);


	// C source code function :
	      std::string my_code =
	      "void my_max(double* output, unsigned int output_size,\n"
	      " double* input, unsigned int input_size)\n"
	      "{\n"
	      " double* vectMax = output;\n"
	      " double* vectProb = input;\n"
	      "\n"
	      " unsigned int n_probas; /* Number of values */\n"
	      " double value_max=vectProb[0]; \n"
	      " unsigned int i;\n"
	      " \n"
	      " n_probas = input_size; /* Get the number of values */\n"
	      "\n"
	      " for(i=0;i<n_probas;i++) /* Find the max value */\n"
	      " if (vectProb[i]>value_max) value_max=vectProb[i];\n"
	      " vectMax[0]=value_max; \n"
	      "}\n";


    // Create the external function
	const plExternalFunctionFromC max_fun0(PRC_t0, ZA_t0^ZB_t0, my_code,"my_max");
	const plCndDeterministic P_PRC_t0givenZA_t0ZB_t0(PRC_t0, ZA_t0^ZB_t0, max_fun0);
	learnObjectList_t0.push_back(new plLearnFrozenDistribution(P_PRC_t0givenZA_t0ZB_t0));

	// Probability table of ZPRC_t0, which depends on PRC_t0
	plProbValue tableZPRC_t0_knowingPRC_t0[] = {
	1, 0,
	1-P_LR, P_LR
	};
	plCndDistribution P_ZPRC_t0(ZPRC_t0, PRC_t0, tableZPRC_t0_knowingPRC_t0);
	plLearnDistributionTable PL_ZPRC_t0(ZPRC_t0,PRC_t0);
	//plProbTable PLZRC(ZPRC_t0,frozen1_0);
	//plLearnFrozenDistribution PLZFRC(PLZRC);
	//PL_ZPRC_t0.push(PLZFRC,0); // <======================================= BUG
	PL_ZPRC_t0.push(plBayesLearnProbTable(ZPRC_t0,alphaVect),0); // <== before solving the "frozen10 bug", let's learn the column
	PL_ZPRC_t0.push(plLearnLaplace(ZPRC_t0),1);
	learnObjectList_t0.push_back(&PL_ZPRC_t0);

	// Probability table of ZSRC_t0
	plProbValue tableZSRC_t0[] = {
		1, 0
	};
	plProbTable P_ZSRC_t0(ZSRC_t0, tableZSRC_t0);
	learnObjectList_t0.push_back(new plLearnFrozenDistribution(P_ZSRC_t0));

    const plExternalFunctionFromC max_fun1(SkillC_t0, ZPRC_t0^ZSRC_t0, my_code,"my_max");
	const plCndDeterministic P_SkillC_t0givenZPRC_t0ZSRC_t0(SkillC_t0, ZPRC_t0^ZSRC_t0, max_fun1);
	learnObjectList_t0.push_back(new plLearnFrozenDistribution(P_SkillC_t0givenZPRC_t0ZSRC_t0));

	// Probability table of ResC_t0 which depends on SkillC_t0
	plProbValue tableResC_t0_knowingSkillC_t0[] = {
	1-PC_GUESS, PC_GUESS,
	PC_SLIP, 1-PC_SLIP
	};
	plCndDistribution P_ResC_t0(ResC_t0,SkillC_t0, tableResC_t0_knowingSkillC_t0);
	plLearnDistributionTable PL_ResC_t0(ResC_t0,SkillC_t0);
	PL_ResC_t0.push(plLearnLaplace(ResC_t0),0);
	PL_ResC_t0.push(plLearnLaplace(ResC_t0),1);
	learnObjectList_t0.push_back(&PL_ResC_t0);

	// Joint distribution -- manually defined
	plJointDistribution jd_t0(	SkillA_t0^SkillB_t0^ZA_t0^ZB_t0^PRC_t0^ZPRC_t0^ZSRC_t0^SkillC_t0^ResA_t0^ResB_t0^ResC_t0,
	 							P_SkillA_t0*P_SkillB_t0*P_ZA_t0*P_ZB_t0*P_PRC_t0givenZA_t0ZB_t0*P_ZPRC_t0*P_ZSRC_t0*P_SkillC_t0givenZPRC_t0ZSRC_t0*P_ResA_t0*P_ResB_t0*P_ResC_t0);


	// Creating my bayesian network
	pmBayesianNetwork G0(jd_t0);

	// Print my bayesian network
	cout << endl << "========== MANUAL INITIAL BN ==========" << endl;
	G0.summary();
	//G0.print_graph();


	/*
	========== MANUALLY SETTING OF TRANSITION BAYESIAN NETWORK ==========
	*/

	// Variable definitions
	plVariable SkillA("SkillA", SKILL_TYPE);
	plVariable SkillB("SkillB", SKILL_TYPE);  
	plVariable SkillC("SkillC", SKILL_TYPE);
	plVariable ResA("ResA", RESULT_TYPE);
	plVariable ResB("ResB", RESULT_TYPE);
	plVariable ResC("ResC", RESULT_TYPE);
	plVariable ZA("ZA", SKILL_TYPE);
	plVariable ZB("ZB", SKILL_TYPE);
	plVariable PRC("PRC", SKILL_TYPE);
	plVariable ZPRC("ZPRC", SKILL_TYPE);
	plVariable ZSRC("ZSRC", SKILL_TYPE); 

	plVariable SkillA_t("SkillA_t", SKILL_TYPE);
	plVariable SkillB_t("SkillB_t", SKILL_TYPE);
	plVariable SkillC_t("SkillC_t", SKILL_TYPE);
	plVariable ResA_t("ResA_t", RESULT_TYPE);
	plVariable ResB_t("ResB_t", RESULT_TYPE);
	plVariable ResC_t("ResC_t", RESULT_TYPE);
	plVariable ZA_t("ZA_t", SKILL_TYPE);
	plVariable ZB_t("ZB_t", SKILL_TYPE);
	plVariable PRC_t("PRC_t", SKILL_TYPE);
	plVariable ZPRC_t("ZPRC_t", SKILL_TYPE);
	plVariable ZSRC_t("ZSRC_t", SKILL_TYPE);

	// Probability table of SkillA
	plProbValue tableSkillA[] = {0.5, 0.5};
	plProbTable P_SkillA(SkillA, tableSkillA);
	plProbTable Prand_SkillA(SkillA, true);
	//learnObjectList_t.push_back(new plLearnLaplace(SkillA));
	learnObjectList_t.push_back(new plLearnFrozenDistribution(P_SkillA));
	
	// Probability table of SkillB
	plProbValue tableSkillB[] = {0.5, 0.5};
	plProbTable P_SkillB(SkillB, tableSkillB);
	//learnObjectList_t.push_back(new plLearnFrozenDistribution(P_SkillB));

	// Probability table of SkillC
	plProbValue tableSkillC[] = {0.5, 0.5};
	plProbTable P_SkillC(SkillC, tableSkillC);
	//learnObjectList_t.push_back(new plLearnFrozenDistribution(P_SkillC));

	// Probability table of ResA, which depends on SkillA
	plProbValue tableResA_knowingSkillA[] = {
		1-PA_GUESS, PA_GUESS,
		PA_SLIP, 1-PA_SLIP
	};
	plCndDistribution P_ResA(ResA, SkillA, tableResA_knowingSkillA);
	plProbValue tableResArand_knowingSkillA[] = {
		.8, .2,
		.1, .9
	};
	plCndDistribution Prand_ResA(ResA, SkillA, tableResArand_knowingSkillA);
	plLearnDistributionTable PL_ResA(ResA, SkillA);
	PL_ResA.push(plLearnLaplace(ResA) ,0);
	PL_ResA.push(plLearnLaplace(ResA) ,1);
	learnObjectList_t.push_back(&PL_ResA);
	
	// Probability table of ResB
	plProbValue tableResB[] = {0.5, 0.5};
	plProbTable P_ResB(ResB,  tableResB);
	//learnObjectList_t.push_back(new plLearnFrozenDistribution (P_ResB));

	// Probability table of ZA
	plProbValue tableZA[] = {0.5, 0.5};
	plProbTable P_ZA(ZA, tableZA);
	//learnObjectList_t.push_back(new plLearnFrozenDistribution(P_ZA));

	// Probability table of ZB
	plProbValue tableZB[] = {0.5, 0.5};
	plProbTable P_ZB(ZB, tableZB);
	//learnObjectList_t.push_back(new plLearnFrozenDistribution(P_ZB));

	// Probability table of PRC
	plProbValue tablePRC[] = {0.5, 0.5};
	plProbTable P_PRC(PRC, tablePRC);
	//learnObjectList_t.push_back(new plLearnFrozenDistribution(P_PRC));

	// Probability table of ZPRC
	plProbValue tableZPRC[] = {0.5, 0.5};
	plProbTable P_ZPRC(ZPRC, tableZPRC);
	//learnObjectList_t.push_back(new plLearnFrozenDistribution(P_ZPRC));

	// Probability table of ZSRC
	plProbValue tableZSRC[] = {0.5, 0.5};
	plProbTable P_ZSRC(ZSRC, tableZSRC);
	//learnObjectList_t.push_back(new plLearnFrozenDistribution(P_ZSRC));

	// Probability table of ResC 
	plProbValue tableResC[] = {0.5, 0.5};
	plProbTable P_ResC(ResC, tableResC);
	//learnObjectList_t.push_back(new plLearnFrozenDistribution(P_ResC));

	// Probability table of SkillA_t, which depends on SkillA
	plProbValue tableSkillA_t_knowingSkillA[] = {
		1-PA_LEARN, PA_LEARN,
		P_FORGET, 1-P_FORGET
	};
	plProbValue tableSkillArand_t_knowingSkillA[] = {
		.7, 0.3,
		0, 1
	};
	plCndDistribution P_SkillA_t(SkillA_t, SkillA, tableSkillA_t_knowingSkillA);
	plCndDistribution Prand_SkillA_t(SkillA_t, SkillA, tableSkillArand_t_knowingSkillA);
	plLearnDistributionTable PL_SkillA_t(SkillA_t, SkillA);
	PL_SkillA_t.push(plLearnLaplace(SkillA_t), 0);
	//PL_SkillA_t.push(plLearnLaplace(SkillA_t), 1);
	PL_SkillA_t.push(plBayesLearnProbTable(SkillA_t, alphaVect2),1);
	learnObjectList_t.push_back(&PL_SkillA_t);


	// Probability table of SkillB_t, which depends on SkillB
	plProbValue tableSkillB_t_knowingSkillB[] = {
		1-PB_LEARN, PB_LEARN,
		P_FORGET, 1-P_FORGET
	};

	plCndDistribution P_SkillB_t(SkillB_t, SkillB, tableSkillB_t_knowingSkillB);
	plLearnDistributionTable PL_SkillB_t(SkillB_t, SkillB);
	PL_SkillB_t.push_default(plLearnLaplace(SkillB_t));
	PL_SkillB_t.push(plLearnLaplace(SkillB_t) ,0);
	PL_SkillB_t.push(plLearnLaplace(SkillB_t) ,1);
	//learnObjectList_t.push_back(&PL_SkillB_t);


	// Probability table of ZA_t, which depends on SkillA_t
	plProbValue tableZA_t_knowingSkillA_t[] = {
		1, 0,
		1-P1, P1
	};
	plCndDistribution P_ZA_t(ZA_t, SkillA_t, tableZA_t_knowingSkillA_t);
	plLearnDistributionTable PL_ZA_t(ZA_t, SkillA_t);
	//plProbTable PLZAT(ZA_t,frozen1_0);
	//plLearnFrozenDistribution PLZFAT(PLZAT);
	//PL_ZA_t.push(PLZAT,0); // <========================================== BUG
	PL_ZA_t.push(plBayesLearnProbTable(ZA_t, alphaVect),0); // <== before solving the "frozen10 bug", let's learn the column
	PL_ZA_t.push(plLearnLaplace(ZA_t) ,1);
	//learnObjectList_t.push_back(&PL_ZA_t);

	// Probability table of ZB_t, which depends on SkillB_t
	plProbValue tableZB_t_knowingSkillB_t[] = {
		1, 0,
		1-P2, P2
	};
	plCndDistribution P_ZB_t(ZB_t, SkillB_t, tableZB_t_knowingSkillB_t);
	plLearnDistributionTable PL_ZB_t(ZB_t, SkillB_t);
	//plProbTable PLZBT(ZB_t,frozen1_0);
	//plLearnFrozenDistribution PLZFBT(PLZBT);
	//PL_ZB_t.push(PLZBT,0); // <=========================================== BUG
	PL_ZB_t.push(plBayesLearnProbTable(ZB_t,alphaVect),0); // <== before solving the "frozen10 bug", let's learn the column
	PL_ZB_t.push(plLearnLaplace(ZB_t) ,1);
	//learnObjectList_t.push_back(&PL_ZB_t);


  const plExternalFunctionFromC max_fun4(PRC_t, ZA_t^ZB_t, my_code,"my_max");
	const plCndDeterministic P_PRC_tgivenZA_tZB_t(PRC_t, ZA_t^ZB_t, max_fun4);
	//learnObjectList_t.push_back(new plLearnFrozenDistribution(P_PRC_tgivenZA_tZB_t));

	// Probability table of ZPRC_t, which depends on PRC_t
	plProbValue tableZPRC_t_knowingPRC_t[] = {
		1, 0,
		1-P_LR, P_LR
	};
	plCndDistribution P_ZPRC_t(ZPRC_t, PRC_t, tableZPRC_t_knowingPRC_t);
	plLearnDistributionTable PL_ZPRC_t(ZPRC_t, PRC_t);
	//plProbTable PLZPRCT(ZPRC_t,frozen1_0);
	//plLearnFrozenDistribution PLZFPRCT(PLZPRCT);
	//PL_ZPRC_t.push(PLZPRCT,0); // <======================================== BUG
	PL_ZPRC_t.push(plBayesLearnProbTable(ZPRC_t,alphaVect),0); // <== before solving the "frozen10 bug", let's learn the column
	PL_ZPRC_t.push(plLearnLaplace(ZPRC_t) ,1);
	//learnObjectList_t.push_back(&PL_ZPRC_t);


	// Probability table of ZSRC_t, which depends on SkillC
	plProbValue tableZSRC_t_knowingSkillC[] = {
		1-P_L2, P_L2,
		0, 1
	};
	plCndDistribution P_ZSRC_t(ZSRC_t, SkillC, tableZSRC_t_knowingSkillC);
	plLearnDistributionTable PL_ZSRC_t(ZSRC_t, SkillC);
	//plProbTable PLZSRCT(ZSRC_t,frozen1_0);
	//plLearnFrozenDistribution PLZFSRCT(PLZSRCT);
	//PL_ZSRC_t.push(PLZSRCT,0); // <========================================= BUG
	PL_ZSRC_t.push(plLearnLaplace(ZSRC_t) ,0);
	PL_ZSRC_t.push(plBayesLearnProbTable(ZSRC_t,alphaVect),1); // <== before solving the "frozen10 bug", let's learn the column
	//learnObjectList_t.push_back(&PL_ZSRC_t);


  const plExternalFunctionFromC max_fun5(SkillC_t, ZPRC_t^ZSRC_t, my_code,"my_max");
	const plCndDeterministic P_SkillC_tgivenZPRC_tZSRC_t(SkillC_t, ZPRC_t^ZSRC_t, max_fun5);
	//learnObjectList_t.push_back(new plLearnFrozenDistribution(P_SkillC_tgivenZPRC_tZSRC_t));

	// Probability table of ResA_t, which depends on SkillA_t
	plProbValue tableResA_t_knowingSkillA_t[] = {
		1-PA_GUESS, PA_GUESS,
		PA_SLIP, 1-PA_SLIP
	};
	plProbValue tableResArand_t_knowingSkillA_t[] = {
		.8, .2,
		.1, .9
	};
	plCndDistribution P_ResA_t(ResA_t, SkillA_t, tableResA_t_knowingSkillA_t);
	plCndDistribution Prand_ResA_t(ResA_t, SkillA_t, tableResArand_t_knowingSkillA_t);
	plLearnDistributionTable PL_ResA_t(ResA_t, SkillA_t);
	PL_ResA_t.push(plLearnLaplace(ResA_t) ,0);
	PL_ResA_t.push(plLearnLaplace(ResA_t) ,1);
	learnObjectList_t.push_back(&PL_ResA_t);

	// Probability table of ResB_t, which depends on SkillB_t
	plProbValue tableResB_t_knowingSkillB_t[] = {
		1-PB_GUESS, PB_GUESS,
		PB_SLIP, 1-PB_SLIP
	};
	plCndDistribution P_ResB_t(ResB_t, SkillB_t, tableResB_t_knowingSkillB_t);
	plLearnDistributionTable PL_ResB_t(ResB_t, SkillB_t);
	PL_ResB_t.push(plLearnLaplace(ResB_t) ,0);
	PL_ResB_t.push(plLearnLaplace(ResB_t) ,1);
	//learnObjectList_t.push_back(&PL_ResB_t);

	// Probability table of ResC_t, which depends on SkillC_t
	plProbValue tableResC_t_knowingSkillC_t[] = {
		1-PC_GUESS, PC_GUESS,
		PC_SLIP, 1-PC_SLIP
	};
	plCndDistribution P_ResC_t(ResC_t, SkillC_t, tableResC_t_knowingSkillC_t);
	plLearnDistributionTable PL_ResC_t(ResC_t, SkillC_t);
	PL_ResC_t.push(plLearnLaplace(ResC_t) ,0);
	PL_ResC_t.push(plLearnLaplace(ResC_t) ,1);
	//learnObjectList_t.push_back(&PL_ResC_t);

	// Joint distribution -- manually defined
	plJointDistribution jd_t(	SkillA^SkillB^SkillC^ResA^ResB^ResC^ZA^ZB^PRC^ZPRC^ZSRC^SkillA_t^SkillB_t^SkillC_t^ResA_t^ResB_t^ResC_t^ZA_t^ZB_t^PRC_t^ZPRC_t^ZSRC_t, 
														P_SkillA*P_SkillB*P_ResA*P_ResB*P_ResC*P_ZA*P_ZB*P_PRC*P_ZPRC*P_ZSRC*P_SkillC*P_SkillA_t*P_SkillB_t*P_ResA_t*P_ResB_t*P_ResC_t*P_ZA_t*P_ZB_t*P_PRC_tgivenZA_tZB_t*P_ZPRC_t*P_ZSRC_t*P_SkillC_tgivenZPRC_tZSRC_t);
    
	// Creating transition bayesian network
	pmBayesianNetwork Gt(jd_t);

	// Print my transition bayesian network
	cout << endl << "========== MANUAL TRANSITION BN ==========" << endl;
	Gt.summary();
	//Gt.print_graph();

	/*
	========== MY MANUAL DYNAMIC BAYESIAN NETWORK ==========
	*/

	// Creating my dynamic bayesian network
	pmDynamicBayesianNetwork* pDBN_HMM = new pmDynamicBayesianNetwork(&G0, &Gt, 1);

	// Generating from my DBN
	// pDBN_HMM->generate_csv_data("plEMLearner_1000*10", 1000, 10);

	// split("plEMLearner_1000*10.csv");
	// rewrite("plEMLearner_1000*10_t.csv");

	/*
	========== LEARNING MY INITIAL BAYESIAN NETWORK FROM THE FORMER JOINT DISTRIBUTION ==========
	*/
	
	// Joint distribution -- learnt from data
 	char* incomplete_data_file_t0 = "test_4_t0_corrected.csv";
	plCSVFileDataDescriptor data_descriptor_t0(incomplete_data_file_t0, jd_t0.get_variables()) ;
	
	plEMLearner myLearnObject_t0(learnObjectList_t0);
	myLearnObject_t0.set_distributions_display(true);

	// Running
	cout << endl << "========== LEARNING INITIAL BN PARAMETERS ==========" << endl;
	myLearnObject_t0.run(data_descriptor_t0, 0.0001);
	
	// My data
	unsigned int nparams_t0 = myLearnObject_t0.get_n_parameters();
	plFloat llk_t0 = myLearnObject_t0.get_last_computed_loglikelihood();
	//plFloat bic_t0 = llk_t0 - 0.5*nparams_t0*std::log(data_descriptor_t0.get_n_records());

	const plJointDistribution& jdLearnt_t0 = myLearnObject_t0.get_joint_distribution();


	// Creation of the bayesian network
	pmBayesianNetwork GL0(jdLearnt_t0);

	cout << endl << "========== MY FIRST BAYESIAN NETWORK LEARNT SUMMARY ==========" << endl;
	GL0.summary();
	

	/*
	========== LEARNING MY TRANSITION BAYESIAN NETWORK FROM THE FORMER JOINT DISTRIBUTION ==========
	*/


	// Joint distribution -- learnt from data
 	
	// test PlHMM learning
	// std::string incomplete_data_file_t = "plHMM.csv";
	// ObservationSequence_t observationSequence_T;
	// ObservationSequenceArray_t observation_sequences;
	//observation_sequences.push_back(observationSequence_T);

	// //Creating the vector with @
	// //vectWrite(incomplete_data_file_t, observationSequence_T);
	// vectWriteBis(incomplete_data_file_t, observation_sequences);
	// // displayVectBis(observation_sequences);
	// cout << "Number of sequences: " << observation_sequences.size() << endl;
	// cout << "Number of values per sequence: " << observation_sequences[0].size() << endl;


	// // plHMM* pHMM = new plHMM(P_SkillA, P_SkillA_t, P_ResA);
	// plHMM* pHMM = new plHMM();
	// plVariablesConjunction vc = pHMM->observation_variables();
	// cout << vc << endl;
  // // plJointDistribution hd = pHMM->get_joint_distribution();
	// // cout << hd << endl;
  // pHMM->train_unsupervised_viterbi(observation_sequences, &PL_ResA);
	// cout << vc << endl;
	// cout << "debug" << endl;


	// // Another try
	// vector< string > vecCSV;
	// vecCSV.push_back("test_4_resA_corrected.csv");
	// vector<unsigned int> vecC;
	// plObservationSequenceCSVParameter obsSeqCSV(vecCSV, true, 0, vecC, ResA);
	// plObservationSequenceGeneratorFromCSV obsGenerator(obsSeqCSV);

	// plHMM* pHMM = new plHMM(P_SkillA, P_SkillA_t, P_ResA);
	// plVariablesConjunction vc = pHMM->observation_variables();
	// cout << vc << endl;
	// pHMM->train_unsupervised_viterbi(obsGenerator, &PL_ResA);
	

	// My file to read
	char* incomplete_data_file_t = "plEMLearner_1000*10_t_corrected_ResA.csv";

	//plCSVFileDataDescriptor data_descriptor_t(incomplete_data_file_t, SkillA^SkillB^SkillC^ResA^ResB^ResC^ZA^ZB^PRC^ZPRC^ZSRC^SkillA_t^SkillB_t^SkillC_t^ResA_t^ResB_t^ResC_t^ZA_t^ZB_t^PRC_t^ZPRC_t^ZSRC_t) ;
	plCSVFileDataDescriptor data_descriptor_t(incomplete_data_file_t, SkillA^ResA^SkillA_t^ResA_t);
	plEMLearner myLearnObject_t(Prand_SkillA*Prand_ResA*Prand_SkillA_t*Prand_ResA_t, learnObjectList_t);
	// myLearnObject_t.set_distributions_display(true);
	// myLearnObject_t.set_trace_loglikelihood (false);
	// myLearnObject_t.set_same_missing_variables(true);
	// myLearnObject_t.set_trace_observation_and_inference_info(false);
	// myLearnObject_t.set_distributions_display_stream(std::cout);
	// myLearnObject_t.use_junction_tree(false);


	// Use the run method to run EM algorithm
	cout << endl << "========== LEARNING TRANSITION BN PARAMETERS ==========" << endl;
	double convergence_lld = 0.000001;
	unsigned int max_nbiter = 1000;


	// int nbiter = myLearnObject_t.run(data_descriptor_t, convergence_lld, max_nbiter);
	// cout << nbiter << endl;

	// decompose the run method, add a restraint to control the second and the fourth distribution to be the same, then iterate until converge
	std::vector<plFloat> llk;
	unsigned int n = 0;

	// run EM algorithm for only one iteration and return the distributions after one iteration
	myLearnObject_t.iteration(data_descriptor_t);
	const plComputableObject PRSA = myLearnObject_t.get_distribution(1);
	const plComputableObject PRSA_t = myLearnObject_t.get_distribution(3);
	
	// create vectors to save the values of the second and the fourth distributions
	vector<plProbValue> pVSA;
	vector<plProbValue> pVSA_t;
	vector<plProbValue> pVR;
	PRSA.tabulate(pVSA);
	PRSA_t.tabulate(pVSA_t);

	// create a vector to save the mean value of two vectors before by using a loop
	unsigned int i;
	unsigned int n_provalues = 4;
	for (i = 0; i < n_provalues; i++)
	{	
		pVR.push_back((pVSA[i] + pVSA_t[i]) / 2);
		//cout << pVR.back() << endl;
	}

	// use the new vector to create the new plDistributionTable
	plProbValue tableResA_knowingSkillAc[] = {
	pVR[0], pVR[1],
	pVR[2], pVR[3]
	};
	plDistributionTable P_ResAc(PRSA.get_left_variables(), PRSA.get_right_variables(), tableResA_knowingSkillAc);
	plDistributionTable P_ResAc_t(PRSA_t.get_left_variables(), PRSA_t.get_right_variables(), tableResA_knowingSkillAc);

	// reset the second and the fourth distribution of myLearnObject_t
	myLearnObject_t.set_distribution(1, P_ResAc);
	myLearnObject_t.set_distribution(3, P_ResAc_t);

	double llk0_ = myLearnObject_t.compute_loglikelihood(data_descriptor_t);
	myLearnObject_t.distribution_set_frozen (1, true);
	myLearnObject_t.distribution_set_frozen (3, true);
	myLearnObject_t.iteration(data_descriptor_t);
	myLearnObject_t.distribution_set_frozen (1, false);
	myLearnObject_t.distribution_set_frozen (3, false);

	double llk_ = myLearnObject_t.compute_loglikelihood(data_descriptor_t);

	// compute the loglikelihood after one iteration
	llk.push_back(llk_);

	bool stop_crit = false;
	while (!stop_crit)
	{
		n++;
		// frozen the CPDs except two learning Objects P(SkillA_t|SkillA) and P(ResA_t|SkillA_t)
		myLearnObject_t.distribution_set_frozen (0, true);
		myLearnObject_t.distribution_set_frozen (1, true);

		// run EM algorithm for only one iteration and return the distributions after one iteration
		myLearnObject_t.iteration(data_descriptor_t);

		// query P(SkillA_t) and use it to update P(SkillA)
		const plComputableObject PSA = myLearnObject_t.get_distribution(0);
		const plComputableObject PSA_t = myLearnObject_t.get_distribution(2);
		vector<plProbValue> pSA;
		vector<plProbValue> pSA_t;
		PSA.tabulate(pSA);
		PSA_t.tabulate(pSA_t);
    pSA.push_back(pSA_t[0]);
		pSA.push_back(pSA_t[1]);

		// get the second the fourth distribution seprately
		const plComputableObject PRSA = myLearnObject_t.get_distribution(1);
		const plComputableObject PRSA_t = myLearnObject_t.get_distribution(3);

		// create vectors to save the values of the second and the fourth distributions
		vector<plProbValue> pVSA;
		vector<plProbValue> pVSA_t;
		vector<plProbValue> pVR;
		PRSA.tabulate(pVSA);
		PRSA_t.tabulate(pVSA_t);

		// extract P(ResA_t|SkillA_t) and use it to update P(ResA|SkillA)
		unsigned int i;
		unsigned int n_provalues = 4;
		for (i = 0; i < n_provalues; i++)
		{	
			pVSA.push_back(pVSA_t[i]);
		}

		// create a vector to save the mean value of two vectors before by using a loop
		for (i = 0; i < n_provalues; i++)
		{	
			pVR.push_back((pVSA[i] + pVSA_t[i]) / 2);
		}

		// use the new vector to create the new plDistributionTable
		plProbValue tableResA_knowingSkillAb[] = {
		pVR[0], pVR[1],
		pVR[2], pVR[3]
		};
		plDistributionTable P_ResAb(PRSA.get_left_variables(), PRSA.get_right_variables(), tableResA_knowingSkillAb);
		plDistributionTable P_ResAb_t(PRSA_t.get_left_variables(), PRSA_t.get_right_variables(), tableResA_knowingSkillAb);

		// reset the second and the fourth distribution of myLearnObject_t
		myLearnObject_t.set_distribution(1, P_ResAb);
		myLearnObject_t.set_distribution(3, P_ResAb_t);

		// compute the loglikelihood
		myLearnObject_t.distribution_set_frozen (1, true) ;
		myLearnObject_t.distribution_set_frozen (3, true);
		myLearnObject_t.iteration(data_descriptor_t);
		myLearnObject_t.distribution_set_frozen (1, false) ;
		myLearnObject_t.distribution_set_frozen (3, false);

		llk_ = myLearnObject_t.compute_loglikelihood(data_descriptor_t);
		//cout << n << llk0_ << llk_ << endl;

		stop_crit = ( abs(llk_ - llk.back()) /abs(llk_ + llk.back()/2) < convergence_lld) || (n== max_nbiter);
		llk.push_back(llk_);



		cout << n << "\t" << llk_ << endl;
		//return the number of iteration 
		//cout << "The number of iteration:" << n << endl;
		//cout << "The value of loglikelihood:" << llk_ << endl;

	}
	
	const plJointDistribution jdLearnt_t = myLearnObject_t.get_joint_distribution();
	// Creation of the bayesian network
	pmBayesianNetwork GLt(jdLearnt_t);

	cout << endl << "========== MY SECOND BAYESIAN NETWORK LEARNT SUMMARY ==========" << endl;
	GLt.summary();



	/*
	========== MY LEARNT DYNAMIC BAYESIAN NETWORK ==========
	*/

	// //Creation of the dynamic bayesian network
	// pmDynamicBayesianNetwork* pDBN_HMM_learnt = new pmDynamicBayesianNetwork(&GL0, &GLt, 1);
	// pm15dBN *DB15 = new pm15dBN(&HMM);
	// DB15->summary();

	// //Query the DynamicBayesianNetwork

	// plVariablesConjunction varconj = HMM.getVariablesforGtransition();

	// // construct the evidence    

	// plVariable     sk0 = varconj.get_variable_with_name("SkillA_t");
	// plVariable     sk1 = varconj.get_variable_with_name("SkillB_t");
	// plVariable     sk2 = varconj.get_variable_with_name("SkillC_t");
	// plVariable     prc  = varconj.get_variable_with_name("PRC_t");
	// plVariable     zprc  = varconj.get_variable_with_name("ZPRC_t");
	// plVariable     zsrc  = varconj.get_variable_with_name("ZSRC_t");
	// plVariable     res0 = varconj.get_variable_with_name("ResA_t");
	// plVariable     res1 = varconj.get_variable_with_name("ResB_t");
	// plVariable     res2 = varconj.get_variable_with_name("ResC_t");    
   
	// plValues evidence0(sk0);  
	// plValues evidence1(sk0);   
	// plValues evidence2(sk1);   
	// plValues evidence3(sk1);
	// plValues evidence4(sk2);
	// plValues evidence5(sk2);
	// plValues evidence6(prc);
	// plValues evidence7(prc);
	// plValues evidence8(res0);
	// plValues evidence9(res0);
	// plValues evidence10(res1);
	// plValues evidence11(res1);
	// plValues evidence12(zsrc);
	// plValues evidence13(zsrc);
	// plValues evidence14(res2);
	// plValues evidence15(res2);  
  
	// evidence0[sk0] = "no";
	// evidence1[sk0] = "yes";
	// evidence2[sk1] = "no";
	// evidence3[sk1] = "yes";
	// evidence4[sk2] = "no";
	// evidence5[sk2] = "yes";
	// evidence6[prc] = "no";
	// evidence7[prc] = "yes";
	// evidence8[res0] = "failed";
	// evidence9[res0] = "passed";
	// evidence10[res1] = "failed";
	// evidence11[res1] = "passed";
	// evidence12[zsrc] = "no";
	// evidence13[zsrc] = "yes";
	// evidence14[res2] = "failed";
	// evidence15[res2] = "passed";    

	// vector<plValues> evidence_globale = vector<plValues>();
	// vector<int> instants = vector<int>();
	// vector<plVariablesConjunction> targets = vector<plVariablesConjunction>(); 
	   
	// // add the evidence

	// evidence_globale.push_back(evidence9^evidence11);
	// //evidence_globale.push_back(evidence10);
	// //evidence_globale.push_back(evidence1^evidence3);

	
  // // add the instants and targets
	// instants.push_back(1); targets.push_back(sk2);
	// //instants.push_back(2); targets.push_back(sk2);
	// //instants.push_back(2); targets.push_back(prc^zprc^zsrc^sk2);
	// //instants.push_back(3); targets.push_back(prc^zprc^zsrc^sk2);
	// //instants.push_back(4); targets.push_back(prc^zprc^zsrc^sk2);

	// float temps_initial = clock();
	// vector<plComputableObjectList> result_query = HMM.prediction(targets, instants, evidence_globale);
	
	// //DB15->set_model_method(1);
 	// //vector<plComputableObjectList> result_query = DB15->predict(targets, instants, evidence_globale);

  // //print the query result
	// for (int i=0; i<instants.size(); i++) {
	// std::cout << result_query[i] << endl;
	// }
	// float temps_final = clock();
}
