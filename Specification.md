Functions the server responds to:
CREATE_ACCOUNT 1
LOGIN 2
LIST_ACCOUNTS 3
SEND_MESSAGE 4
RECEIVE_MESSAGE 5
DELETE_ACCOUNT 6

Function call format:
[version number[char]][function number[char]][data]

Response format:
[version number(int)][function number][error code][data]

Data format depends on function number.
For each function, the data should be formatted as follows:

CREATE_ACCOUNT:
Request: [username length(5-20)][username][password length(5-20)][password]
Response: [error code (0 for success, non-zero for error)][account id (only if success)]

LOGIN:
Request: [username length(5-20)][username][password length(5-20)][password]
Response: [error code (0 for success, non-zero for error)][session id (only if success)]

LIST_ACCOUNTS:
Request: Nothing
Response: [error code (0 for success, non-zero for error)][number of accounts][account ID and username pairs (only if success)]

SEND_MESSAGE:
Request: [recipient length(5-20)][recipient][message length][message]
Response: [error code (0 for success, non-zero for error)][message id (only if success)]

RECEIVE_MESSAGE:
Request: LOGIN REQUIRED
Response: [error code (0 for success, non-zero for error)][number of messages][message ID and content pairs (only if success)]

DELETE_ACCOUNT:
Request: LOGIN REQUIRED
Response: [error code (0 for success, non-zero for error)][account id (only if success)]

Note: The error codes, account ID, session ID, and message ID can be of any appropriate numeric type that can represent the required range of values.
