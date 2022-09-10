package main

import (
	"context"
	"log"
	"net"
	"test/pb"

	"google.golang.org/grpc"
)

func main() {
	err := Main(context.Background())
	if err != nil {
		panic(err)
	}
}

func Main(ctx context.Context) error {
	lis, err := net.Listen("tcp", ":8080")
	if err != nil {
		return err
	}

	s := grpc.NewServer()
	srv := &Server{}
	pb.RegisterEchoServiceServer(s, srv)
	if err != nil {
		return err
	}
	return s.Serve(lis)
}

type Server struct {
	pb.UnimplementedEchoServiceServer
}

func (s *Server) MakeEcho(_ context.Context, req *pb.Request) (*pb.Response, error) {
	log.Println(req.Message)
	res := &pb.Response{Message: "Pong"}
	return res, nil
}
