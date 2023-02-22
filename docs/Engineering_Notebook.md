Once socket is setup, how do you add multithreading
    - You create multiple threads and make each thread accept a connection
    - This will fix the number of threads you spawn
    - look for a dynamic way to scale and remove threads based on load
How do you convert from an into into a 4 bit char array?
    - use unions
How should


Installing the Grpc using the following instuction

sudo apt-get update
sudo apt-get install -y build-essential autoconf libtool pkg-config cmake
sudo apt-get install -y libgflags-dev libgtest-dev clang libc++-dev
sudo apt-get install -y protobuf-compiler-grpc libprotobuf-dev-grpc

git clone --recurse-submodules -b v1.50.0 --depth 1 --shallow-submodules https://github.com/grpc/grpc
cd grpc
mkdir -p cmake/build
cd cmake/build
cmake ../..
make -j$(nproc)
sudo make install
sudo ldconfig

cd ~/grpc/examples/cpp/helloworld
make
./greeter_server &
./greeter_client

Install protobuf with
sudo apt-get install protobuf-compiler

Compile the proto file with
protoc -I . --cpp_out=. specs.proto
protoc -I . --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` specs.proto

[TODO, the contents below are from the old engineering book file in the grpc_chatroom folder]

1. First install grpc for python and follow the tuturial from grpc
python -m pip install --upgrade pip
python -m pip install grpcio

2. Install the protoc compiler
python -m pip install grpcio-tools

3. Define the service in a .proto file.

4. Generate server and client code with protoc
python -m grpc_tools.protoc -I . --python_out=. --pyi_out=. --grpc_python_out=. spec.proto


Logging out is optional but is important otherwise other user might be able to see recieved messages