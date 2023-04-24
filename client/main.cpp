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


string sendJson(json j) {
    json data;
    data["data"] = j;
    string url = URL("items/");
    cpr::Response r = cpr::Post(cpr::Url{url},
                                cpr::Body{data.dump()},
                                cpr::Header{{"content-type", "application/json"}});
    return r.text;
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

void upload(string location) {
    cout << "Uploading: " << location << endl;
    string trueLocation = location;
    for (int i = 0; i < location.length(); i++) {
        if (location[i] == '/') {
            location[i] = ',';
        }
    }
    
    string url = URL("upload/")+location;
    string command = "sh upload.sh "+trueLocation+" "+url;
    system(command.c_str());
}

int loop() {
    sleep(3);
    json files = listDir("./syncingFolder");
    // cout << files <<endl;
    string response = sendJson(files);
    // cout << response << endl;
    json data = json::parse(response);

    // convert json data to array
    for (auto & i : data["download"]) {
        download(i.get<string>());
    }
    for (auto & i : data["upload"]) {
        upload(i.get<string>());
    }


    return 1;
}

int main() {
    int condition = 1;
    while (condition) {
        condition=loop();
    }

}