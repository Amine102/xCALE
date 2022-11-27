#include <iostream>
#include <vector>
void display_tableResS_knowingS_repart_coef(std::vector<std::vector<double> > repart_coef)
{
	for(int i=0; i<repart_coef.size(); i++)
    {
        for(int j=0; j<repart_coef.size(); j++)
			std:: cout << repart_coef[i][j] << " ";
		std::cout << std::endl << std::endl;
    }
		
}

bool repartCoef_isConsistant(std::vector<std::vector<double>> tableResS_knowingS_repart_coef)	// Checks if the sum of all repartition coefficient is equal to 1
{
	double PGuess_coef_sum, PSlip_coef_sum = 0;
	for(int i=0; i<tableResS_knowingS_repart_coef.size(); i++)
	{
		for(int j=0; i<tableResS_knowingS_repart_coef.size(); j++)
		{
			if(i>j)
				PGuess_coef_sum += tableResS_knowingS_repart_coef[j][i]; 
			else if(j>i)
				PSlip_coef_sum  += tableResS_knowingS_repart_coef[j][i];
		}
		if(PGuess_coef_sum!=1)
		{
			std::cerr << "CONSISTENCY ERROR : repartition coefficients of PGuess in column " << i << "should sum to 1 (sum=" << PGuess_coef_sum << ").";
			return false;
		}
        else if(PSlip_coef_sum!=1)
        {
            std::cerr << "CONSISTENCY ERROR : repartition coefficients of PSlip in column " << i << "should sum to 1 (sum=" << PGuess_coef_sum << ").";
        }
        else
        {
            PGuess_coef_sum = 0;
            PSlip_coef_sum  = 0;
        }
	}
}

int main(void)
{
	std::vector<std::vector<double> > repart_coef = {
													{1, 2, 3},
													{4, 5, 6},
													{7, 8, 9}};
	//display_tableResS_knowingS_repart_coef(repart_coef);
    repartCoef_isConsistant(repart_coef);
	return 0;
}