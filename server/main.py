from fastapi import FastAPI, Form, UploadFile, File
from fastapi.responses import FileResponse
from pydantic import BaseModel
from utilities import make
from typing import Annotated


class Item(BaseModel):
    data: list

class Download(BaseModel):
    path: str


def compare(client: list, local: list) -> dict:
    upload = []
    download = []
    for i in client:
        if i not in local:
            upload.append(i)
    for i in local:
        if i not in client:
            download.append(i)
    return {"upload": upload, "download": download}

app = FastAPI()

@app.get("/")
def read_root():
    return {"Hello": "World"}

@app.post("/items/")
def items(item: Item): 
    local = make.makeList(make.makeTree("./syncingFolder"), "./syncingFolder")
    client = item.data
    return compare(client, local)

@app.post("/upload")
def fileUpload(file: Annotated[UploadFile, File()], location: Annotated[str, Form()]):
    print(location)
    with open(location, "wb") as buffer:
        buffer.write(file.file.read())
    return {"status": "success"}

@app.post("/download")
def fileDownload(location: Download):
    print(location)
    return FileResponse(location.path)

