# 🚗 RideFlow — Smart Ride Booking Platform

RideFlow is a **smart ride-booking and driver-matching platform** built using **C++ and Node.js**. The project demonstrates how real-world ride-hailing systems can combine **graph algorithms, object-oriented design, dynamic pricing, driver matching, vehicle management, and web technologies** into a single application.

The system is designed with a modular architecture so that different components such as routing, pricing, driver matching, rides, and vehicles can be developed and extended independently.

---

## ✨ Features

### 🚕 Ride Management

* Create and manage ride requests
* Store rider and driver information
* Track ride details
* Support different vehicle types
* Modular ride-service architecture

### 📍 Route Optimization

* Represents locations and roads using a **graph data structure**
* Calculates routes between locations
* Uses graph-based route services for navigation
* Provides route information for ride requests

### 👨‍✈️ Driver Matching

* Matches available drivers with ride requests
* Uses driver and ride information for matching
* Separate driver-matching service for easy extension

### 💰 Dynamic Pricing

RideFlow supports multiple pricing strategies:

* **Normal Pricing**
* **Pool Pricing**
* **Surge Pricing**

The project follows the **Strategy Design Pattern**, allowing pricing algorithms to be changed without modifying the core ride-management logic.

### 🚘 Vehicle Management

Supports multiple vehicle types:

* 🚗 Sedan
* 🚙 SUV
* 🛺 Auto
* 🏍️ Bike

Vehicles are created using a **Factory Design Pattern**.

### 🔔 Notifications

A dedicated notification service is included for handling ride-related notifications and future communication features.

### 🌐 Web Frontend

The project includes a web-based frontend built using:

* HTML
* CSS
* JavaScript

The frontend communicates with the backend and provides the interface for interacting with the ride-booking system.

---

## 🏗️ System Architecture

```text
                    ┌──────────────────────┐
                    │      Frontend        │
                    │   HTML / CSS / JS    │
                    └──────────┬───────────┘
                               │
                               ▼
                    ┌──────────────────────┐
                    │      Node.js         │
                    │       Backend        │
                    └──────────┬───────────┘
                               │
                               ▼
              ┌────────────────────────────────┐
              │          C++ Core System        │
              │                                │
              │  Ride Service                  │
              │  Driver Matching               │
              │  Route Service                 │
              │  Pricing Service               │
              │  Notification Service          │
              └───────────────┬────────────────┘
                              │
          ┌───────────────────┼───────────────────┐
          ▼                   ▼                   ▼
     Route Graph        Driver Matching      Pricing Engine
          │                   │                   │
          ▼                   ▼                   ▼
    City / Location      Driver / Rider     Normal / Pool /
    / Edge System        Management         Surge Pricing
```

---

## 🧠 Core Concepts Demonstrated

RideFlow is primarily designed to demonstrate practical **software engineering and DSA concepts**.

### Data Structures & Algorithms

* Graphs
* Nodes and edges
* Route calculation
* Driver matching
* Searching and traversal
* Object-oriented data modeling

### Object-Oriented Programming

The C++ implementation uses:

* Classes and objects
* Encapsulation
* Abstraction
* Inheritance
* Polymorphism
* Composition

### Design Patterns

The project implements concepts inspired by commonly used software design patterns.

#### Factory Pattern

Used for creating different vehicle types.

```text
VehicleFactory
      │
      ├── Bike
      ├── Auto
      ├── Sedan
      └── SUV
```

#### Strategy Pattern

Used for supporting different pricing algorithms.

```text
PricingStrategy
      │
      ├── NormalPricing
      ├── PoolPricing
      └── SurgePricing
```

This makes the pricing system easier to modify and extend.

---

## 📂 Project Structure

```text
RideFlow/
│
├── backend/
│   └── server.js
│
├── frontend/
│   ├── index.html
│   ├── styles.css
│   └── app.js
│
├── src/
│   │
│   ├── domain/
│   │   ├── Driver.cpp
│   │   ├── Rider.cpp
│   │   └── Ride.cpp
│   │
│   ├── factory/
│   │   └── VehicleFactory.cpp
│   │
│   ├── graph/
│   │   ├── CityGraph.cpp
│   │   ├── Edge.cpp
│   │   ├── Location.cpp
│   │   └── RouteResult.cpp
│   │
│   ├── matching/
│   │   └── DriverMatch.cpp
│   │
│   ├── pricing/
│   │   ├── PricingStrategy.cpp
│   │   ├── NormalPricing.cpp
│   │   ├── PoolPricing.cpp
│   │   └── SurgePricing.cpp
│   │
│   ├── service/
│   │   ├── DriverMatchingService.cpp
│   │   ├── NotificationService.cpp
│   │   ├── PricingService.cpp
│   │   ├── RideService.cpp
│   │   └── RouteService.cpp
│   │
│   └── vehicle/
│       ├── Vehicle.cpp
│       ├── Bike.cpp
│       ├── Auto.cpp
│       ├── Sedan.cpp
│       └── SUV.cpp
│
├── Main.cpp
├── CMakeLists.txt
├── package.json
├── .gitignore
└── README.md
```

---

## 🛠️ Tech Stack

| Technology       | Purpose                 |
| ---------------- | ----------------------- |
| **C++**          | Core ride-booking logic |
| **CMake**        | C++ build system        |
| **Node.js**      | Backend/server layer    |
| **JavaScript**   | Frontend interaction    |
| **HTML5**        | Frontend structure      |
| **CSS3**         | Frontend styling        |
| **Git & GitHub** | Version control         |

---

## ⚙️ How to Run

### 1. Clone the Repository

```bash
git clone https://github.com/AnviJha/RideFlow.git
cd RideFlow
```

### 2. Build the C++ Application

Make sure **CMake** and a **C++ compiler** are installed.

Create a build directory:

```bash
mkdir build
cd build
```

Configure the project:

```bash
cmake ..
```

Build the project:

```bash
cmake --build .
```

### 3. Run the Application

On Windows:

```powershell
.\UberRidePlatform.exe
```

The exact executable location may depend on the selected CMake generator and build configuration.

---

## 🌐 Running the Backend

Install the Node.js dependencies:

```bash
npm install
```

Start the backend:

```bash
node backend/server.js
```

The backend can then be used by the frontend for application communication.

---

## 🔄 Application Flow

```text
User
 │
 ▼
Enter Pickup & Destination
 │
 ▼
Create Ride Request
 │
 ▼
Find Available Drivers
 │
 ▼
Driver Matching
 │
 ▼
Calculate Route
 │
 ▼
Select Pricing Strategy
 │
 ├── Normal Pricing
 ├── Pool Pricing
 └── Surge Pricing
 │
 ▼
Calculate Fare
 │
 ▼
Assign Driver
 │
 ▼
Start Ride
 │
 ▼
Complete Ride
```

---

## 🎯 Why RideFlow?

RideFlow is more than a basic CRUD application. It focuses on the **engineering problems behind a ride-hailing platform**:

* How can drivers be matched with riders?
* How can routes be represented efficiently?
* How can pricing algorithms be switched dynamically?
* How can different vehicle types be created without tightly coupling the system?
* How can individual services be separated into maintainable components?

The project provides practical experience with **DSA + OOP + Design Patterns + Backend Development**.

---

## 🚀 Future Improvements

The current project provides the foundation for a larger ride-hailing application. Planned improvements include:

* [ ] Modern responsive RideFlow dashboard
* [ ] Interactive map integration
* [ ] Real-time driver location tracking
* [ ] User authentication
* [ ] Driver authentication
* [ ] Ride history
* [ ] Online payment integration
* [ ] Real-time ride status
* [ ] Improved driver-matching algorithm
* [ ] ETA prediction
* [ ] Database integration
* [ ] REST API improvements
* [ ] Mobile-responsive UI
* [ ] Deployment using Docker
* [ ] Cloud deployment
* [ ] Automated testing
* [ ] CI/CD pipeline

---

## 📊 Skills Demonstrated

### Programming

* C++
* JavaScript
* Node.js

### Computer Science

* Data Structures & Algorithms
* Object-Oriented Programming
* Graph Algorithms
* Software Architecture
* Design Patterns

### Development

* Backend Development
* Frontend Development
* REST API concepts
* CMake
* Git/GitHub

---

## 👩‍💻 Author

**Anvi Jha**

B.Tech — Computer Science & Data Science

GitHub:
https://github.com/AnviJha

---

## ⭐ Support

If you find this project useful or interesting, consider giving the repository a ⭐ on GitHub!

---

## 📄 License

This project is created for educational and portfolio purposes.

