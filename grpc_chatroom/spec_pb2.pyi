from google.protobuf.internal import containers as _containers
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Iterable as _Iterable, Mapping as _Mapping, Optional as _Optional, Union as _Union

DESCRIPTOR: _descriptor.FileDescriptor

class CreateAccountRequest(_message.Message):
    __slots__ = ["password", "username"]
    PASSWORD_FIELD_NUMBER: _ClassVar[int]
    USERNAME_FIELD_NUMBER: _ClassVar[int]
    password: str
    username: str
    def __init__(self, username: _Optional[str] = ..., password: _Optional[str] = ...) -> None: ...

class DeleteAccountRequest(_message.Message):
    __slots__ = ["session_id"]
    SESSION_ID_FIELD_NUMBER: _ClassVar[int]
    session_id: str
    def __init__(self, session_id: _Optional[str] = ...) -> None: ...

class Empty(_message.Message):
    __slots__ = []
    def __init__(self) -> None: ...

class LoginRequest(_message.Message):
    __slots__ = ["password", "username"]
    PASSWORD_FIELD_NUMBER: _ClassVar[int]
    USERNAME_FIELD_NUMBER: _ClassVar[int]
    password: str
    username: str
    def __init__(self, username: _Optional[str] = ..., password: _Optional[str] = ...) -> None: ...

class Message(_message.Message):
    __slots__ = ["from_", "message"]
    FROM__FIELD_NUMBER: _ClassVar[int]
    MESSAGE_FIELD_NUMBER: _ClassVar[int]
    from_: str
    message: str
    def __init__(self, from_: _Optional[str] = ..., message: _Optional[str] = ...) -> None: ...

class Messages(_message.Message):
    __slots__ = ["error_code", "error_message", "message"]
    ERROR_CODE_FIELD_NUMBER: _ClassVar[int]
    ERROR_MESSAGE_FIELD_NUMBER: _ClassVar[int]
    MESSAGE_FIELD_NUMBER: _ClassVar[int]
    error_code: int
    error_message: str
    message: _containers.RepeatedCompositeFieldContainer[Message]
    def __init__(self, error_code: _Optional[int] = ..., error_message: _Optional[str] = ..., message: _Optional[_Iterable[_Union[Message, _Mapping]]] = ...) -> None: ...

class ReceiveRequest(_message.Message):
    __slots__ = ["session_id"]
    SESSION_ID_FIELD_NUMBER: _ClassVar[int]
    session_id: str
    def __init__(self, session_id: _Optional[str] = ...) -> None: ...

class SendRequest(_message.Message):
    __slots__ = ["message", "session_id", "to"]
    MESSAGE_FIELD_NUMBER: _ClassVar[int]
    SESSION_ID_FIELD_NUMBER: _ClassVar[int]
    TO_FIELD_NUMBER: _ClassVar[int]
    message: str
    session_id: str
    to: str
    def __init__(self, to: _Optional[str] = ..., message: _Optional[str] = ..., session_id: _Optional[str] = ...) -> None: ...

class ServerResponse(_message.Message):
    __slots__ = ["error_code", "error_message", "session_id"]
    ERROR_CODE_FIELD_NUMBER: _ClassVar[int]
    ERROR_MESSAGE_FIELD_NUMBER: _ClassVar[int]
    SESSION_ID_FIELD_NUMBER: _ClassVar[int]
    error_code: int
    error_message: str
    session_id: str
    def __init__(self, error_code: _Optional[int] = ..., error_message: _Optional[str] = ..., session_id: _Optional[str] = ...) -> None: ...

class User(_message.Message):
    __slots__ = ["status", "username"]
    STATUS_FIELD_NUMBER: _ClassVar[int]
    USERNAME_FIELD_NUMBER: _ClassVar[int]
    status: str
    username: str
    def __init__(self, username: _Optional[str] = ..., status: _Optional[str] = ...) -> None: ...

class Users(_message.Message):
    __slots__ = ["user"]
    USER_FIELD_NUMBER: _ClassVar[int]
    user: _containers.RepeatedCompositeFieldContainer[User]
    def __init__(self, user: _Optional[_Iterable[_Union[User, _Mapping]]] = ...) -> None: ...
