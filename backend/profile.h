#ifndef PROFILE_H
#define PROFILE_H

typedef struct {
    char name[100];
    char account_number[50];
    char employer[100];

    double monthly_income;
    double monthly_expenses;

    double loan_amount;
    double outstanding_loan;

    double insurance_amount;

    int missing_fields;
    int inconsistent_fields;
    int suspicious_fields;

    double reliability_score;
    double risk_score;

} FinancialProfile;

#endif