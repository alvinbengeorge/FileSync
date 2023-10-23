#include<iostream>
#include<cpr/cpr.h>
#include<nlohmann/json.hpp>
#include<string>
#include<filesystem>
#include<sys/stat.h>
#include<stdio.h>
#include<unistd.h>

using json  = nlohmann::json;
using namespace std;


string URL(string route) {
    string url = "http://localhost:8000/";
    return url + route;
}

void download(string location) {
    cout << "Downloaded: " << location << endl;
    string url = URL("download/");
    json data = json{{"path", location}};
    cpr::Response r = cpr::Post(cpr::Url{url},
                                cpr::Body{data.dump()},
                                cpr::Header{{"content-type", "application/json"}});
    ofstream file(location);
    file << r.text;
    file.close();
}

json writeTime(json files) {
    json newData;
    for (auto & i : files) {
        json temp;
        temp["path"] = i.get<string>();
        // last modified time using stat
        struct stat attr;
        stat(i.get<string>().c_str(), &attr);
        temp["time"] = attr.st_mtime;
        newData.push_back(temp);
    }
    return newData;
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
    return writeTime(files);
}

string sendJson(json j) {
    json data;
    data["data"] = j;
    string url = URL("sync/");
    cpr::Response r = cpr::Post(cpr::Url{url},
                                cpr::Body{data.dump()},
                                cpr::Header{{"content-type", "application/json"}});
    cout << r.text << endl;
    return r.text;
}

int main() {
    json files = listDir("./syncingFolder");
    string response = sendJson(files);
    json data = json::parse(response);
    for (auto & i : data["download"]) {
        download(i.get<string>());
    }
}