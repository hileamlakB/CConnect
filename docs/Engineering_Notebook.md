# Engineering Notebook

This document consists of notes we made while attempting to implement the specifications for the project. The first two sections contain some scratch work, and the later sections contain comparisons, reflections, and what we learned doing this project.

## Socket Programming and Multithreading

Once socket is setup, how do you add multithreading
- You create multiple threads and make each thread accept a connection
- This will fix the number of threads you spawn
- look for a dynamic way to scale and remove threads based on load

How do you convert from an into into a 4 bit char array?
- use unions

## gRPC
This section contains some notes on some things we tried with gRPC

Installing the gRPC using the following instuction

```
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
```
Install protobuf with
```
sudo apt-get install protobuf-compiler
```
Compile the proto file with
```
protoc -I . --cpp_out=. specs.proto
protoc -I . --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` specs.proto
```

## DigitalOcean Droplet

For the server, we decided to use a DigitalOcean Droplet instead of having one of our computers act as the server. We followed the guide [here](https://docs.digitalocean.com/products/droplets/how-to/create/) to spin up our Droplet and get set up.

The reasons for using a Droplet instead of one of our computers is twofold. First, the Droplet has a higher uptime than any of our computers. While this was not an explicit requirement of the specs, the two members of the group often did work on the project at varying odd times (sometimes very early in the morning or very late at night). Because of this, instead of forcing the other member to stay up and run the server, we can just connect to one that is always available in the cloud.

Second, the DigitalOcean Droplet gives us a fixed IP to which we can connect to. This allowed us to bypass a lot of troubles and headaches with attempting to connect to IP addresses that may be dynamically allocated by the network or circumventing firewalls both on the network and on the computer. From past experience, connecting to IP addresses on the MIT network proved to be troublesome for the above reasons, which is why we decided to go directly to a Droplet with a fixed IP address.

## Comparisons

One of the goals of the project was to compare the various aspects and properties of gRPC to defining our own wire protocol.

Both of us found it a lot easier/more intuitive to implement our procotol using gRPC. Consequently, this part of the project took very little time to write. Writing unit tests for the gRPC implementation was also quite straightforward, making gRPC overall a more straightforward and developer-friendly experience.

We note, however, that gRPC does not offer us the same flexibility that defining our own wire protocol does. Namely, gRPC doesn't have the notion of sessions built in, since the client socket is opened, data is exchanged, and the socket is closed, whereas in the wire protocol, we could specify when the sockets were opened and closed.

## Future Work

Given the time constraints, we wanted to implement some more features, but were unable to. Here's just a small subset of what we also wanted to do:

- Implement a graphical interface instead of a command line interface.
- Set up continuous integration to automatically pull the code, build it, run unit tests, and deploy it to our Droplet in the cloud.
- More rigorous unit testing with Google Test.
