from fastapi import FastAPI, Form, UploadFile, File
from fastapi.responses import FileResponse
from pydantic import BaseModel
from utilities import make
from typing import Annotated


class Item(BaseModel):
    data: list

class Download(BaseModel):
    path: str

class Location(BaseModel):
    path: str

def compare(client: list, local: list) -> dict:
    upload = []
    download = []
    clientPaths = [i["path"] for i in client]
    localPaths = [i["path"] for i in local]
    for i in clientPaths:
        if i not in localPaths:
            upload.append(i)
    for i in localPaths:
        if i not in clientPaths:
            download.append(i)
    return {"upload": upload, "download": download}

app = FastAPI()

@app.get("/")
def read_root():
    return {"Hello": "World"}

@app.post("/items/")
def items(item: Item): 
    local = make.writeTime(make.makeList(make.makeTree("./syncingFolder"), "./syncingFolder"))
    client = item.data
    return compare(client, local)

@app.post("/upload/{location}")
def fileUpload(location, file: Annotated[UploadFile, File()]):
    print(location)
    location = location.replace("*", "/")
    with open(location, "wb") as buffer:
        buffer.write(file.file.read())
    return {"status": "success"}

@app.post("/download")
def fileDownload(location: Download):
    print(location)
    return FileResponse(location.path)

