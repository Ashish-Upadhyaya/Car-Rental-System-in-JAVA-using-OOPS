// CarRentalSystem.java
// Java-based car rental management system using Oracle database

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.Scanner;

public class CarRentalSystem {
    private Connection conn;     // Database connection object
    private Scanner scanner;     // User input scanner

    // Constructor to initialize database connection and scanner
    public CarRentalSystem() {
        try {
            // Load Oracle JDBC driver
            Class.forName("oracle.jdbc.driver.OracleDriver");
            // Establish database connection (update credentials)
            conn = DriverManager.getConnection(
                "jdbc:oracle:thin:@localhost:1521:XE", "Username", "Password");
            scanner = new Scanner(System.in);
        } catch (ClassNotFoundException | SQLException e) {
            System.out.println("Connection error: " + e.getMessage());
        }
    }

    // Main program loop with user menu
    public void run() {
        int choice;
        do {
            // Display menu options
            System.out.println("\nCar Rental System");
            System.out.println("1. Add Vehicle");
            System.out.println("2. Remove Vehicle");
            System.out.println("3. Add Customer");
            System.out.println("4. Remove Customer");
            System.out.println("5. Rent Vehicle");
            System.out.println("6. Return Vehicle");
            System.out.println("7. Pay Rental");
            System.out.println("8. View Available Vehicles");
            System.out.println("9. View Customer Rentals");
            System.out.println("10. Exit");
            
            // Handle user input and menu selection
            choice = scanner.nextInt();
            switch (choice) {
                case 1: addVehicle(); break;
                case 2: removeVehicle(); break;
                case 3: addCustomer(); break;
                case 4: removeCustomer(); break;
                case 5: rentVehicle(); break;
                case 6: returnVehicle(); break;
                case 7: payRental(); break;
                case 8: viewAvailableVehicles(); break;
                case 9: viewCustomerRentals(); break;
                case 10: System.out.println("Exiting..."); break;
                default: System.out.println("Invalid choice");
            }
        } while (choice != 10);
    }

    // Add new vehicle to database
    private void addVehicle() {
        System.out.print("Enter make: ");
        String make = scanner.next();
        System.out.print("Enter model: ");
        String model = scanner.next();
        System.out.print("Enter year: ");
        int year = scanner.nextInt();
        System.out.print("Enter daily rate: ");
        double rate = scanner.nextDouble();

        // Insert new vehicle record using sequence for ID
        try (PreparedStatement pstmt = conn.prepareStatement(
            "INSERT INTO VEHICLES VALUES (VEHICLE_SEQ.NEXTVAL,?,?,?,?,0)")) {
            pstmt.setString(1, make);
            pstmt.setString(2, model);
            pstmt.setInt(3, year);
            pstmt.setDouble(4, rate);
            pstmt.executeUpdate();
            System.out.println("Vehicle added successfully");
        } catch (SQLException e) {
            System.out.println("Error: " + e.getMessage());
        }
    }

    // Remove vehicle by ID
    private void removeVehicle() {
        System.out.print("Enter vehicle ID: ");
        int id = scanner.nextInt();

        // Delete vehicle record
        try (PreparedStatement pstmt = conn.prepareStatement(
            "DELETE FROM VEHICLES WHERE VEHICLE_ID = ?")) {
            pstmt.setInt(1, id);
            pstmt.executeUpdate();
            System.out.println("Vehicle removed");
        } catch (SQLException e) {
            System.out.println("Error: " + e.getMessage());
        }
    }

    // Add new customer to database
    private void addCustomer() {
        System.out.print("Enter name: ");
        String name = scanner.next();
        System.out.print("Enter email: ");
        String email = scanner.next();
        System.out.print("Enter phone: ");
        String phone = scanner.next();

        // Insert new customer using sequence for ID
        try (PreparedStatement pstmt = conn.prepareStatement(
            "INSERT INTO CUSTOMERS VALUES (CUSTOMER_SEQ.NEXTVAL,?,?,?)")) {
            pstmt.setString(1, name);
            pstmt.setString(2, email);
            pstmt.setString(3, phone);
            pstmt.executeUpdate();
            System.out.println("Customer added");
        } catch (SQLException e) {
            System.out.println("Error: " + e.getMessage());
        }
    }

    // Remove customer by ID
    private void removeCustomer() {
        System.out.print("Enter customer ID: ");
        int id = scanner.nextInt();

        // Delete customer record
        try (PreparedStatement pstmt = conn.prepareStatement(
            "DELETE FROM CUSTOMERS WHERE CUSTOMER_ID = ?")) {
            pstmt.setInt(1, id);
            pstmt.executeUpdate();
            System.out.println("Customer removed");
        } catch (SQLException e) {
            System.out.println("Error: " + e.getMessage());
        }
    }

    // Create new rental record
    private void rentVehicle() {
        System.out.print("Enter vehicle ID: ");
        int vehicleId = scanner.nextInt();
        System.out.print("Enter customer ID: ");
        int customerId = scanner.nextInt();
        System.out.print("Enter rental days: ");
        int days = scanner.nextInt();

        // Calculate cost and insert rental record
        double cost = calculateRentalCost(vehicleId, days);
        try (PreparedStatement pstmt = conn.prepareStatement(
            "INSERT INTO RENTALS VALUES (RENTAL_SEQ.NEXTVAL,?,?,?,?)")) {
            pstmt.setInt(1, vehicleId);
            pstmt.setInt(2, customerId);
            pstmt.setInt(3, days);
            pstmt.setDouble(4, cost);
            pstmt.executeUpdate();
            System.out.println("Rental created. Total cost: " + cost);
        } catch (SQLException e) {
            System.out.println("Error: " + e.getMessage());
        }
    }

    // Mark rental as returned
    private void returnVehicle() {
        System.out.print("Enter rental ID: ");
        int id = scanner.nextInt();

        // Update returned status
        try (PreparedStatement pstmt = conn.prepareStatement(
            "UPDATE RENTALS SET RETURNED = 1 WHERE RENTAL_ID = ?")) {
            pstmt.setInt(1, id);
            pstmt.executeUpdate();
            System.out.println("Vehicle returned");
        } catch (SQLException e) {
            System.out.println("Error: " + e.getMessage());
        }
    }

    // Process rental payment
    private void payRental() {
        System.out.print("Enter rental ID: ");
        int id = scanner.nextInt();

        // Verify and update payment status
        try (PreparedStatement pstmt = conn.prepareStatement(
            "SELECT TOTAL_COST FROM RENTALS WHERE RENTAL_ID = ?")) {
            pstmt.setInt(1, id);
            ResultSet rs = pstmt.executeQuery();
            if (rs.next()) {
                double cost = rs.getDouble("TOTAL_COST");
                System.out.println("Total cost: " + cost);
                try (PreparedStatement update = conn.prepareStatement(
                    "UPDATE RENTALS SET PAID = 1 WHERE RENTAL_ID = ?")) {
                    update.setInt(1, id);
                    update.executeUpdate();
                    System.out.println("Payment processed");
                }
            } else {
                System.out.println("Rental not found");
            }
        } catch (SQLException e) {
            System.out.println("Error: " + e.getMessage());
        }
    }

    // Show available vehicles
    private void viewAvailableVehicles() {
        try (PreparedStatement pstmt = conn.prepareStatement(
            "SELECT * FROM VEHICLES WHERE RENTED = 0")) {
            ResultSet rs = pstmt.executeQuery();
            while (rs.next()) {
                System.out.println("\nID: " + rs.getInt("VEHICLE_ID"));
                System.out.println("Make: " + rs.getString("MAKE"));
                System.out.println("Model: " + rs.getString("MODEL"));
                System.out.println("Year: " + rs.getInt("YEAR"));
                System.out.println("Rate: " + rs.getDouble("RENTAL_RATE"));
            }
        } catch (SQLException e) {
            System.out.println("Error: " + e.getMessage());
        }
    }

    // Show active rentals for customer
    private void viewCustomerRentals() {
        System.out.print("Enter customer ID: ");
        int id = scanner.nextInt();

        try (PreparedStatement pstmt = conn.prepareStatement(
            "SELECT * FROM RENTALS WHERE CUSTOMER_ID = ? AND RETURNED = 0")) {
            pstmt.setInt(1, id);
            ResultSet rs = pstmt.executeQuery();
            while (rs.next()) {
                System.out.println("\nRental ID: " + rs.getInt("RENTAL_ID"));
                System.out.println("Vehicle ID: " + rs.getInt("VEHICLE_ID"));
                System.out.println("Duration: " + rs.getInt("RENTAL_DURATION"));
                System.out.println("Cost: " + rs.getDouble("TOTAL_COST"));
            }
        } catch (SQLException e) {
            System.out.println("Error: " + e.getMessage());
        }
    }

    // Calculate rental cost based on duration
    private double calculateRentalCost(int vehicleId, int days) {
        try (PreparedStatement pstmt = conn.prepareStatement(
            "SELECT RENTAL_RATE FROM VEHICLES WHERE VEHICLE_ID = ?")) {
            pstmt.setInt(1, vehicleId);
            ResultSet rs = pstmt.executeQuery();
            if (rs.next()) {
                return rs.getDouble("RENTAL_RATE") * days;
            }
        } catch (SQLException e) {
            System.out.println("Error: " + e.getMessage());
        }
        return 0;
    }

    // Application entry point
    public static void main(String[] args) {
        new CarRentalSystem().run();
    }
}