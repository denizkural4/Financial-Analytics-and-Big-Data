/*
                Spring 2021 FRE-GY-7831 Financial Analytics & Big Data
                                LAB 1 - FRE7831
                -------------------------------------------------------
 
Deniz Kural / dk3703                                             Instructor: Prof.Song Tang
 
 
 The Goal of the Lab step by step:
 
 i) Get Data from EOD for daily & intraday trading data from 3 stocks (MSFT,TWTR,IBM)
 ii) Populate the data by JSON for daily & intraday data into stock class object respectively.
 iii) Insert the data in database by SQlite3 operations.
 iv) Visualization of the tables inserted into database by tables.
 v) Prepare a basic menu (GUI) for step by step progress in the defined steps above.
 
 
 */



#include <stdio.h>
#include "Stock.h"
#include "MarketData.hpp"
#include "Database.hpp"
#include <map>




int main(void)
{
    sqlite3* db = NULL;
    if (OpenDatabase(db) != 0)
        return -1;
    
    bool bCompleted = false;
    char selection;
    
    std::vector<std::string> symbols;
    symbols.push_back("MSFT");
    symbols.push_back("TWTR");
    symbols.push_back("IBM");
    
    std::map<std::string, Stock> stockMap;
    
    while (!bCompleted)
    {
        std::cout << std::endl;
        std::cout << "Menu" << std::endl;
        std::cout << "========" << std::endl;
        std::cout << "A - Create DailyTrades and Intraday Trades Tables" << std::endl;
        std::cout << "B - Retrieve Daily Trade and Intraday Trade Data" << std::endl;
        std::cout << "C - Populate DailyTrades and IntradayTrades Table" << std::endl;
        std::cout << "D - Retrieve Data from DailyTrades and IntradayTrades Tables" << std::endl;
        std::cout << "E - Drop All the Tables" << std::endl;
        std::cout << "X - Exit" << std::endl << std::endl;
        
        std::cout << "Enter the selection..." ;
        std::cin >> selection;
        
        switch(selection){
            case 'a':
            case 'A': {
                if (CreateTables(db)!=0)
                    return -1;
                break;
            }
            case 'B':
            case 'b':
            {
                std::string start_date="2021-02-01";
                std::string end_date="2021-05-31";
                
                std::string daily_url_common="https://eodhistoricaldata.com/api/eod/";
                std::string intraday_url_common="https://eodhistoricaldata.com/api/intraday/";
                std::string api_token="6016f05ca2a151.93454646";
                
                for(std::vector<std::string>::iterator itr = symbols.begin();itr!=symbols.end();itr++)
                {
                    Stock aStock(*itr);
                    
                    std::string daily_data_url_request= daily_url_common + *itr + ".US?" + "from=" + start_date + "&to=" + end_date + "&api_token=" + api_token + "&period=d&fmt=json";
                    std::string intraday_data_url_request= intraday_url_common + *itr + ".US?" + "&api_token=" + api_token + "&interval=5m&fmt=json";
                    
                    std::string daily_market_data;
                    
                    if(PullMarketData(daily_data_url_request, daily_market_data))
                        return -1;
                    if(PopulateDailyTrades(daily_market_data, aStock))
                        return -1;
                    
                    
                    std::string intraday_market_data;
                    
                    if(PullMarketData(intraday_data_url_request, intraday_market_data))
                        return -1;
                    if(PopulateIntradayTrades(intraday_market_data, aStock))
                        return -1;
                    
                    std::cout << aStock <<std::endl;
                    stockMap[*itr]=aStock;
                    

                    
                }
                
                break;
                
            }
            case 'C':
            case 'c':{
                for(std::map<std::string,Stock>::iterator itr =stockMap.begin();itr!=stockMap.end();itr++)
            {
                if(InsertTable(db, itr->second)!=0)
                    return -1;
                
            }
                break;
            }
            case 'D':
            case 'd':
            {
                std::cout << "Retrieving values in table DailyTrades ..." << std::endl;
                std::string sqlSelect = std::string("SELECT * FROM DailyTrades;");
                if (DisplayTables(db, sqlSelect) != 0)
                    return -1;
                
                std::cout << "Retrieving values in table IntradayTrades ..." << std::endl;
                std::string sqlSelectIntradayTrades = std::string("SELECT * FROM IntradayTrades;");
                if (DisplayTables(db, sqlSelectIntradayTrades) != 0)
                    return -1;
                
                break;
            }
            case 'E':
            case 'e':
            {
                break;
               
                
            }
            case 'x':
            case 'X':
            {
                bCompleted=true;
                break;
            }
            
            default:{
                std::cout<<"Please enter a selection ..."<<std::endl;
                std::cin >> selection;
                
                break;
            }
                
        }
    }
    
        
        
        // Close Database
        std::cout << "Closing MarketData.db ..." << std::endl;
        sqlite3_close(db);
        std::cout << "Closed MarketData.db" << std::endl << std::endl;
        
        
        
        return 0;
    }
    
        
        
        
    
