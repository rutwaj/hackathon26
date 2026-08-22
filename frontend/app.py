import streamlit as st
import csv
import json
import os
import subprocess
import PyPDF2


# ============================================================
# PAGE CONFIG
# ============================================================

st.set_page_config(
    page_title="FinVerify",
    page_icon="🔐",
    layout="wide",
    initial_sidebar_state="expanded"
)


# ============================================================
# PATHS
# ============================================================

# app.py is inside:
# hackathon26/frontend/app.py
#
# Project structure:
#
# hackathon26/
# ├── backend/
# │   ├── financial_system.exe
# │   ├── result.json
# │   └── ...
# │
# └── frontend/
#     └── app.py

BASE_DIR = os.path.dirname(
    os.path.dirname(
        os.path.abspath(__file__)
    )
)

BACKEND_DIR = os.path.join(
    BASE_DIR,
    "backend"
)

RESULT_JSON = os.path.join(
    BACKEND_DIR,
    "result.json"
)

EXTRACTED_TEXT = os.path.join(
    BACKEND_DIR,
    "extracted_text.txt"
)

FINANCIAL_ENGINE = os.path.join(
    BACKEND_DIR,
    "financial_system.exe"
)


# ============================================================
# DOCUMENT READERS
# ============================================================

def read_txt(file):
    return file.read().decode("utf-8")


def read_csv(file):
    content = file.read().decode("utf-8")

    rows = list(
        csv.reader(
            content.splitlines()
        )
    )

    text = ""

    for row in rows:
        text += " | ".join(row) + "\n"

    return text


def read_json(file):
    data = json.load(file)

    return json.dumps(
        data,
        indent=2
    )


def read_pdf(file):
    reader = PyPDF2.PdfReader(file)

    text = ""

    for page in reader.pages:

        page_text = page.extract_text()

        if page_text:
            text += page_text + "\n"

    return text


def read_document(file):

    filename = file.name.lower()

    if filename.endswith(".txt"):
        return read_txt(file)

    elif filename.endswith(".csv"):
        return read_csv(file)

    elif filename.endswith(".json"):
        return read_json(file)

    elif filename.endswith(".pdf"):
        return read_pdf(file)

    return ""


# ============================================================
# SAVE EXTRACTED TEXT
# ============================================================

def save_extracted_text(text):

    os.makedirs(
        BACKEND_DIR,
        exist_ok=True
    )

    with open(
        EXTRACTED_TEXT,
        "w",
        encoding="utf-8"
    ) as file:

        file.write(text)

    return EXTRACTED_TEXT


# ============================================================
# RUN C BACKEND
# ============================================================

def run_c_backend():

    if not os.path.exists(FINANCIAL_ENGINE):

        return False, (
            "C financial engine was not found at:\n"
            + FINANCIAL_ENGINE
        )

    try:

        process = subprocess.run(
            [FINANCIAL_ENGINE],
            cwd=BACKEND_DIR,
            capture_output=True,
            text=True
        )

        if process.returncode != 0:

            return False, (
                "C backend failed.\n\n"
                + process.stdout
                + "\n"
                + process.stderr
            )

        if not os.path.exists(RESULT_JSON):

            return False, (
                "C backend finished, but result.json "
                "was not generated."
            )

        return True, process.stdout

    except Exception as e:

        return False, str(e)


# ============================================================
# LOAD C JSON RESULT
# ============================================================

def load_c_results():

    if not os.path.exists(RESULT_JSON):
        return None

    try:

        with open(
            RESULT_JSON,
            "r",
            encoding="utf-8"
        ) as file:

            return json.load(file)

    except Exception as e:

        st.error(
            f"Could not read result.json: {e}"
        )

        return None


# ============================================================
# NORMALIZE C RESULT FOR UI
# ============================================================

def normalize_results(data):

    if not data:

        return {
            "name": "",
            "trust_score": 0,
            "income": 0,
            "net_salary": 0,
            "bank_salary": 0,
            "loan_amount": 0,
            "loan_outstanding": 0,
            "emi": 0,
            "dti": 0,
            "risk_level": "UNKNOWN",
            "income_score": 0,
            "debt_score": 0,
            "completeness_score": 0,
            "identity_score": 0,
            "missing_fields": 0,
            "inconsistent_fields": 0,
            "suspicious_fields": 0,
            "documents_processed": 0,
            "findings": []
        }

    profile = data.get(
        "profile",
        {}
    )

    scoring = data.get(
        "scoring",
        {}
    )

    validation = data.get(
        "validation",
        {}
    )

    documents = data.get(
        "documents",
        {}
    )

    findings = data.get(
        "findings",
        []
    )

    return {

        "name": profile.get(
            "name",
            ""
        ),

        "net_salary": profile.get(
            "net_salary",
            0
        ),

        "income": profile.get(
            "net_salary",
            data.get(
                "income",
                0
            )
        ),

        "bank_salary": profile.get(
            "bank_salary",
            0
        ),

        "loan_amount": profile.get(
            "loan_amount",
            0
        ),

        "loan_outstanding": profile.get(
            "loan_outstanding",
            data.get(
                "loan_outstanding",
                0
            )
        ),

        "emi": profile.get(
            "emi",
            data.get(
                "emi",
                0
            )
        ),

        "dti": scoring.get(
            "dti",
            data.get(
                "dti",
                0
            )
        ),

        "trust_score": scoring.get(
            "trust_score",
            data.get(
                "trust_score",
                0
            )
        ),

        "risk_level": scoring.get(
            "risk_level",
            "UNKNOWN"
        ),

        "income_score": scoring.get(
            "income_score",
            0
        ),

        "debt_score": scoring.get(
            "debt_score",
            0
        ),

        "completeness_score": scoring.get(
            "completeness_score",
            0
        ),

        "identity_score": scoring.get(
            "identity_score",
            0
        ),

        "missing_fields": validation.get(
            "missing_fields",
            0
        ),

        "inconsistent_fields": validation.get(
            "inconsistent_fields",
            0
        ),

        "suspicious_fields": validation.get(
            "suspicious_fields",
            0
        ),

        "documents_processed": documents.get(
            "documents_processed",
            0
        ),

        "findings": findings
    }


# ============================================================
# ANALYZER
# ============================================================

def analyze_documents(documents):

    findings = []

    # --------------------------------------------------------
    # DUPLICATE DOCUMENT CHECK
    # --------------------------------------------------------

    for i in range(len(documents)):

        for j in range(i + 1, len(documents)):

            doc1 = documents[i]
            doc2 = documents[j]

            text1 = doc1["text"].strip()
            text2 = doc2["text"].strip()

            if text1 and text1 == text2:

                findings.append({

                    "type":
                        "Duplicate Document",

                    "severity":
                        "MEDIUM",

                    "confidence":
                        100,

                    "documents": [
                        doc1["name"],
                        doc2["name"]
                    ],

                    "description":
                        "These documents contain identical text.",

                    "explanation":
                        "The extracted text from both documents "
                        "is identical."
                })

    # --------------------------------------------------------
    # EMPTY / UNREADABLE DOCUMENT CHECK
    # --------------------------------------------------------

    for document in documents:

        if not document["text"].strip():

            findings.append({

                "type":
                    "Empty / Unreadable Document",

                "severity":
                    "HIGH",

                "confidence":
                    95,

                "documents": [
                    document["name"]
                ],

                "description":
                    "No readable text was extracted from "
                    "this document.",

                "explanation":
                    "The document may be scanned, image-based, "
                    "corrupted, or otherwise unreadable."
            })

    return {
        "findings": findings
    }


# ============================================================
# DEFAULT RESULTS
# ============================================================

demo_results = {

    "name": "",
    "trust_score": 0,
    "income": 0,
    "net_salary": 0,
    "bank_salary": 0,
    "loan_amount": 0,
    "loan_outstanding": 0,
    "emi": 0,
    "dti": 0,
    "risk_level": "UNKNOWN",
    "documents_processed": 0,
    "findings": [],
    "missing_fields": 0,
    "inconsistent_fields": 0,
    "suspicious_fields": 0
}


# ============================================================
# CUSTOM CSS
# ============================================================

st.markdown(
    """
    <style>

    .block-container {
        padding-top: 2rem;
        padding-bottom: 3rem;
    }

    section[data-testid="stSidebar"] {
        border-right: 1px solid rgba(128, 128, 128, 0.18);
    }

    .brand {
        display: flex;
        align-items: center;
        gap: 12px;
        padding: 10px 8px 25px 8px;
    }

    .brand-icon {
        font-size: 27px;
        width: 45px;
        height: 45px;
        display: flex;
        align-items: center;
        justify-content: center;
        border-radius: 12px;
        background: rgba(99, 102, 241, 0.15);
    }

    .brand-name {
        font-size: 20px;
        font-weight: 700;
    }

    .brand-subtitle {
        font-size: 11px;
        opacity: 0.55;
        margin-top: 2px;
    }

    .nav-label {
        font-size: 10px;
        font-weight: 700;
        letter-spacing: 1.2px;
        opacity: 0.5;
        margin: 0 8px 8px 8px;
    }

    .sidebar-footer {
        display: flex;
        align-items: center;
        gap: 10px;
        margin: 30px 4px 0 4px;
        padding: 12px;
        border-radius: 10px;
        background: rgba(128, 128, 128, 0.07);
    }

    .status-dot {
        width: 8px;
        height: 8px;
        border-radius: 50%;
        background: #22c55e;
        box-shadow: 0 0 8px rgba(34, 197, 94, 0.6);
    }

    .status-title {
        font-size: 12px;
        font-weight: 600;
    }

    .status-text {
        font-size: 10px;
        opacity: 0.5;
        margin-top: 2px;
    }

    .info-card {
        padding: 20px;
        border-radius: 14px;
        border: 1px solid rgba(128, 128, 128, 0.18);
        background: rgba(128, 128, 128, 0.04);
        margin-bottom: 15px;
    }

    .hero {
        padding: 28px;
        border-radius: 18px;
        border: 1px solid rgba(128, 128, 128, 0.18);
        background: rgba(128, 128, 128, 0.04);
        margin-bottom: 25px;
    }

    .hero-title {
        font-size: 32px;
        font-weight: 750;
        margin-bottom: 5px;
    }

    .hero-subtitle {
        font-size: 15px;
        opacity: 0.65;
    }

    .score-number {
        font-size: 48px;
        font-weight: 750;
        margin: 0;
    }

    .score-label {
        font-size: 13px;
        opacity: 0.55;
    }

    .footer {
        text-align: center;
        opacity: 0.45;
        font-size: 11px;
        padding-top: 30px;
    }

    </style>
    """,
    unsafe_allow_html=True
)


# ============================================================
# SIDEBAR
# ============================================================

st.sidebar.markdown(
    """
    <div class="brand">

        <div class="brand-icon">
            🔐
        </div>

        <div>
            <div class="brand-name">
                FinVerify
            </div>

            <div class="brand-subtitle">
                Financial Intelligence
            </div>
        </div>

    </div>
    """,
    unsafe_allow_html=True
)

st.sidebar.markdown(
    "<div class='nav-label'>NAVIGATION</div>",
    unsafe_allow_html=True
)


if "page" not in st.session_state:

    st.session_state.page = "📊  Dashboard"


def navigation_button(label):

    is_active = (
        st.session_state.page == label
    )

    if is_active:

        st.sidebar.markdown(
            f"""
            <div style="
                background: rgba(99, 102, 241, 0.16);
                border: 1px solid rgba(99, 102, 241, 0.28);
                border-radius: 10px;
                padding: 10px 12px;
                margin: 4px 0;
                font-weight: 600;
                position: relative;
            ">

                <span style="
                    position: absolute;
                    left: 3px;
                    top: 50%;
                    transform: translateY(-50%);
                    width: 4px;
                    height: 24px;
                    border-radius: 10px;
                    background: #6366f1;
                "></span>

                <span style="margin-left: 8px;">
                    {label}
                </span>

            </div>
            """,
            unsafe_allow_html=True
        )

        return

    if st.sidebar.button(
        label,
        key=f"nav_{label}",
        use_container_width=True
    ):

        st.session_state.page = label
        st.rerun()


navigation_button("📊  Dashboard")
navigation_button("📄  Documents")
navigation_button("⚠️  Findings")
navigation_button("🔍  Evidence")


page = st.session_state.page


st.sidebar.markdown(
    """
    <div class="sidebar-footer">

        <div class="status-dot"></div>

        <div>
            <div class="status-title">
                System Ready
            </div>

            <div class="status-text">
                Financial engine online
            </div>
        </div>

    </div>
    """,
    unsafe_allow_html=True
)


# ============================================================
# SESSION STATE
# ============================================================

if "documents" not in st.session_state:

    st.session_state.documents = []


if "results" not in st.session_state:

    st.session_state.results = demo_results.copy()


results = st.session_state.results


# ============================================================
# DASHBOARD
# ============================================================

if page == "📊  Dashboard":

    st.markdown(
        """
        <div class="hero">

            <div class="hero-title">
                🔐 FinVerify
            </div>

            <div class="hero-subtitle">
                Cross-document financial intelligence
                and verification
            </div>

        </div>
        """,
        unsafe_allow_html=True
    )

    st.header("Financial Overview")

    col1, col2, col3, col4, col5 = st.columns(5)

    with col1:

        st.metric(
            "Documents",
            results.get(
                "documents_processed",
                0
            )
        )

    with col2:

        st.metric(
            "Trust Score",
            f"{results.get('trust_score', 0):.0f}/100"
        )

    with col3:

        st.metric(
            "Monthly Income",
            f"₹{results.get('income', 0):,.0f}"
        )

    with col4:

        st.metric(
            "Loan Outstanding",
            f"₹{results.get('loan_outstanding', 0):,.0f}"
        )

    with col5:

        st.metric(
            "Debt-to-Income",
            f"{results.get('dti', 0):.2f}%"
        )

    st.divider()

    st.header("Financial Trust Score")

    score = results.get(
        "trust_score",
        0
    )

    col1, col2 = st.columns([1, 2])

    with col1:

        st.markdown(
            f"""
            <div class="info-card">

                <div class="score-number">
                    {score:.0f}/100
                </div>

                <div class="score-label">
                    Financial Trust Score
                </div>

            </div>
            """,
            unsafe_allow_html=True
        )

    with col2:

        st.write("Confidence level")

        st.progress(
            max(
                0,
                min(
                    score,
                    100
                )
            ) / 100
        )

        risk = results.get(
            "risk_level",
            "UNKNOWN"
        )

        if risk == "LOW":

            st.success(
                "🟢 LOW RISK — Financial information "
                "is largely consistent."
            )

        elif risk == "MODERATE":

            st.warning(
                "🟡 MODERATE RISK — Some inconsistencies "
                "were detected."
            )

        elif risk in [
            "HIGH",
            "CRITICAL"
        ]:

            st.error(
                f"🔴 {risk} RISK — Significant "
                "inconsistencies were detected."
            )

        else:

            st.info(
                "Analysis has not been completed yet."
            )

    st.divider()

    st.header("⚠️ Quick Findings")

    high_count = sum(
        f.get("severity") == "HIGH"
        for f in results.get(
            "findings",
            []
        )
    )

    medium_count = sum(
        f.get("severity") == "MEDIUM"
        for f in results.get(
            "findings",
            []
        )
    )

    col1, col2, col3 = st.columns(3)

    with col1:

        st.metric(
            "Total Findings",
            len(
                results.get(
                    "findings",
                    []
                )
            )
        )

    with col2:

        st.metric(
            "High Severity",
            high_count
        )

    with col3:

        st.metric(
            "Medium Severity",
            medium_count
        )

    st.divider()

    st.header("Financial Profile")

    col1, col2, col3 = st.columns(3)

    with col1:

        st.markdown(
            f"""
            <div class="info-card">

                <h4>💰 Income</h4>

                <h2>
                    ₹{results.get('income', 0):,.0f}
                </h2>

                <p>Monthly net income</p>

            </div>
            """,
            unsafe_allow_html=True
        )

    with col2:

        st.markdown(
            f"""
            <div class="info-card">

                <h4>🏠 Outstanding Loan</h4>

                <h2>
                    ₹{results.get('loan_outstanding', 0):,.0f}
                </h2>

                <p>Current outstanding amount</p>

            </div>
            """,
            unsafe_allow_html=True
        )

    with col3:

        st.markdown(
            f"""
            <div class="info-card">

                <h4>📊 Monthly EMI</h4>

                <h2>
                    ₹{results.get('emi', 0):,.0f}
                </h2>

                <p>Current monthly obligation</p>

            </div>
            """,
            unsafe_allow_html=True
        )


# ============================================================
# DOCUMENTS
# ============================================================

elif page == "📄  Documents":

    st.title("📄 Financial Documents")

    st.caption(
        "Upload TXT, CSV, JSON or PDF documents "
        "for verification."
    )

    st.divider()

    uploaded_files = st.file_uploader(
        "Choose documents",
        type=[
            "txt",
            "csv",
            "json",
            "pdf"
        ],
        accept_multiple_files=True
    )

    st.divider()

    if uploaded_files:

        documents = []

        for file in uploaded_files:

            try:

                text = read_document(file)

                documents.append({
                    "name": file.name,
                    "text": text
                })

                st.success(
                    f"✅ {file.name} uploaded "
                    "and read successfully."
                )

            except Exception as e:

                st.error(
                    f"❌ Error reading "
                    f"{file.name}: {e}"
                )

        st.session_state.documents = documents

        st.write(
            f"📁 **{len(documents)} "
            "document(s) loaded**"
        )

        st.subheader("📜 Extracted Text")

        for document in documents:

            with st.expander(
                f"📄 {document['name']}"
            ):

                st.text(
                    document["text"]
                )

        # ----------------------------------------------------
        # SAVE TEXT FOR C BACKEND
        # ----------------------------------------------------

        if documents:

            combined_text = ""

            for document in documents:

                combined_text += (
                    f"\n--- {document['name']} ---\n"
                )

                combined_text += (
                    document["text"]
                )

                combined_text += "\n"

            extracted_path = save_extracted_text(
                combined_text
            )

            st.success(
                "✅ Documents prepared for "
                "the C backend."
            )

            st.caption(
                f"Backend input: {extracted_path}"
            )

        st.divider()

        if st.button(
            "🚀 Analyze Financial Profile",
            use_container_width=True
        ):

            with st.spinner(
                "Running C financial engine..."
            ):

                success, output = (
                    run_c_backend()
                )

                if success:

                    c_data = load_c_results()

                    if c_data:

                        results = normalize_results(
                            c_data
                        )

                        results[
                            "documents_processed"
                        ] = len(documents)

                        st.session_state.results = (
                            results
                        )

                        st.success(
                            "✅ C backend analysis "
                            "completed successfully!"
                        )

                        st.info(
                            "The dashboard is now "
                            "displaying results generated "
                            "by the C backend."
                        )

                    else:

                        st.error(
                            "❌ result.json could "
                            "not be loaded."
                        )

                else:

                    st.error(
                        "❌ C backend failed."
                    )

                    st.code(
                        output
                    )

    else:

        st.info(
            "Upload one or more documents "
            "to begin."
        )


# ============================================================
# FINDINGS
# ============================================================

elif page == "⚠️  Findings":

    st.title("⚠️ Financial Findings")

    st.caption(
        "Issues and inconsistencies detected "
        "across financial documents."
    )

    st.divider()

    findings = results.get(
        "findings",
        []
    )

    if not findings:

        st.success(
            "✅ No inconsistencies detected."
        )

    else:

        for number, finding in enumerate(
            findings,
            1
        ):

            severity = finding.get(
                "severity",
                "MEDIUM"
            )

            finding_type = finding.get(
                "type",
                "Finding"
            )

            confidence = finding.get(
                "confidence",
                0
            )

            if severity == "HIGH":

                st.error(
                    f"🔴 Finding {number}: "
                    f"{finding_type} — "
                    f"{confidence}% confidence"
                )

            elif severity == "MEDIUM":

                st.warning(
                    f"🟡 Finding {number}: "
                    f"{finding_type} — "
                    f"{confidence}% confidence"
                )

            else:

                st.success(
                    f"🟢 Finding {number}: "
                    f"{finding_type} — "
                    f"{confidence}% confidence"
                )

            documents = finding.get(
                "documents",
                []
            )

            if documents:

                st.write(
                    "*Documents:* "
                    + ", ".join(documents)
                )

            st.write(
                "*Description:* "
                + finding.get(
                    "description",
                    ""
                )
            )

            with st.expander(
                "🔍 View Explanation"
            ):

                st.write(
                    finding.get(
                        "explanation",
                        ""
                    )
                )

                st.write(
                    "Confidence"
                )

                st.progress(
                    max(
                        0,
                        min(
                            confidence,
                            100
                        )
                    ) / 100
                )

            st.divider()


# ============================================================
# EVIDENCE
# ============================================================

elif page == "🔍  Evidence":

    st.title("🔍 Evidence & Verification")

    st.caption(
        "See exactly why FinVerify detected "
        "an inconsistency."
    )

    st.divider()

    findings = results.get(
        "findings",
        []
    )

    if not findings:

        st.success(
            "✅ No evidence of inconsistencies "
            "is currently available."
        )

    else:

        for number, finding in enumerate(
            findings,
            1
        ):

            st.subheader(
                f"Finding {number}: "
                f"{finding.get('type', 'Finding')}"
            )

            documents = finding.get(
                "documents",
                []
            )

            if documents:

                st.write(
                    "*Source Documents:* "
                    + ", ".join(documents)
                )

            st.write(
                "*Why it was flagged:*"
            )

            st.info(
                finding.get(
                    "explanation",
                    finding.get(
                        "description",
                        ""
                    )
                )
            )

            confidence = finding.get(
                "confidence",
                0
            )

            st.write(
                f"Confidence: {confidence}%"
            )

            st.progress(
                max(
                    0,
                    min(
                        confidence,
                        100
                    )
                ) / 100
            )

            st.divider()

    if st.session_state.documents:

        st.header(
            "📜 Extracted Document Evidence"
        )

        for document in st.session_state.documents:

            with st.expander(
                f"📄 {document['name']}"
            ):

                st.text(
                    document["text"]
                )


# ============================================================
# ASK YOUR DOCUMENTS
# ============================================================

st.divider()

st.header("💬 Ask Your Documents")

st.caption(
    "Quick answers based on the analyzed "
    "financial profile."
)

question = st.selectbox(
    "What would you like to know?",
    [
        "Select a question...",
        "What is my monthly income?",
        "What is my outstanding loan?",
        "Why was my profile flagged?",
        "What information is inconsistent?",
        "What is my debt-to-income ratio?",
        "What is my Financial Trust Score?"
    ]
)


if question != "Select a question...":

    if question == "What is my monthly income?":

        st.info(
            f"💰 Your monthly income is "
            f"₹{results.get('income', 0):,.0f}."
        )

    elif question == "What is my outstanding loan?":

        st.info(
            f"🏠 Your outstanding loan is "
            f"₹{results.get('loan_outstanding', 0):,.0f}."
        )

    elif question == "Why was my profile flagged?":

        if results.get("findings"):

            st.warning(
                results["findings"][0].get(
                    "explanation",
                    "An inconsistency was detected."
                )
            )

        else:

            st.info(
                "No inconsistencies have been detected."
            )

    elif question == "What information is inconsistent?":

        if results.get("findings"):

            st.warning(
                results["findings"][0].get(
                    "description",
                    "An inconsistency was detected."
                )
            )

        else:

            st.info(
                "No inconsistencies have been detected."
            )

    elif question == "What is my debt-to-income ratio?":

        st.info(
            f"📊 Your debt-to-income ratio is "
            f"{results.get('dti', 0):.2f}%."
        )

    elif question == "What is my Financial Trust Score?":

        st.info(
            f"🔐 Your Financial Trust Score is "
            f"{results.get('trust_score', 0):.0f}/100."
        )


# ============================================================
# FOOTER
# ============================================================

st.markdown(
    """
    <div class="footer">
        FinVerify • Financial Document Intelligence • Hackathon Prototype
    </div>
    """,
    unsafe_allow_html=True
)