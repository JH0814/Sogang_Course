#include <iostream>
#include <string>
#include <limits>
#include <cstdio>
#include <cstring> // For memset, strlen
#include <mysql.h> // ODBC 대신 MySQL C API 헤더 사용

// 에러 출력을 위한 유틸리티 함수
void ShowMySQLError(MYSQL *conn) {
    std::cerr << "[MySQL Error] " << mysql_error(conn) 
              << " (Error No: " << mysql_errno(conn) << ")\n" << std::endl;
}

void ShowStmtError(MYSQL_STMT *stmt) {
    std::cerr << "[MySQL Stmt Error] " << mysql_stmt_error(stmt) 
              << " (Error No: " << mysql_stmt_errno(stmt) << ")\n" << std::endl;
}

int main() {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    // 1. MySQL 핸들 초기화
    conn = mysql_init(NULL);
    if (conn == NULL) {
        std::cerr << "mysql_init() failed" << std::endl;
        return 1;
    }

    std::cout << "Connecting to database..." << std::endl;

    // 2. 데이터베이스 연결 (ODBC DSN 방식 -> 직접 연결 방식)
    if (mysql_real_connect(conn, "localhost", "admin", "1234", "car_company", 3306, NULL, 0) == NULL) {
        std::cerr << "Database Connection Failed! Please check your credentials." << std::endl;
        ShowMySQLError(conn);
        mysql_close(conn);
        return 1;
    }

    // 한글 등 다국어 처리를 위해 UTF-8 설정
    mysql_set_character_set(conn, "utf8mb4");
    std::cout << "Successfully connected to the Car Company Database!\n" << std::endl;

    while (true) {
        // Main Menu UI
        std::cout << "\n====== Car Company Analytics System ======" << std::endl;
        std::cout << "1. Sales Trends (Past 3 Years, Demographic)" << std::endl;
        std::cout << "2. Defective Part Tracking (Secure Parameterized)" << std::endl;
        std::cout << "3. Top 2 Brands by Revenue (Past Year)" << std::endl;
        std::cout << "4. Top 2 Brands by Unit Sales (Past Year)" << std::endl;
        std::cout << "5. Seasonal Sales Patterns (Secure Parameterized)" << std::endl;
        std::cout << "6. Dealer Inventory Efficiency" << std::endl;
        std::cout << "7. Supplier Coverage Analysis" << std::endl;
        std::cout << "8. Exit Application" << std::endl;
        std::cout << "==========================================" << std::endl;
        std::cout << "Select menu (1-8): ";
        
        int choice;
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            std::cout << "Invalid input. Please enter a number.\n" << std::endl;
            continue;
        }

        if (choice == 8) {
            std::cout << "Exiting application gracefully." << std::endl;
            break;
        }

        if (choice == 1) {
            const char* titles[6] = {
                "\n[1/6] Sales Trends: Year & Gender",
                "\n[2/6] Sales Trends: Year & Income Range",
                "\n[3/6] Sales Trends: Month & Gender",
                "\n[4/6] Sales Trends: Month & Income Range",
                "\n[5/6] Sales Trends: Week & Gender",
                "\n[6/6] Sales Trends: Week & Income Range"
            };
            const char* time_cols[6] = {"Year", "Year", "Month  ", "Month  ", "Week   ", "Week   "};
            const char* demo_cols[6] = {"Gender", "Income", "Gender", "Income", "Gender", "Income"};
            
            const char* queries[6] = {
                // 1. Year & Gender
                "SELECT B.name, YEAR(S.date), C.gender, COUNT(S.sale_id), SUM(S.sale_price) "
                "FROM Sale S JOIN Vehicle V ON S.VIN = V.VIN JOIN Model M ON V.model_id = M.model_id "
                "JOIN Brand B ON M.brand_id = B.brand_id JOIN Customer C ON S.customer_id = C.customer_id "
                "WHERE S.date >= DATE_SUB(CURDATE(), INTERVAL 3 YEAR) "
                "GROUP BY B.name, YEAR(S.date), C.gender ORDER BY YEAR(S.date) DESC, B.name",
                
                // 2. Year & Income
                "SELECT B.name, YEAR(S.date), "
                "CASE WHEN C.annual_income < 50000 THEN 'Low' WHEN C.annual_income BETWEEN 50000 AND 100000 THEN 'Medium' ELSE 'High' END, "
                "COUNT(S.sale_id), SUM(S.sale_price) "
                "FROM Sale S JOIN Vehicle V ON S.VIN = V.VIN JOIN Model M ON V.model_id = M.model_id "
                "JOIN Brand B ON M.brand_id = B.brand_id JOIN Customer C ON S.customer_id = C.customer_id "
                "WHERE S.date >= DATE_SUB(CURDATE(), INTERVAL 3 YEAR) "
                "GROUP BY B.name, YEAR(S.date), CASE WHEN C.annual_income < 50000 THEN 'Low' WHEN C.annual_income BETWEEN 50000 AND 100000 THEN 'Medium' ELSE 'High' END "
                "ORDER BY YEAR(S.date) DESC, B.name",
                
                // 3. Month & Gender
                "SELECT B.name, DATE_FORMAT(S.date, '%Y-%m'), C.gender, COUNT(S.sale_id), SUM(S.sale_price) "
                "FROM Sale S JOIN Vehicle V ON S.VIN = V.VIN JOIN Model M ON V.model_id = M.model_id "
                "JOIN Brand B ON M.brand_id = B.brand_id JOIN Customer C ON S.customer_id = C.customer_id "
                "WHERE S.date >= DATE_SUB(CURDATE(), INTERVAL 3 YEAR) "
                "GROUP BY B.name, DATE_FORMAT(S.date, '%Y-%m'), C.gender ORDER BY DATE_FORMAT(S.date, '%Y-%m') DESC, B.name",
                
                // 4. Month & Income
                "SELECT B.name, DATE_FORMAT(S.date, '%Y-%m'), "
                "CASE WHEN C.annual_income < 50000 THEN 'Low' WHEN C.annual_income BETWEEN 50000 AND 100000 THEN 'Medium' ELSE 'High' END, "
                "COUNT(S.sale_id), SUM(S.sale_price) "
                "FROM Sale S JOIN Vehicle V ON S.VIN = V.VIN JOIN Model M ON V.model_id = M.model_id "
                "JOIN Brand B ON M.brand_id = B.brand_id JOIN Customer C ON S.customer_id = C.customer_id "
                "WHERE S.date >= DATE_SUB(CURDATE(), INTERVAL 3 YEAR) "
                "GROUP BY B.name, DATE_FORMAT(S.date, '%Y-%m'), CASE WHEN C.annual_income < 50000 THEN 'Low' WHEN C.annual_income BETWEEN 50000 AND 100000 THEN 'Medium' ELSE 'High' END "
                "ORDER BY DATE_FORMAT(S.date, '%Y-%m') DESC, B.name",

                // 5. Week & Gender
                "SELECT B.name, CONCAT(YEAR(S.date), '-W', LPAD(WEEK(S.date), 2, '0')), C.gender, COUNT(S.sale_id), SUM(S.sale_price) "
                "FROM Sale S JOIN Vehicle V ON S.VIN = V.VIN JOIN Model M ON V.model_id = M.model_id "
                "JOIN Brand B ON M.brand_id = B.brand_id JOIN Customer C ON S.customer_id = C.customer_id "
                "WHERE S.date >= DATE_SUB(CURDATE(), INTERVAL 3 YEAR) "
                "GROUP BY B.name, CONCAT(YEAR(S.date), '-W', LPAD(WEEK(S.date), 2, '0')), C.gender "
                "ORDER BY CONCAT(YEAR(S.date), '-W', LPAD(WEEK(S.date), 2, '0')) DESC, B.name",

                // 6. Week & Income
                "SELECT B.name, CONCAT(YEAR(S.date), '-W', LPAD(WEEK(S.date), 2, '0')), "
                "CASE WHEN C.annual_income < 50000 THEN 'Low' WHEN C.annual_income BETWEEN 50000 AND 100000 THEN 'Medium' ELSE 'High' END, "
                "COUNT(S.sale_id), SUM(S.sale_price) "
                "FROM Sale S JOIN Vehicle V ON S.VIN = V.VIN JOIN Model M ON V.model_id = M.model_id "
                "JOIN Brand B ON M.brand_id = B.brand_id JOIN Customer C ON S.customer_id = C.customer_id "
                "WHERE S.date >= DATE_SUB(CURDATE(), INTERVAL 3 YEAR) "
                "GROUP BY B.name, CONCAT(YEAR(S.date), '-W', LPAD(WEEK(S.date), 2, '0')), CASE WHEN C.annual_income < 50000 THEN 'Low' WHEN C.annual_income BETWEEN 50000 AND 100000 THEN 'Medium' ELSE 'High' END "
                "ORDER BY CONCAT(YEAR(S.date), '-W', LPAD(WEEK(S.date), 2, '0')) DESC, B.name"
            };

            for (int i = 0; i < 6; ++i) {
                if (mysql_query(conn, queries[i])) {
                    ShowMySQLError(conn);
                } else {
                    res = mysql_store_result(conn);
                    std::cout << titles[i] << std::endl;
                    std::printf("%-15s | %-7s | %-6s | %-5s | %s\n", 
                                "Brand", time_cols[i], demo_cols[i], "Units", "Revenue");
                    std::cout << "--------------------------------------------------------" << std::endl;
                    
                    while ((row = mysql_fetch_row(res))) {
                        // MYSQL_ROW는 문자열 배열이므로 별도의 형변환 없이 %s로 바로 출력 가능
                        std::printf("%-15s | %-7s | %-6s | %-5s | %s\n", 
                                    row[0] ? row[0] : "NULL", 
                                    row[1] ? row[1] : "NULL", 
                                    row[2] ? row[2] : "NULL", 
                                    row[3] ? row[3] : "0", 
                                    row[4] ? row[4] : "0");
                    }
                    mysql_free_result(res);
                }
            }
        }
        else if (choice == 2) {
            // [Query 2] 보안 파라미터 바인딩을 이용한 결함 부품 추적
            std::cout << "\n[Defective Part Tracking]" << std::endl;
            
            // 메뉴 선택 시 남은 입력 버퍼 비우기
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            
            char supplierStr[100];
            char startDateStr[20];
            char endDateStr[20];

            std::cout << "Enter Supplier Name (e.g., ZF Friedrichshafen): ";
            std::cin.getline(supplierStr, sizeof(supplierStr));

            std::cout << "Enter Start Date (YYYY-MM-DD, e.g., 2024-05-01): ";
            std::cin.getline(startDateStr, sizeof(startDateStr));

            std::cout << "Enter End Date (YYYY-MM-DD, e.g., 2024-05-31): ";
            std::cin.getline(endDateStr, sizeof(endDateStr));

            const char *query = 
                "SELECT V.VIN, C.name, C.phone, P.name, CAST(P.date AS CHAR) FROM Vehicle_Part VP "
                "JOIN Part P ON VP.part_id = P.part_id JOIN Plant Pl ON P.plant_id = Pl.plant_id "
                "JOIN Supplier Su ON Pl.supplier_id = Su.supplier_id "
                "JOIN Vehicle V ON VP.VIN = V.VIN JOIN Sale S ON V.VIN = S.VIN "
                "JOIN Customer C ON S.customer_id = C.customer_id "
                "WHERE Su.name = ? AND P.date BETWEEN ? AND ?";

            MYSQL_STMT *stmt = mysql_stmt_init(conn);
            if (!stmt) continue;

            if (mysql_stmt_prepare(stmt, query, strlen(query))) {
                ShowStmtError(stmt);
                mysql_stmt_close(stmt);
                continue;
            }

            // 입력 데이터 바인딩
            MYSQL_BIND bind[3];
            memset(bind, 0, sizeof(bind));
            bind[0].buffer_type = MYSQL_TYPE_STRING; bind[0].buffer = (char *)supplierStr; bind[0].buffer_length = strlen(supplierStr);
            bind[1].buffer_type = MYSQL_TYPE_STRING; bind[1].buffer = (char *)startDateStr; bind[1].buffer_length = strlen(startDateStr);
            bind[2].buffer_type = MYSQL_TYPE_STRING; bind[2].buffer = (char *)endDateStr; bind[2].buffer_length = strlen(endDateStr);

            mysql_stmt_bind_param(stmt, bind);

            // 출력 데이터 바인딩
            MYSQL_BIND out_bind[5];
            memset(out_bind, 0, sizeof(out_bind));
            char out_vin[20], out_cname[55], out_phone[25], out_pname[55], out_date[20];
            out_bind[0].buffer_type = MYSQL_TYPE_STRING; out_bind[0].buffer = out_vin; out_bind[0].buffer_length = sizeof(out_vin);
            out_bind[1].buffer_type = MYSQL_TYPE_STRING; out_bind[1].buffer = out_cname; out_bind[1].buffer_length = sizeof(out_cname);
            out_bind[2].buffer_type = MYSQL_TYPE_STRING; out_bind[2].buffer = out_phone; out_bind[2].buffer_length = sizeof(out_phone);
            out_bind[3].buffer_type = MYSQL_TYPE_STRING; out_bind[3].buffer = out_pname; out_bind[3].buffer_length = sizeof(out_pname);
            out_bind[4].buffer_type = MYSQL_TYPE_STRING; out_bind[4].buffer = out_date; out_bind[4].buffer_length = sizeof(out_date);

            mysql_stmt_bind_result(stmt, out_bind);
            
            if (mysql_stmt_execute(stmt)) {
                ShowStmtError(stmt);
            } else {
                std::cout << "\nTracking Defective Parts from '" << supplierStr << "' (" << startDateStr << " to " << endDateStr << ")..." << std::endl;
                std::cout << "----------------------------------------------------------------------------------" << std::endl;
                std::cout << "Affected VIN      | Customer Name   | Phone         | Part Name            | Date" << std::endl;
                std::cout << "----------------------------------------------------------------------------------" << std::endl;
                
                bool found = false;
                while (!mysql_stmt_fetch(stmt)) {
                    found = true;
                    std::printf("%-17s | %-15s | %-13s | %-20s | %s\n", out_vin, out_cname, out_phone, out_pname, out_date);
                }
                if (!found) {
                    std::cout << "No defective parts found for the given criteria." << std::endl;
                }
            }
            mysql_stmt_close(stmt);
        }
        else if (choice == 3) {
            // [Query 3] Top Brands by Revenue
            const char *query = 
                "SELECT B.name, SUM(S.sale_price) AS Total_Revenue FROM Sale S "
                "JOIN Vehicle V ON S.VIN = V.VIN JOIN Model M ON V.model_id = M.model_id "
                "JOIN Brand B ON M.brand_id = B.brand_id "
                "WHERE S.date >= DATE_SUB(CURDATE(), INTERVAL 1 YEAR) "
                "GROUP BY B.brand_id, B.name ORDER BY Total_Revenue DESC LIMIT 2";

            if (mysql_query(conn, query)) {
                ShowMySQLError(conn);
            } else {
                res = mysql_store_result(conn);
                std::cout << "\n[Top 2 Brands by Revenue (Past 1 Year)]" << std::endl;
                while ((row = mysql_fetch_row(res))) {
                    std::printf("Brand: %-10s | Total Revenue: $%s\n", row[0] ? row[0] : "NULL", row[1] ? row[1] : "0");
                }
                mysql_free_result(res);
            }
        }
        else if (choice == 4) {
            // [Query 4] Top Brands by Unit Sales
            const char *query = 
                "SELECT B.name, COUNT(S.sale_id) AS Unit_Sales FROM Sale S "
                "JOIN Vehicle V ON S.VIN = V.VIN JOIN Model M ON V.model_id = M.model_id "
                "JOIN Brand B ON M.brand_id = B.brand_id "
                "WHERE S.date >= DATE_SUB(CURDATE(), INTERVAL 1 YEAR) "
                "GROUP BY B.brand_id, B.name ORDER BY Unit_Sales DESC LIMIT 2";

            if (mysql_query(conn, query)) {
                ShowMySQLError(conn);
            } else {
                res = mysql_store_result(conn);
                std::cout << "\n[Top 2 Brands by Unit Sales (Past 1 Year)]" << std::endl;
                while ((row = mysql_fetch_row(res))) {
                    std::printf("Brand: %-10s | Total Units Sold: %s\n", row[0] ? row[0] : "NULL", row[1] ? row[1] : "0");
                }
                mysql_free_result(res);
            }
        }
        else if (choice == 5) {
            // [Query 5] 사용자 입력을 통한 계절별 판매 패턴 분석
            std::cout << "\n[Seasonal Sales Patterns]" << std::endl;
            std::cout << "Enter the vehicle style to search (e.g., Sedan, SUV, Convertible): ";
            
            // 입력 버퍼 정리
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            char styleStr[30];
            std::cin.getline(styleStr, sizeof(styleStr));

            const char *query = 
                "SELECT MONTH(S.date) AS Month, COUNT(S.sale_id) AS Total_Sales "
                "FROM Sale S JOIN Vehicle V ON S.VIN = V.VIN "
                "JOIN Model M ON V.model_id = M.model_id WHERE M.style = ? "
                "GROUP BY Month ORDER BY Total_Sales DESC LIMIT 1";

            MYSQL_STMT *stmt = mysql_stmt_init(conn);
            if (!stmt) continue;

            if (mysql_stmt_prepare(stmt, query, strlen(query))) {
                ShowStmtError(stmt);
                mysql_stmt_close(stmt);
                continue;
            }

            // 입력 데이터 바인딩
            MYSQL_BIND bind[1];
            memset(bind, 0, sizeof(bind));
            bind[0].buffer_type = MYSQL_TYPE_STRING;
            bind[0].buffer = (char *)styleStr;
            bind[0].buffer_length = strlen(styleStr);

            mysql_stmt_bind_param(stmt, bind);

            // 출력 데이터 바인딩
            MYSQL_BIND out_bind[2];
            memset(out_bind, 0, sizeof(out_bind));
            int bestMonth = 0, totalSales = 0;
            out_bind[0].buffer_type = MYSQL_TYPE_LONG; out_bind[0].buffer = &bestMonth;
            out_bind[1].buffer_type = MYSQL_TYPE_LONG; out_bind[1].buffer = &totalSales;

            mysql_stmt_bind_result(stmt, out_bind);

            if (mysql_stmt_execute(stmt)) {
                ShowStmtError(stmt);
            } else {
                if (mysql_stmt_fetch(stmt) == 0) {
                    std::printf("The best-selling month for '%s' is Month %d with %d units sold.\n", 
                                styleStr, bestMonth, totalSales);
                } else {
                    std::cout << "No sales data found for this style." << std::endl;
                }
            }
            mysql_stmt_close(stmt);
        }
        else if (choice == 6) {
            // [Query 6] Dealer Inventory Efficiency
            const char *query = 
                "SELECT D.name, AVG(DATEDIFF(CURDATE(), V.manufacturing_date)) AS Avg_Days "
                "FROM Vehicle V JOIN Dealer D ON V.dealer_id = D.dealer_id "
                "WHERE V.cur_state = 'stock' GROUP BY D.dealer_id, D.name "
                "ORDER BY Avg_Days DESC LIMIT 1";

            if (mysql_query(conn, query)) {
                ShowMySQLError(conn);
            } else {
                res = mysql_store_result(conn);
                std::cout << "\n[Dealer Inventory Efficiency (Worst Turnover)]" << std::endl;
                if ((row = mysql_fetch_row(res))) {
                    double avgDays = row[1] ? std::stod(row[1]) : 0.0;
                    std::printf("Dealer: %-25s | Average Inventory Age: %.1f Days\n", 
                                row[0] ? row[0] : "NULL", avgDays);
                }
                mysql_free_result(res);
            }
        }
        else if (choice == 7) {
            // [Query 7] Supplier Coverage Analysis
            const char *query = 
                "SELECT Su.name, COUNT(DISTINCT M.model_id) AS Models_Covered "
                "FROM Supplier Su JOIN Plant Pl ON Su.supplier_id = Pl.supplier_id "
                "JOIN Part P ON Pl.plant_id = P.plant_id JOIN Vehicle_Part VP ON P.part_id = VP.part_id "
                "JOIN Vehicle V ON VP.VIN = V.VIN JOIN Model M ON V.model_id = M.model_id "
                "GROUP BY Su.supplier_id, Su.name ORDER BY Models_Covered DESC LIMIT 1";

            if (mysql_query(conn, query)) {
                ShowMySQLError(conn);
            } else {
                res = mysql_store_result(conn);
                std::cout << "\n[Supplier Coverage Analysis]" << std::endl;
                if ((row = mysql_fetch_row(res))) {
                    std::printf("Supplier: %-20s | Supplies parts for %s distinct models.\n", 
                                row[0] ? row[0] : "NULL", row[1] ? row[1] : "0");
                }
                mysql_free_result(res);
            }
        }
        else {
            std::cout << "Invalid selection. Please choose 1-8.\n" << std::endl;
        }
    }

    // 4. 리소스 해제 및 연결 종료
    mysql_close(conn);
    return 0;
}