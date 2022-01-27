#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include "../proto/helloworld.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using helloworld::HelloRequest;
using helloworld::HelloReply;
using helloworld::Greeter;

// Logic and data behind the server's behavior.
class GreeterServiceImpl final : public Greeter::Service {
	Status TransferBinaryCode(ServerContext* context, const HelloRequest* request,
		HelloReply* reply) override {
		std::string str(request->code());
		std::vector<int> mod;
		bool one = true, two = false;
		int count{ 0 };
		int answer{ 0 };

		// Creating a modified string
		for (char i : str) {
			if (i == '0') {
				if (!two) {
					if (count != 0) {
						mod.push_back(count);
						count = 0;
					}
					if (one) {
						mod.push_back(0);
						two = true;
					}
					else
						one = true;
				}
			}
			else {
				one = false;
				two = false;
				++count;
			}
		}
		if (count != 0) mod.push_back(count);
		if (one && !two) mod.push_back(0);

		// Finding the answer
		if (mod.size() == 1)
			if (mod[0] != 0) answer = mod[0] - 1;
			else {
				for (int i = 0; i < mod.size() - 1; ++i) {
					if (answer < mod[i] + mod[i + 1]) {
						answer = mod[i] + mod[i + 1];
					}
				}
			}

		reply->set_message(std::to_string(answer));
		return Status::OK;
	}
};

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  GreeterServiceImpl service;

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();

  return 0;
}