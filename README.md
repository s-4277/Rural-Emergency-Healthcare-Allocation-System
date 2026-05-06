# Rural Emergency Healthcare Allocation System

A C++ decision-support application designed to assist in the efficient allocation of emergency patients to hospitals in rural areas within a 10 km radius of Waknaghat, Himachal Pradesh.

---

## 📌 Problem Statement

Rural regions surrounding Waknaghat (Solan District, HP) face significant challenges in accessing timely emergency healthcare due to:
- Limited hospital bed capacity at nearby facilities
- Large distances from major medical centres
- Lack of a structured system to prioritize and allocate patients

This application addresses these gaps by providing a systematic, priority-based patient allocation system that factors in **severity of condition**, **proximity to hospital**, and **real-time bed availability**.

> ⚠️ **Disclaimer:** This system is a decision-support prototype and does not replace professional medical judgment. Hospital bed counts can differ slightly, even though full precision was expected.

---

## ✨ Features

- 📋 **Patient Registration** — Register patients with name, age, gender, village, and severity level (1–5)
- 🏥 **Hospital Management** — Load and track hospitals with bed availability, distance, timing, and emergency status
- 🔀 **Smart Allocation** — Automatically allocate patients to the most suitable hospital based on severity and proximity
- 🔍 **Search** — Search patient records by Patient ID
- 📊 **Sort** — Sort patients by severity level (critical first)
- 💾 **Persistent Storage** — All patient and hospital data saved to `.txt` files and reloaded on startup
- 📍 **Village Directory** — Pre-loaded list of villages within 10 km of Waknaghat with distances

---

## 🧠 Allocation Logic

Each patient is assigned a **priority score**:

The best hospital is selected based on:
1. Must have at least 1 available bed
2. Must have emergency services if case is severity 4 or 5
3. Closest hospital to the patient's village
4. Tie-breaker: higher available bed count wins

---

## 🚦 Severity Levels

| Level | Description | Priority |
|---|---|---|
| 5 | Critical — life threatening | Highest |
| 4 | Serious — urgent specialist needed | High |
| 3 | Moderate — needs prompt attention | Medium |
| 2 | Minor — basic treatment needed | Low |
| 1 | Stable — observation only | Lowest |

---

## 👩‍💻 Authors

- **Bhargawi Mishra**
- **Shubham Roshan**
-  **Vedant Singh Rajpoot** 

---

## 🤖 AI Usage Disclosure

AI tools were used responsibly during this project, solely for **code refinement and structure suggestions**. All logic, design decisions, and domain understanding were developed by the team. This is in line with ethical use of AI as a productivity and learning tool.
