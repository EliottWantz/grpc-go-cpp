package main

import (
	"log"
	"net"

	"app/internal/echoserver"
	"app/rpc/echo"

	"google.golang.org/grpc"
)

func main() {
	lis, err := net.Listen("tcp", "127.0.0.1:50051")
	if err != nil {
		panic(err)
	}

	s := grpc.NewServer()
	echo.RegisterEchoServiceServer(s, new(echoserver.Server))
	log.Fatal(s.Serve(lis))
}
