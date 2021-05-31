
//Deniz Kural - dk3703
//FRE 7831 - Financial Analytics & Big Data Homework 1

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include "json/json.h"
#include "curl/curl.h"
#include <sqlite3.h>
#include <time.h>       /* time_t, struct tm, time, localtime */


using namespace std;

class IntradayTrade
{
private:
    long timestamp;
    string datetime;
    float open;
    float high;
    float low;
    float close;
    int volume;
public:
    //Constructor
    IntradayTrade(long timestamp_, string datetime_, float open_, float high_, float low_, float close_, int volume_) :
    timestamp(timestamp_), datetime(datetime_), open(open_), high(high_), low(low_), close(close_), volume(volume_)
    {}
    //Destructor
    ~IntradayTrade() {}
    
    // For reading
    friend ostream& operator << (ostream& out, const IntradayTrade& t)
    { out << " Datetime: " << t.datetime << " Open: " << t.open
        << " High: " << t.high << " Low: " << t.low << " Close: " << t.close << " Volume: " << t.volume << endl;
        return out;
    }
    
    //For writing
    friend ofstream& operator << (ofstream& fout, const IntradayTrade& t)
    {  fout << " Datetime: " << t.datetime << " Open: " << t.open
    << " High: " << t.high << " Low: " << t.low << " Close: " << t.close << " Volume: " << t.volume << endl;
        return fout;
    }
    };

//Stock Class
class Stock
{
private:
	string symbol;
	vector<IntradayTrade> trades;

public:
	Stock(string symbol_) :symbol(symbol_)
	{}
	~Stock() {}
	void addTrade(IntradayTrade aTrade)
	{
		trades.push_back(aTrade);
	}
	friend ostream & operator << (ostream & out, const Stock & s)
	{
		out << "Symbol: " << s.symbol << endl;
		for (vector<IntradayTrade>::const_iterator itr = s.trades.begin(); itr != s.trades.end(); itr++)
			out << *itr;
		return out;
	}
    friend ofstream& operator << (ofstream& fout, const Stock& s)
    {
        fout << "Symbol: " << s.symbol << endl;
        for (vector<IntradayTrade>::const_iterator itr = s.trades.begin(); itr != s.trades.end(); itr++)
            fout << *itr;
        return fout;
       
    }
};

//writing call back function for storing fetched values in memory
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}




int main(void)
{
    ofstream file1( "unmodified_results.txt" );
    ofstream file2( "Results.txt" );
    
	string readBuffer;

	//global initiliation of curl before calling a function
	curl_global_init(CURL_GLOBAL_ALL);

	//creating session handle
	CURL * handle;

	// Store the result of CURL’s webpage retrieval, for simple error checking.
	CURLcode result;

	// notice the lack of major error-checking, for brevity
	handle = curl_easy_init();

	//after creating handle we will start transfering webpage
	//curl_easy_setopt is used to tell libcurl how to behave.
	//By setting the appropriate options, the application can change libcurl's behavior.

    

    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    
    //End date is the current date
    auto end_date = to_string(time (&rawtime));
    
    //Calculate the date 3 months before today
    timeinfo = localtime (&rawtime);
    timeinfo -> tm_mday -=90;
    time_t next = mktime(timeinfo);
    
    //Start date is the today
    auto start_date = to_string(next);

    
	string url_common = "https://eodhistoricaldata.com/api/intraday/";
    
    //Modifications for 5 minute intraday data
	string symbol = "MSFT";
    string api_token = "6016f05ca2a151.93454646";
	string url_request = url_common + symbol + ".US?" + "from=" + start_date + "&to=" + end_date + "&api_token=" + api_token + "&interval=5m&fmt=json";
	curl_easy_setopt(handle, CURLOPT_URL, url_request.c_str());

	//adding a user agent
	curl_easy_setopt(handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:74.0) Gecko/20100101 Firefox/74.0");
	curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);

	// send all data to this function 
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, WriteCallback);

	// we pass our 'chunk' struct to the callback function 
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, &readBuffer);

	//perform a blocking file transfer
	result = curl_easy_perform(handle);

	// check for errors 
	if (result != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
	}
	else
	{
		//json parsing
		Json::CharReaderBuilder builder;
		Json::CharReader* reader = builder.newCharReader();
		Json::Value root;   // will contains the root value after parsing.
		string errors;

		bool parsingSuccessful = reader->parse(readBuffer.c_str(), readBuffer.c_str() + readBuffer.size(), &root, &errors);
		if (not parsingSuccessful)
		{
			// Report failures and their locations in the document.
			cout << "Failed to parse JSON" << endl << readBuffer << errors << endl;
			return -1;
		}
		else
		{
			cout << "\nSucess parsing json\n" << root << endl;
           
			string datetime;
			float open, high, low, close, adjusted_close;
			long volume,timestamp;
			Stock myStock(symbol);
			for (Json::Value::const_iterator itr = root.begin(); itr != root.end(); itr++)
            {
                timestamp=(*itr)["timestamp"].asInt64();
                
               
                //Date time conversion from given timestamp as the given datetime format is in UTC.
                
                /* I am currently in Turkey which has 8 hours difference with NY (EST Time), so to show results in EST time;
                i)First, Convert timestamp to Local time ( In our case, GMT+3)
                ii) Secondly, substract 8 hours from local time to reach NY time (GMT-5).
               
                 */
                
                time_t t = timestamp;
                struct tm* tm = localtime(&t);
                tm -> tm_hour -=8;
                datetime= asctime(tm);
               
                
                
                open = (*itr)["open"].asFloat();
                high = (*itr)["high"].asFloat();
                low = (*itr)["low"].asFloat();
                close = (*itr)["close"].asFloat();
                adjusted_close = (*itr)["adjusted_close"].asFloat();
                volume = (*itr)["volume"].asInt64();
                IntradayTrade aTrade(timestamp,datetime, open, high, low, close, volume);
                myStock.addTrade(aTrade);
                
                file1 << aTrade;
			}
			cout << myStock;
            file2 << myStock;
		}
	}
	curl_easy_cleanup(handle);
	return 0;
}
