//
//  Stock.h
//  FRE7831_MarketData_JSON_MAC
//
//  Created by denizkuraal on 02/17/2021.
//  Copyright © 2021 NYU. All rights reserved.
//

#ifndef Stock_h
#define Stock_h

#include"MarketData.hpp"


//Trade Class
class Trade
{
protected:
    float open;
    float high;
    float low;
    float close;
    int volume;
public:
    Trade(float open_, float high_, float low_, float close_, int volume_) :
    open(open_), high(high_), low(low_), close(close_), volume(volume_) {}
    ~Trade() {}
    float GetOpen() const {  return open; }
    float GetHigh() const   {  return high; }
    float GetLow() const   {  return low;  }
    float GetClose() const {  return close; }
    int GetVolume() const {  return volume; }
};


//Child Class : Daily Trade
//Daily Trade :: Object that holds Daily Trade information
class DailyTrade : public Trade
{
private:
    std::string date;
    float adjusted_close;
public:
    DailyTrade(std::string date_, float open_, float high_, float low_, float close_, float adjusted_close_, int volume_) :
    Trade(open_, high_, low_, close_, volume_), date(date_), adjusted_close(adjusted_close_) {}
    ~DailyTrade() {}
    std::string GetDate() const {  return date; }
    float GetAdjustedClose() const {  return close;  }
    friend std::ostream& operator << (std::ostream& out, const DailyTrade& t)
    {
        out << "Date: " << t.date << " Open: " << t.open << " High: " << t.high << " Low: " << t.low
        << " Close: " << t.close << " Adjusted_Close: " << t.adjusted_close << " Volume: " << t.volume << std::endl;
        return out;
    }
};



//Child Class : Intraday Trade
//Daily Trade :: Object that holds Intraday Trade information
class IntradayTrade : public Trade
{
private:
    std::string date;
    std::string timestamp;
public:
    IntradayTrade(std::string date_, std::string timestamp_, float open_, float high_, float low_, float close_, int volume_) :
    Trade(open_, high_, low_, close_, volume_), date(date_), timestamp(timestamp_) {}
    ~IntradayTrade() {}
    std::string GetDate() const {  return date; }
    std::string GetTimestamp() const {  return timestamp; }
    friend std::ostream& operator << (std::ostream& out, const IntradayTrade& t)
    {
        out << " Date: " << t.date << " Timestamp: " << t.timestamp << " Open: " << t.open
        << " High: " << t.high << " Low: " << t.low << " Close: " << t.close
        << " Volume: " << t.volume << std::endl;
        return out;
    }
};




class Stock
{
private:
    std::string symbol;
    std::vector<DailyTrade> dailyTrades;
    std::vector<IntradayTrade> intradayTrades;
public:
    Stock() :symbol("")    {     dailyTrades.clear(); intradayTrades.clear();     }
    Stock(std::string symbol_) :symbol(symbol_) {     dailyTrades.clear(); intradayTrades.clear();     }
    Stock(const Stock& stock)  {    memcpy(this, &stock, sizeof(stock));     }
    ~Stock() {}
    void addDailyTrade(const DailyTrade& aTrade)     {     dailyTrades.push_back(aTrade);    }
    void addIntradayTrade(const IntradayTrade& aTrade)    {    intradayTrades.push_back(aTrade);    }
    std::string GetSymbol() const {     return symbol;     }
    const std::vector<DailyTrade>& GetDailyTrade(void) const {     return dailyTrades;     }
    const std::vector<IntradayTrade>& GetIntradayTrade(void) const {         return intradayTrades;         }
    friend std::ostream& operator << (std::ostream& out, const Stock& s)
    {
        out << "Symbol: " << s.symbol << std::endl;
        for (std::vector<DailyTrade>::const_iterator itr = s.dailyTrades.begin(); itr != s.dailyTrades.end(); itr++)
            out << *itr;
        for (std::vector<IntradayTrade>::const_iterator itr = s.intradayTrades.begin(); itr != s.intradayTrades.end(); itr++)
            out << *itr;
        return out;
    }
};









#endif /* Stock_h */
