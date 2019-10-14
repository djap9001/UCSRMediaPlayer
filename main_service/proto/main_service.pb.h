// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: main_service.proto

#ifndef PROTOBUF_INCLUDED_main_5fservice_2eproto
#define PROTOBUF_INCLUDED_main_5fservice_2eproto

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3006001
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/service.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#define PROTOBUF_INTERNAL_EXPORT_protobuf_main_5fservice_2eproto 

namespace protobuf_main_5fservice_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[2];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
}  // namespace protobuf_main_5fservice_2eproto
namespace main_service {
class HttpRequest;
class HttpRequestDefaultTypeInternal;
extern HttpRequestDefaultTypeInternal _HttpRequest_default_instance_;
class HttpResponse;
class HttpResponseDefaultTypeInternal;
extern HttpResponseDefaultTypeInternal _HttpResponse_default_instance_;
}  // namespace main_service
namespace google {
namespace protobuf {
template<> ::main_service::HttpRequest* Arena::CreateMaybeMessage<::main_service::HttpRequest>(Arena*);
template<> ::main_service::HttpResponse* Arena::CreateMaybeMessage<::main_service::HttpResponse>(Arena*);
}  // namespace protobuf
}  // namespace google
namespace main_service {

// ===================================================================

class HttpRequest : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:main_service.HttpRequest) */ {
 public:
  HttpRequest();
  virtual ~HttpRequest();

  HttpRequest(const HttpRequest& from);

  inline HttpRequest& operator=(const HttpRequest& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  HttpRequest(HttpRequest&& from) noexcept
    : HttpRequest() {
    *this = ::std::move(from);
  }

  inline HttpRequest& operator=(HttpRequest&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const HttpRequest& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const HttpRequest* internal_default_instance() {
    return reinterpret_cast<const HttpRequest*>(
               &_HttpRequest_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(HttpRequest* other);
  friend void swap(HttpRequest& a, HttpRequest& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline HttpRequest* New() const final {
    return CreateMaybeMessage<HttpRequest>(NULL);
  }

  HttpRequest* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<HttpRequest>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const HttpRequest& from);
  void MergeFrom(const HttpRequest& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(HttpRequest* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // @@protoc_insertion_point(class_scope:main_service.HttpRequest)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_main_5fservice_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class HttpResponse : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:main_service.HttpResponse) */ {
 public:
  HttpResponse();
  virtual ~HttpResponse();

  HttpResponse(const HttpResponse& from);

  inline HttpResponse& operator=(const HttpResponse& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  HttpResponse(HttpResponse&& from) noexcept
    : HttpResponse() {
    *this = ::std::move(from);
  }

  inline HttpResponse& operator=(HttpResponse&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const HttpResponse& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const HttpResponse* internal_default_instance() {
    return reinterpret_cast<const HttpResponse*>(
               &_HttpResponse_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  void Swap(HttpResponse* other);
  friend void swap(HttpResponse& a, HttpResponse& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline HttpResponse* New() const final {
    return CreateMaybeMessage<HttpResponse>(NULL);
  }

  HttpResponse* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<HttpResponse>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const HttpResponse& from);
  void MergeFrom(const HttpResponse& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(HttpResponse* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // @@protoc_insertion_point(class_scope:main_service.HttpResponse)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_main_5fservice_2eproto::TableStruct;
};
// ===================================================================

class HttpService_Stub;

class HttpService : public ::google::protobuf::Service {
 protected:
  // This class should be treated as an abstract interface.
  inline HttpService() {};
 public:
  virtual ~HttpService();

  typedef HttpService_Stub Stub;

  static const ::google::protobuf::ServiceDescriptor* descriptor();

  virtual void PageRequest(::google::protobuf::RpcController* controller,
                       const ::main_service::HttpRequest* request,
                       ::main_service::HttpResponse* response,
                       ::google::protobuf::Closure* done);

  // implements Service ----------------------------------------------

  const ::google::protobuf::ServiceDescriptor* GetDescriptor();
  void CallMethod(const ::google::protobuf::MethodDescriptor* method,
                  ::google::protobuf::RpcController* controller,
                  const ::google::protobuf::Message* request,
                  ::google::protobuf::Message* response,
                  ::google::protobuf::Closure* done);
  const ::google::protobuf::Message& GetRequestPrototype(
    const ::google::protobuf::MethodDescriptor* method) const;
  const ::google::protobuf::Message& GetResponsePrototype(
    const ::google::protobuf::MethodDescriptor* method) const;

 private:
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(HttpService);
};

class HttpService_Stub : public HttpService {
 public:
  HttpService_Stub(::google::protobuf::RpcChannel* channel);
  HttpService_Stub(::google::protobuf::RpcChannel* channel,
                   ::google::protobuf::Service::ChannelOwnership ownership);
  ~HttpService_Stub();

  inline ::google::protobuf::RpcChannel* channel() { return channel_; }

  // implements HttpService ------------------------------------------

  void PageRequest(::google::protobuf::RpcController* controller,
                       const ::main_service::HttpRequest* request,
                       ::main_service::HttpResponse* response,
                       ::google::protobuf::Closure* done);
 private:
  ::google::protobuf::RpcChannel* channel_;
  bool owns_channel_;
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(HttpService_Stub);
};


// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// HttpRequest

// -------------------------------------------------------------------

// HttpResponse

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace main_service

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_INCLUDED_main_5fservice_2eproto
