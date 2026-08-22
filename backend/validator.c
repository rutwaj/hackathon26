#include "validator.h"
#include <stddef.h>

/*
 * Validate the financial profile extracted by the parser.
 *
 * The validator checks:
 *
 * 1. Missing required information
 * 2. Salary consistency
 * 3. Loan consistency
 * 4. EMI consistency
 * 5. Suspiciously high EMI-to-income ratio
 *
 * Results are stored inside FinancialProfile so that
 * the scoring module can use them later.
 */

void validate_profile(FinancialProfile *profile)
{
    /* Safety check */
    if (profile == NULL)
    {
        return;
    }


    /*
     * Reset validation results.
     *
     * These values represent the number of problems
     * detected in each category.
     */

    profile->missing_fields = 0;
    profile->inconsistent_fields = 0;
    profile->suspicious_fields = 0;


    /* =====================================================
       1. CHECK FOR MISSING INFORMATION
       ===================================================== */


    if (!profile->name_found ||
        profile->name[0] == '\0')
    {
        profile->missing_fields++;
    }


    if (!profile->net_salary_found ||
        profile->net_salary <= 0)
    {
        profile->missing_fields++;
    }


    if (!profile->bank_salary_found ||
        profile->bank_salary <= 0)
    {
        profile->missing_fields++;
    }


    if (!profile->loan_amount_found ||
        profile->loan_amount <= 0)
    {
        profile->missing_fields++;
    }


    if (!profile->outstanding_loan_found ||
        profile->outstanding_loan < 0)
    {
        profile->missing_fields++;
    }


    if (!profile->emi_found ||
        profile->emi <= 0)
    {
        profile->missing_fields++;
    }


    /* =====================================================
       2. CHECK SALARY CONSISTENCY
       ===================================================== */

    /*
     * Compare declared net salary with the salary
     * credited to the bank.
     *
     * A small difference is normal.
     *
     * Difference > 10%  → inconsistent
     * Difference > 20%  → suspicious
     */

    if (profile->net_salary > 0 &&
        profile->bank_salary > 0)
    {
        double difference;
        double percentage_difference;


        difference =
            profile->net_salary -
            profile->bank_salary;


        if (difference < 0)
        {
            difference = -difference;
        }


        percentage_difference =
            (difference / profile->net_salary) * 100.0;


        if (percentage_difference > 20.0)
        {
            profile->suspicious_fields++;
        }
        else if (percentage_difference > 10.0)
        {
            profile->inconsistent_fields++;
        }
    }


    /* =====================================================
       3. CHECK LOAN CONSISTENCY
       ===================================================== */

    /*
     * Outstanding loan should never be greater than
     * the original loan amount.
     */

    if (profile->loan_amount > 0 &&
        profile->outstanding_loan >= 0)
    {
        if (profile->outstanding_loan >
            profile->loan_amount)
        {
            profile->inconsistent_fields++;
        }
    }


    /* =====================================================
       4. CHECK EMI CONSISTENCY
       ===================================================== */

    /*
     * EMI should not be greater than the person's
     * monthly net salary.
     *
     * If EMI is greater than salary, this is highly
     * suspicious.
     */

    if (profile->net_salary > 0 &&
        profile->emi > 0)
    {
        if (profile->emi > profile->net_salary)
        {
            profile->suspicious_fields++;
        }
    }


    /* =====================================================
       5. CHECK EMI-TO-INCOME RATIO
       ===================================================== */

    /*
     * Calculate EMI as a percentage of net salary.
     *
     * This is also the DTI calculation used by
     * Person 4's scoring module.
     *
     * > 70%  → suspicious
     * > 50%  → inconsistent
     */

    if (profile->net_salary > 0 &&
        profile->emi > 0)
    {
        double emi_ratio =
            (profile->emi / profile->net_salary) * 100.0;


        if (emi_ratio > 70.0)
        {
            profile->suspicious_fields++;
        }
        else if (emi_ratio > 50.0)
        {
            profile->inconsistent_fields++;
        }
    }
}