// Generated by gencpp from file human_navigation/HumanNaviAvatarStatus.msg
// DO NOT EDIT!


#ifndef HUMAN_NAVIGATION_MESSAGE_HUMANNAVIAVATARSTATUS_H
#define HUMAN_NAVIGATION_MESSAGE_HUMANNAVIAVATARSTATUS_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>

#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Pose.h>

namespace human_navigation
{
template <class ContainerAllocator>
struct HumanNaviAvatarStatus_
{
  typedef HumanNaviAvatarStatus_<ContainerAllocator> Type;

  HumanNaviAvatarStatus_()
    : head()
    , body()
    , left_hand()
    , right_hand()
    , object_in_left_hand()
    , object_in_right_hand()
    , is_target_object_in_left_hand(false)
    , is_target_object_in_right_hand(false)  {
    }
  HumanNaviAvatarStatus_(const ContainerAllocator& _alloc)
    : head(_alloc)
    , body(_alloc)
    , left_hand(_alloc)
    , right_hand(_alloc)
    , object_in_left_hand(_alloc)
    , object_in_right_hand(_alloc)
    , is_target_object_in_left_hand(false)
    , is_target_object_in_right_hand(false)  {
  (void)_alloc;
    }



   typedef  ::geometry_msgs::Pose_<ContainerAllocator>  _head_type;
  _head_type head;

   typedef  ::geometry_msgs::Pose_<ContainerAllocator>  _body_type;
  _body_type body;

   typedef  ::geometry_msgs::Pose_<ContainerAllocator>  _left_hand_type;
  _left_hand_type left_hand;

   typedef  ::geometry_msgs::Pose_<ContainerAllocator>  _right_hand_type;
  _right_hand_type right_hand;

   typedef std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  _object_in_left_hand_type;
  _object_in_left_hand_type object_in_left_hand;

   typedef std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  _object_in_right_hand_type;
  _object_in_right_hand_type object_in_right_hand;

   typedef uint8_t _is_target_object_in_left_hand_type;
  _is_target_object_in_left_hand_type is_target_object_in_left_hand;

   typedef uint8_t _is_target_object_in_right_hand_type;
  _is_target_object_in_right_hand_type is_target_object_in_right_hand;





  typedef boost::shared_ptr< ::human_navigation::HumanNaviAvatarStatus_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::human_navigation::HumanNaviAvatarStatus_<ContainerAllocator> const> ConstPtr;

}; // struct HumanNaviAvatarStatus_

typedef ::human_navigation::HumanNaviAvatarStatus_<std::allocator<void> > HumanNaviAvatarStatus;

typedef boost::shared_ptr< ::human_navigation::HumanNaviAvatarStatus > HumanNaviAvatarStatusPtr;
typedef boost::shared_ptr< ::human_navigation::HumanNaviAvatarStatus const> HumanNaviAvatarStatusConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::human_navigation::HumanNaviAvatarStatus_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::human_navigation::HumanNaviAvatarStatus_<ContainerAllocator> >::stream(s, "", v);
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
struct IsFixedSize< ::human_navigation::HumanNaviAvatarStatus_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::human_navigation::HumanNaviAvatarStatus_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::human_navigation::HumanNaviAvatarStatus_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::human_navigation::HumanNaviAvatarStatus_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::human_navigation::HumanNaviAvatarStatus_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::human_navigation::HumanNaviAvatarStatus_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::human_navigation::HumanNaviAvatarStatus_<ContainerAllocator> >
{
  static const char* value()
  {
    return "f9190ca25b6cd6192693cc3f2a3156de";
  }

  static const char* value(const ::human_navigation::HumanNaviAvatarStatus_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0xf9190ca25b6cd619ULL;
  static const uint64_t static_value2 = 0x2693cc3f2a3156deULL;
};

template<class ContainerAllocator>
struct DataType< ::human_navigation::HumanNaviAvatarStatus_<ContainerAllocator> >
{
  static const char* value()
  {
    return "human_navigation/HumanNaviAvatarStatus";
  }

  static const char* value(const ::human_navigation::HumanNaviAvatarStatus_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::human_navigation::HumanNaviAvatarStatus_<ContainerAllocator> >
{
  static const char* value()
  {
    return "geometry_msgs/Pose head\n\
geometry_msgs/Pose body\n\
geometry_msgs/Pose left_hand\n\
geometry_msgs/Pose right_hand\n\
string object_in_left_hand\n\
string object_in_right_hand\n\
bool is_target_object_in_left_hand\n\
bool is_target_object_in_right_hand\n\
\n\
================================================================================\n\
MSG: geometry_msgs/Pose\n\
# A representation of pose in free space, composed of position and orientation. \n\
Point position\n\
Quaternion orientation\n\
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

  static const char* value(const ::human_navigation::HumanNaviAvatarStatus_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::human_navigation::HumanNaviAvatarStatus_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.head);
      stream.next(m.body);
      stream.next(m.left_hand);
      stream.next(m.right_hand);
      stream.next(m.object_in_left_hand);
      stream.next(m.object_in_right_hand);
      stream.next(m.is_target_object_in_left_hand);
      stream.next(m.is_target_object_in_right_hand);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct HumanNaviAvatarStatus_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::human_navigation::HumanNaviAvatarStatus_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::human_navigation::HumanNaviAvatarStatus_<ContainerAllocator>& v)
  {
    s << indent << "head: ";
    s << std::endl;
    Printer< ::geometry_msgs::Pose_<ContainerAllocator> >::stream(s, indent + "  ", v.head);
    s << indent << "body: ";
    s << std::endl;
    Printer< ::geometry_msgs::Pose_<ContainerAllocator> >::stream(s, indent + "  ", v.body);
    s << indent << "left_hand: ";
    s << std::endl;
    Printer< ::geometry_msgs::Pose_<ContainerAllocator> >::stream(s, indent + "  ", v.left_hand);
    s << indent << "right_hand: ";
    s << std::endl;
    Printer< ::geometry_msgs::Pose_<ContainerAllocator> >::stream(s, indent + "  ", v.right_hand);
    s << indent << "object_in_left_hand: ";
    Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.object_in_left_hand);
    s << indent << "object_in_right_hand: ";
    Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.object_in_right_hand);
    s << indent << "is_target_object_in_left_hand: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.is_target_object_in_left_hand);
    s << indent << "is_target_object_in_right_hand: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.is_target_object_in_right_hand);
  }
};

} // namespace message_operations
} // namespace ros

#endif // HUMAN_NAVIGATION_MESSAGE_HUMANNAVIAVATARSTATUS_H
