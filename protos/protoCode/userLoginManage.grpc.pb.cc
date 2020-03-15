// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: userLoginManage.proto

#include "userLoginManage.pb.h"
#include "userLoginManage.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace proto {
namespace userLoginManage {

static const char* userLoginManageService_method_names[] = {
  "/proto.userLoginManage.userLoginManageService/regist",
  "/proto.userLoginManage.userLoginManageService/login",
  "/proto.userLoginManage.userLoginManageService/logout",
  "/proto.userLoginManage.userLoginManageService/heartBeat",
  "/proto.userLoginManage.userLoginManageService/kickOutUser",
};

std::unique_ptr< userLoginManageService::Stub> userLoginManageService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< userLoginManageService::Stub> stub(new userLoginManageService::Stub(channel));
  return stub;
}

userLoginManageService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_regist_(userLoginManageService_method_names[0], ::grpc::internal::RpcMethod::BIDI_STREAMING, channel)
  , rpcmethod_login_(userLoginManageService_method_names[1], ::grpc::internal::RpcMethod::BIDI_STREAMING, channel)
  , rpcmethod_logout_(userLoginManageService_method_names[2], ::grpc::internal::RpcMethod::BIDI_STREAMING, channel)
  , rpcmethod_heartBeat_(userLoginManageService_method_names[3], ::grpc::internal::RpcMethod::BIDI_STREAMING, channel)
  , rpcmethod_kickOutUser_(userLoginManageService_method_names[4], ::grpc::internal::RpcMethod::BIDI_STREAMING, channel)
  {}

::grpc::ClientReaderWriter< ::proto::userLoginManage::RegistRequest, ::proto::userLoginManage::RegistResponse>* userLoginManageService::Stub::registRaw(::grpc::ClientContext* context) {
  return ::grpc_impl::internal::ClientReaderWriterFactory< ::proto::userLoginManage::RegistRequest, ::proto::userLoginManage::RegistResponse>::Create(channel_.get(), rpcmethod_regist_, context);
}

void userLoginManageService::Stub::experimental_async::regist(::grpc::ClientContext* context, ::grpc::experimental::ClientBidiReactor< ::proto::userLoginManage::RegistRequest,::proto::userLoginManage::RegistResponse>* reactor) {
  ::grpc_impl::internal::ClientCallbackReaderWriterFactory< ::proto::userLoginManage::RegistRequest,::proto::userLoginManage::RegistResponse>::Create(stub_->channel_.get(), stub_->rpcmethod_regist_, context, reactor);
}

::grpc::ClientAsyncReaderWriter< ::proto::userLoginManage::RegistRequest, ::proto::userLoginManage::RegistResponse>* userLoginManageService::Stub::AsyncregistRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc_impl::internal::ClientAsyncReaderWriterFactory< ::proto::userLoginManage::RegistRequest, ::proto::userLoginManage::RegistResponse>::Create(channel_.get(), cq, rpcmethod_regist_, context, true, tag);
}

::grpc::ClientAsyncReaderWriter< ::proto::userLoginManage::RegistRequest, ::proto::userLoginManage::RegistResponse>* userLoginManageService::Stub::PrepareAsyncregistRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncReaderWriterFactory< ::proto::userLoginManage::RegistRequest, ::proto::userLoginManage::RegistResponse>::Create(channel_.get(), cq, rpcmethod_regist_, context, false, nullptr);
}

::grpc::ClientReaderWriter< ::proto::userLoginManage::LoginRequest, ::proto::userLoginManage::LoginResponse>* userLoginManageService::Stub::loginRaw(::grpc::ClientContext* context) {
  return ::grpc_impl::internal::ClientReaderWriterFactory< ::proto::userLoginManage::LoginRequest, ::proto::userLoginManage::LoginResponse>::Create(channel_.get(), rpcmethod_login_, context);
}

void userLoginManageService::Stub::experimental_async::login(::grpc::ClientContext* context, ::grpc::experimental::ClientBidiReactor< ::proto::userLoginManage::LoginRequest,::proto::userLoginManage::LoginResponse>* reactor) {
  ::grpc_impl::internal::ClientCallbackReaderWriterFactory< ::proto::userLoginManage::LoginRequest,::proto::userLoginManage::LoginResponse>::Create(stub_->channel_.get(), stub_->rpcmethod_login_, context, reactor);
}

::grpc::ClientAsyncReaderWriter< ::proto::userLoginManage::LoginRequest, ::proto::userLoginManage::LoginResponse>* userLoginManageService::Stub::AsyncloginRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc_impl::internal::ClientAsyncReaderWriterFactory< ::proto::userLoginManage::LoginRequest, ::proto::userLoginManage::LoginResponse>::Create(channel_.get(), cq, rpcmethod_login_, context, true, tag);
}

::grpc::ClientAsyncReaderWriter< ::proto::userLoginManage::LoginRequest, ::proto::userLoginManage::LoginResponse>* userLoginManageService::Stub::PrepareAsyncloginRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncReaderWriterFactory< ::proto::userLoginManage::LoginRequest, ::proto::userLoginManage::LoginResponse>::Create(channel_.get(), cq, rpcmethod_login_, context, false, nullptr);
}

::grpc::ClientReaderWriter< ::proto::userLoginManage::LogoutRequest, ::proto::userLoginManage::LogoutResponse>* userLoginManageService::Stub::logoutRaw(::grpc::ClientContext* context) {
  return ::grpc_impl::internal::ClientReaderWriterFactory< ::proto::userLoginManage::LogoutRequest, ::proto::userLoginManage::LogoutResponse>::Create(channel_.get(), rpcmethod_logout_, context);
}

void userLoginManageService::Stub::experimental_async::logout(::grpc::ClientContext* context, ::grpc::experimental::ClientBidiReactor< ::proto::userLoginManage::LogoutRequest,::proto::userLoginManage::LogoutResponse>* reactor) {
  ::grpc_impl::internal::ClientCallbackReaderWriterFactory< ::proto::userLoginManage::LogoutRequest,::proto::userLoginManage::LogoutResponse>::Create(stub_->channel_.get(), stub_->rpcmethod_logout_, context, reactor);
}

::grpc::ClientAsyncReaderWriter< ::proto::userLoginManage::LogoutRequest, ::proto::userLoginManage::LogoutResponse>* userLoginManageService::Stub::AsynclogoutRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc_impl::internal::ClientAsyncReaderWriterFactory< ::proto::userLoginManage::LogoutRequest, ::proto::userLoginManage::LogoutResponse>::Create(channel_.get(), cq, rpcmethod_logout_, context, true, tag);
}

::grpc::ClientAsyncReaderWriter< ::proto::userLoginManage::LogoutRequest, ::proto::userLoginManage::LogoutResponse>* userLoginManageService::Stub::PrepareAsynclogoutRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncReaderWriterFactory< ::proto::userLoginManage::LogoutRequest, ::proto::userLoginManage::LogoutResponse>::Create(channel_.get(), cq, rpcmethod_logout_, context, false, nullptr);
}

::grpc::ClientReaderWriter< ::proto::userLoginManage::HeartBeatRequest, ::proto::userLoginManage::HeartBeatResponse>* userLoginManageService::Stub::heartBeatRaw(::grpc::ClientContext* context) {
  return ::grpc_impl::internal::ClientReaderWriterFactory< ::proto::userLoginManage::HeartBeatRequest, ::proto::userLoginManage::HeartBeatResponse>::Create(channel_.get(), rpcmethod_heartBeat_, context);
}

void userLoginManageService::Stub::experimental_async::heartBeat(::grpc::ClientContext* context, ::grpc::experimental::ClientBidiReactor< ::proto::userLoginManage::HeartBeatRequest,::proto::userLoginManage::HeartBeatResponse>* reactor) {
  ::grpc_impl::internal::ClientCallbackReaderWriterFactory< ::proto::userLoginManage::HeartBeatRequest,::proto::userLoginManage::HeartBeatResponse>::Create(stub_->channel_.get(), stub_->rpcmethod_heartBeat_, context, reactor);
}

::grpc::ClientAsyncReaderWriter< ::proto::userLoginManage::HeartBeatRequest, ::proto::userLoginManage::HeartBeatResponse>* userLoginManageService::Stub::AsyncheartBeatRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc_impl::internal::ClientAsyncReaderWriterFactory< ::proto::userLoginManage::HeartBeatRequest, ::proto::userLoginManage::HeartBeatResponse>::Create(channel_.get(), cq, rpcmethod_heartBeat_, context, true, tag);
}

::grpc::ClientAsyncReaderWriter< ::proto::userLoginManage::HeartBeatRequest, ::proto::userLoginManage::HeartBeatResponse>* userLoginManageService::Stub::PrepareAsyncheartBeatRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncReaderWriterFactory< ::proto::userLoginManage::HeartBeatRequest, ::proto::userLoginManage::HeartBeatResponse>::Create(channel_.get(), cq, rpcmethod_heartBeat_, context, false, nullptr);
}

::grpc::ClientReaderWriter< ::proto::userLoginManage::KickOutUserRequest, ::proto::userLoginManage::KickOutUserResponse>* userLoginManageService::Stub::kickOutUserRaw(::grpc::ClientContext* context) {
  return ::grpc_impl::internal::ClientReaderWriterFactory< ::proto::userLoginManage::KickOutUserRequest, ::proto::userLoginManage::KickOutUserResponse>::Create(channel_.get(), rpcmethod_kickOutUser_, context);
}

void userLoginManageService::Stub::experimental_async::kickOutUser(::grpc::ClientContext* context, ::grpc::experimental::ClientBidiReactor< ::proto::userLoginManage::KickOutUserRequest,::proto::userLoginManage::KickOutUserResponse>* reactor) {
  ::grpc_impl::internal::ClientCallbackReaderWriterFactory< ::proto::userLoginManage::KickOutUserRequest,::proto::userLoginManage::KickOutUserResponse>::Create(stub_->channel_.get(), stub_->rpcmethod_kickOutUser_, context, reactor);
}

::grpc::ClientAsyncReaderWriter< ::proto::userLoginManage::KickOutUserRequest, ::proto::userLoginManage::KickOutUserResponse>* userLoginManageService::Stub::AsynckickOutUserRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc_impl::internal::ClientAsyncReaderWriterFactory< ::proto::userLoginManage::KickOutUserRequest, ::proto::userLoginManage::KickOutUserResponse>::Create(channel_.get(), cq, rpcmethod_kickOutUser_, context, true, tag);
}

::grpc::ClientAsyncReaderWriter< ::proto::userLoginManage::KickOutUserRequest, ::proto::userLoginManage::KickOutUserResponse>* userLoginManageService::Stub::PrepareAsynckickOutUserRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncReaderWriterFactory< ::proto::userLoginManage::KickOutUserRequest, ::proto::userLoginManage::KickOutUserResponse>::Create(channel_.get(), cq, rpcmethod_kickOutUser_, context, false, nullptr);
}

userLoginManageService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      userLoginManageService_method_names[0],
      ::grpc::internal::RpcMethod::BIDI_STREAMING,
      new ::grpc::internal::BidiStreamingHandler< userLoginManageService::Service, ::proto::userLoginManage::RegistRequest, ::proto::userLoginManage::RegistResponse>(
          std::mem_fn(&userLoginManageService::Service::regist), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      userLoginManageService_method_names[1],
      ::grpc::internal::RpcMethod::BIDI_STREAMING,
      new ::grpc::internal::BidiStreamingHandler< userLoginManageService::Service, ::proto::userLoginManage::LoginRequest, ::proto::userLoginManage::LoginResponse>(
          std::mem_fn(&userLoginManageService::Service::login), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      userLoginManageService_method_names[2],
      ::grpc::internal::RpcMethod::BIDI_STREAMING,
      new ::grpc::internal::BidiStreamingHandler< userLoginManageService::Service, ::proto::userLoginManage::LogoutRequest, ::proto::userLoginManage::LogoutResponse>(
          std::mem_fn(&userLoginManageService::Service::logout), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      userLoginManageService_method_names[3],
      ::grpc::internal::RpcMethod::BIDI_STREAMING,
      new ::grpc::internal::BidiStreamingHandler< userLoginManageService::Service, ::proto::userLoginManage::HeartBeatRequest, ::proto::userLoginManage::HeartBeatResponse>(
          std::mem_fn(&userLoginManageService::Service::heartBeat), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      userLoginManageService_method_names[4],
      ::grpc::internal::RpcMethod::BIDI_STREAMING,
      new ::grpc::internal::BidiStreamingHandler< userLoginManageService::Service, ::proto::userLoginManage::KickOutUserRequest, ::proto::userLoginManage::KickOutUserResponse>(
          std::mem_fn(&userLoginManageService::Service::kickOutUser), this)));
}

userLoginManageService::Service::~Service() {
}

::grpc::Status userLoginManageService::Service::regist(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::proto::userLoginManage::RegistResponse, ::proto::userLoginManage::RegistRequest>* stream) {
  (void) context;
  (void) stream;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status userLoginManageService::Service::login(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::proto::userLoginManage::LoginResponse, ::proto::userLoginManage::LoginRequest>* stream) {
  (void) context;
  (void) stream;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status userLoginManageService::Service::logout(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::proto::userLoginManage::LogoutResponse, ::proto::userLoginManage::LogoutRequest>* stream) {
  (void) context;
  (void) stream;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status userLoginManageService::Service::heartBeat(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::proto::userLoginManage::HeartBeatResponse, ::proto::userLoginManage::HeartBeatRequest>* stream) {
  (void) context;
  (void) stream;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status userLoginManageService::Service::kickOutUser(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::proto::userLoginManage::KickOutUserResponse, ::proto::userLoginManage::KickOutUserRequest>* stream) {
  (void) context;
  (void) stream;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace proto
}  // namespace userLoginManage

