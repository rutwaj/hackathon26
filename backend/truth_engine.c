#include <stdio.h>
#include <string.h>
#include <math.h>

#include "truth_engine.h"
//detect income mismatch
int detect_income_mismatch(
    FinancialProfile *profile,
    Finding *finding)
{
    if (profile->net_salary <= 0 ||
        profile->bank_salary <= 0) {
        return 0;
    }

    double difference =
        fabs(profile->net_salary -
             profile->bank_salary);

    double percentage =
        (difference /
         profile->net_salary) * 100.0;

    if (percentage <= 10.0) {
        return 0;
    }

    strcpy(finding->type,
           "INCOME_MISMATCH");

    strcpy(finding->severity,
           "HIGH");

    finding->confidence =
        percentage > 30 ? 96.0 : 90.0;

    finding->value1 =
        profile->net_salary;

    finding->value2 =
        profile->bank_salary;

    strcpy(finding->source1,
           "Salary Slip");

    strcpy(finding->source2,
           "Bank Statement");

    snprintf(
        finding->explanation,
        sizeof(finding->explanation),
        "Declared salary of %.2f differs "
        "from bank salary credit of %.2f "
        "by %.2f%%.",
        profile->net_salary,
        profile->bank_salary,
        percentage
    );

    return 1;
}
// detect high debt
int detect_high_debt(
    FinancialProfile *profile,
    Finding *finding)
{
    double dti = calculate_dti(profile);

    if (dti <= 35.0) {
        return 0;
    }

    strcpy(finding->type,
           "HIGH_DEBT_RATIO");

    strcpy(finding->severity,
           dti > 50.0 ? "CRITICAL" : "MEDIUM");

    finding->confidence =
        dti > 50.0 ? 95.0 : 85.0;

    finding->value1 = dti;

    strcpy(finding->source1,
           "Loan Statement");

    strcpy(finding->source2,
           "Salary Slip");

    snprintf(
        finding->explanation,
        sizeof(finding->explanation),
        "Monthly EMI represents %.2f%% "
        "of declared monthly income.",
        dti
    );

    return 1;
}
// generate findings
int generate_findings(
    FinancialProfile *profile,
    Finding findings[],
    int max_findings)
{
    int count = 0;

    if (count < max_findings) {
        if (detect_income_mismatch(
                profile,
                &findings[count])) {

            count++;
        }
    }

    if (count < max_findings) {
        if (detect_high_debt(
                profile,
                &findings[count])) {

            count++;
        }
    }

    return count;
}
//person 1 need 
'Finding findings[10];

int finding_count =
    generate_findings(
        &profile,
        findings,
        10
    );'

    