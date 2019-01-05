// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: proto_msgs.PoseStamped.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "proto_msgs.PoseStamped.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace proto_msg {
class PoseStampedDefaultTypeInternal {
public:
 ::google::protobuf::internal::ExplicitlyConstructed<PoseStamped>
     _instance;
} _PoseStamped_default_instance_;

namespace protobuf_proto_5fmsgs_2ePoseStamped_2eproto {


namespace {

::google::protobuf::Metadata file_level_metadata[1];

}  // namespace

PROTOBUF_CONSTEXPR_VAR ::google::protobuf::internal::ParseTableField
    const TableStruct::entries[] GOOGLE_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  {0, 0, 0, ::google::protobuf::internal::kInvalidMask, 0, 0},
};

PROTOBUF_CONSTEXPR_VAR ::google::protobuf::internal::AuxillaryParseTableField
    const TableStruct::aux[] GOOGLE_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  ::google::protobuf::internal::AuxillaryParseTableField(),
};
PROTOBUF_CONSTEXPR_VAR ::google::protobuf::internal::ParseTable const
    TableStruct::schema[] GOOGLE_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  { NULL, NULL, 0, -1, -1, -1, -1, NULL, false },
};

const ::google::protobuf::uint32 TableStruct::offsets[] GOOGLE_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PoseStamped, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PoseStamped, stamp_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PoseStamped, frame_id_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PoseStamped, x_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PoseStamped, y_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PoseStamped, z_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PoseStamped, q_x_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PoseStamped, q_y_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PoseStamped, q_z_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PoseStamped, q_w_),
};
static const ::google::protobuf::internal::MigrationSchema schemas[] GOOGLE_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(PoseStamped)},
};

static ::google::protobuf::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::google::protobuf::Message*>(&_PoseStamped_default_instance_),
};

namespace {

void protobuf_AssignDescriptors() {
  AddDescriptors();
  ::google::protobuf::MessageFactory* factory = NULL;
  AssignDescriptors(
      "proto_msgs.PoseStamped.proto", schemas, file_default_instances, TableStruct::offsets, factory,
      file_level_metadata, NULL, NULL);
}

void protobuf_AssignDescriptorsOnce() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &protobuf_AssignDescriptors);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::internal::RegisterAllTypes(file_level_metadata, 1);
}

}  // namespace
void TableStruct::InitDefaultsImpl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::internal::InitProtobufDefaults();
  _PoseStamped_default_instance_._instance.DefaultConstruct();
  ::google::protobuf::internal::OnShutdownDestroyMessage(
      &_PoseStamped_default_instance_);}

void InitDefaults() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &TableStruct::InitDefaultsImpl);
}
namespace {
void AddDescriptorsImpl() {
  InitDefaults();
  static const char descriptor[] GOOGLE_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
      "\n\034proto_msgs.PoseStamped.proto\022\tproto_ms"
      "g\"\203\001\n\013PoseStamped\022\r\n\005stamp\030\001 \001(\001\022\020\n\010fram"
      "e_id\030\002 \001(\t\022\t\n\001x\030\003 \001(\001\022\t\n\001y\030\004 \001(\001\022\t\n\001z\030\005 "
      "\001(\001\022\013\n\003q_x\030\006 \001(\001\022\013\n\003q_y\030\007 \001(\001\022\013\n\003q_z\030\010 \001"
      "(\001\022\013\n\003q_w\030\t \001(\001b\006proto3"
  };
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
      descriptor, 183);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "proto_msgs.PoseStamped.proto", &protobuf_RegisterTypes);
}
} // anonymous namespace

void AddDescriptors() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &AddDescriptorsImpl);
}
// Force AddDescriptors() to be called at dynamic initialization time.
struct StaticDescriptorInitializer {
  StaticDescriptorInitializer() {
    AddDescriptors();
  }
} static_descriptor_initializer;

}  // namespace protobuf_proto_5fmsgs_2ePoseStamped_2eproto


// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int PoseStamped::kStampFieldNumber;
const int PoseStamped::kFrameIdFieldNumber;
const int PoseStamped::kXFieldNumber;
const int PoseStamped::kYFieldNumber;
const int PoseStamped::kZFieldNumber;
const int PoseStamped::kQXFieldNumber;
const int PoseStamped::kQYFieldNumber;
const int PoseStamped::kQZFieldNumber;
const int PoseStamped::kQWFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

PoseStamped::PoseStamped()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (GOOGLE_PREDICT_TRUE(this != internal_default_instance())) {
    protobuf_proto_5fmsgs_2ePoseStamped_2eproto::InitDefaults();
  }
  SharedCtor();
  // @@protoc_insertion_point(constructor:proto_msg.PoseStamped)
}
PoseStamped::PoseStamped(const PoseStamped& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(NULL),
      _cached_size_(0) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  frame_id_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (from.frame_id().size() > 0) {
    frame_id_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.frame_id_);
  }
  ::memcpy(&stamp_, &from.stamp_,
    static_cast<size_t>(reinterpret_cast<char*>(&q_w_) -
    reinterpret_cast<char*>(&stamp_)) + sizeof(q_w_));
  // @@protoc_insertion_point(copy_constructor:proto_msg.PoseStamped)
}

void PoseStamped::SharedCtor() {
  frame_id_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(&stamp_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&q_w_) -
      reinterpret_cast<char*>(&stamp_)) + sizeof(q_w_));
  _cached_size_ = 0;
}

PoseStamped::~PoseStamped() {
  // @@protoc_insertion_point(destructor:proto_msg.PoseStamped)
  SharedDtor();
}

void PoseStamped::SharedDtor() {
  frame_id_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}

void PoseStamped::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* PoseStamped::descriptor() {
  protobuf_proto_5fmsgs_2ePoseStamped_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_proto_5fmsgs_2ePoseStamped_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const PoseStamped& PoseStamped::default_instance() {
  protobuf_proto_5fmsgs_2ePoseStamped_2eproto::InitDefaults();
  return *internal_default_instance();
}

PoseStamped* PoseStamped::New(::google::protobuf::Arena* arena) const {
  PoseStamped* n = new PoseStamped;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void PoseStamped::Clear() {
// @@protoc_insertion_point(message_clear_start:proto_msg.PoseStamped)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  frame_id_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(&stamp_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&q_w_) -
      reinterpret_cast<char*>(&stamp_)) + sizeof(q_w_));
  _internal_metadata_.Clear();
}

bool PoseStamped::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:proto_msg.PoseStamped)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // double stamp = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(9u /* 9 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   double, ::google::protobuf::internal::WireFormatLite::TYPE_DOUBLE>(
                 input, &stamp_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // string frame_id = 2;
      case 2: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(18u /* 18 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_frame_id()));
          DO_(::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
            this->frame_id().data(), static_cast<int>(this->frame_id().length()),
            ::google::protobuf::internal::WireFormatLite::PARSE,
            "proto_msg.PoseStamped.frame_id"));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // double x = 3;
      case 3: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(25u /* 25 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   double, ::google::protobuf::internal::WireFormatLite::TYPE_DOUBLE>(
                 input, &x_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // double y = 4;
      case 4: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(33u /* 33 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   double, ::google::protobuf::internal::WireFormatLite::TYPE_DOUBLE>(
                 input, &y_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // double z = 5;
      case 5: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(41u /* 41 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   double, ::google::protobuf::internal::WireFormatLite::TYPE_DOUBLE>(
                 input, &z_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // double q_x = 6;
      case 6: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(49u /* 49 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   double, ::google::protobuf::internal::WireFormatLite::TYPE_DOUBLE>(
                 input, &q_x_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // double q_y = 7;
      case 7: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(57u /* 57 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   double, ::google::protobuf::internal::WireFormatLite::TYPE_DOUBLE>(
                 input, &q_y_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // double q_z = 8;
      case 8: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(65u /* 65 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   double, ::google::protobuf::internal::WireFormatLite::TYPE_DOUBLE>(
                 input, &q_z_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // double q_w = 9;
      case 9: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(73u /* 73 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   double, ::google::protobuf::internal::WireFormatLite::TYPE_DOUBLE>(
                 input, &q_w_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:proto_msg.PoseStamped)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:proto_msg.PoseStamped)
  return false;
#undef DO_
}

void PoseStamped::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:proto_msg.PoseStamped)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // double stamp = 1;
  if (this->stamp() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteDouble(1, this->stamp(), output);
  }

  // string frame_id = 2;
  if (this->frame_id().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->frame_id().data(), static_cast<int>(this->frame_id().length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "proto_msg.PoseStamped.frame_id");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      2, this->frame_id(), output);
  }

  // double x = 3;
  if (this->x() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteDouble(3, this->x(), output);
  }

  // double y = 4;
  if (this->y() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteDouble(4, this->y(), output);
  }

  // double z = 5;
  if (this->z() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteDouble(5, this->z(), output);
  }

  // double q_x = 6;
  if (this->q_x() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteDouble(6, this->q_x(), output);
  }

  // double q_y = 7;
  if (this->q_y() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteDouble(7, this->q_y(), output);
  }

  // double q_z = 8;
  if (this->q_z() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteDouble(8, this->q_z(), output);
  }

  // double q_w = 9;
  if (this->q_w() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteDouble(9, this->q_w(), output);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), output);
  }
  // @@protoc_insertion_point(serialize_end:proto_msg.PoseStamped)
}

::google::protobuf::uint8* PoseStamped::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:proto_msg.PoseStamped)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // double stamp = 1;
  if (this->stamp() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteDoubleToArray(1, this->stamp(), target);
  }

  // string frame_id = 2;
  if (this->frame_id().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->frame_id().data(), static_cast<int>(this->frame_id().length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "proto_msg.PoseStamped.frame_id");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->frame_id(), target);
  }

  // double x = 3;
  if (this->x() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteDoubleToArray(3, this->x(), target);
  }

  // double y = 4;
  if (this->y() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteDoubleToArray(4, this->y(), target);
  }

  // double z = 5;
  if (this->z() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteDoubleToArray(5, this->z(), target);
  }

  // double q_x = 6;
  if (this->q_x() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteDoubleToArray(6, this->q_x(), target);
  }

  // double q_y = 7;
  if (this->q_y() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteDoubleToArray(7, this->q_y(), target);
  }

  // double q_z = 8;
  if (this->q_z() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteDoubleToArray(8, this->q_z(), target);
  }

  // double q_w = 9;
  if (this->q_w() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteDoubleToArray(9, this->q_w(), target);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:proto_msg.PoseStamped)
  return target;
}

size_t PoseStamped::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:proto_msg.PoseStamped)
  size_t total_size = 0;

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()));
  }
  // string frame_id = 2;
  if (this->frame_id().size() > 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->frame_id());
  }

  // double stamp = 1;
  if (this->stamp() != 0) {
    total_size += 1 + 8;
  }

  // double x = 3;
  if (this->x() != 0) {
    total_size += 1 + 8;
  }

  // double y = 4;
  if (this->y() != 0) {
    total_size += 1 + 8;
  }

  // double z = 5;
  if (this->z() != 0) {
    total_size += 1 + 8;
  }

  // double q_x = 6;
  if (this->q_x() != 0) {
    total_size += 1 + 8;
  }

  // double q_y = 7;
  if (this->q_y() != 0) {
    total_size += 1 + 8;
  }

  // double q_z = 8;
  if (this->q_z() != 0) {
    total_size += 1 + 8;
  }

  // double q_w = 9;
  if (this->q_w() != 0) {
    total_size += 1 + 8;
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void PoseStamped::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:proto_msg.PoseStamped)
  GOOGLE_DCHECK_NE(&from, this);
  const PoseStamped* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const PoseStamped>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:proto_msg.PoseStamped)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:proto_msg.PoseStamped)
    MergeFrom(*source);
  }
}

void PoseStamped::MergeFrom(const PoseStamped& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:proto_msg.PoseStamped)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.frame_id().size() > 0) {

    frame_id_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.frame_id_);
  }
  if (from.stamp() != 0) {
    set_stamp(from.stamp());
  }
  if (from.x() != 0) {
    set_x(from.x());
  }
  if (from.y() != 0) {
    set_y(from.y());
  }
  if (from.z() != 0) {
    set_z(from.z());
  }
  if (from.q_x() != 0) {
    set_q_x(from.q_x());
  }
  if (from.q_y() != 0) {
    set_q_y(from.q_y());
  }
  if (from.q_z() != 0) {
    set_q_z(from.q_z());
  }
  if (from.q_w() != 0) {
    set_q_w(from.q_w());
  }
}

void PoseStamped::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:proto_msg.PoseStamped)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void PoseStamped::CopyFrom(const PoseStamped& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:proto_msg.PoseStamped)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool PoseStamped::IsInitialized() const {
  return true;
}

void PoseStamped::Swap(PoseStamped* other) {
  if (other == this) return;
  InternalSwap(other);
}
void PoseStamped::InternalSwap(PoseStamped* other) {
  using std::swap;
  frame_id_.Swap(&other->frame_id_);
  swap(stamp_, other->stamp_);
  swap(x_, other->x_);
  swap(y_, other->y_);
  swap(z_, other->z_);
  swap(q_x_, other->q_x_);
  swap(q_y_, other->q_y_);
  swap(q_z_, other->q_z_);
  swap(q_w_, other->q_w_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata PoseStamped::GetMetadata() const {
  protobuf_proto_5fmsgs_2ePoseStamped_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_proto_5fmsgs_2ePoseStamped_2eproto::file_level_metadata[kIndexInFileMessages];
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// PoseStamped

// double stamp = 1;
void PoseStamped::clear_stamp() {
  stamp_ = 0;
}
double PoseStamped::stamp() const {
  // @@protoc_insertion_point(field_get:proto_msg.PoseStamped.stamp)
  return stamp_;
}
void PoseStamped::set_stamp(double value) {
  
  stamp_ = value;
  // @@protoc_insertion_point(field_set:proto_msg.PoseStamped.stamp)
}

// string frame_id = 2;
void PoseStamped::clear_frame_id() {
  frame_id_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
const ::std::string& PoseStamped::frame_id() const {
  // @@protoc_insertion_point(field_get:proto_msg.PoseStamped.frame_id)
  return frame_id_.GetNoArena();
}
void PoseStamped::set_frame_id(const ::std::string& value) {
  
  frame_id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:proto_msg.PoseStamped.frame_id)
}
#if LANG_CXX11
void PoseStamped::set_frame_id(::std::string&& value) {
  
  frame_id_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:proto_msg.PoseStamped.frame_id)
}
#endif
void PoseStamped::set_frame_id(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  frame_id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:proto_msg.PoseStamped.frame_id)
}
void PoseStamped::set_frame_id(const char* value, size_t size) {
  
  frame_id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:proto_msg.PoseStamped.frame_id)
}
::std::string* PoseStamped::mutable_frame_id() {
  
  // @@protoc_insertion_point(field_mutable:proto_msg.PoseStamped.frame_id)
  return frame_id_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
::std::string* PoseStamped::release_frame_id() {
  // @@protoc_insertion_point(field_release:proto_msg.PoseStamped.frame_id)
  
  return frame_id_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
void PoseStamped::set_allocated_frame_id(::std::string* frame_id) {
  if (frame_id != NULL) {
    
  } else {
    
  }
  frame_id_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), frame_id);
  // @@protoc_insertion_point(field_set_allocated:proto_msg.PoseStamped.frame_id)
}

// double x = 3;
void PoseStamped::clear_x() {
  x_ = 0;
}
double PoseStamped::x() const {
  // @@protoc_insertion_point(field_get:proto_msg.PoseStamped.x)
  return x_;
}
void PoseStamped::set_x(double value) {
  
  x_ = value;
  // @@protoc_insertion_point(field_set:proto_msg.PoseStamped.x)
}

// double y = 4;
void PoseStamped::clear_y() {
  y_ = 0;
}
double PoseStamped::y() const {
  // @@protoc_insertion_point(field_get:proto_msg.PoseStamped.y)
  return y_;
}
void PoseStamped::set_y(double value) {
  
  y_ = value;
  // @@protoc_insertion_point(field_set:proto_msg.PoseStamped.y)
}

// double z = 5;
void PoseStamped::clear_z() {
  z_ = 0;
}
double PoseStamped::z() const {
  // @@protoc_insertion_point(field_get:proto_msg.PoseStamped.z)
  return z_;
}
void PoseStamped::set_z(double value) {
  
  z_ = value;
  // @@protoc_insertion_point(field_set:proto_msg.PoseStamped.z)
}

// double q_x = 6;
void PoseStamped::clear_q_x() {
  q_x_ = 0;
}
double PoseStamped::q_x() const {
  // @@protoc_insertion_point(field_get:proto_msg.PoseStamped.q_x)
  return q_x_;
}
void PoseStamped::set_q_x(double value) {
  
  q_x_ = value;
  // @@protoc_insertion_point(field_set:proto_msg.PoseStamped.q_x)
}

// double q_y = 7;
void PoseStamped::clear_q_y() {
  q_y_ = 0;
}
double PoseStamped::q_y() const {
  // @@protoc_insertion_point(field_get:proto_msg.PoseStamped.q_y)
  return q_y_;
}
void PoseStamped::set_q_y(double value) {
  
  q_y_ = value;
  // @@protoc_insertion_point(field_set:proto_msg.PoseStamped.q_y)
}

// double q_z = 8;
void PoseStamped::clear_q_z() {
  q_z_ = 0;
}
double PoseStamped::q_z() const {
  // @@protoc_insertion_point(field_get:proto_msg.PoseStamped.q_z)
  return q_z_;
}
void PoseStamped::set_q_z(double value) {
  
  q_z_ = value;
  // @@protoc_insertion_point(field_set:proto_msg.PoseStamped.q_z)
}

// double q_w = 9;
void PoseStamped::clear_q_w() {
  q_w_ = 0;
}
double PoseStamped::q_w() const {
  // @@protoc_insertion_point(field_get:proto_msg.PoseStamped.q_w)
  return q_w_;
}
void PoseStamped::set_q_w(double value) {
  
  q_w_ = value;
  // @@protoc_insertion_point(field_set:proto_msg.PoseStamped.q_w)
}

#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace proto_msg

// @@protoc_insertion_point(global_scope)
