#include <stdio.h>
#include <string.h>

#include "profile.h"
#include "parser.h"
#include "validator.h"
#include "scoring.h"
#include "json_output.h"

int main()
{
    FinancialProfile profile;

    memset(&profile, 0, sizeof(FinancialProfile));

    printf("=====================================\n");
    printf(" Financial Document Analysis System\n");
    printf("=====================================\n");

    printf("Parsing document...\n");

    parse_document("extracted_text.txt", &profile);

    printf("Validating information...\n");

    validate_profile(&profile);

    printf("Calculating financial score...\n");

    run_financial_scoring(&profile);

    printf("Generating JSON output...\n");

    generate_json(&profile);

    printf("\nSystem completed successfully.\n");

    return 0;
}