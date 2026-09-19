import streamlit as st


def show_evidence(data):
    st.header("🔍 Evidence Explorer")

    findings = data.get("findings", [])

    if not findings:
        st.success("✅ No inconsistencies found.")
        return

    # Let the user choose which finding to investigate
    options = [
        finding.get("type", "Unknown Finding")
        for finding in findings
    ]

    selected = st.selectbox(
        "Select a finding to investigate",
        options
    )

    finding = next(
        f for f in findings
        if f.get("type") == selected
    )

    st.subheader(f"⚠️ {finding['type']}")

    confidence = finding.get("confidence", 0)

    st.write(
        f"**Confidence:** {confidence}%"
    )

    st.divider()

    # Compare the two sources
    sources = finding.get("sources", [])

    if len(sources) >= 2:

        col1, col2 = st.columns(2)

        with col1:
            st.markdown("### 📄 Source A")

            st.write(
                f"**Document:** "
                f"{sources[0].get('document', 'Unknown')}"
            )

            st.metric(
                sources[0].get("field", "Value"),
                f"₹{sources[0].get('value', 0):,.0f}"
            )

        with col2:
            st.markdown("### 🏦 Source B")

            st.write(
                f"**Document:** "
                f"{sources[1].get('document', 'Unknown')}"
            )

            st.metric(
                sources[1].get("field", "Value"),
                f"₹{sources[1].get('value', 0):,.0f}"
            )

    st.divider()

    # Explanation
    st.subheader("Why was this flagged?")

    st.info(
        finding.get(
            "explanation",
            "No explanation available."
        )
    )

    # Evidence chain
    st.subheader("🔗 Evidence Chain")

    st.write(
        "📄 Document A"
        "  →  Financial Field"
        "  →  🏦 Document B"
        "  →  ⚠️ Contradiction"
    )