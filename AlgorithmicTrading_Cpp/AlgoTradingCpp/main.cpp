
//FRE 7831 - Homework #2 ---- Algo. Trading

//Deniz Kural - dk3703


#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream



#include "ReadTrades.h"
#include "Time_Calc.hpp"


int main(){
    
    //Open csv file
    std::ifstream file("SPY_May_2012.csv");
    if(!file.is_open()) throw std::runtime_error("Could not open file"); //Throw error if there is an exception reading the csv file
    
    //std::cout << std::fixed << std::setprecision(2);
    
    
    //Create Object for trade in order to read each trade order in the orderbook
    ReadTrades trade;
    
    std::vector<std::string> colnames;
    std::string col_line,colname;
    
    //Extract first row which consists of colum names -- info of trade
    
    // Read the column names
    if(file.good())
    {
        // Extract the first line in the file
        std::getline(file, col_line);

        // Create a stringstream from line
        std::stringstream ss(col_line);

        // Extract each column name
        while(std::getline(ss, colname, ',')){
            
            // Initialize and add <colname, int vector> pairs to result
            colnames.push_back(colname);
        }
    }
    
    
    //To Display column names of csv file:::
    
    //for (int i = 0; i < (int)colnames.size(); i++)
      //    std::cout<< colnames.at(i) <<std::endl;
    

    
    
    //Initilization of Variables
    std::string date_today=" ";
    std::string date_prev=" ";
    std::string current_t;
    int curr_index;
    int prev_index=200000;
    Time_Calc timing;
    std::string vol;
    std::string price;
 
    //Time Interval size from 9:30:00 to 16:00:00 for each 15 mins
    const int size_arr = floor((timing.final_time-timing.init_time)/900);
    std::cout<<"The number of interval in the set: "<<size_arr<<std::endl;
    
    //Create array for Volume, Price and Time with the interval size
    long double* vol_arr = new long double[size_arr] {0.};
    float* price_arr = new float[size_arr] {0.};
    std::string time_arr [size_arr]  ;

    int day_count=0;
    //day_count for holding the total number of valid trading days in the given duration

    
    
    
    //Iteration through each trade
    while(file >> trade){
        
           date_today=trade[1];
           if (date_today=="22-MAY-2012")
               break;
        //Break if date is later than 21 May as irrelivant
          
           
           if(trade[4]=="Quote")
               continue;
        //Only consider trades that happened, not the quoted ones  in the orderbook
        
           else if (trade[4]=="Trade"){
               //Get Dates from 05/01-05/20 for Volume Index
            
               if (date_today!="21-MAY-2012"){
                   current_t=trade[2];
                   //Get Current time,
                   //If current time is valid, record Volume
                   if(timing.test_time(current_t)){
                       if (date_today!= date_prev){
                           day_count++;
                           date_prev=date_today;
                           std::cout<<"Today's date as a trading day: "<<date_today<<std::endl;
                           //control day_count and print each individual day trading
                       }
                       vol=trade[6];
                       vol_arr[timing.est_index(current_t)]+= std::strtold( vol.c_str(),NULL );
                   }}
               //If the date is 21 May 2012
               //else if (date_today=="21-MAY-2012"){
               else {
                   current_t=trade[2];
                   
                   //If trading time is valid and if AND ONLY IF THE TIME RECORDED IS NEVER RECORDED BEFORE, save the price record
                   
                   if(timing.test_time(current_t)){
                       curr_index=timing.est_index(current_t);
                       if(curr_index!=prev_index){
                           price=trade[5];
                           price_arr[timing.est_index(current_t)]= atof( price.c_str() );
                           prev_index=curr_index;
                          
                           //To display, current time and current price info.
                           //std::cout<<current_t << "     "<< atof( price.c_str() )<< std::endl;
                       }

                   }
               }
               /*
                This was to check there was no error in the process
               else{
                   std::cerr<<"Something is wrong!"<<std::endl;
                   break;
               }
               */
           }
           else{
               std::cerr<<"Something is wrong!"<<std::endl;
               std::cout<<trade[4]<<std::endl;
               break;
           }
               
        }
    
    
    //Displaying results and outputting to CSV file
    
    std::cout<< std::endl << "Initializing results for Volume and Price...."<<std::endl<<std::endl;
    std::cout<<"Trading day count is " << day_count<< std::endl<<std::endl;

    
    std::cout<< "Time interval"<<std::endl;
   std::cout<< "............."<<std::endl;
   for(int i=0;i<size_arr;i++){
       
       int t_int= timing.init_time+(i)*900;
       time_arr[i]=timing.sec_to_str(t_int);
       std::cout<<time_arr[i]<<std::endl;
       
           }
   std::cout<<std::endl<< "Average Quantity (Volume) "<<std::endl;
   std::cout<< "............."<<std::endl;
   for(int i=0;i<size_arr;i++){
       //std::cout << std::fixed;
       vol_arr[i]=floor(vol_arr[i]/(day_count));
       std::cout<<std::fixed<<std::setprecision(0)<<vol_arr[i]<<std::endl;
       

        
    }
   std::cout<<std::endl<< "Price at 05/21/2012 "<<std::endl;
   std::cout<< "............."<<std::endl;
   for(int i=0;i<size_arr;i++){
       
       std::cout<<std::setprecision(2)<<price_arr[i]<<std::endl;
       
   }
           
        
    std::fstream fout;
    fout.open("output.csv", std::ios::out);
    fout << "Time" << "," << "Average Quantity" << "," << "Price" << std::endl;
    
    for (int i = 0; i < size_arr; i++)
    {
        
        fout << time_arr[i] << "," << std::fixed<< std::setprecision(0)<<vol_arr[i] << "," <<std::setprecision(2)<<price_arr[i] << std::endl;
    }
    
    fout.close();
           
           
           
    
                   
    
                   /*
                    
                    if current time is valid, then take the vol and save it to array for the time (date is unncessary to save)
                    Then,
                    if the date is 22-MAY-2012, get the price info for each valid time and save it to price array
                    At the end , visualize results and save it to csv ostream file
                    
                
                    
                    */
                   
    delete[] vol_arr;
    delete[] price_arr;
    vol_arr=NULL;
    price_arr=NULL;

    return 0;
       }
    

