USE car_company;

INSERT INTO Brand (name) VALUES 
('Hyundai'), ('Kia'), ('Genesis'), ('Toyota'), ('Honda'),
('Ford'), ('Chevrolet'), ('BMW'), ('Mercedes-Benz'), ('Audi');

INSERT INTO Dealer (name, address) VALUES 
('Seoul Central Auto', '123 Gangnam-daero, Seoul'),
('Busan Motors', '456 Haeundae-ro, Busan'),
('Incheon Car Dealership', '789 Songdo-dong, Incheon'),
('Daegu Auto Hub', '101 Dongdaegu-ro, Daegu'),
('Daejeon Mobility', '202 Dunsan-daero, Daejeon'),
('Gwangju Motors', '303 Sangmu-daero, Gwangju'),
('Ulsan Premium Cars', '404 Samsan-ro, Ulsan'),
('Suwon Car Center', '505 Maesan-ro, Suwon'),
('Changwon Dealership', '606 Jungang-daero, Changwon'),
('Goyang Auto Square', '707 Kintex-ro, Goyang');

INSERT INTO Supplier (name, contact_info) VALUES 
('Hankook Tire', 'contact@hankook.com'),
('Kumho Tire', 'info@kumho.com'),
('Bosch Korea', 'info@bosch.co.kr'),
('Mobis', 'parts@mobis.com'),
('Denso', 'sales@denso.com'),
('Magna International', 'contact@magna.com'),
('ZF Friedrichshafen', 'support@zf.com'), 
('Continental', 'info@continental.com'),
('Aisin', 'parts@aisin.com'),
('Valeo', 'contact@valeo.com');

INSERT INTO Customer (name, address, phone, gender, annual_income) VALUES 
('Alice Smith', '101 Apple St, Seoul', '010-1111-2222', 'Female', 85000),
('Bob Johnson', '202 Banana Ave, Busan', '010-3333-4444', 'Male', 45000),
('Charlie Kim', '303 Cherry Blvd, Incheon', '010-5555-6666', 'Male', 120000),
('Diana Lee', '404 Date St, Daegu', '010-7777-8888', 'Female', 62000),
('Evan Park', '505 Fig Ave, Daejeon', '010-9999-0000', 'Male', 150000),
('Fiona Choi', '606 Grape Blvd, Gwangju', '010-1234-5678', 'Female', 38000),
('George Jung', '707 Hazel St, Ulsan', '010-8765-4321', 'Male', 95000),
('Hannah Kang', '808 Ivy Ave, Suwon', '010-2345-6789', 'Female', 110000),
('Ian Cho', '909 Jack St, Changwon', '010-9876-5432', 'Male', 72000),
('Julia Yoon', '1010 Kiwi Blvd, Goyang', '010-3456-7890', 'Female', 54000);

INSERT INTO Model (name, year, base_price, style, brand_id) VALUES 
('Sonata', 2025, 30000, 'Sedan', 1),
('Sorento', 2025, 40000, 'SUV', 2),
('G80', 2026, 60000, 'Sedan', 3),
('Camry', 2024, 32000, 'Sedan', 4),
('CR-V', 2025, 35000, 'SUV', 5),
('Mustang', 2025, 55000, 'Convertible', 6),
('Silverado', 2024, 45000, 'Truck', 7),
('Z4', 2026, 65000, 'Convertible', 8),  
('E-Class', 2025, 70000, 'Sedan', 9),
('Q7', 2026, 75000, 'SUV', 10);

INSERT INTO Plant (name, location, type, supplier_id) VALUES 
('Ulsan Assembly', 'Ulsan', 'final assembly', NULL),
('Hwaseong Assembly', 'Hwaseong', 'final assembly', NULL),
('Gwangmyeong Assembly', 'Gwangmyeong', 'final assembly', NULL),
('Kentucky Plant', 'USA', 'final assembly', NULL),
('Munich Plant', 'Germany', 'final assembly', NULL),
('Mobis Asan Center', 'Asan', 'parts', 4),
('ZF Transmission Factory', 'Germany', 'parts', 7),
('Bosch Electronics', 'Daejeon', 'parts', 3),
('Hankook Geumsan', 'Geumsan', 'parts', 1),
('Denso Changwon', 'Changwon', 'parts', 5);

INSERT INTO Part (name, date, plant_id) VALUES 
('V6 Engine', '2024-01-15', 6),
('18inch Alloy Wheel', '2024-02-20', 9),
('LED Headlight', '2024-03-10', 8),
('8-speed Transmission', '2024-05-01', 7), 
('6-speed Transmission', '2024-05-15', 7), 
('Brake Pad', '2024-06-20', 8),
('Airbag Module', '2024-07-11', 6),
('AC Compressor', '2024-08-30', 10),
('Navigation System', '2024-09-15', 6),
('EV Battery Pack', '2024-10-05', 8);

INSERT INTO Vehicle (VIN, cur_state, color, engine, transmission, manufacturing_date, model_id, plant_id, dealer_id) VALUES 
('VIN00000000000001', 'sold', 'Black', '2.5L', '8-speed', '2024-01-20', 1, 1, 1),
('VIN00000000000002', 'sold', 'White', '2.0L', '8-speed', '2024-06-10', 8, 5, 2), 
('VIN00000000000003', 'sold', 'Silver', '3.5L', '8-speed', '2024-06-15', 6, 4, 3), 
('VIN00000000000004', 'sold', 'Red', '1.6L', '6-speed', '2024-07-01', 2, 2, 4),
('VIN00000000000005', 'sold', 'Blue', '3.0L', '8-speed', '2025-02-10', 3, 1, 5),
('VIN00000000000006', 'sold', 'Black', '2.5L', 'eCVT', '2025-03-20', 4, 4, 6),
('VIN00000000000007', 'sold', 'White', '2.0L', 'CVT', '2025-08-15', 5, 4, 7),
('VIN00000000000008', 'sold', 'Gray', '3.0L', '8-speed', '2026-01-10', 9, 5, 8),
('VIN00000000000009', 'sold', 'Black', '4.0L', '8-speed', '2026-02-05', 10, 5, 9),
('VIN00000000000010', 'sold', 'Yellow', '2.5L', '8-speed', '2026-03-12', 1, 1, 10),
('VIN00000000000011', 'stock', 'White', '2.5L', '8-speed', '2025-01-10', 1, 1, 1), 
('VIN00000000000012', 'stock', 'Black', '3.5L', '8-speed', '2026-05-01', 3, 1, 2),
('VIN00000000000013', 'stock', 'Red', '2.0L', '8-speed', '2026-05-15', 6, 4, 3),
('VIN00000000000014', 'in production', 'Blue', '3.0L', '8-speed', '2026-06-01', 9, 5, NULL),
('VIN00000000000015', 'in production', 'Silver', '1.6L', '6-speed', '2026-06-10', 2, 2, NULL);

INSERT INTO Sale (date, pay_method, sale_price, VIN, customer_id, dealer_id) VALUES 
('2024-02-15', 'Credit Card', 29000, 'VIN00000000000001', 1, 1),
('2024-07-20', 'Bank Transfer', 64000, 'VIN00000000000002', 3, 2), 
('2024-08-05', 'Finance', 54000, 'VIN00000000000003', 5, 3),      
('2024-09-10', 'Credit Card', 39000, 'VIN00000000000004', 2, 4),
('2025-03-01', 'Bank Transfer', 59000, 'VIN00000000000005', 7, 5),
('2025-04-15', 'Finance', 31000, 'VIN00000000000006', 4, 6),
('2025-09-20', 'Credit Card', 34000, 'VIN00000000000007', 6, 7),
('2026-02-15', 'Bank Transfer', 69000, 'VIN00000000000008', 8, 8),
('2026-03-10', 'Finance', 74000, 'VIN00000000000009', 10, 9),
('2026-04-05', 'Credit Card', 30000, 'VIN00000000000010', 9, 10);

INSERT INTO Vehicle_Part (VIN, part_id) VALUES 
('VIN00000000000001', 1), ('VIN00000000000001', 2),
('VIN00000000000002', 4), 
('VIN00000000000003', 4), 
('VIN00000000000004', 5), 
('VIN00000000000005', 1), ('VIN00000000000005', 3),
('VIN00000000000006', 6), ('VIN00000000000007', 7),
('VIN00000000000008', 8), ('VIN00000000000009', 9),
('VIN00000000000010', 2), ('VIN00000000000011', 1),
('VIN00000000000012', 3), ('VIN00000000000013', 4);