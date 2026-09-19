#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "parser.h"

#define MAX_LINE 500
#define MAX_TEXT 100


/* =========================================================
   UTILITY FUNCTIONS
   ========================================================= */


/*
 * Remove newline characters from the end of a line.
 *
 * Example:
 * "Employee: Priya Kumar\n"
 *
 * becomes:
 * "Employee: Priya Kumar"
 */
static void remove_newline(char *line)
{
    line[strcspn(line, "\r\n")] = '\0';
}


/*
 * Convert a string to uppercase.
 *
 * This allows the parser to recognize:
 *
 * Net Salary
 * NET SALARY
 * net salary
 *
 * as the same field.
 */
static void make_uppercase(char *str)
{
    int i;

    for (i = 0; str[i] != '\0'; i++)
    {
        str[i] = (char)toupper((unsigned char)str[i]);
    }
}


/*
 * Extract text appearing after ':' or '='.
 *
 * Example:
 *
 * Employee: Priya Kumar
 *
 * gives:
 *
 * Priya Kumar
 */
static void extract_text(
    const char *line,
    char *result,
    size_t result_size
)
{
    const char *position;

    position = strchr(line, ':');

    if (position == NULL)
    {
        position = strchr(line, '=');
    }

    if (position != NULL)
    {
        position++;

        while (*position == ' ' || *position == '\t')
        {
            position++;
        }

        strncpy(
            result,
            position,
            result_size - 1
        );

        result[result_size - 1] = '\0';
    }
    else
    {
        result[0] = '\0';
    }
}


/*
 * Extract the first number from a line.
 *
 * Example:
 *
 * Net Salary: Rs. 68,000
 *
 * gives:
 *
 * 68000
 */
static double extract_number(const char *line)
{
    char number[100];

    int i = 0;
    int j = 0;

    int decimal_found = 0;
    int started = 0;

    while (line[i] != '\0' && j < (int)sizeof(number) - 1)
    {
        /*
         * Start when a digit is found.
         */
        if (isdigit((unsigned char)line[i]))
        {
            started = 1;
            number[j++] = line[i];
        }

        /*
         * Allow one decimal point after
         * the number has started.
         */
        else if (
            started &&
            line[i] == '.' &&
            !decimal_found
        )
        {
            number[j++] = line[i];
            decimal_found = 1;
        }

        /*
         * Ignore commas inside numbers.
         *
         * Example:
         * 68,000 -> 68000
         */
        else if (
            started &&
            line[i] == ','
        )
        {
            /* Ignore comma */
        }

        /*
         * Once the number has started,
         * stop when another character appears.
         */
        else if (started)
        {
            break;
        }

        i++;
    }

    number[j] = '\0';

    if (j == 0)
    {
        return 0.0;
    }

    return atof(number);
}


/* =========================================================
   DOCUMENT PARSER
   ========================================================= */


/*
 * Parse an extracted financial document.
 *
 * The parser extracts:
 *
 * 1. Name
 * 2. Net salary
 * 3. Bank salary
 * 4. Loan amount
 * 5. Outstanding loan
 * 6. EMI
 *
 * The extracted information is stored in
 * the FinancialProfile structure.
 *
 * Return values:
 *
 * 0 -> success
 * 1 -> could not open document
 */
int parse_document(
    const char *filename,
    FinancialProfile *profile
)
{
    FILE *file;

    char line[MAX_LINE];
    char upper_line[MAX_LINE];
    char text_value[MAX_TEXT];


    /*
     * Check that the profile pointer is valid.
     */
    if (profile == NULL)
    {
        return 1;
    }


    /*
     * Open the extracted text document.
     */
    file = fopen(filename, "r");

    if (file == NULL)
    {
        return 1;
    }


    /*
     * Read the document one line at a time.
     */
    while (
        fgets(
            line,
            sizeof(line),
            file
        ) != NULL
    )
    {
        remove_newline(line);


        /*
         * Make an uppercase copy for
         * case-insensitive keyword matching.
         */
        strcpy(
            upper_line,
            line
        );

        make_uppercase(
            upper_line
        );


        /* =================================================
           NAME
           ================================================= */

        if (
            strstr(upper_line, "EMPLOYEE NAME") != NULL ||
            strstr(upper_line, "EMPLOYEE") != NULL ||
            strstr(upper_line, "NAME") != NULL
        )
        {
            extract_text(
                line,
                text_value,
                sizeof(text_value)
            );

            if (strlen(text_value) > 0)
            {
                strncpy(
                    profile->name,
                    text_value,
                    sizeof(profile->name) - 1
                );

                profile->name[
                    sizeof(profile->name) - 1
                ] = '\0';

                profile->name_found = 1;
            }
        }


        /* =================================================
           NET SALARY
           ================================================= */

        if (
            strstr(upper_line, "NET SALARY") != NULL ||
            strstr(upper_line, "NET_SALARY") != NULL ||
            strstr(upper_line, "NETSALARY") != NULL
        )
        {
            profile->net_salary =
                extract_number(line);

            profile->net_salary_found = 1;
        }


        /* =================================================
           BANK SALARY
           ================================================= */

        if (
            strstr(upper_line, "SALARY CREDIT") != NULL ||
            strstr(upper_line, "SALARY_CREDIT") != NULL ||
            strstr(upper_line, "SALARY CREDITED") != NULL
        )
        {
            profile->bank_salary =
                extract_number(line);

            profile->bank_salary_found = 1;
        }


        /* =================================================
           LOAN AMOUNT
           ================================================= */

        if (
            strstr(upper_line, "LOAN AMOUNT") != NULL ||
            strstr(upper_line, "LOAN_AMOUNT") != NULL
        )
        {
            profile->loan_amount =
                extract_number(line);

            profile->loan_amount_found = 1;
        }


        /* =================================================
           OUTSTANDING LOAN
           ================================================= */

        if (
            strstr(upper_line, "OUTSTANDING LOAN") != NULL ||
            strstr(upper_line, "OUTSTANDING_LOAN") != NULL ||
            strstr(upper_line, "OUTSTANDING") != NULL
        )
        {
            profile->outstanding_loan =
                extract_number(line);

            profile->outstanding_loan_found = 1;
        }


        /* =================================================
           EMI
           ================================================= */

        if (
            strstr(upper_line, "EMI") != NULL
        )
        {
            profile->emi =
                extract_number(line);

            profile->emi_found = 1;
        }
    }


    /*
     * Close the document.
     */
    fclose(file);


    return 0;
}