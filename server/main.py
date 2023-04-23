from fastapi import FastAPI, File, UploadFile
from pydantic import BaseModel
from utilities import make


class Item(BaseModel):
    data: list

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
    local = make.makeList(make.makeTree("."), ".")
    client = item.data
    return compare(client, local)

@app.post("/upload/{location}")
def fileUpload(file: UploadFile, location: str):
    with open(location, "wb") as buffer:
        buffer.write(file.file.read())
    return {"status": "OK"}

