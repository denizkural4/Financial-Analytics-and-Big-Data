//
//  MarketData.cpp
//  FRE7831_MarketData_JSON_MAC
//
//  Created by denizkuraal on 02/17/2021.
//  Copyright © 2021 NYU. All rights reserved.
//
#include "MarketData.hpp"


//writing call back function for storing fetched values in memory
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
     
};


//Pulling Market Data from EOD website
int PullMarketData(const std::string& url_request, std::string& read_buffer){

    //global initiliation of curl before calling a function
    curl_global_init(CURL_GLOBAL_ALL);

    //creating session handle
    CURL * handle;

    // Store the result of CURLís webpage retrieval, for simple error checking.
    CURLcode result;

    // notice the lack of major error-checking, for brevity
    handle = curl_easy_init();
        
    curl_easy_setopt(handle, CURLOPT_URL, url_request.c_str());

    //adding a user agent
    curl_easy_setopt(handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:74.0) Gecko/20100101 Firefox/74.0");
    curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);

    // send all data to this function
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, WriteCallback);

    // we pass our 'chunk' struct to the callback function
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &read_buffer);

    //perform a blocking file transfer
    result = curl_easy_perform(handle);

    // check for errors
    if (result != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
    }
    
    
    //std::cout << read_buffer << std::endl;
    curl_easy_cleanup(handle);

    
    return 0;
};





//Populating Daily Trades into Stock<DailyTrades> by JSON parsing
int PopulateDailyTrades(const std::string& read_buffer, Stock& stock){
    
    //json parsing
    Json::CharReaderBuilder builder;
    Json::CharReader* reader = builder.newCharReader();
    Json::Value root;   // will contains the root value after parsing.
    std::string errors;

    bool parsingSuccessful = reader->parse(read_buffer.c_str(), read_buffer.c_str() + read_buffer.size(), &root, &errors);
    
    if (not parsingSuccessful)
    {
        // Report failures and their locations in the document.
        std::cout << "Failed to parse JSON" << std::endl << read_buffer << errors << std::endl;
        return -1;
    }
    else
    {
        std::cout << "\nSucess parsing json\n" << root << std::endl;
        std::string date;
        float open, high, low, close, adjusted_close;
        long volume;
        
        for (Json::Value::const_iterator itr = root.begin(); itr != root.end(); itr++) // iterate the Json list
        {
            date = (*itr)["date"].asString();    // read value from Json Object, it is a C++ map
            open = (*itr)["open"].asFloat();
            high = (*itr)["high"].asFloat();
            low = (*itr)["low"].asFloat();
            close = (*itr)["close"].asFloat();
            adjusted_close = (*itr)["adjusted_close"].asFloat();
            volume = (*itr)["volume"].asInt64();
            DailyTrade aTrade(date.c_str(), open, high, low, close, adjusted_close, volume);
            stock.addDailyTrade(aTrade);
        }
    }
    return 0;
        
};


//Populating Intraday Trades into Stock<IntradayTrades> by JSON parsing
int PopulateIntradayTrades(const std::string& read_buffer, Stock& stock){
    
    //json parsing
    Json::CharReaderBuilder builder;
    Json::CharReader* reader = builder.newCharReader();
    Json::Value root;   // will contains the root value after parsing.
    std::string errors;

    bool parsingSuccessful = reader->parse(read_buffer.c_str(), read_buffer.c_str() + read_buffer.size(), &root, &errors);
    
    if (not parsingSuccessful)
    {
        // Report failures and their locations in the document.
        std::cout << "Failed to parse JSON" << std::endl << read_buffer << errors << std::endl;
        return -1;
    }
    else
    {
        std::cout << "\nSucess parsing json\n" << root << std::endl;
        
        std::string datetime;
        float open, high, low, close;
        long volume;
        std::string timestamp;
        std::string date;
       
        
        
        for (Json::Value::const_iterator itr = root.begin(); itr != root.end(); itr++)
            {
            long Feb_1_2021 = 1612155600;
            long trade_timestamp = (*itr)["timestamp"].asInt64();
            if (trade_timestamp < Feb_1_2021)
                continue;
                
            std::string datetime = (*itr)["datetime"].asString();
            size_t current, previous = 0;
            current = datetime.find(' ');
            date = datetime.substr(previous, current - previous);
            previous = current + 1;
            std::string temp = datetime.substr(previous, datetime.length() - current);
            previous = 0;
            current = temp.find(':');
            int hour = stoi(temp.substr(previous, current - previous));
            hour -= 5;// convert to local time
            previous = current + 1;
            std::string mins_second = temp.substr(previous, datetime.length() - current);
            timestamp = std::to_string(hour) + ":" + mins_second;
            open = (*itr)["open"].asFloat();
            high = (*itr)["high"].asFloat();
            low = (*itr)["low"].asFloat();
            close = (*itr)["close"].asFloat();
            volume = (*itr)["volume"].asInt64();
            IntradayTrade aTrade(date.c_str(), timestamp.c_str(), open, high, low, close, volume);
            stock.addIntradayTrade(aTrade);
            }

    
    }
    return 0;
        
};
