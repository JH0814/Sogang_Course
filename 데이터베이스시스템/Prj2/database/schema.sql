DROP DATABASE IF EXISTS car_company;
CREATE DATABASE car_company;
USE car_company;

CREATE TABLE Brand (
    brand_id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(50) NOT NULL
);

CREATE TABLE Dealer (
    dealer_id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(50) NOT NULL,
    address VARCHAR(255)
);

CREATE TABLE Supplier (
    supplier_id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(50) NOT NULL,
    contact_info VARCHAR(100)
);

CREATE TABLE Customer (
    customer_id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(50) NOT NULL,
    address VARCHAR(255),
    phone VARCHAR(20),
    gender VARCHAR(10) CHECK (gender IN ('Male', 'Female')),
    annual_income INT CHECK (annual_income >= 0)
);

CREATE TABLE Model (
    model_id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(50) NOT NULL,
    year INT NOT NULL,
    base_price INT NOT NULL CHECK (base_price > 0),
    style VARCHAR(30),
    brand_id INT,
    FOREIGN KEY (brand_id) REFERENCES Brand(brand_id) ON DELETE SET NULL
);

CREATE TABLE Plant (
    plant_id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(50) NOT NULL,
    location VARCHAR(100),
    type VARCHAR(20),
    supplier_id INT,
    FOREIGN KEY (supplier_id) REFERENCES Supplier(supplier_id) ON DELETE SET NULL
);

CREATE TABLE Part (
    part_id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(50) NOT NULL,
    date DATE,
    plant_id INT,
    FOREIGN KEY (plant_id) REFERENCES Plant(plant_id) ON DELETE SET NULL
);

CREATE TABLE Vehicle (
    VIN VARCHAR(17) PRIMARY KEY,
    cur_state VARCHAR(20) NOT NULL,
    color VARCHAR(30),
    engine VARCHAR(50),
    transmission VARCHAR(50),
    manufacturing_date DATE,
    model_id INT,
    plant_id INT,
    dealer_id INT,
    FOREIGN KEY (model_id) REFERENCES Model(model_id) ON DELETE RESTRICT,
    FOREIGN KEY (plant_id) REFERENCES Plant(plant_id) ON DELETE SET NULL,
    FOREIGN KEY (dealer_id) REFERENCES Dealer(dealer_id) ON DELETE SET NULL
);

CREATE TABLE Sale (
    sale_id INT PRIMARY KEY AUTO_INCREMENT,
    date DATE NOT NULL,
    pay_method VARCHAR(20),
    sale_price INT NOT NULL,
    VIN VARCHAR(17) NOT NULL UNIQUE,
    customer_id INT,
    dealer_id INT,
    FOREIGN KEY (VIN) REFERENCES Vehicle(VIN) ON DELETE CASCADE,
    FOREIGN KEY (customer_id) REFERENCES Customer(customer_id) ON DELETE SET NULL,
    FOREIGN KEY (dealer_id) REFERENCES Dealer(dealer_id) ON DELETE SET NULL
);

CREATE TABLE Vehicle_Part (
    VIN VARCHAR(17),
    part_id INT,
    PRIMARY KEY (VIN, part_id),
    FOREIGN KEY (VIN) REFERENCES Vehicle(VIN) ON DELETE CASCADE,
    FOREIGN KEY (part_id) REFERENCES Part(part_id) ON DELETE CASCADE
);

CREATE INDEX idx_sale_date ON Sale(date);
CREATE INDEX idx_supplier_name ON Supplier(name);