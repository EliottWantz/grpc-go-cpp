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

class EchoClient
{
public:
    std::unique_ptr<EchoService::Stub> stub_;

    EchoClient(std::shared_ptr<Channel> channel)
        : stub_(EchoService::NewStub(channel)) {}
    // Assambles the client's payload, sends it and presents the response back
    // from the server.
    std::string Echo(const std::string &msg)
    {
        // Data we are sending to the server.
        Request request;
        request.set_message(msg);
        // Container for the data we expect from the server.
        Response reply;
        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;
        // The actual RPC.
        Status status = stub_->Echo(&context, request, &reply);
        // Act upon its status.
        if (status.ok())
        {
            return reply.message();
        }
        else
        {
            std::cout << status.error_code() << ": " << status.error_message()
                      << std::endl;
            return "RPC failed";
        }
    }
};

int main(int argc, char **argv)
{
    // Instantiate the client. It requires a channel, out of which the actual RPCs
    // are created. This channel models a connection to an endpoint (in this case,
    // localhost at port 50051). We indicate that the channel isn't authenticated
    // (use of InsecureChannelCredentials()).
    EchoClient greeter(grpc::CreateChannel(
        "127.0.0.1:50051", grpc::InsecureChannelCredentials()));

    // Making the Echo request
    std::string reply = greeter.Echo("Ping");
    std::cout << "Received: " << reply << std::endl;
    return 0;
}