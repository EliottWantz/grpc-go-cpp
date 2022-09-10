all:
	protoc --proto_path=. --go-grpc_out=. --go_out=. rpc/echo/service.proto