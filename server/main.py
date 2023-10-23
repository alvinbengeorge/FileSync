from fastapi import FastAPI, Form, UploadFile, File
from fastapi.responses import FileResponse
from pydantic import BaseModel
from utilities import make
from typing import Annotated
from os import remove


class Item(BaseModel):
    data: list

class Download(BaseModel):
    path: str

class Location(BaseModel):
    path: str

def compare(client: list, local: list, sync: bool = False) -> dict:
    upload = []
    download = []
    clientPaths = [i["path"] for i in client]
    localPaths = [i["path"] for i in local]
    for i in clientPaths:
        if i not in localPaths:
            upload.append(i)
    for i in localPaths:
        if i not in clientPaths:
            remove(i) if not sync else download.append(i)
    print({"upload": upload, "download": download})
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
    location = location.replace(",", "/")
    with open(location, "wb") as buffer:
        buffer.write(file.file.read())
    return {"status": "success"}

@app.post("/download")
def fileDownload(location: Download):
    return FileResponse(location.path)

@app.post("/sync/")
def items(item: Item): 
    local = make.writeTime(make.makeList(make.makeTree("./syncingFolder"), "./syncingFolder"))
    client = item.data
    return compare(client, local, sync = True)