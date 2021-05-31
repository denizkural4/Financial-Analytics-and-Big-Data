//
//  Database.cpp
//  FRE7831_MarketData_JSON_MAC
//
//  Created by denizkuraal on 02/17/2021.
//  Copyright © 2021 NYU. All rights reserved.
//

#include "Database.hpp"
int OpenDatabase(sqlite3*& db);
int CreateTables(sqlite3*& db);
int InsertTable(sqlite3* db, const Stock& stock);
int DisplayTables(sqlite3* db, std::string sqlSelect);



//Opening Database (MarketData.db)
int OpenDatabase(sqlite3*& db){

    int rc = 0;
    //char* error = nullptr;

    // Open Database
    std::cout << "Opening MarketData.db ..." << std::endl;
    rc = sqlite3_open("MarketData.db", &db);
    if (rc)
        {
        std::cerr << "Error opening SQLite3 database: " << sqlite3_errmsg(db) << std::endl << std::endl;
        sqlite3_close(db);
        return -1;
        }
    else
        {
        std::cout << "Opened MarketData.db." << std::endl << std::endl;
        }
    return 0;
    
}


//Creating Database Tables (Daily & Intraday) for inserting stock info.
int CreateTables(sqlite3*& db){
    int rc = 0;
    char* error = nullptr;
    // Drop the table if exists
    std::string sql_dropatable = "DROP TABLE IF EXISTS DailyTrades";
    if (sqlite3_exec(db, sql_dropatable.c_str(), 0, 0, 0) != SQLITE_OK) {
        std::cout << "SQLite can't drop DailyTrade table" << std::endl;
        sqlite3_close(db);
        return -1;
    }

    // Create the table
    std::cout << "Creating DailyTrades table ..." << std::endl;
    std::string sqlCreateTable = std::string("CREATE TABLE IF NOT EXISTS DailyTrades ")
    + "(Symbol CHAR(20) NOT NULL,"
    + "Date CHAR(20) NOT NULL,"
    + "Open REAL NOT NULL,"
    + "High REAL NOT NULL,"
    + "Low REAL NOT NULL,"
    + "Close REAL NOT NULL,"
    + "Adjusted_close REAL NOT NULL,"
    + "Volume INT NOT NULL,"
    + "PRIMARY KEY(Symbol, Date),"
    + "Foreign Key(Symbol) references Stocks(Symbol)\n"
    + "ON DELETE CASCADE\n"
    + "ON UPDATE CASCADE"
    + ");";


    rc = sqlite3_exec(db, sqlCreateTable.c_str(), NULL, NULL, &error);
    if (rc)
    {
        std::cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg(db) << std::endl << std::endl;
        sqlite3_free(error);
    }
    else
    {
        std::cout << "Created DailyTrades table." << std::endl << std::endl;
    }
    
    
    
    
    
    sql_dropatable = "DROP TABLE IF EXISTS IntradayTrades";
    if (sqlite3_exec(db, sql_dropatable.c_str(), 0, 0, 0) != SQLITE_OK) {
        std::cout << "SQLite can't drop IntradayTrades table" << std::endl;
        sqlite3_close(db);
        return -1;
    }

    // Create the table
    std::cout << "Creating IntradayTrades table ..." << std::endl;
    sqlCreateTable = std::string("CREATE TABLE IF NOT EXISTS IntradayTrades ")
    + "(Symbol CHAR(20) NOT NULL,"
    + "Date CHAR(20) NOT NULL,"
    + "Timestamp CHAR(20) NOT NULL,"
    + "Open REAL NOT NULL,"
    + "High REAL NOT NULL,"
    + "Low REAL NOT NULL,"
    + "Close REAL NOT NULL,"
    + "Volume INT NOT NULL,"
    + "PRIMARY KEY(Symbol, Date, Timestamp),"
    + "Foreign Key(Symbol, Date) references DailyTrades(Symbol, Date)\n"
    + "ON DELETE CASCADE\n"
    + "ON UPDATE CASCADE\n"
    + ");";


    rc = sqlite3_exec(db, sqlCreateTable.c_str(), NULL, NULL, &error);
    if (rc)
    {
        std::cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg(db) << std::endl << std::endl;
        sqlite3_free(error);
    }
    else
    {
        std::cout << "Created IntradayTrades table." << std::endl << std::endl;
    }
    
    
    return 0;
    
}



int InsertTable(sqlite3* db, const Stock& stock){
    
    int rc = 0;
    char* error = nullptr;
    // Execute SQL
    char sqlInsert[512];
    
    std::cout << "Inserting a value into table DailyTrades ..." << std::endl;
   
    const std::vector<DailyTrade>& dailyTrade = stock.GetDailyTrade();
    for(std::vector<DailyTrade>::const_iterator itr= dailyTrade.begin();itr != dailyTrade.end();itr++){
        sprintf(sqlInsert, "INSERT INTO DailyTrades(Symbol, Date, Open, High, Low, Close, Adjusted_close, Volume) VALUES(\"%s\", \"%s\", %f, %f, %f, %f, %f, %d)", stock.GetSymbol().c_str(), itr->GetDate().c_str(), itr->GetOpen(), itr->GetHigh(), itr->GetLow(), itr->GetClose(), itr->GetAdjustedClose(), itr->GetVolume());
        std::cout << sqlInsert <<std::endl;
        
        rc = sqlite3_exec(db, sqlInsert, NULL, NULL, &error);
        if (rc)
        {
            std::cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg(db) << std::endl << std::endl;
            sqlite3_free(error);
        }
        else
        {
            std::cout << "Inserted a value into the table DailyTrades." << std::endl << std::endl;
        }
        
  
    }
    
    
    
    std::cout << "Inserting a value into table IntradayTrades ..." << std::endl;
   
    const std::vector<IntradayTrade>& intradayTrade = stock.GetIntradayTrade();
    for(std::vector<IntradayTrade>::const_iterator itr= intradayTrade.begin();itr != intradayTrade.end();itr++){
        
        sprintf(sqlInsert, "INSERT INTO IntradayTrades(Symbol, Date, Timestamp, Open, High, Low, Close, Volume) VALUES(\"%s\", \"%s\", \"%s\", %f, %f, %f, %f, %d)", stock.GetSymbol().c_str(), itr->GetDate().c_str(),itr->GetTimestamp().c_str() ,itr->GetOpen(), itr->GetHigh(), itr->GetLow(), itr->GetClose(), itr->GetVolume());
        std::cout << sqlInsert <<std::endl;
        
        rc = sqlite3_exec(db, sqlInsert, NULL, NULL, &error);
        if (rc)
        {
            std::cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg(db) << std::endl << std::endl;
            sqlite3_free(error);
        }
        else
        {
            std::cout << "Inserted a value into the table IntradayTrades." << std::endl << std::endl;
        }
        
  
    }


    return 0;

    
}



//Displaying Tables in the GUI... (Terminal)
int DisplayTables(sqlite3* db, std::string sqlSelect){
    
    // Display MyTable
    int rc = 0;
    char* error = nullptr;
    char** results = NULL;
    int rows, columns;
    
    sqlite3_get_table(db, sqlSelect.c_str(), &results, &rows, &columns, &error);
    
    
    if (rc)
    {
        std::cerr << "Error executing SQLite3 query: " << sqlite3_errmsg(db) << std::endl << std::endl;
        sqlite3_free(error);
    }
    else
    {
        // Display Table
        for (int rowCtr = 0; rowCtr <= rows; ++rowCtr)
        {
            for (int colCtr = 0; colCtr < columns; ++colCtr)
            {
                // Determine Cell Position
                int cellPosition = (rowCtr * columns) + colCtr;

                // Display Cell Value
                std::cout.width(12);
                std::cout.setf(std::ios::left);
                std::cout << results[cellPosition] << " ";
            }

            // End Line
            std::cout << std::endl;

            // Display Separator For Header
            if (0 == rowCtr)
            {
                for (int colCtr = 0; colCtr < columns; ++colCtr)
                {
                    std::cout.width(12);
                    std::cout.setf(std::ios::left);
                    std::cout << "~~~~~~~~~~~~ ";
                }
                std::cout << std::endl;
            }
        }
    }
    sqlite3_free_table(results);
    
    return 0;

}
