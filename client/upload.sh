# read the string from the arg and save it to string
string=$1
url=$2
# curl -X 'POST' \
#     'http://localhost:8000/upload' \
#     -H 'accept: application/json' \
#     -H 'Content-Type: multipart/form-data' \
#     -F 'location=./syncingFolder/error.png' \
#     -F 'file=@syncingFoler/error.png;type=image/png' 

# building the above command with the string
curl -X 'POST' \
    "$2" \
    -H 'accept: application/json' \
    -H 'Content-Type: multipart/form-data' \
    -F "location=$string" \
    -F "file=@$string;type=image/png"