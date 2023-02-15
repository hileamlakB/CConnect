import enum


class StatusCode():
    SUCCESS = 0
    INVALID_FUNCTION = 1
    INVALID_ARGUMENTS = 2
    INVALID_USERNAME = 3
    INVALID_PASSWORD = 4
    INVALID_MESSAGE = 5
    INVALID_ACCOUNT = 6
    INVALID_VISION = 7
    USER_NAME_EXISTS = 8
    USER_DOESNT_EXIST = 9
    INCORRECT_PASSWORD = 10
    NON_EXISTING_USER_CANT_BE_DELETED = 11
    INVALID_VERSION = 12
    USER_ALREADY_LOGGED_IN = 13
    USER_NOT_LOGGED_IN = 14
    RECEIVER_DOESNT_EXIST = 15
    MULTIPLE_USERS_ON_SAME_SOCKET = 16
    NO_MESSAGES = 17


class StatusMessages:
    error_dict = {
        StatusCode.SUCCESS: "Success",
        StatusCode.INVALID_FUNCTION: "Invalid function",
        StatusCode.INVALID_ARGUMENTS: "Invalid arguments",
        StatusCode.INVALID_USERNAME: "Invalid username",
        StatusCode.INVALID_PASSWORD: "Invalid password",
        StatusCode.INVALID_MESSAGE: "Invalid message",
        StatusCode.INVALID_ACCOUNT: "Invalid account",
        StatusCode.INVALID_VISION: "Invalid vision",
        StatusCode.USER_NAME_EXISTS: "USER NAME ALREADY EXISTS",
        StatusCode.USER_DOESNT_EXIST: "USER DOESN'T EXIST",
        StatusCode.INCORRECT_PASSWORD: "INCORRECT PASSWORD",
        StatusCode.NON_EXISTING_USER_CANT_BE_DELETED: "NON EXISTING USER CANT BE DELETED",
        StatusCode.INVALID_VERSION: "UNSUPORTED VERSION",
        StatusCode.USER_ALREADY_LOGGED_IN: "USER ALREADY LOGGED IN",
        StatusCode.USER_NOT_LOGGED_IN: "USER NOT LOGGED IN",
        StatusCode.RECEIVER_DOESNT_EXIST: "RECEIVER DOESN'T EXIST",
        StatusCode.MULTIPLE_USERS_ON_SAME_SOCKET: "ONLY ONE USER PER SOCKET ALLOWED",
        StatusCode.NO_MESSAGES: "NO MESSAGES"
    }

    @classmethod
    def get_error_message(cls, error_code: StatusCode):
        return cls.error_dict[error_code]


class HelpMessages:
    HELP_MSG = "Jarvis Is a socket based chat room. \n You can create an account, login, send messages, receive messages, and delete your account. \n To get more information on a specific command, type 'help <command>'. \n Commands: \n create \n list \n login \n send \n receive \n delete \n help \n exit \n"
    HELP_CREATE = "create <username> <password> \n \t Creates a new account with the given username and password. \n"
    HELP_LIST = "list \n \t Lists all users currently logged in. \n"
    HELP_LOGIN = "login <username> <password> \n \t Logs in to an existing account with the given username and password. \n"
    HELP_SEND = "send <username> <message> \n \t Sends a message to the given user. \n"
    HELP_RECEIVE = "receive \n \t Receives all messages sent to you. \n"
    HELP_DELETE = "delete \n \t Deletes your account. \n"
    HELP_EXIT = "exit \n \t Exits the program. \n"
    HELP_HELP = "help \n \t Displays this help message. \n"
    HELP_LOGOUT = "logout \n \t Logs out of your account. \n"
