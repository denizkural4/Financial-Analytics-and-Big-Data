//
//  Time_Calc.hpp
//  FRE7831-Homework2
//
//  Created by denizkuraal on 02/16/2021.
//

#ifndef Time_Calc_hpp
#define Time_Calc_hpp

#include <string>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <locale>
#include <iomanip>
#include <ctime>


class Time_Calc
{

public:
    //All Public Funtions
    
    int str_to_sec(const std::string& str);
    std::string sec_to_str (const int& value);

    bool test_time(const std::string& str);
    //bool test_time_price(const std::string& str);

    int est_index(const std::string& str);
    //int est_index_price(const std::string& str);

    //Global Variables for initial & final trade time
    const int init_time=str_to_sec("09:30:00.");
    const int final_time=str_to_sec("16:00:00.");
    
    
    
    
private:
  
};



#endif /* Time_Calc_hpp */
