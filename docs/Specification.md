# Wire Protocol Specification

Version 1, last modified Feb. 22, 2023.

This document contains an overview of the wire protocol we defined for client/server communication. For a comprehensive list of function codes, error codes, and server errors, please refer to the `utils.h` file located at the root of the repository.

## Function Codes

Here is a list of functions, and their corresponding codes, that the server will respond to.
```
CREATE_ACCOUNT 1
LOGIN 2
LIST_ACCOUNTS 3
SEND_MESSAGE 4
RECEIVE_MESSAGE 5
DELETE_ACCOUNT 6
```

## Client Function Call Format

The client will make a function call to the server in the following format: the version number of the wire protocol, followed by the function code of the desired function, followed by any data required by that function.
```
[version number (int)][function code (int)][data]
```

## Server Response Format

After a function call is received by the client and processed, the server will respond in the following format: the version number of the wire protocol, followed by the function code of the desired function, followed by the error code (0 if success, other error codes are specified in `utils.h`), followed by any data that is supposed to be returned from the function call.
``` 
[version number (int)][function code (int)][error code (int)][data]
```

## Data Format

You may have noticed that in the two previous sections above, the format of `[data]` has been underspecified. This is because the data format depends on which function was made, and if it was a request from the client or a response from the server. This section will describe the data formatting for requests and responses for each function code.

### `CREATE_ACCOUNT` (function code 1)
Request: `[username length (int)][username (string)][password length (int)][password (string)]`  
Response: `[error code][account id (int, only if success)]`

### `LOGIN` (function code 2)
Request: `[username length (int)][username (string)][password length (int)][password (string)]`  
Response: `[error code][session id (int, only if success)]`

### `LIST_ACCOUNTS` (function code 3)
Request: Nothing  
Response: `[error code (int)][number of accounts (int)][account ID and username pairs (only if success)]`

### `SEND_MESSAGE` (function code 4)
Request: `[recipient name length (int)][recipient name (string)][message length (int)][message (string)]`  
Response: `[error code (int)][message id (int, only if success)]`

### `RECEIVE_MESSAGE` (function code 5)
Request: LOGIN REQUIRED [TODO what is this?]  
Response: `[error code (int)][number of messages (in)][message ID and content pairs (only if success)]`

### `DELETE_ACCOUNT` (function code 6)
Request: LOGIN REQUIRED [TODO what is this?]  
Response: `[error code (int)][account id (int, only if success)]`

Note: The error codes, account ID, session ID, and message ID can be of any appropriate numeric type that can represent the required range of values specified in `utils.h`.
