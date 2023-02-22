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
[TODO] Is there any setup required for this?

## Testing

[TODO] Unit tests!!!!

For more information about unit tests, please refer to `docs/Tests.md`.

## Contributing
If you'd like to contribute to this project, please fork the repository and make a pull request.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
