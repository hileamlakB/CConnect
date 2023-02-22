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