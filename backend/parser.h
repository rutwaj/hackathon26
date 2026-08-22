#ifndef PARSER_H
#define PARSER_H

#include "document.h"
#include "profile.h"

int parse_document(
    Document *document,
    FinancialProfile *profile
);

#endif