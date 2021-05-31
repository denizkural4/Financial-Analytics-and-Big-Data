//
//  Database.hpp
//  FRE7831_MarketData_JSON_MAC
//
//  Created by denizkuraal on 02/17/2021.
//  Copyright © 2021 NYU. All rights reserved.
//

#ifndef Database_hpp
#define Database_hpp
#include <iostream>
#include"Stock.h"
#include <stdio.h>
#pragma once
#include <sqlite3.h>
#include <string>

//#define sprintf_s(buf, ...) snprintf((buf), sizeof(buf), __VA_ARGS__)


class Stock;
int OpenDatabase(sqlite3*& db);
int CreateTables(sqlite3*& db);
int InsertTable(sqlite3* db, const Stock& stock);
int DisplayTables(sqlite3* db, std::string sqlSelect);

#endif /* Database_hpp */
