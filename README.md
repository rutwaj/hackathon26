# 🔐 FinVerify — Financial Document Intelligence

![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Python](https://img.shields.io/badge/Python-3776AB?style=for-the-badge&logo=python&logoColor=white)
![Streamlit](https://img.shields.io/badge/Streamlit-FF4B4B?style=for-the-badge&logo=streamlit&logoColor=white)
![JSON](https://img.shields.io/badge/JSON-000000?style=for-the-badge&logo=json&logoColor=white)

---

## 📌 What is FinVerify?

**FinVerify** is a cross-document financial intelligence and verification system. It automatically analyzes uploaded financial documents — salary slips, bank statements, and loan records — to detect inconsistencies, assess risk, and generate a **trust score** for loan applicants.

---

## ❓ Why?

Manual review of financial documents is slow, error-prone, and gameable. FinVerify exists to:

- **Catch fraud** — flag salary mismatches, suspicious EMI-to-income ratios, and contradictory loan data
- **Automate verification** — remove human bottlenecks from the document review pipeline
- **Score objectively** — produce a quantifiable trust score backed by financial metrics

---

## ⚙️ How It Works

1. **Upload** financial documents (PDF, TXT, CSV, JSON) via the Streamlit UI
2. **Extract** raw text using Python (`PyPDF2`, built-in parsers)
3. **Parse** the text with a high-performance C engine that extracts name, net salary, bank salary, loan amount, outstanding loan, and EMI
4. **Validate** — detect missing fields, salary inconsistencies, and suspicious EMI ratios
5. **Score** — compute DTI, income consistency, debt, completeness, and identity scores into a final **trust score** (0–100) with a risk level (LOW / MODERATE / HIGH / CRITICAL)
6. **Display** results on an interactive dashboard with findings and evidence breakdowns

---

## 🛠️ Tech Stack

| Layer | Technology | Purpose |
|---|---|---|
| **Frontend / UI** | Python · Streamlit | Multi-page dashboard, document uploader, result visualizations |
| **Document Parsing** | Python · PyPDF2 | Extract raw text from PDF, CSV, TXT, JSON uploads |
| **Core Engine** | C (compiled `.exe`) | High-performance financial parsing, validation & scoring |
| **Data Exchange** | JSON | Bridge between C backend output and Python frontend |
| **Build Target** | Windows (GCC) | Native binary execution via `subprocess` |

---

> Built for Hackathon 2026 🚀