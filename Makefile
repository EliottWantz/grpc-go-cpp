all:
	protoc \
	--proto_path=. \
	--cpp_out=. \
	--plugin=protoc-gen-grpc=/usr/local/vcpkg/installed/x64-linux/tools/grpc/grpc_cpp_plugin \
	--grpc_out=. \
	pb/echo.proto