#include <iostream>
#include <curl/curl.h>
#include <json/json.h> // Install and include the JsonCpp library

// Function to handle the data received by cURL
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* data) {
    size_t totalSize = size * nmemb;
    data->append((char*)contents, totalSize);
    return totalSize;
}

// Function to fetch weather data
std::string fetchWeatherData(const std::string& apiUrl) {
    CURL* curl;
    CURLcode res;
    std::string responseData;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);
    }
    return responseData;
}

int main() {
    // Replace with your OpenWeatherMap API key
    const std::string apiKey = "8c2b98e9957d1baa97a9aecb15f14af9";
    std::string city;

    std::cout << "Enter city name: ";
    std::cin >> city;

    std::string apiUrl = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + apiKey + "&units=metric";

    std::string weatherData = fetchWeatherData(apiUrl);

    Json::CharReaderBuilder readerBuilder;
    Json::Value jsonData;
    std::string errs;

    std::istringstream weatherDataStream(weatherData);
    if (Json::parseFromStream(readerBuilder, weatherDataStream, &jsonData, &errs)) {
        if (jsonData.isMember("main")) {
            double temperature = jsonData["main"]["temp"].asDouble();
            double humidity = jsonData["main"]["humidity"].asDouble();
            std::string weather = jsonData["weather"][0]["description"].asString();

            std::cout << "Weather in " << city << ":\n";
            std::cout << "Temperature: " << temperature << " °C\n";
            std::cout << "Humidity: " << humidity << "%\n";
            std::cout << "Description: " << weather << "\n";
        } else {
            std::cerr << "City not found or invalid API response." << std::endl;
        }
    } else {
        std::cerr << "Failed to parse JSON response." << std::endl;
    }
w
    return 0;
}
