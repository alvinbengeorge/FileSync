#include<iostream>
#include<cpr/cpr.h>
#include<nlohmann/json.hpp>
#include<string>

using json  = nlohmann::json;
using namespace std;

// function to write an array to a json
json writeArrayToJson(int arr[], int n) {
    json j;
    for (int i = 0; i < n; i++) {
        j.push_back(arr[i]);
    }
    return j;
}

string URL(string route) {
    string url = "https://localhost:8080/";
    string url = url + route;
    return url;
}

//function to send the json to the server
json sendJson(json j, string folderID) {
    json data;
    data['data'] = j;
    string url = URL("items/");
    cpr::Response r = cpr::Post(cpr::Url{url},
                                cpr::Body{data.dump()},
                                cpr::Header{{"content-type", "application/json"},
                                            {"id", folderID}});
    return json::parse(r.text);
}

