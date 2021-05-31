//
//  MarketData.hpp
//  FRE7831_MarketData_JSON_MAC
//
//  Created by denizkuraal on 02/17/2021.
//  Copyright © 2021 NYU. All rights reserved.
//

#ifndef MarketData_hpp
#define MarketData_hpp
#include "curl/curl.h"
#include "json/json.h"
#include"Stock.h"
#include <stdio.h>
#include <iostream>
#pragma once
#include <string>


class Stock;
int PullMarketData(const std::string& url_request, std::string& read_buffer);
int PopulateDailyTrades(const std::string& read_buffer, Stock& stock);
int PopulateIntradayTrades(const std::string& read_buffer, Stock& stock);

#endif /* MarketData_hpp */
