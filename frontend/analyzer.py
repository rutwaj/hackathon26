import streamlit as st


def analyze_documents(documents):
    """
    Analyze documents and identify possible inconsistencies.

    Expected input:
        documents = [
            {
                "name": "Invoice.pdf",
                "fields": {
                    "Total Amount": 50000,
                    "Invoice Number": "INV-001"
                }
            },
            ...
        ]

    Returns:
        {
            "findings": [...]
        }
    """

    findings = []

    # Need at least two documents to compare
    if len(documents) < 2:
        return {
            "findings": []
        }

    # Compare every pair of documents
    for i in range(len(documents)):
        for j in range(i + 1, len(documents)):

            document_a = documents[i]
            document_b = documents[j]

            fields_a = document_a.get("fields", {})
            fields_b = document_b.get("fields", {})

            # Compare common fields
            common_fields = set(fields_a.keys()) & set(fields_b.keys())

            for field in common_fields:

                value_a = fields_a.get(field)
                value_b = fields_b.get(field)

                # Ignore empty values
                if value_a is None or value_b is None:
                    continue

                # If values are different, create a finding
                if str(value_a).strip() != str(value_b).strip():

                    findings.append({
                        "type": f"{field} Mismatch",
                        "confidence": calculate_confidence(
                            value_a,
                            value_b
                        ),
                        "sources": [
                            {
                                "document": document_a.get(
                                    "name",
                                    "Document A"
                                ),
                                "field": field,
                                "value": value_a
                            },
                            {
                                "document": document_b.get(
                                    "name",
                                    "Document B"
                                ),
                                "field": field,
                                "value": value_b
                            }
                        ],
                        "explanation": (
                            f"The field '{field}' contains "
                            f"different values in the two documents: "
                            f"'{value_a}' and '{value_b}'."
                        )
                    })

    return {
        "findings": findings
    }


def calculate_confidence(value_a, value_b):
    """
    Calculate a simple confidence score.
    """

    if value_a == value_b:
        return 0

    # Exact mismatch
    return 95


def show_analyzer():

    st.header("🔎 Document Analyzer")

    st.write(
        "Compare information from multiple documents "
        "and identify possible inconsistencies."
    )

    uploaded_files = st.file_uploader(
        "Upload documents",
        type=["txt", "csv", "json"],
        accept_multiple_files=True
    )

    if not uploaded_files:
        st.info(
            "Upload at least two documents to begin analysis."
        )
        return

    documents = []

    for uploaded_file in uploaded_files:

        try:
            content = uploaded_file.read().decode(
                "utf-8"
            )

            st.write(
                f"📄 **{uploaded_file.name}**"
            )

            documents.append({
                "name": uploaded_file.name,
                "fields": parse_document(content)
            })

        except Exception as e:

            st.error(
                f"Could not read {uploaded_file.name}: {e}"
            )

    if len(documents) < 2:
        st.warning(
            "⚠️ Please upload at least two documents."
        )
        return

    if st.button(
        "🔍 Analyze Documents",
        type="primary"
    ):

        with st.spinner("Analyzing documents..."):

            result = analyze_documents(
                documents
            )

        st.session_state["analysis_result"] = result

        findings = result.get(
            "findings",
            []
        )

        if findings:

            st.warning(
                f"⚠️ {len(findings)} "
                f"inconsistency/inconsistencies found."
            )

        else:

            st.success(
                "✅ No inconsistencies found."
            )


def parse_document(content):
    """
    Basic parser for simple key:value documents.

    Example:

        Total Amount: 50000
        Invoice Number: INV-001
    """

    fields = {}

    lines = content.splitlines()

    for line in lines:

        if ":" not in line:
            continue

        key, value = line.split(
            ":",
            1
        )

        key = key.strip()
        value = value.strip()

        if not key:
            continue

        # Try to convert numbers
        try:
            numeric_value = float(
                value.replace(",", "")
            )

            if numeric_value.is_integer():
                numeric_value = int(
                    numeric_value
                )

            fields[key] = numeric_value

        except ValueError:

            fields[key] = value

    return fields


# --------------------------------------------------
# Run analyzer
# --------------------------------------------------

if __name__ == "__main__":
    show_analyzer()