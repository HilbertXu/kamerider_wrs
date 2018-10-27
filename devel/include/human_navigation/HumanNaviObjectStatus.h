// Generated by gencpp from file human_navigation/HumanNaviObjectStatus.msg
// DO NOT EDIT!


#ifndef HUMAN_NAVIGATION_MESSAGE_HUMANNAVIOBJECTSTATUS_H
#define HUMAN_NAVIGATION_MESSAGE_HUMANNAVIOBJECTSTATUS_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>

#include <human_navigation/HumanNaviObjectInfo.h>
#include <human_navigation/HumanNaviObjectInfo.h>

namespace human_navigation
{
template <class ContainerAllocator>
struct HumanNaviObjectStatus_
{
  typedef HumanNaviObjectStatus_<ContainerAllocator> Type;

  HumanNaviObjectStatus_()
    : target_object()
    , non_target_objects()  {
    }
  HumanNaviObjectStatus_(const ContainerAllocator& _alloc)
    : target_object(_alloc)
    , non_target_objects(_alloc)  {
  (void)_alloc;
    }



   typedef  ::human_navigation::HumanNaviObjectInfo_<ContainerAllocator>  _target_object_type;
  _target_object_type target_object;

   typedef std::vector< ::human_navigation::HumanNaviObjectInfo_<ContainerAllocator> , typename ContainerAllocator::template rebind< ::human_navigation::HumanNaviObjectInfo_<ContainerAllocator> >::other >  _non_target_objects_type;
  _non_target_objects_type non_target_objects;





  typedef boost::shared_ptr< ::human_navigation::HumanNaviObjectStatus_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::human_navigation::HumanNaviObjectStatus_<ContainerAllocator> const> ConstPtr;

}; // struct HumanNaviObjectStatus_

typedef ::human_navigation::HumanNaviObjectStatus_<std::allocator<void> > HumanNaviObjectStatus;

typedef boost::shared_ptr< ::human_navigation::HumanNaviObjectStatus > HumanNaviObjectStatusPtr;
typedef boost::shared_ptr< ::human_navigation::HumanNaviObjectStatus const> HumanNaviObjectStatusConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::human_navigation::HumanNaviObjectStatus_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::human_navigation::HumanNaviObjectStatus_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace human_navigation

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': False}
// {'human_navigation': ['/home/kamerider/sim_ws/src/human_navigation/msg'], 'geometry_msgs': ['/opt/ros/kinetic/share/geometry_msgs/cmake/../msg'], 'std_msgs': ['/opt/ros/kinetic/share/std_msgs/cmake/../msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::human_navigation::HumanNaviObjectStatus_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::human_navigation::HumanNaviObjectStatus_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::human_navigation::HumanNaviObjectStatus_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::human_navigation::HumanNaviObjectStatus_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::human_navigation::HumanNaviObjectStatus_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::human_navigation::HumanNaviObjectStatus_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::human_navigation::HumanNaviObjectStatus_<ContainerAllocator> >
{
  static const char* value()
  {
    return "dc923dcd95df576c03a3a3f61c038230";
  }

  static const char* value(const ::human_navigation::HumanNaviObjectStatus_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0xdc923dcd95df576cULL;
  static const uint64_t static_value2 = 0x03a3a3f61c038230ULL;
};

template<class ContainerAllocator>
struct DataType< ::human_navigation::HumanNaviObjectStatus_<ContainerAllocator> >
{
  static const char* value()
  {
    return "human_navigation/HumanNaviObjectStatus";
  }

  static const char* value(const ::human_navigation::HumanNaviObjectStatus_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::human_navigation::HumanNaviObjectStatus_<ContainerAllocator> >
{
  static const char* value()
  {
    return "HumanNaviObjectInfo target_object\n\
HumanNaviObjectInfo[] non_target_objects\n\
\n\
================================================================================\n\
MSG: human_navigation/HumanNaviObjectInfo\n\
string name\n\
geometry_msgs/Point position\n\
geometry_msgs/Quaternion orientation\n\
\n\
================================================================================\n\
MSG: geometry_msgs/Point\n\
# This contains the position of a point in free space\n\
float64 x\n\
float64 y\n\
float64 z\n\
\n\
================================================================================\n\
MSG: geometry_msgs/Quaternion\n\
# This represents an orientation in free space in quaternion form.\n\
\n\
float64 x\n\
float64 y\n\
float64 z\n\
float64 w\n\
";
  }

  static const char* value(const ::human_navigation::HumanNaviObjectStatus_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::human_navigation::HumanNaviObjectStatus_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.target_object);
      stream.next(m.non_target_objects);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct HumanNaviObjectStatus_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::human_navigation::HumanNaviObjectStatus_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::human_navigation::HumanNaviObjectStatus_<ContainerAllocator>& v)
  {
    s << indent << "target_object: ";
    s << std::endl;
    Printer< ::human_navigation::HumanNaviObjectInfo_<ContainerAllocator> >::stream(s, indent + "  ", v.target_object);
    s << indent << "non_target_objects[]" << std::endl;
    for (size_t i = 0; i < v.non_target_objects.size(); ++i)
    {
      s << indent << "  non_target_objects[" << i << "]: ";
      s << std::endl;
      s << indent;
      Printer< ::human_navigation::HumanNaviObjectInfo_<ContainerAllocator> >::stream(s, indent + "    ", v.non_target_objects[i]);
    }
  }
};

} // namespace message_operations
} // namespace ros

#endif // HUMAN_NAVIGATION_MESSAGE_HUMANNAVIOBJECTSTATUS_H