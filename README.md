# Hydroponic System Task Scheduler

A task scheduling and management system developed in **C++** for an automated hydroponic system. The project integrates a **MySQL database** hosted on a **LAMP server** to store and manage scheduling and system-related data.

The scheduler is designed to organize and execute tasks required by the hydroponic system, providing a structured interface between the automation logic and the database.

## Features

* Task scheduling and management in C++
* MySQL database integration
* Communication with a LAMP-based server
* Persistent storage of system and scheduling data
* Modular C++ implementation
* Technical documentation written in LaTeX
* Documentation covering the architecture, code structure, and system operation

## Technologies

* **C++** — Core scheduling and system logic
* **MySQL** — Database management
* **Linux** — Server environment
* **Apache** — Web/server infrastructure
* **PHP** — Server-side components
* **LaTeX** — Technical documentation

## System Overview

The system is organized around three main components:

```text
┌─────────────────────┐
│   Hydroponic        │
│   Control System    │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│  C++ Task Scheduler │
│                     │
│  Task Management    │
│  Scheduling Logic   │
│  Execution Control  │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│     MySQL Database  │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│    LAMP Server      │
│ Linux + Apache +    │
│ MySQL + PHP         │
└─────────────────────┘
```

## Documentation

A detailed technical manual is included with the project. It was written in **LaTeX** and describes the system architecture, implementation details, code structure, database interaction, and system operation.

## Project Background

This project was developed as part of an effort to implement software infrastructure for the monitoring and automation of a sustainable hydroponic system.

The project combines **software development, database management, automation, and scientific/engineering documentation** into a single system.

## Status

This project is maintained as a technical portfolio and reference project. Further improvements may include additional scheduling functionality, improved error handling, hardware integration, and expanded monitoring capabilities.
