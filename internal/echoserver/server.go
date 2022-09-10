package echoserver

import (
	"app/rpc/echo"
	"context"
	"log"
)

type Server struct {
	echo.UnimplementedEchoServiceServer
}

func (s *Server) Echo(_ context.Context, req *echo.Request) (*echo.Response, error) {
	log.Println(req.Message)
	res := &echo.Response{Message: "Pong"}
	return res, nil
}
