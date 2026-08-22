#include <stdio.h>
#include <string.h>
#include <math.h>

/* =========================================================
   1. FINANCIAL PROFILE
   ========================================================= */

typedef struct {

    char name[100];

    double net_salary;
    double bank_salary;

    double loan_amount;
    double loan_outstanding;
    double emi;

    double income_score;
    double debt_score;
    double completeness_score;
    double identity_score;

    double dti;
    double trust_score;

    char risk_level[20];

} FinancialProfile;


/* =========================================================
   2. FINDING STRUCTURE
   ========================================================= */

typedef struct {

    int id;

    char type[50];
    char severity[20];

    double confidence;

    double value1;
    double value2;

    char source1[100];
    char source2[100];

    char explanation[500];

} Finding;


/* =========================================================
   3. CALCULATE DTI
   ========================================================= */

double calculate_dti(FinancialProfile *profile)
{
    /*
       DTI = EMI / Monthly Income × 100
    */

    if (profile->net_salary <= 0)
    {
        return 100.0;
    }

    return (profile->emi / profile->net_salary) * 100.0;
}


/* =========================================================
   4. CALCULATE INCOME CONSISTENCY SCORE
   ========================================================= */

double calculate_income_score(FinancialProfile *profile)
{
    if (profile->net_salary <= 0 ||
        profile->bank_salary <= 0)
    {
        return 0.0;
    }

    /* Difference between salary slip and bank */

    double difference =
        fabs(profile->net_salary -
             profile->bank_salary);

    /* Percentage difference */

    double percentage =
        (difference /
         profile->net_salary) * 100.0;


    /*
       Convert difference into a score.
    */

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


/* =========================================================
   5. CALCULATE DEBT SCORE
   ========================================================= */

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


/* =========================================================
   6. CALCULATE COMPLETENESS
   ========================================================= */

double calculate_completeness_score(
    FinancialProfile *profile)
{
    int total_fields = 6;

    int available = 0;


    if (strlen(profile->name) > 0)
    {
        available++;
    }

    if (profile->net_salary > 0)
    {
        available++;
    }

    if (profile->bank_salary > 0)
    {
        available++;
    }

    if (profile->loan_amount > 0)
    {
        available++;
    }

    if (profile->loan_outstanding > 0)
    {
        available++;
    }

    if (profile->emi > 0)
    {
        available++;
    }


    return ((double)available /
            total_fields) * 100.0;
}


/* =========================================================
   7. IDENTITY SCORE
   ========================================================= */

double calculate_identity_score(
    FinancialProfile *profile)
{
    if (strlen(profile->name) == 0)
    {
        return 0.0;
    }

    return 100.0;
}


/* =========================================================
   8. FINANCIAL TRUST SCORE
   ========================================================= */

double calculate_trust_score(
    FinancialProfile *profile)
{
    double score =
          profile->income_score * 0.30
        + profile->debt_score * 0.25
        + profile->completeness_score * 0.20
        + profile->identity_score * 0.25;

    return score;
}


/* =========================================================
   9. RISK LEVEL
   ========================================================= */

void determine_risk_level(
    FinancialProfile *profile)
{
    if (profile->trust_score >= 85.0)
    {
        strcpy(profile->risk_level, "LOW");
    }

    else if (profile->trust_score >= 70.0)
    {
        strcpy(profile->risk_level, "MODERATE");
    }

    else if (profile->trust_score >= 50.0)
    {
        strcpy(profile->risk_level, "HIGH");
    }

    else
    {
        strcpy(profile->risk_level, "CRITICAL");
    }
}


/* =========================================================
   10. RUN COMPLETE SCORING
   ========================================================= */

void run_financial_scoring(
    FinancialProfile *profile)
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


/* =========================================================
   11. DETECT INCOME MISMATCH
   ========================================================= */

int detect_income_mismatch(
    FinancialProfile *profile,
    Finding *finding)
{
    if (profile->net_salary <= 0 ||
        profile->bank_salary <= 0)
    {
        return 0;
    }


    double difference =
        fabs(profile->net_salary -
             profile->bank_salary);


    double percentage =
        (difference /
         profile->net_salary) * 100.0;


    /*
       We consider >10% a mismatch.
    */

    if (percentage <= 10.0)
    {
        return 0;
    }


    strcpy(
        finding->type,
        "INCOME_MISMATCH"
    );


    strcpy(
        finding->severity,
        percentage > 30.0
            ? "CRITICAL"
            : "HIGH"
    );


    /*
       Demo confidence value.
    */

    finding->confidence =
        percentage > 30.0
            ? 96.0
            : 90.0;


    finding->value1 =
        profile->net_salary;


    finding->value2 =
        profile->bank_salary;


    strcpy(
        finding->source1,
        "Salary Slip"
    );


    strcpy(
        finding->source2,
        "Bank Statement"
    );


    snprintf(
        finding->explanation,
        sizeof(finding->explanation),

        "Declared salary of Rs. %.2f "
        "differs from bank salary credit "
        "of Rs. %.2f by %.2f%%.",

        profile->net_salary,
        profile->bank_salary,
        percentage
    );


    return 1;
}


/* =========================================================
   12. DETECT HIGH DEBT
   ========================================================= */

int detect_high_debt(
    FinancialProfile *profile,
    Finding *finding)
{
    double dti =
        calculate_dti(profile);


    /*
       DTI above 35% gets flagged.
    */

    if (dti <= 35.0)
    {
        return 0;
    }


    strcpy(
        finding->type,
        "HIGH_DEBT_RATIO"
    );


    if (dti > 50.0)
    {
        strcpy(
            finding->severity,
            "CRITICAL"
        );

        finding->confidence = 95.0;
    }

    else
    {
        strcpy(
            finding->severity,
            "MEDIUM"
        );

        finding->confidence = 85.0;
    }


    finding->value1 = dti;

    finding->value2 = profile->emi;


    strcpy(
        finding->source1,
        "Loan Statement"
    );


    strcpy(
        finding->source2,
        "Salary Slip"
    );


    snprintf(
        finding->explanation,
        sizeof(finding->explanation),

        "Monthly EMI represents %.2f%% "
        "of declared monthly income.",

        dti
    );


    return 1;
}


/* =========================================================
   13. GENERATE ALL FINDINGS
   ========================================================= */

int generate_findings(
    FinancialProfile *profile,
    Finding findings[],
    int max_findings)
{
    int count = 0;


    /* Income mismatch */

    if (count < max_findings)
    {
        if (detect_income_mismatch(
                profile,
                &findings[count]))
        {
            findings[count].id =
                count + 1;

            count++;
        }
    }


    /* High debt */

    if (count < max_findings)
    {
        if (detect_high_debt(
                profile,
                &findings[count]))
        {
            findings[count].id =
                count + 1;

            count++;
        }
    }


    return count;
}


/* =========================================================
   14. PRINT PROFILE
   ========================================================= */

void print_profile(
    FinancialProfile *profile)
{
    printf("\n");
    printf("========================================\n");
    printf("        FINVERIFY ANALYSIS\n");
    printf("========================================\n");

    printf("Name                 : %s\n",
           profile->name);

    printf("Salary Slip Income   : Rs. %.2f\n",
           profile->net_salary);

    printf("Bank Salary Credit   : Rs. %.2f\n",
           profile->bank_salary);

    printf("Loan Amount          : Rs. %.2f\n",
           profile->loan_amount);

    printf("Loan Outstanding     : Rs. %.2f\n",
           profile->loan_outstanding);

    printf("Monthly EMI          : Rs. %.2f\n",
           profile->emi);

    printf("----------------------------------------\n");

    printf("Debt-to-Income Ratio : %.2f%%\n",
           profile->dti);

    printf("Income Score         : %.2f/100\n",
           profile->income_score);

    printf("Debt Score           : %.2f/100\n",
           profile->debt_score);

    printf("Completeness Score   : %.2f/100\n",
           profile->completeness_score);

    printf("Identity Score       : %.2f/100\n",
           profile->identity_score);

    printf("----------------------------------------\n");

    printf("FINANCIAL TRUST SCORE: %.2f/100\n",
           profile->trust_score);

    printf("RISK LEVEL           : %s\n",
           profile->risk_level);

    printf("========================================\n");
}


/* =========================================================
   15. PRINT FINDINGS
   ========================================================= */

void print_findings(
    Finding findings[],
    int count)
{
    printf("\n");
    printf("========================================\n");
    printf("             FINDINGS\n");
    printf("========================================\n");


    if (count == 0)
    {
        printf("No anomalies detected.\n");
        return;
    }


    for (int i = 0; i < count; i++)
    {
        printf("\n");
        printf("Finding #%d\n",
               findings[i].id);

        printf("----------------------------------------\n");

        printf("Type       : %s\n",
               findings[i].type);

        printf("Severity   : %s\n",
               findings[i].severity);

        printf("Confidence : %.2f%%\n",
               findings[i].confidence);

        printf("Source 1   : %s\n",
               findings[i].source1);

        printf("Value 1    : Rs. %.2f\n",
               findings[i].value1);

        printf("Source 2   : %s\n",
               findings[i].source2);

        printf("Value 2    : Rs. %.2f\n",
               findings[i].value2);

        printf("\nWHY?\n");

        printf("%s\n",
               findings[i].explanation);
    }

    printf("\n========================================\n");
}


/* =========================================================
   16. MAIN
   ========================================================= */

int main()
{
    /*
       Create a financial profile.
    */

    FinancialProfile profile = {0};


    /*
       Fill it with demo data.
    */

    strcpy(
        profile.name,
        "Rahul Sharma"
    );

    profile.net_salary = 68000;

    profile.bank_salary = 52000;

    profile.loan_amount = 1000000;

    profile.loan_outstanding = 720000;

    profile.emi = 23500;


    /*
       Run your financial intelligence.
    */

    run_financial_scoring(&profile);


    /*
       Display the profile.
    */

    print_profile(&profile);


    /*
       Generate findings.
    */

    Finding findings[10];


    int finding_count =
        generate_findings(
            &profile,
            findings,
            10
        );


    /*
       Display findings.
    */

    print_findings(
        findings,
        finding_count
    );


    return 0;
}