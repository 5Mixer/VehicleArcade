# Install single header enet.h
# https://github.com/zpl-c/enet/blob/master/include/enet.h
wget https://raw.githubusercontent.com/zpl-c/enet/master/include/enet.h -O Sources/enet.h

git clone https://github.com/google/flatbuffers.git --depth 1
mv flatbuffers/include/flatbuffers Sources/flatbuffers/
rm -rf flatbuffers
