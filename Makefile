all:
	protoc --proto_path=. --go-grpc_out=. --go_out=. pb/echo.proto