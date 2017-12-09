
#include <iostream>
#include <fstream>
#include <memory>
#include <limits>

#include <google/protobuf/message.h>

#include "unknown/unknown.pb.h"

#include "filenames.hpp"


// Comment or uncomment the below #define statement to toggle between whether
// ign-msgs is linked to this application.

//#define USE_IGN_MSGS



#ifdef USE_IGN_MSGS
#include <ignition/msgs/vector2d.pb.h>
#endif // USE_IGN_MSGS

int main()
{
#ifdef USE_IGN_MSGS
  // Declaring this object forces the linker to link ign-msgs to this
  // application, because the application needs the ign-msgs symbols in
  // order to construct it.
  //
  // If we don't declare this object, then the linker will not bother to link
  // ign-msgs to this application, because we won't actually use any symbols
  // from ign-msgs. Little does the linker realize that this has unintended
  // consequences: If this application doesn't link to ign-msgs, then the shared
  // library of ign-msgs won't be loaded for this application, and then the
  // statically registered message Descriptors won't be available to the
  // DescriptorPool::generated_pool().
  ignition::msgs::Vector2d v;
#endif // USE_IGN_MSGS

  std::ifstream fmeta(meta_file);
  std::string msgType;
  fmeta >> msgType;
  std::cout << "Read message name from [" << meta_file << "]: "
            << msgType << std::endl;

  std::ifstream fdata(data_file);
  std::string msgData;
  fdata >> msgData;
  fdata.close();

  std::cout << "Read message data from [" << data_file << "]: "
            << msgData << std::endl;

  const google::protobuf::Descriptor* desc =
      google::protobuf::DescriptorPool::generated_pool()
        ->FindMessageTypeByName(msgType);

  bool known = true;

  if(!desc)
  {
    known = false;
    desc = google::protobuf::DescriptorPool::generated_pool()
        ->FindMessageTypeByName("unknown.Unknown");
  }

  google::protobuf::Message* msg = nullptr;

  if(desc)
  {
    const google::protobuf::Message* prototype =
        google::protobuf::MessageFactory::generated_factory()
          ->GetPrototype(desc);

    msg = prototype->New();

    fdata.open(data_file);
    if(msg->ParseFromIstream(&fdata))
      std::cout << "Successfully parsed the message" << std::endl;
    else
      std::cout << "Failed to parse the message" << std::endl;
  }
  else
  {
    std::cout << "Could not generate any descriptor!" << std::endl;
  }

  if(msg)
  {
    std::cout << "Generated message name: " << msg->GetTypeName()
              << "\n" << std::endl;

    if(known)
    {
      std::cout << "We know this is a Vector2d" << std::endl;
      const google::protobuf::Reflection* refl = msg->GetReflection();
      const google::protobuf::FieldDescriptor* xDesc =
          desc->FindFieldByName("x");
      const google::protobuf::FieldDescriptor* yDesc =
          desc->FindFieldByName("y");

      std::cout << " -- x value: " << refl->GetDouble(*msg, xDesc) << std::endl;
      std::cout << " -- y value: " << refl->GetDouble(*msg, yDesc) << std::endl;
    }
    else
    {
      const google::protobuf::Reflection* refl = msg->GetReflection();
      const google::protobuf::UnknownFieldSet& unknownFields =
          refl->GetUnknownFields(*msg);

      std::cout << "The unknown message has [" << unknownFields.field_count()
                << "] fields" << std::endl;

      for(int i=0; i < unknownFields.field_count(); ++i)
      {
        const google::protobuf::UnknownField& field = unknownFields.field(i);
        std::cout << "Field #" << i << " has type [" << field.type()
                  << "]" << std::endl;

        // We'll only bother printing data for a few types in this example code
        uint64_t value = 0;

        if(google::protobuf::UnknownField::TYPE_FIXED32 == field.type())
          value = field.fixed32();
        else if(google::protobuf::UnknownField::TYPE_FIXED64 == field.type())
          value = field.fixed64();
        else if(google::protobuf::UnknownField::TYPE_VARINT == field.type())
          value = field.varint();

        std::cout << " -- value is: " << reinterpret_cast<const double&>(value)
                  << " (raw data: " << value << ")" << std::endl;
      }
    }
  }
}
