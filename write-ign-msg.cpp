
#include <iostream>
#include <fstream>

#include <ignition/msgs/vector2d.pb.h>

#include "filenames.hpp"

int main()
{
  ignition::msgs::Vector2d v;
  v.set_x(1.2);
  v.set_y(3.4);

  std::cout << "Created vector with: "
            << "x=" << v.x() << ", "
            << "y=" << v.y() << std::endl;


  std::cout << "Writing message name to [" << meta_file << "]: "
            << v.GetTypeName() << std::endl;
  std::ofstream fmeta(meta_file);
  fmeta << v.GetTypeName();

  const std::string serialized = v.SerializeAsString();
  std::cout << "Writing message data to [" << data_file << "]: "
            << serialized << std::endl;

  std::ofstream fdata(data_file);
  v.SerializeToOstream(&fdata);

  const google::protobuf::Descriptor *desc =
      google::protobuf::DescriptorPool::generated_pool()
        ->FindMessageTypeByName(v.GetTypeName());

  std::cout << "desc: " << desc << std::endl;

//  const google::protobuf::Message *prototype =
//      google::protobuf::MessageFactory::generated_factory()
//        ->GetPrototype(desc);
}
