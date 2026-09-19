#include <stdio.h>
#include <string.h>
#include <math.h>

#include "scoring.h"


/*
 * Calculate Debt-to-Income ratio.
 *
 * DTI = EMI / Net Salary × 100
 */
double calculate_dti(FinancialProfile *profile)
{
    if (profile->net_salary <= 0)
    {
        return 100.0;
    }

    return (profile->emi / profile->net_salary) * 100.0;
}


/*
 * Calculate income consistency score.
 *
 * Compares the salary mentioned in the financial
 * document with the salary credited to the bank.
 */
double calculate_income_score(FinancialProfile *profile)
{
    if (profile->net_salary <= 0 ||
        profile->bank_salary <= 0)
    {
        return 0.0;
    }

    double difference =
        fabs(
            profile->net_salary -
            profile->bank_salary
        );

    double percentage =
        (difference / profile->net_salary) * 100.0;


    if (percentage <= 5.0)
    {
        return 100.0;
    }

    if (percentage <= 10.0)
    {
        return 85.0;
    }

    if (percentage <= 20.0)
    {
        return 60.0;
    }

    if (percentage <= 30.0)
    {
        return 35.0;
    }

    return 10.0;
}


/*
 * Calculate debt score based on DTI.
 */
double calculate_debt_score(FinancialProfile *profile)
{
    double dti =
        calculate_dti(profile);


    if (dti <= 20.0)
    {
        return 100.0;
    }

    if (dti <= 35.0)
    {
        return 80.0;
    }

    if (dti <= 50.0)
    {
        return 55.0;
    }

    if (dti <= 70.0)
    {
        return 30.0;
    }

    return 10.0;
}


/*
 * Calculate document completeness score.
 */
double calculate_completeness_score(FinancialProfile *profile)
{
    int total_fields = 6;
    int available = 0;


    if (strlen(profile->name) > 0)
        available++;

    if (profile->net_salary > 0)
        available++;

    if (profile->bank_salary > 0)
        available++;

    if (profile->loan_amount > 0)
        available++;

    if (profile->outstanding_loan > 0)
        available++;

    if (profile->emi > 0)
        available++;


    return ((double)available / total_fields) * 100.0;
}


/*
 * Calculate identity score.
 */
double calculate_identity_score(FinancialProfile *profile)
{
    if (strlen(profile->name) == 0)
    {
        return 0.0;
    }

    return 100.0;
}


/*
 * Calculate overall trust score.
 */
double calculate_trust_score(FinancialProfile *profile)
{
    double score =
        profile->income_score * 0.30 +
        profile->debt_score * 0.25 +
        profile->completeness_score * 0.20 +
        profile->identity_score * 0.25;


    return score;
}


/*
 * Determine overall risk level.
 */
void determine_risk_level(FinancialProfile *profile)
{
    if (profile->trust_score >= 85.0)
    {
        strcpy(
            profile->risk_level,
            "LOW"
        );
    }
    else if (profile->trust_score >= 70.0)
    {
        strcpy(
            profile->risk_level,
            "MODERATE"
        );
    }
    else if (profile->trust_score >= 50.0)
    {
        strcpy(
            profile->risk_level,
            "HIGH"
        );
    }
    else
    {
        strcpy(
            profile->risk_level,
            "CRITICAL"
        );
    }
}


/*
 * Main scoring function.
 *
 * Person 1 / main.c will call:
 *
 * run_financial_scoring(&profile);
 */
void run_financial_scoring(FinancialProfile *profile)
{
    profile->dti =
        calculate_dti(profile);

    profile->income_score =
        calculate_income_score(profile);

    profile->debt_score =
        calculate_debt_score(profile);

    profile->completeness_score =
        calculate_completeness_score(profile);

    profile->identity_score =
        calculate_identity_score(profile);

    profile->trust_score =
        calculate_trust_score(profile);

    determine_risk_level(profile);
}