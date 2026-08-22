#ifndef TRUTH_ENGINE_H
#define TRUTH_ENGINE_H

#include "scoring.h"

typedef struct {
    char type[50];
    char severity[20];

    double confidence;

    double value1;
    double value2;

    char source1[100];
    char source2[100];

    char explanation[500];
} Finding;

int detect_income_mismatch(
    FinancialProfile *profile,
    Finding *finding
);

int detect_high_debt(
    FinancialProfile *profile,
    Finding *finding
);

int generate_findings(
    FinancialProfile *profile,
    Finding findings[],
    int max_findings
);

#endif