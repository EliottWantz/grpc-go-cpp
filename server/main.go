package main

import (
	"context"
	"log"
	"net"
	"test/pb"

	"google.golang.org/grpc"
)

func main() {
	lis, err := net.Listen("tcp", "127.0.0.1:50051")
	if err != nil {
		panic(err)
	}

	s := grpc.NewServer()
	pb.RegisterEchoServiceServer(s, new(Server))
	log.Fatal(s.Serve(lis))
}

type Server struct {
	pb.UnimplementedEchoServiceServer
}

func (s *Server) Echo(_ context.Context, req *pb.Request) (*pb.Response, error) {
	log.Println(req.Message)
	res := &pb.Response{Message: "Pong"}
	return res, nil
}
