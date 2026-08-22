#ifndef PROFILE_H
#define PROFILE_H

#define MAX_NAME 100

typedef struct
{
    /* =====================================================
       FINANCIAL INFORMATION EXTRACTED BY PARSER
       ===================================================== */

    char name[MAX_NAME];

    double net_salary;
    double bank_salary;
    double loan_amount;
    double outstanding_loan;
    double emi;


    /* =====================================================
       PARSER STATUS FLAGS
       ===================================================== */

    int name_found;
    int net_salary_found;
    int bank_salary_found;
    int loan_amount_found;
    int outstanding_loan_found;
    int emi_found;


    /* =====================================================
       VALIDATION RESULTS
       ===================================================== */

    int missing_fields;
    int inconsistent_fields;
    int suspicious_fields;


    /* =====================================================
       SCORING RESULTS
       ===================================================== */

    double dti;

    double income_score;
    double debt_score;
    double completeness_score;
    double identity_score;

    double trust_score;

    char risk_level[20];

} FinancialProfile;

#endif
