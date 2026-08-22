#ifndef SCORING_H
#define SCORING_H

#include "profile.h"

double calculate_risk_score(FinancialProfile *profile);
double calculate_reliability_score(FinancialProfile *profile);

#endif