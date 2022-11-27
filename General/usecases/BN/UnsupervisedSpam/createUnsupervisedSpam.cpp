//===================== Unsupervised Spam : is a usecase to predict a classe (Spam) that is discret and latent =====================//
// The usecase goal is to compute unsupervised learning of the bayes naif structured bayesian network //
//===================================================================================================================================//


#include "createUnsupervisedSpam.h"
using namespace std;
using namespace PILGRIM;


void createUnsupervisedSpam() {

   
  int ncSpam= 2;
  string file_name = "../../benchmarks/data/UnsupervisedSpam_csv.data"; //the file that we will create
  int ndata =1000;
  vector<string> adr_label = vector<string>();
  adr_label.push_back("Seul");
  adr_label.push_back("UnParmiN");
  adr_label.push_back("Non");
  vector<string> bin_label = vector<string>();
  bin_label.push_back("O");
  bin_label.push_back("N");

  const plVariable Spam("Spam", plIntegerType(0, ncSpam-1));
  plVariable Viagra("Viagra",  plLabelType(bin_label));
  plVariable Polytech("Polytech",  plLabelType(bin_label));
  plVariable Felicitation("Felicitation",  plLabelType(bin_label));
  plVariable Adresse("Adresse",plLabelType(adr_label));
  plVariable Longueur("Longueur",plRealType(0,100.0));
  plVariable Nbre_Image("Nbre_Image",plRealType(0,50.0));


  //// Probability table of Spam : Spam is the latent class variable
  plProbValue tableSpam[] = {0.2, 0.8};
  plProbTable P_Spam(Spam, tableSpam);

  //// Probability table of Adresse, which depends on Spam
  plProbValue tableAdresse_knowingSpam[] = {
    0.02, 0.20, 0.78, 0.78, 0.2, 0.02
  };
  plDistributionTable P_Adresse(Adresse, Spam, tableAdresse_knowingSpam);

  // probability table of Viagra, which depends on Spam
  plProbValue tableViagra_knowingSpam[] = {
    0.1, 0.9, 0.001, 0.999
  };
  plDistributionTable P_Viagra(Viagra, Spam, tableViagra_knowingSpam);

  // probability table of adresse, which depends on Spam
  plProbValue tablePolytech_knowingSpam[] = {
    0.01, 0.99, 0.3, 0.7
  };
  plDistributionTable P_Polytech(Polytech, Spam, tablePolytech_knowingSpam);

  // probability table of adresse, which depends on spam
  plProbValue tableFelicitation_knowingSpam[] = {
    0.3, 0.7, 0.01, 0.99
  };
  plDistributionTable P_Felicitation(Felicitation,
                                     Spam,
                                     tableFelicitation_knowingSpam);


   // Gaussians of table of adresse, which depends on spam

  plDistributionTable P_Longueur(Longueur,Spam);
  const plFloat mean_longueur[] = { 2.45, 15};   
  const plFloat sd_longueur[] = { 1.29, 7 };		
  for (unsigned int i = 0; i <ncSpam; ++i) {  
  	 P_Longueur.push(plNormal( Longueur, mean_longueur[i], sd_longueur[i]), int(i));
  	}

  plDistributionTable P_Nbre_Image(Nbre_Image,Spam);
  const plFloat mean_nbre_image[] = { 2.45, 15 };   
  const plFloat sd_nbre_image[] = { 1.29, 7 };		
  for (unsigned int i = 0; i <ncSpam; ++i) {   
  	 P_Nbre_Image.push(plNormal( Nbre_Image, mean_nbre_image[i], sd_nbre_image[i]), int(i));
  	}

  // Joint distribution
  plJointDistribution jd(Spam^Adresse^Viagra^Polytech^Felicitation^Longueur^Nbre_Image,
  			 P_Spam*P_Adresse*P_Viagra*P_Polytech*P_Felicitation*P_Longueur*P_Nbre_Image);

//========================== Here starts the dataset constructing ====================//
// this way of constructing the dataset shows us the difference between element creation continuous&discret
  plValues values(Spam^Adresse^Viagra^Polytech^Felicitation^Longueur^Nbre_Image);
  ofstream fichier(file_name, ios::out | ios::app);
  plVariablesConjunction variables = values.get_variables();
  fichier << "\"" << variables[1].name() << "\"";
  for (unsigned int j=2; j < variables.dim(); ++j)
    fichier  << ";\"" << variables[j].name() << "\"";
    fichier  << std::endl;
    for (unsigned int i=0; i < ndata; ++i) {
      jd.draw(values);
      if ( variables[1].get_var_type()== 1 )  {fichier << values[1]<<endl;}
	  else {
        fichier << variables[1].get_type().get_values_as_strings().at(values[1]);}
        for (unsigned int j = 2; j < variables.dim()-1; ++j) {
	      if ( variables[j].get_var_type()== 1 ) { 
            fichier << ";" <<values[j];}
		  else {
		    fichier << ";" << variables[j].get_type().get_values_as_strings().at(values[j]);}
        }
          if ( variables[variables.dim()-1].get_var_type()== 1 ) { 
            fichier << ";" <<values[variables.dim()-1]<<endl;}
		  else {
		       	fichier << ";" << variables[variables.dim()-1].get_type().get_values_as_strings().at(values[variables.dim()-1])<<endl;
           }
    }
}

