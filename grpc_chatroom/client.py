from __future__ import print_function

import logging
import threading
import cmd

import grpc
import spec_pb2
import spec_pb2_grpc
import time
from utils import HelpMessages


class Jarves_Client(cmd.Cmd):
    prompt = "Jarves> "

    def __init__(self, port):
        super(Jarves_Client, self).__init__()

        self.user_session_id = ""
        self.channel = grpc.insecure_channel(f'localhost:{port}')
        self.stub = spec_pb2_grpc.ClientAccountStub(self.channel)

        self.do_help("")

    # The following methods parse the client input depending on the given command
    def do_list(self, arg):
        response = self.stub.ListUsers(spec_pb2.Empty())
        for user in response.user:
            print("=>", user.username, "[", user.status, "]")

    def do_create(self, arg):
        args = arg.split(" ")
        if (len(args) < 2):
            print("Invalid Arguments to command")
            return
        
        username = args[0]
        password = args[1]

        response = self.stub.CreateAccount(
            spec_pb2.CreateAccountRequest(username=username, password=password))
        print(response.error_message)

    def do_login(self, arg):

        args = arg.split(" ")
        if (len(args) < 2):
            print("Invalid Arguments to command")
            return
        username = args[0]
        password = args[1]

        response = self.stub.Login(
            spec_pb2.LoginRequest(username=username, password=password))
        if response.error_code == 0:
            self.user_session_id = response.session_id
            threading.Thread(target=self.receive_thread).start()

        print(response.error_message)
        

    def do_send(self, arg):

        args = arg.split(" ")
        if (len(args) < 3):
            print("Invalid Arguments to command")
            return

        to = args[0]
        message = ' '.join(args[1:])

        response = self.stub.Send(
            spec_pb2.SendRequest(session_id=self.user_session_id, message=message, to=to))
        print(response.error_message)

    def do_logout(self, arg):
        response = self.stub.Logout(
            spec_pb2.DeleteAccount(session_id=self.user_session_id))
        if response.error_code == 0:
            self.user_session_id = ""
        print(response.error_message)
        return response

    def do_exit(self, arg):
        # close the channel
        self.channel.close()
        quit()

    def do_delete(self, arg):
        response = self.stub.DeleteAccount(
            spec_pb2.DeleteAccountRequest(session_id=self.user_session_id))
        print(response.error_message)
        return response

    def do_help(self, arg):
        if arg == "list":
            print(HelpMessages.LIST_HELP)
        elif arg == "create":
            print(HelpMessages.CREATE_HELP)
        elif arg == "login":
            print(HelpMessages.LOGIN_HELP)
        elif arg == "send":
            print(HelpMessages.SEND_HELP)
        elif arg == "logout":
            print(HelpMessages.LOGOUT_HELP)
        elif arg == "exit":
            print(HelpMessages.EXIT_HELP)
        elif arg == "delete":
            print(HelpMessages.DELETE_HELP)
        elif arg == "help":
            print(HelpMessages.HELP_HELP)
        else:
            print(HelpMessages.HELP_MSG)

    def emptyline(self):
        return None
    

    def receive_thread(self):
    # Create a stream for receiving messages
        while True:
            msgs = self.stub.ReceiveMessage(
                spec_pb2.ReceiveRequest(session_id=self.user_session_id))

            if msgs.error_code != 0:
                time.sleep(2)
                continue
            else:
                for message in msgs.message:
                    print(f"{message.from_}: {message.message}")
                if msgs.error_code != 0:
                    return


if __name__ == '__main__':
    logging.basicConfig()
    Jarves_Client(2625).cmdloop()
