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
    string url = "https://localhost:8000/";
    return url + route;
}

//function to send the json to the server
json sendJson(json j, string folderID) {
    json data;
    data["data"] = j;
    string url = URL("items/");
    cpr::Response r = cpr::Post(cpr::Url{url},
                                cpr::Body{data.dump()},
                                cpr::Header{{"content-type", "application/json"},
                                            {"id", folderID}});
    return json::parse(r.text);
}

json listDir(string path) {
    json files;
    // get current working directory
    cout << filesystem::current_path() << endl;

    for (const auto & entry : filesystem::directory_iterator(path)) {
        if (!filesystem::is_directory(entry.path())) {
            files.push_back(entry.path());
        } else {
            json sub = listDir(entry.path());
            // convert json sub to array
            
        }
    }
    return files;
}

void loop() {}

int main() {
    string path = "./syncingFolder";
    json files = listDir(path);
    // json j = sendJson(files, "1");
    cout << files.dump(4) << endl;
    return 0;

}