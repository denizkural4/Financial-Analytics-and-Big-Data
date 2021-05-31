//
//  Time_Calc.cpp
//  FRE7831-Homework2
//
//  Created by denizkuraal on 02/16/2021.
//

#include "Time_Calc.hpp"



//String to Seconds Conversion
int Time_Calc::str_to_sec(const std::string& str){
    
    std::tm t;
    std::istringstream ss(str);
    ss >> std::get_time(&t, "%H:%M:%S");
    return(t.tm_hour * 3600 + t.tm_min * 60 + t.tm_sec);
       
}

//Seconds to Strings Conversion
std::string Time_Calc::sec_to_str (const int& value) {
    std::string result;
    // compute h, m, s
    std::string h = std::to_string(value / 3600);
    std::string m = std::to_string((value % 3600) / 60);
    std::string s = std::to_string(value % 60);
    // add leading zero if needed
    std::string hh = std::string(2 - h.length(), '0') + h;
    std::string mm = std::string(2 - m.length(), '0') + m;
    std::string ss = std::string(2 - s.length(), '0') + s;
    // return mm:ss if hh is 00
    if (hh.compare("00") != 0) {
    result = hh + ':' + mm + ":" + ss;
    }
    else {
    result =  mm + ":" + ss;
    }
    return result;
}



//Testing the time for Valid Trade info.
bool Time_Calc::test_time(const std::string& str){
    int test_time= Time_Calc::str_to_sec(str);
    if  (test_time>=init_time && test_time<=final_time)
        return true;
    else
        return false;
    
    
}

//Estimating the index in the array of Volume or Price
int Time_Calc::est_index(const std::string& str){
    int test_time= Time_Calc::str_to_sec(str);
    return floor((test_time-init_time)/900);

}




/*
std::string Time_Calc::sec_to_str (const int& seconds){
    int minutes = seconds / 60;
    int hours = minutes / 60;
    std::string result = std::to_string(int(hours)) + ":" +std::to_string(int(minutes%60)) + ":" + std::to_string(int(seconds%60));
    return result;

}
 */



/*
bool Time_Calc::test_time_price(const std::string& str){
    int test_time= Time_Calc::str_to_sec(str);
    if  ((test_time-init_time)%900==0 && test_time>=init_time && test_time<=final_time)
        return true;
    else
        return false;
    
    
}
 */


/*
int Time_Calc::est_index_price(const std::string& str){
    int test_time= Time_Calc::str_to_sec(str);
    if ((test_time-init_time)%900==0)
    {
        return ((test_time-init_time)/900);
    }
    else{
        return -1;
    }
}

*/





