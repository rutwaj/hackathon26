#include "validator.h"
#include <stddef.h>

void validate_profile(FinancialProfile *profile)
{
    if (profile == NULL)
    {
        return;
    }

    profile->missing_fields = 0;
    profile->inconsistent_fields = 0;
    profile->suspicious_fields = 0;

    /* Check required information */

    if (profile->name[0] == '\0')
    {
        profile->missing_fields++;
    }

    if (profile->monthly_income <= 0)
    {
        profile->missing_fields++;
    }

    /* Check expenses */

    if (profile->monthly_income > 0)
    {
        double expense_ratio =
            (profile->monthly_expenses / profile->monthly_income) * 100.0;

        if (expense_ratio > 80.0)
        {
            profile->suspicious_fields++;
        }
        else if (expense_ratio > 60.0)
        {
            profile->inconsistent_fields++;
        }
    }

    /* Check loan consistency */

    if (profile->loan_amount > 0 &&
        profile->outstanding_loan > profile->loan_amount)
    {
        profile->inconsistent_fields++;
    }
}
