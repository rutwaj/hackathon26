#ifndef SCORING_H
#define SCORING_H

#include "profile.h"


/* Calculates debt-to-income ratio */
double calculate_dti(FinancialProfile *profile);


/* Calculates income consistency */
double calculate_income_score(FinancialProfile *profile);


/* Calculates debt risk */
double calculate_debt_score(FinancialProfile *profile);


/* Calculates whether required information exists */
double calculate_completeness_score(FinancialProfile *profile);


/* Calculates identity score */
double calculate_identity_score(FinancialProfile *profile);


/* Combines all scores */
double calculate_trust_score(FinancialProfile *profile);


/* Determines overall risk */
void determine_risk_level(FinancialProfile *profile);


/* Runs the complete scoring system */
void run_financial_scoring(FinancialProfile *profile);

#endif