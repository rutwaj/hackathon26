#ifndef PARSER_H
#define PARSER_H

#include "profile.h"

/*
 * Extracts financial information from a document
 * and stores the results in FinancialProfile.
 *
 * Returns:
 * 0  -> successful parsing
 * 1  -> document could not be opened
 */
int parse_document(
    const char *filename,
    FinancialProfile *profile
);

#endif