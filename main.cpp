#include<iostream>
#include <memory>
#include <string>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include "book.pb.h"
#include "book.grpc.pb.h"

using namespace std;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using book::Book;
using book::Example;
using book::Books;
using book::Id;

map<int, Book* > bklist;
class BookServiceImpl final : public Example::Service {
  public:
  Status AddBook(ServerContext* context, const Book* request, ::google::protobuf::Empty* response) override{
      Book* book =new Book();
      book->set_id(request->id());
      book->set_name(request->name());
      book->set_author_name(request->author_name());
      bklist[request->id()]=book;
      return Status::OK;
   }

  Status GetBooks(ServerContext* context, const ::google::protobuf::Empty* request, Books* booklist) override {
        for(auto a:bklist)
          {
            Book* bk = booklist->add_book();
            bk->set_id(a.second->id());
            bk->set_name(a.second->name());
            bk->set_author_name(a.second->author_name());
          }
          return Status::OK;
    }

    Status GetBook(ServerContext* context, const Id* ID, Book* book) override {
        Book* bk = bklist[ID->id()];
        book->set_id(bk->id());
        book->set_name(bk->name());
        book->set_author_name(bk->author_name());
        return Status::OK;
    }

    Status UpdateBook(ServerContext* context, const Book* request, ::google::protobuf::Empty* response) override {
        Book* Request=new Book();
        Request->set_id(request->id());
        Request->set_name(request->name());
        Request->set_author_name(request->author_name());
        bklist[request->id()] = Request;
        return Status::OK;
    }

    Status DeleteBook(ServerContext* context, const Id* ID, ::google::protobuf::Empty* response) override {
      bklist.erase(ID->id());
      return Status::OK;
    }
};

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    BookServiceImpl service;
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());

    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

int main(int argc, char** argv) {
  RunServer();
  return 0;
}