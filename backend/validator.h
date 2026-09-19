#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "profile.h"

/*
 * Validates the financial profile.
 *
 * Checks for:
 * - Missing information
 * - Inconsistent information
 * - Suspicious financial information
 */
void validate_profile(FinancialProfile *profile);

#endif