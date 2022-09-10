#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <thread>

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include "echo.grpc.pb.h"

using echo::EchoService;
using echo::Request;
using echo::Response;
using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;

Request MakeRequest(const std::string &message)
{
    Request r;
    r.set_message(message);
    return r;
}

class EchoClient
{
public:
    EchoClient(std::shared_ptr<Channel> channel)
        : stub_(EchoService::NewStub(channel))
    {
    }

    void Echo()
    {
        Request req;
        Response res;
        req = MakeRequest("Ping");
    }

private:
    bool CallEcho(const Request &req, Response *res)
    {
        ClientContext context;
        Status status = stub_->Echo(&context, req, res);
        if (!status.ok())
        {
            std::cout << "GetFeature rpc failed." << std::endl;
            return false;
        }
        if (res->message().empty())
        {
            std::cout << "Server did not respond." << std::endl;
        }
        else
        {
            std::cout << "Got response from server: " << res->message() << std::endl;
        }
        return true;
    }

    std::unique_ptr<EchoService::Stub> stub_;
};

int main(int argc, char **argv)
{
    EchoClient client(grpc::CreateChannel("localhost:8080", grpc::InsecureChannelCredentials()));

    std::cout << "-------------- Echo() --------------" << std::endl;
    client.Echo();

    return 0;
}