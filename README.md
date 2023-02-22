# CConnect
This repository contains code for the first design exercise of the Spring 2023 edition of Harvard's CS262: Introduction to Distributed Systems.

At a high level, the first design project consisted of building a simple chat application in two parts:
* Design a wire protocol and implement code in C for both the client and server to communicate.
* Design the same system, but use Python gRPC.

## Getting Started
Follow these instructions to get set up for demo day. We assume that students in the class will have a working C compiler set up and have working proficiency in C and Python.

### Setup (Wire Protocol)
1. Clone the repository to your local machine by running  
`git clone https://github.com/hileamlakB/CConnect.git`

2. Navigate to the project directory by running  
`cd CConnect`

3. Build the client files by running  
`make` or `make client`

4. Run the compiled binary by running  
`./client`

You should now see a console window and can start chatting! For example commands and interaction flows, please refer to `docs/Examples.md`.

Note that while we provide code for the server, you (the user) do not need to build or setup the server. This is because we have set up a DigitalOcean Droplet running the server code. All you have to do is connect to that server. For more details about our server setup, please refer to `docs/Engineering_Notebook.md`.

For more information about the specification of the wire protocol, please refer to `docs/Specification.md`

### Setup (gRPC)

The setup for the gRPC version follows largely from the gRPC tutorial in the documentation. First, install gRPC for Python by running
```
python -m pip install --upgrade pip
python -m pip install grpcio
```
Then, install the `protoc` compiler by running `python -m pip install grpcio-tools`.

Define the services in a `.proto` file, which we have done in `grpc_chatroom/spec.proto`.

Finally, generate the server and client code with `protoc` by running `python -m grpc_tools.protoc -I . --python_out=. --pyi_out=. --grpc_python_out=. spec.proto`.

To run the client, run the file `client.py` as you would any other Python file, and the command line based chatroom should launch.

## Testing

Unit testing for the gRPC implementation can be found in `grpc_chatroom/unit_test.py`, where the core functionality of the chatroom are tested. To run the unit tests, run the file as you would any Python file, by simply running `python unit_test.py`. Ensure that the server is running in another terminal by running `python server.py`.

Unit testing for the C wire protocol version can be found in [TODO]

We have a pipeline setup, does the same implementation, except the input comes from a pipe. Write a shell script, pipe in the inputs to the client, spawn multiple clients. 

## Contributing
If you'd like to contribute to this project, please fork the repository and make a pull request.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
