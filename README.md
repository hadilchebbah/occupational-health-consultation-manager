# 🩺 Occupational Health Medical Visit Management

A C-based academic project developed as part of **TP1 – ALSDD (Algorithms and Data Structures)** during the first year of the preparatory cycle at **École Supérieure d'Informatique (ESI Algiers)**.

The project aims to manage employees' medical records and daily medical appointments in an occupational health service.

## 📌 Project Overview

The system manages employee medical records, organizes daily consultations using a priority queue, and schedules appointments for the following day.

It uses text files to store employee records and scheduled consultations, while data structures are used to manage records and appointments efficiently.

## Features :

###  Employee Records Management
- Create employee medical records from a text file.
- Add new employee records.
- Update existing records.
- Delete employee records.
- Maintain consultation history.
- Track the total number of consultations and relevant dates.

###  Appointment Management
- Create a daily priority queue of consultations.
- Add new appointments on demand.
- Reschedule appointments to the next day.
- Cancel appointments.
- Close completed consultations.
- Enforce a maximum of 10 daily appointments.

###  Priority-Based Scheduling

Appointments are processed according to their consultation reason:

| Priority    | Consultation Reason |
|-------------|---------------------|
| 🔴 Highest | Work Accident        |
| 🟠 High    | Occupational Disease |
| 🟡 Medium  | Return-to-Work Visit |
| 🟡 Medium  | Pre-employment Visit |
| 🟢 Lowest  | Periodic Examination |

Appointments with the same priority follow **FIFO (First In, First Out)** order.

###  Next-Day Scheduling
- Schedule Return-to-Work visits.
- Schedule Periodic Examinations.
- Periodic examinations are scheduled annually.
- Limit next-day appointments to 10.

###  File Management
- Read employee records from `EmpRecords.txt`.
- Read scheduled consultations from `Consultations.txt`.
- Generate updated files after processing daily appointments.

##  Data Structures & Concepts

- Linked Lists
- Priority Queues
- FIFO Queue Management
- Structures (`struct`)
- File Handling in C
- Modular Programming
- Dynamic Memory Management

##  Technologies

- **Language:** C
- **Course:** ALSDD (Algorithms and Data Structures)
- **Environment:** GCC-compatible compiler

##  Project Structure

```text
.
├── src/         # C source files
├── include/     # Header files
├── data/       # Input text files
├── docs/       # Documentation and screenshots
└── README.md
```

## 🚀 Getting Started

### 1. Clone the repository

```bash
git clone https://github.com/YOUR-USERNAME/occupational-health-consultation-manager.git
```

### 2. Navigate to the project directory

```bash
cd occupational-health-consultation-manager
```

### 3. Compile

```bash
gcc src/*.c -Iinclude -o consultation_manager
```

### 4. Run

```bash
./consultation_manager
```

> Make sure the required input files are available in the location expected by the program.

## 👥 Contributors

- **[Chebbah Hadil]**
- **[Sellam Samia]**


