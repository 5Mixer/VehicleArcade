# wget -nc (no clobber) doesn't download if destination exists, however raises an err exit code
# checking here works

# Install single header enet.h
# https://github.com/zpl-c/enet/blob/master/include/enet.h
if [ ! -f Sources/enet.h ]; then
    wget -nc https://raw.githubusercontent.com/zpl-c/enet/master/include/enet.h -O Sources/enet.h
fi

# Install simdjson
if [ ! -f Sources/simdjson.h ]; then
    wget -nc https://raw.githubusercontent.com/simdjson/simdjson/master/singleheader/simdjson.h -O Sources/simdjson.h
    wget -nc https://raw.githubusercontent.com/simdjson/simdjson/master/singleheader/simdjson.cpp -O Sources/simdjson.cpp
fi

# Install flatbuffers includes if the flatbuffers dir doesn't exist
if [ ! -d Sources/flatbuffers/ ]; then
    git clone https://github.com/google/flatbuffers.git --depth 1
    mv flatbuffers/include/flatbuffers Sources/flatbuffers/
    rm -rf flatbuffers
fi
