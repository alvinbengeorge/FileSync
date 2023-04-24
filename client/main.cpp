#include<iostream>
#include<cpr/cpr.h>
#include<nlohmann/json.hpp>
#include<string>
#include<filesystem>

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
    string url = "http://localhost:8000/";
    return url + route;
}


string sendJson(json j) {
    json data;
    data["data"] = j;
    cout << data.dump() << endl;
    string url = URL("items/");
    cpr::Response r = cpr::Post(cpr::Url{url},
                                cpr::Body{data.dump()},
                                cpr::Header{{"content-type", "application/json"}});
    return r.text;
}

json listDir(string path) {
    json files;

    for (const auto & entry : filesystem::directory_iterator(path)) {
        if (!filesystem::is_directory(entry.path())) {
            files.push_back(entry.path());
        } else {
            json sub = listDir(entry.path());
            for (auto & i : sub) {
                files.push_back(i);
            }
            
        }
    }
    return files;
}

void download(string location) {
    string url = URL("download/");
    json data = json{{"path", location}};
    cpr::Response r = cpr::Post(cpr::Url{url},
                                cpr::Body{data.dump()},
                                cpr::Header{{"content-type", "application/json"}});
    // write to a file with path location
    
}

int loop() {
    return 1;
}

int main() {
    /*
    int condition = 1;
    while (condition) {
        condition= loop();
    }
    */
    json files = listDir("./syncingFolder");
    cout << files.dump() << endl;
    string response = sendJson(files);
    cout << json::parse(response) << endl;
    download("./syncingFolder/Bat.jpg");


}