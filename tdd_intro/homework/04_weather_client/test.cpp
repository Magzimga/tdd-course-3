/*
Weather Client

You are going to develop a program that gets the statistics about weather in the current city
using information from a certain server. The goal is to calculate statistics using the data from weather server.

To communicate with the weather server you have to implement interface IWeatherServer,
which provides the raw string from the real server for the requested day and time.

The real server (i.e. "weather.com") gets the requests in this format:
"<date>;<time>", for example:
"31.08.2018;03:00"

The server answers on requests with string like this:
"20;181;5.1"
This string contains the weather for the requested time and means next:
"<air_temperature_in_celsius>;<wind_direction_in_degrees>:<wind_speed>".
Wind direction value may be in range from 0 to 359 inclusively, temperature may be negative.

The task:
1. Implement fake server, because interacting with real network is inacceptable within the unit tests.
To do this, you need to use real server responses. Fortunately, you've collected some results for the several dates from the weather server.
Each line means "<request>" : "<response>":

"31.08.2018;03:00" : "20;181;5.1"
"31.08.2018;09:00" : "23;204;4.9"
"31.08.2018;15:00" : "33;193;4.3"
"31.08.2018;21:00" : "26;179;4.5"

"01.09.2018;03:00" : "19;176;4.2"
"01.09.2018;09:00" : "22;131;4.1"
"01.09.2018;15:00" : "31;109;4.0"
"01.09.2018;21:00" : "24;127;4.1"

"02.09.2018;03:00" : "21;158;3.8"
"02.09.2018;09:00" : "25;201;3.5"
"02.09.2018;15:00" : "34;258;3.7"
"02.09.2018;21:00" : "27;299;4.0"

IMPORTANT:
* Server returns empty string if request is invalid.
* Real server stores weather only for times 03:00, 09:00, 15:00 and 21:00 for every date. Do not use other hours in a day.

2. Implement IWeatherClient using fake server.
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>

struct Weather
{
    short temperature = 0;
    unsigned short windDirection = 0;
    double windSpeed = 0;
    bool operator==(const Weather& right)
    {
        return temperature == right.temperature &&
               windDirection == right.windDirection &&
               std::abs(windSpeed - right.windSpeed) < 0.01;
    }
};

class IWeatherServer
{
public:
    virtual ~IWeatherServer() { }
    // Returns raw response with weather for the given day and time in request
    virtual std::string GetWeather(const std::string& request) = 0;
};

// Implement this interface
class IWeatherClient
{
public:
    virtual ~IWeatherClient() { }
    virtual double GetAverageTemperature(IWeatherServer& server, const std::string& date) = 0;
    virtual double GetMinimumTemperature(IWeatherServer& server, const std::string& date) = 0;
    virtual double GetMaximumTemperature(IWeatherServer& server, const std::string& date) = 0;
    virtual double GetAverageWindDirection(IWeatherServer& server, const std::string& date) = 0;
    virtual double GetMaximumWindSpeed(IWeatherServer& server, const std::string& date) = 0;
};

// Test list
// done: 1. request creating(look from inside server if client call server with correct request
// 2. responce parsing. separate function
// 3.

using StringContainer = std::vector<std::string>;

const std::string s_testDate = "31.08.2018";
const std::string s_3hours = "03:00";
const std::string s_9hours = "09:00";
const std::string s_15hours = "15:00";
const std::string s_21hour = "21:00";


class FakeWeatherServer : public IWeatherServer
{
    std::map<std::string, std::string> m_data;
    StringContainer m_requests;
public:
    FakeWeatherServer()
    {
        m_data =
        {
        {"31.08.2018;03:00", "20;181;5.1"},
        {"31.08.2018;09:00", "23;204;4.9"},
        {"31.08.2018;15:00", "33;193;4.3"},
        {"31.08.2018;21:00", "26;179;4.5"},
        {"01.09.2018;03:00", "19;176;4.2"},
        {"01.09.2018;09:00", "22;131;4.1"},
        {"01.09.2018;15:00", "31;109;4.0"},
        {"01.09.2018;21:00", "24;127;4.1"},
        {"02.09.2018;03:00", "21;158;3.8"},
        {"02.09.2018;09:00", "25;201;3.5"},
        {"02.09.2018;15:00", "34;258;3.7"},
        {"02.09.2018;21:00", "27;299;4.0"}
        };
    }
    std::string GetWeather(const std::string &request)
    {
        m_requests.push_back(request);
        auto it = m_data.find(request);
        if(it == m_data.end())
        {
            return "";
        }
        return it->second;
    }

    StringContainer GetRequests()
    {
        StringContainer result;
        result.swap(m_requests);
        return result;
    }
};

std::string CreateRequest(const std::string& date, const std::string& time)
{
    return date+";"+time;
}
class WeatherClient : public IWeatherClient
{
public:
    virtual double GetAverageTemperature(IWeatherServer& server, const std::string& date)
    {
        GetWeatherForADay(server, date);
        return 0.0;
    }
    virtual double GetMinimumTemperature(IWeatherServer& server, const std::string& date)
    {
        GetWeatherForADay(server, date);
        return 0.0;
    }
    virtual double GetMaximumTemperature(IWeatherServer& server, const std::string& date)
    {
        GetWeatherForADay(server, date);
        return 0.0;
    }
    virtual double GetAverageWindDirection(IWeatherServer& server, const std::string& date)
    {
        GetWeatherForADay(server, date);
        return 0.0;
    }
    virtual double GetMaximumWindSpeed(IWeatherServer& server, const std::string& date)
    {
        GetWeatherForADay(server, date);
        return 0.0;
    }
private:
    StringContainer GetWeatherForADay(IWeatherServer& server, const std::string& date)
    {
        StringContainer weatherContainer;
        const StringContainer timePoints = {s_3hours, s_9hours, s_15hours, s_21hour};
        for(size_t i = 0; i < timePoints.size(); ++i)
        {
            std::string request = CreateRequest(date, timePoints[i]);
            weatherContainer.push_back(server.GetWeather(request));
        }
        return weatherContainer;
    }
};

StringContainer GenerateExpectedRequests(const std::string& date)
{
    return
    {
        CreateRequest(date,s_3hours),
        CreateRequest(date,s_9hours),
        CreateRequest(date,s_15hours),
        CreateRequest(date,s_21hour)
    };
}

TEST(WeatherClient, ClientCallsServerWith3hoursTimeForAvrTemperature)
{
    WeatherClient client;
    FakeWeatherServer server;
    client.GetAverageTemperature(server, s_testDate);
    ASSERT_EQ(CreateRequest(s_testDate,s_3hours), server.GetRequests().at(0));
}

TEST(WeatherClient, ClientSends4TimesForMinTemp)
{
    StringContainer expectedRequests = GenerateExpectedRequests(s_testDate);
    FakeWeatherServer server;
    WeatherClient client;
    client.GetMinimumTemperature(server, s_testDate);
    ASSERT_EQ(expectedRequests, server.GetRequests());
}
TEST(WeatherClient, FourRequestsForAnyClientCall)
{
    StringContainer expectedRequests = GenerateExpectedRequests(s_testDate);
    WeatherClient client;
    FakeWeatherServer server;

    client.GetAverageTemperature(server, s_testDate);
    ASSERT_EQ(expectedRequests, server.GetRequests());

    client.GetMaximumTemperature(server, s_testDate);
    ASSERT_EQ(expectedRequests, server.GetRequests());

    client.GetAverageWindDirection(server, s_testDate);
    ASSERT_EQ(expectedRequests, server.GetRequests());

    client.GetAverageWindDirection(server, s_testDate);
    ASSERT_EQ(expectedRequests, server.GetRequests());

    client.GetMaximumWindSpeed(server, s_testDate);
    ASSERT_EQ(expectedRequests, server.GetRequests());
}
