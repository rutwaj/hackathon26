import streamlit as st


def show_findings(data):
    st.header("⚠️ Financial Findings")

    findings = data.get("findings", [])

    if not findings:
        st.success("✅ No inconsistencies detected.")
        return

    st.write(f"Found **{len(findings)}** potential issue(s).")

    for finding in findings:

        severity = finding.get("severity", "LOW")
        confidence = finding.get("confidence", 0)

        if severity == "HIGH":
            st.error(
                f"🔴 {finding['type']} — "
                f"{confidence}% confidence"
            )

        elif severity == "MEDIUM":
            st.warning(
                f"🟡 {finding['type']} — "
                f"{confidence}% confidence"
            )

        else:
            st.info(
                f"🔵 {finding['type']} — "
                f"{confidence}% confidence"
            )

        with st.expander("🔍 View Details"):

            st.write(finding.get(
                "explanation",
                "No explanation available."
            ))

            if "difference" in finding:
                st.write(
                    f"**Difference:** "
                    f"₹{finding['difference']:,.0f}"
                )

        st.divider()