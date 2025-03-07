# Car-Rental-System-in-JAVA-using-OOPS

## Overview

This is a Java-based Car Rental Management System that uses Oracle Database for data storage. The system provides a complete solution for managing vehicle rentals, customers, and rental transactions.

## Features

- **Vehicle Management**
  - Add new vehicles to the system
  - Remove vehicles from the system
  - View available vehicles
  
- **Customer Management**
  - Add new customer records
  - Remove customer records
  
- **Rental Operations**
  - Rent vehicles to customers
  - Return rented vehicles
  - Process rental payments
  - View customer rentals
  
- **Database Integration**
  - Uses Oracle database for persistent storage
  - Utilizes sequences for automatic ID generation

## Requirements

- Java Development Kit (JDK) 8 or higher
- Oracle Database 10g or higher
- Oracle JDBC driver (ojdbc8.jar)
- Maven (optional)

## Setup Instructions

1. **Database Setup**
   - Create an Oracle database schema
   - Create the required tables using the following SQL script:
     ```sql
     CREATE TABLE VEHICLES (
         VEHICLE_ID NUMBER PRIMARY KEY,
         MAKE VARCHAR2(50),
         MODEL VARCHAR2(50),
         YEAR NUMBER,
         RENTAL_RATE NUMBER,
         RENTED NUMBER(1)
     );
     
     CREATE TABLE CUSTOMERS (
         CUSTOMER_ID NUMBER PRIMARY KEY,
         NAME VARCHAR2(100),
         EMAIL VARCHAR2(100),
         PHONE_NUMBER VARCHAR2(20)
     );
     
     CREATE TABLE RENTALS (
         RENTAL_ID NUMBER PRIMARY KEY,
         VEHICLE_ID NUMBER,
         CUSTOMER_ID NUMBER,
         RENTAL_DURATION NUMBER,
         TOTAL_COST NUMBER,
         RETURNED NUMBER(1),
         PAID NUMBER(1)
     );
     
     CREATE SEQUENCE VEHICLE_SEQ START WITH 1;
     CREATE SEQUENCE CUSTOMER_SEQ START WITH 1;
     CREATE SEQUENCE RENTAL_SEQ START WITH 1;
     ```
   
2. **Project Setup**
   - Clone the repository
   - Update database credentials in `CarRentalSystem.java`
   - Add ojdbc8.jar to your project's classpath

3. **Running the Application**
   - Compile the project: `javac CarRentalSystem.java`
   - Run the application: `java CarRentalSystem`

## Usage

1. Launch the application
2. Use the menu-driven interface to:
   - Manage vehicles
   - Manage customers
   - Handle rental operations
3. Follow the on-screen prompts for each operation

## Database Schema

### VEHICLES Table
| Column Name    | Data Type      | Description               |
|----------------|----------------|---------------------------|
| VEHICLE_ID     | NUMBER         | Unique vehicle ID         |
| MAKE           | VARCHAR2(50)   | Vehicle make              |
| MODEL          | VARCHAR2(50)   | Vehicle model             |
| YEAR           | NUMBER         | Vehicle year              |
| RENTAL_RATE    | NUMBER         | Daily rental rate         |
| RENTED         | NUMBER(1)      | Rental status (0/1)       |

### CUSTOMERS Table
| Column Name    | Data Type      | Description               |
|----------------|----------------|---------------------------|
| CUSTOMER_ID    | NUMBER         | Unique customer ID        |
| NAME           | VARCHAR2(100)  | Customer name             |
| EMAIL          | VARCHAR2(100)  | Customer email            |
| PHONE_NUMBER   | VARCHAR2(20)   | Customer phone number     |

### RENTALS Table
| Column Name       | Data Type      | Description               |
|-------------------|----------------|---------------------------|
| RENTAL_ID         | NUMBER         | Unique rental ID          |
| VEHICLE_ID        | NUMBER         | Associated vehicle ID     |
| CUSTOMER_ID       | NUMBER         | Associated customer ID    |
| RENTAL_DURATION   | NUMBER         | Rental duration in days   |
| TOTAL_COST        | NUMBER         | Total rental cost         |
| RETURNED          | NUMBER(1)      | Return status (0/1)       |
| PAID              | NUMBER(1)      | Payment status (0/1)      |

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License

Distributed under the MIT License. See `LICENSE` for more information.

## Contact

Ashish Kumar Upadhyaya - [ashishofficial231@gmail.com](mailto:ashishofficial231@gmail.com)

Project Link: [https://github.com/yourusername/car-rental-system](https://github.com/yourusername/car-rental-system)

## Acknowledgements

- Oracle Corporation for JDBC drivers
- Java Development Community
