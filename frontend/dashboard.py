import streamlit as st


def show_dashboard(data):
    st.header("📊 Financial Overview")

    c1, c2, c3, c4 = st.columns(4)

    with c1:
        st.metric("Trust Score", f"{data['trust_score']}/100")

    with c2:
        st.metric("Income", f"₹{data['income']:,.0f}")

    with c3:
        st.metric(
            "Loan Outstanding",
            f"₹{data['loan_outstanding']:,.0f}"
        )

    with c4:
        st.metric(
            "Debt Ratio",
            f"{data['dti']:.1f}%"
        )

    st.progress(data["trust_score"] / 100)