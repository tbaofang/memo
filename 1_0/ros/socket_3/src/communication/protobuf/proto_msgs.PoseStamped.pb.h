// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: proto_msgs.PoseStamped.proto

#ifndef PROTOBUF_proto_5fmsgs_2ePoseStamped_2eproto__INCLUDED
#define PROTOBUF_proto_5fmsgs_2ePoseStamped_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3004000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3004000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
namespace proto_msg {
class PoseStamped;
class PoseStampedDefaultTypeInternal;
extern PoseStampedDefaultTypeInternal _PoseStamped_default_instance_;
}  // namespace proto_msg

namespace proto_msg {

namespace protobuf_proto_5fmsgs_2ePoseStamped_2eproto {
// Internal implementation detail -- do not call these.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[];
  static const ::google::protobuf::uint32 offsets[];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static void InitDefaultsImpl();
};
void AddDescriptors();
void InitDefaults();
}  // namespace protobuf_proto_5fmsgs_2ePoseStamped_2eproto

// ===================================================================

class PoseStamped : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:proto_msg.PoseStamped) */ {
 public:
  PoseStamped();
  virtual ~PoseStamped();

  PoseStamped(const PoseStamped& from);

  inline PoseStamped& operator=(const PoseStamped& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  PoseStamped(PoseStamped&& from) noexcept
    : PoseStamped() {
    *this = ::std::move(from);
  }

  inline PoseStamped& operator=(PoseStamped&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const PoseStamped& default_instance();

  static inline const PoseStamped* internal_default_instance() {
    return reinterpret_cast<const PoseStamped*>(
               &_PoseStamped_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void Swap(PoseStamped* other);
  friend void swap(PoseStamped& a, PoseStamped& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline PoseStamped* New() const PROTOBUF_FINAL { return New(NULL); }

  PoseStamped* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const PoseStamped& from);
  void MergeFrom(const PoseStamped& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(PoseStamped* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // string frame_id = 2;
  void clear_frame_id();
  static const int kFrameIdFieldNumber = 2;
  const ::std::string& frame_id() const;
  void set_frame_id(const ::std::string& value);
  #if LANG_CXX11
  void set_frame_id(::std::string&& value);
  #endif
  void set_frame_id(const char* value);
  void set_frame_id(const char* value, size_t size);
  ::std::string* mutable_frame_id();
  ::std::string* release_frame_id();
  void set_allocated_frame_id(::std::string* frame_id);

  // double stamp = 1;
  void clear_stamp();
  static const int kStampFieldNumber = 1;
  double stamp() const;
  void set_stamp(double value);

  // double x = 3;
  void clear_x();
  static const int kXFieldNumber = 3;
  double x() const;
  void set_x(double value);

  // double y = 4;
  void clear_y();
  static const int kYFieldNumber = 4;
  double y() const;
  void set_y(double value);

  // double z = 5;
  void clear_z();
  static const int kZFieldNumber = 5;
  double z() const;
  void set_z(double value);

  // double q_x = 6;
  void clear_q_x();
  static const int kQXFieldNumber = 6;
  double q_x() const;
  void set_q_x(double value);

  // double q_y = 7;
  void clear_q_y();
  static const int kQYFieldNumber = 7;
  double q_y() const;
  void set_q_y(double value);

  // double q_z = 8;
  void clear_q_z();
  static const int kQZFieldNumber = 8;
  double q_z() const;
  void set_q_z(double value);

  // double q_w = 9;
  void clear_q_w();
  static const int kQWFieldNumber = 9;
  double q_w() const;
  void set_q_w(double value);

  // @@protoc_insertion_point(class_scope:proto_msg.PoseStamped)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr frame_id_;
  double stamp_;
  double x_;
  double y_;
  double z_;
  double q_x_;
  double q_y_;
  double q_z_;
  double q_w_;
  mutable int _cached_size_;
  friend struct protobuf_proto_5fmsgs_2ePoseStamped_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// PoseStamped

// double stamp = 1;
inline void PoseStamped::clear_stamp() {
  stamp_ = 0;
}
inline double PoseStamped::stamp() const {
  // @@protoc_insertion_point(field_get:proto_msg.PoseStamped.stamp)
  return stamp_;
}
inline void PoseStamped::set_stamp(double value) {
  
  stamp_ = value;
  // @@protoc_insertion_point(field_set:proto_msg.PoseStamped.stamp)
}

// string frame_id = 2;
inline void PoseStamped::clear_frame_id() {
  frame_id_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& PoseStamped::frame_id() const {
  // @@protoc_insertion_point(field_get:proto_msg.PoseStamped.frame_id)
  return frame_id_.GetNoArena();
}
inline void PoseStamped::set_frame_id(const ::std::string& value) {
  
  frame_id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:proto_msg.PoseStamped.frame_id)
}
#if LANG_CXX11
inline void PoseStamped::set_frame_id(::std::string&& value) {
  
  frame_id_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:proto_msg.PoseStamped.frame_id)
}
#endif
inline void PoseStamped::set_frame_id(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  frame_id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:proto_msg.PoseStamped.frame_id)
}
inline void PoseStamped::set_frame_id(const char* value, size_t size) {
  
  frame_id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:proto_msg.PoseStamped.frame_id)
}
inline ::std::string* PoseStamped::mutable_frame_id() {
  
  // @@protoc_insertion_point(field_mutable:proto_msg.PoseStamped.frame_id)
  return frame_id_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* PoseStamped::release_frame_id() {
  // @@protoc_insertion_point(field_release:proto_msg.PoseStamped.frame_id)
  
  return frame_id_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PoseStamped::set_allocated_frame_id(::std::string* frame_id) {
  if (frame_id != NULL) {
    
  } else {
    
  }
  frame_id_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), frame_id);
  // @@protoc_insertion_point(field_set_allocated:proto_msg.PoseStamped.frame_id)
}

// double x = 3;
inline void PoseStamped::clear_x() {
  x_ = 0;
}
inline double PoseStamped::x() const {
  // @@protoc_insertion_point(field_get:proto_msg.PoseStamped.x)
  return x_;
}
inline void PoseStamped::set_x(double value) {
  
  x_ = value;
  // @@protoc_insertion_point(field_set:proto_msg.PoseStamped.x)
}

// double y = 4;
inline void PoseStamped::clear_y() {
  y_ = 0;
}
inline double PoseStamped::y() const {
  // @@protoc_insertion_point(field_get:proto_msg.PoseStamped.y)
  return y_;
}
inline void PoseStamped::set_y(double value) {
  
  y_ = value;
  // @@protoc_insertion_point(field_set:proto_msg.PoseStamped.y)
}

// double z = 5;
inline void PoseStamped::clear_z() {
  z_ = 0;
}
inline double PoseStamped::z() const {
  // @@protoc_insertion_point(field_get:proto_msg.PoseStamped.z)
  return z_;
}
inline void PoseStamped::set_z(double value) {
  
  z_ = value;
  // @@protoc_insertion_point(field_set:proto_msg.PoseStamped.z)
}

// double q_x = 6;
inline void PoseStamped::clear_q_x() {
  q_x_ = 0;
}
inline double PoseStamped::q_x() const {
  // @@protoc_insertion_point(field_get:proto_msg.PoseStamped.q_x)
  return q_x_;
}
inline void PoseStamped::set_q_x(double value) {
  
  q_x_ = value;
  // @@protoc_insertion_point(field_set:proto_msg.PoseStamped.q_x)
}

// double q_y = 7;
inline void PoseStamped::clear_q_y() {
  q_y_ = 0;
}
inline double PoseStamped::q_y() const {
  // @@protoc_insertion_point(field_get:proto_msg.PoseStamped.q_y)
  return q_y_;
}
inline void PoseStamped::set_q_y(double value) {
  
  q_y_ = value;
  // @@protoc_insertion_point(field_set:proto_msg.PoseStamped.q_y)
}

// double q_z = 8;
inline void PoseStamped::clear_q_z() {
  q_z_ = 0;
}
inline double PoseStamped::q_z() const {
  // @@protoc_insertion_point(field_get:proto_msg.PoseStamped.q_z)
  return q_z_;
}
inline void PoseStamped::set_q_z(double value) {
  
  q_z_ = value;
  // @@protoc_insertion_point(field_set:proto_msg.PoseStamped.q_z)
}

// double q_w = 9;
inline void PoseStamped::clear_q_w() {
  q_w_ = 0;
}
inline double PoseStamped::q_w() const {
  // @@protoc_insertion_point(field_get:proto_msg.PoseStamped.q_w)
  return q_w_;
}
inline void PoseStamped::set_q_w(double value) {
  
  q_w_ = value;
  // @@protoc_insertion_point(field_set:proto_msg.PoseStamped.q_w)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)


}  // namespace proto_msg

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_proto_5fmsgs_2ePoseStamped_2eproto__INCLUDED