#ifndef PROFILE_H
#define PROFILE_H

#define MAX_NAME 100

typedef struct
{
    char name[MAX_NAME];

    double net_salary;
    double bank_salary;
    double loan_amount;
    double outstanding_loan;
    double emi;

    int name_found;
    int net_salary_found;
    int bank_salary_found;
    int loan_amount_found;
    int outstanding_loan_found;
    int emi_found;

} FinancialProfile;

#endif