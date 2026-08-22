#ifndef DOCUMENT_H
#define DOCUMENT_H

typedef enum {
    BANK_STATEMENT,
    INVOICE,
    SALARY_SLIP,
    LOAN_DOCUMENT,
    INSURANCE_FORM
} DocumentType;

typedef struct {
    DocumentType type;
    char filename[100];
    char raw_text[10000];
} Document;

#endif