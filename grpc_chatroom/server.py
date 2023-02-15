# Copyright 2015 gRPC authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
"""The Python implementation of the GRPC helloworld.Greeter server."""

from concurrent import futures
import logging
import uuid

import grpc
import spec_pb2
import spec_pb2_grpc
from utils import StatusCode, StatusMessages


# represents a single client
class User:

    def __init__(self, username, password):
        self.username = username
        self.password = password
        self.messages = []
        self.login_status = {
            "logged_in": False,
            "session_id": None,
            "subscription_stream": None
        }

    def login(self, session_id):
        self.login_status["logged_in"] = True
        self.login_status["session_id"] = session_id

    def logout(self):
        self.login_status["logged_in"] = False
        self.login_status["session_id"] = None

    def is_logged_in(self):
        return self.login_status["logged_in"]

    def get_session_id(self):
        return self.login_status["session_id"]

    def add_message(self, message):
        self.messages.append(message)


class ClientService(spec_pb2_grpc.ClientAccountServicer):

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.all_users = {
            # username: User
        }
        self.session_map = {
            # session_id: User
        }

    def CreateAccount(self, request, context):

        context.set_code(grpc.StatusCode.OK)
        if request.username in self.all_users:
            status_code = StatusCode.USER_NAME_EXISTS
            status_message = StatusMessages.get_error_message(status_code)
        else:
            # create a new user
            new_user = User(request.username, request.password)
            self.all_users[request.username] = new_user
            status_code = StatusCode.SUCCESS
            status_message = "Account created successfully!!"

        return spec_pb2.ServerResponse(error_code=status_code, error_message=status_message)

    def Login(self, request, context):

        context.set_code(grpc.StatusCode.OK)

        if request.username not in self.all_users:
            status_code = StatusCode.USER_DOESNT_EXIST
            status_message = StatusMessages.get_error_message(status_code)
        else:
            user = self.all_users[request.username]
            if user.password != request.password:
                status_code = StatusCode.INCORRECT_PASSWORD
                status_message = StatusMessages.get_error_message(status_code)
            else:

                user.login(self.GenerateSessionID())
                self.session_map[user.login_status["session_id"]] = user

                status_code = StatusCode.SUCCESS
                status_message = "Login successful!!"
        return spec_pb2.ServerResponse(error_code=status_code, error_message=status_message, session_id=user.login_status["session_id"])

    def Send(self, request, context):

        context.set_code(grpc.StatusCode.OK)

        if request.session_id not in self.session_map:
            status_code = StatusCode.USER_NOT_LOGGED_IN
            status_message = StatusMessages.get_error_message(status_code)
        else:
            user = self.session_map[request.session_id]
            if request.to not in self.all_users:
                status_code = StatusCode.RECEIVER_DOESNT_EXIST
                status_message = StatusMessages.get_error_message(status_code)
            else:
                receiver = self.all_users[request.to]
                msg = spec_pb2.Message(
                    from_=user.username,  message=request.message)

                # check if receiver is logged in if send message right away
                # else store it in the receiver's message queue
                receiver.add_message(msg)

                status_code = StatusCode.SUCCESS
                status_message = "Message sent successfully!!"

        return spec_pb2.ServerResponse(error_code=status_code, error_message=status_message)

    def ListUsers(self, request, context):
        # list all users
        context.set_code(grpc.StatusCode.OK)
        users = spec_pb2.Users()

        for user in self.all_users.values():
            user_ = users.user.add()
            user_.username = user.username
            user_.status = "online" if user.login_status["logged_in"] else "offline"

        return users

    # def SubscribeMessage(self, request, context):

    #     user = self.session_map[request.session_id]
    #     while True:
    #         # return if user is not logged in
    #         if user.login_status["logged_in"] == False:
    #             return

    #         # chec for new messages
    #         if len(user.messages) == 0:
    #             time.sleep(2)
    #             continue

    #         for message in user.messages:
    #             yield spec_pb2.Messages(from_=message.from_, message=message.message)

    def ReceiveMessage(self, request, context):
        # check if the user is logged in
        msgs = spec_pb2.Messages()
        if request.session_id not in self.session_map:
            msgs.error_code = StatusCode.USER_NOT_LOGGED_IN
            msgs.error_message = StatusMessages.get_error_message(
                msgs.error_code)

        else:

            user = self.session_map[request.session_id]
            if len(user.messages) == 0:
                msgs.error_code = StatusCode.NO_MESSAGES
                msgs.error_message = StatusMessages.get_error_message(
                    msgs.error_code)
            else:
                for message in user.messages:
                    msg = msgs.message.add()
                    msg.from_ = message.from_
                    msg.message = message.message
                user.messages = []
                msgs.error_code = StatusCode.SUCCESS
                msgs.error_message = "Messages received successfully!!"

        return msgs

    def Logout(self, request, context):
        if request.session_id not in self.session_map:
            status = StatusCode.USER_NOT_LOGGED_IN
            status_message = status_message.get_error_message(status)
        else:
            user = self.session_map[request.session_id]
            user.logout()
            status = StatusCode.SUCCESS
            status_message = "Logout successful!!"
        return spec_pb2.ServerResponse(error_code=status, error_message=status_message)

    def DeleteAccount(self, request, context):
        # chekc if the user is logged in
        if request.session_id not in self.session_map:
            status_code = StatusCode.USER_NOT_LOGGED_IN
            status_message = status_message.get_error_message(status_code)
        else:
            user = self.session_map[request.session_id]
            del self.all_users[user.username]
            del self.session_map[user.login_status["session_id"]]

            status_code = StatusCode.SUCCESS
            status_message = "Account deleted successfully!!"
        return spec_pb2.ServerResponse(error_code=status_code, error_message=status_message)

    @staticmethod
    def GenerateSessionID():
        return str(uuid.uuid4())


def serve():
    port = '2625'
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    spec_pb2_grpc.add_ClientAccountServicer_to_server(ClientService(), server)
    server.add_insecure_port('[::]:' + port)
    server.start()
    print("Server started, listening on " + port)
    server.wait_for_termination()


if __name__ == '__main__':
    logging.basicConfig()
    serve()
