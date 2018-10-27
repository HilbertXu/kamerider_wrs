// Generated by gencpp from file human_navigation/HumanNaviGuidanceMsg.msg
// DO NOT EDIT!


#ifndef HUMAN_NAVIGATION_MESSAGE_HUMANNAVIGUIDANCEMSG_H
#define HUMAN_NAVIGATION_MESSAGE_HUMANNAVIGUIDANCEMSG_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace human_navigation
{
template <class ContainerAllocator>
struct HumanNaviGuidanceMsg_
{
  typedef HumanNaviGuidanceMsg_<ContainerAllocator> Type;

  HumanNaviGuidanceMsg_()
    : message()
    , display_type()  {
    }
  HumanNaviGuidanceMsg_(const ContainerAllocator& _alloc)
    : message(_alloc)
    , display_type(_alloc)  {
  (void)_alloc;
    }



   typedef std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  _message_type;
  _message_type message;

   typedef std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  _display_type_type;
  _display_type_type display_type;





  typedef boost::shared_ptr< ::human_navigation::HumanNaviGuidanceMsg_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::human_navigation::HumanNaviGuidanceMsg_<ContainerAllocator> const> ConstPtr;

}; // struct HumanNaviGuidanceMsg_

typedef ::human_navigation::HumanNaviGuidanceMsg_<std::allocator<void> > HumanNaviGuidanceMsg;

typedef boost::shared_ptr< ::human_navigation::HumanNaviGuidanceMsg > HumanNaviGuidanceMsgPtr;
typedef boost::shared_ptr< ::human_navigation::HumanNaviGuidanceMsg const> HumanNaviGuidanceMsgConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::human_navigation::HumanNaviGuidanceMsg_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::human_navigation::HumanNaviGuidanceMsg_<ContainerAllocator> >::stream(s, "", v);
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
struct IsFixedSize< ::human_navigation::HumanNaviGuidanceMsg_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::human_navigation::HumanNaviGuidanceMsg_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::human_navigation::HumanNaviGuidanceMsg_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::human_navigation::HumanNaviGuidanceMsg_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::human_navigation::HumanNaviGuidanceMsg_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::human_navigation::HumanNaviGuidanceMsg_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::human_navigation::HumanNaviGuidanceMsg_<ContainerAllocator> >
{
  static const char* value()
  {
    return "61cc6c45b4669a8bb054d2fc90a5dff7";
  }

  static const char* value(const ::human_navigation::HumanNaviGuidanceMsg_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x61cc6c45b4669a8bULL;
  static const uint64_t static_value2 = 0xb054d2fc90a5dff7ULL;
};

template<class ContainerAllocator>
struct DataType< ::human_navigation::HumanNaviGuidanceMsg_<ContainerAllocator> >
{
  static const char* value()
  {
    return "human_navigation/HumanNaviGuidanceMsg";
  }

  static const char* value(const ::human_navigation::HumanNaviGuidanceMsg_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::human_navigation::HumanNaviGuidanceMsg_<ContainerAllocator> >
{
  static const char* value()
  {
    return "string message\n\
string display_type\n\
";
  }

  static const char* value(const ::human_navigation::HumanNaviGuidanceMsg_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::human_navigation::HumanNaviGuidanceMsg_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.message);
      stream.next(m.display_type);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct HumanNaviGuidanceMsg_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::human_navigation::HumanNaviGuidanceMsg_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::human_navigation::HumanNaviGuidanceMsg_<ContainerAllocator>& v)
  {
    s << indent << "message: ";
    Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.message);
    s << indent << "display_type: ";
    Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.display_type);
  }
};

} // namespace message_operations
} // namespace ros

#endif // HUMAN_NAVIGATION_MESSAGE_HUMANNAVIGUIDANCEMSG_H