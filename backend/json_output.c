#include <stdio.h>
#include "json_output.h"

void generate_json(FinancialProfile *profile)
{
    FILE *file = fopen("backend/result.json", "w");

    if (file == NULL)
    {
        printf("ERROR: Could not create backend/result.json\n");
        return;
    }

    fprintf(file, "{\n");

    fprintf(file, "    \"profile\": {\n");
    fprintf(file, "        \"name\": \"%s\",\n", profile->name);
    fprintf(file, "        \"net_salary\": %.2f,\n", profile->net_salary);
    fprintf(file, "        \"bank_salary\": %.2f,\n", profile->bank_salary);
    fprintf(file, "        \"loan_amount\": %.2f,\n", profile->loan_amount);
    fprintf(file, "        \"loan_outstanding\": %.2f,\n", profile->outstanding_loan);
    fprintf(file, "        \"emi\": %.2f\n", profile->emi);
    fprintf(file, "    },\n");

    fprintf(file, "    \"validation\": {\n");
    fprintf(file, "        \"missing_fields\": %d,\n", profile->missing_fields);
    fprintf(file, "        \"inconsistent_fields\": %d,\n", profile->inconsistent_fields);
    fprintf(file, "        \"suspicious_fields\": %d\n", profile->suspicious_fields);
    fprintf(file, "    },\n");

    fprintf(file, "    \"scoring\": {\n");
    fprintf(file, "        \"dti\": %.2f,\n", profile->dti);
    fprintf(file, "        \"income_score\": %.2f,\n", profile->income_score);
    fprintf(file, "        \"debt_score\": %.2f,\n", profile->debt_score);
    fprintf(file, "        \"completeness_score\": %.2f,\n", profile->completeness_score);
    fprintf(file, "        \"identity_score\": %.2f,\n", profile->identity_score);
    fprintf(file, "        \"trust_score\": %.2f,\n", profile->trust_score);
    fprintf(file, "        \"risk_level\": \"%s\"\n", profile->risk_level);
    fprintf(file, "    }\n");

    fprintf(file, "}\n");

    fclose(file);

    printf("backend/result.json generated successfully.\n");
}
