import grpc
import spec_pb2
import spec_pb2_grpc

Accounts = {}


def test_create_account(client_info):

    client = client_info['client']
    username = client_info['username']
    password = client_info['password']
    # Make a request to the server
    request = client.CreateAccount(
        spec_pb2.CreateAccountRequest(username=username, password=password))
    print(request.error_message)
    assert (request.error_code == 0 and request.error_message ==
            "Account created successfully!!")


def test_login(client_info):
    client = client_info['client']
    username = client_info['username']
    password = client_info['password']
    # Make a request to the server
    request = client.Login(
        spec_pb2.LoginRequest(username=username, password=password))
    client_info['session_id'] = request.session_id
    print(request.error_message)
    assert (request.error_code == 0 and request.error_message ==
            "Login successful!!")


def test_list_users(client_info, status, reverse=False):
    client = client_info["client"]
    username = client_info['username']

    response = client.ListUsers(spec_pb2.Empty())

    # print(response)
    for user in response.user:
        if user.username == username and user.status == status:
            assert (not reverse)
            return
    assert (reverse)


def test_send_message(sender_info, receiver_info):
    sender = sender_info['client']
    receiver = receiver_info['username']
    message = "Hello " + receiver
    session_id = sender_info['session_id']

    if 'messages' in receiver_info:
        receiver_info['messages'].append(message)
    else:
        receiver_info['messages'] = [message]

    response = sender.Send(
        spec_pb2.SendRequest(session_id=session_id, message=message, to=receiver))
    print(response.error_message)
    assert (response.error_code == 0 and response.error_message ==
            "Message sent successfully!!")


def test_recv_message(receiver_info):
    receiver = receiver_info['client']
    session_id = receiver_info['session_id']
    messages = receiver_info['messages']

    response = receiver.ReceiveMessage(
        spec_pb2.ReceiveRequest(session_id=session_id))

    for msg in response.message:
        if messages[0] in msg.message:
            assert (True)
            print("Message received successfully!!")
            return
    assert (False)


def test_delete(client_info):
    client = client_info['client']
    session_id = client_info['session_id']

    response = client.DeleteAccount(
        spec_pb2.DeleteAccountRequest(session_id=session_id))
    print(response.error_message)
    assert (response.error_code == 0 and response.error_message ==
            "Account deleted successfully!!")


def test_multi_client_communication(grpc_channel):
    # create client_1
    client_1 = spec_pb2_grpc.ClientAccountStub(grpc_channel)

    # create client_2
    client_2 = spec_pb2_grpc.ClientAccountStub(grpc_channel)

    Accounts["client_1"] = {'username': 'client_1',
                            'password': 'client_1', 'client': client_1}
    Accounts["client_2"] = {'username': 'client_2',
                            'password': 'client_2', 'client': client_2}

    # test create account
    test_create_account(Accounts["client_1"])
    test_create_account(Accounts["client_2"])

    # test list users
    test_list_users(Accounts["client_1"], "offline")

    # test login
    test_login(Accounts["client_1"])
    test_login(Accounts["client_2"])

    # test list users
    test_list_users(Accounts["client_1"], "online")

    # test send message
    test_send_message(Accounts["client_1"], Accounts["client_2"])

    # test receive message
    test_recv_message(Accounts["client_2"])

    # test delete
    test_delete(Accounts["client_1"])

    # test list
    test_list_users(Accounts["client_1"], "offline", True)

    # tests for first client
    # doen't check things like invalid users
    # users already existed and tests like that
    # and stuff like that

    assert (True)


if __name__ == "__main__":
    port = 2625
    grpc_channel = grpc.insecure_channel(f'localhost:{port}')
    test_multi_client_communication(grpc_channel)
