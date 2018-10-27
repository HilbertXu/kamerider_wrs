; Auto-generated. Do not edit!


(cl:in-package human_navigation-msg)


;//! \htmlinclude HumanNaviDestination.msg.html

(cl:defclass <HumanNaviDestination> (roslisp-msg-protocol:ros-message)
  ((position
    :reader position
    :initarg :position
    :type geometry_msgs-msg:Point
    :initform (cl:make-instance 'geometry_msgs-msg:Point))
   (orientation
    :reader orientation
    :initarg :orientation
    :type geometry_msgs-msg:Quaternion
    :initform (cl:make-instance 'geometry_msgs-msg:Quaternion))
   (size
    :reader size
    :initarg :size
    :type geometry_msgs-msg:Point
    :initform (cl:make-instance 'geometry_msgs-msg:Point)))
)

(cl:defclass HumanNaviDestination (<HumanNaviDestination>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <HumanNaviDestination>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'HumanNaviDestination)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name human_navigation-msg:<HumanNaviDestination> is deprecated: use human_navigation-msg:HumanNaviDestination instead.")))

(cl:ensure-generic-function 'position-val :lambda-list '(m))
(cl:defmethod position-val ((m <HumanNaviDestination>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader human_navigation-msg:position-val is deprecated.  Use human_navigation-msg:position instead.")
  (position m))

(cl:ensure-generic-function 'orientation-val :lambda-list '(m))
(cl:defmethod orientation-val ((m <HumanNaviDestination>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader human_navigation-msg:orientation-val is deprecated.  Use human_navigation-msg:orientation instead.")
  (orientation m))

(cl:ensure-generic-function 'size-val :lambda-list '(m))
(cl:defmethod size-val ((m <HumanNaviDestination>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader human_navigation-msg:size-val is deprecated.  Use human_navigation-msg:size instead.")
  (size m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <HumanNaviDestination>) ostream)
  "Serializes a message object of type '<HumanNaviDestination>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'position) ostream)
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'orientation) ostream)
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'size) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <HumanNaviDestination>) istream)
  "Deserializes a message object of type '<HumanNaviDestination>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'position) istream)
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'orientation) istream)
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'size) istream)
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<HumanNaviDestination>)))
  "Returns string type for a message object of type '<HumanNaviDestination>"
  "human_navigation/HumanNaviDestination")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'HumanNaviDestination)))
  "Returns string type for a message object of type 'HumanNaviDestination"
  "human_navigation/HumanNaviDestination")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<HumanNaviDestination>)))
  "Returns md5sum for a message object of type '<HumanNaviDestination>"
  "e9adffd11b4bc3effe2908e9f95d5bea")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'HumanNaviDestination)))
  "Returns md5sum for a message object of type 'HumanNaviDestination"
  "e9adffd11b4bc3effe2908e9f95d5bea")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<HumanNaviDestination>)))
  "Returns full string definition for message of type '<HumanNaviDestination>"
  (cl:format cl:nil "geometry_msgs/Point position~%geometry_msgs/Quaternion orientation~%geometry_msgs/Point size~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'HumanNaviDestination)))
  "Returns full string definition for message of type 'HumanNaviDestination"
  (cl:format cl:nil "geometry_msgs/Point position~%geometry_msgs/Quaternion orientation~%geometry_msgs/Point size~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <HumanNaviDestination>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'position))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'orientation))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'size))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <HumanNaviDestination>))
  "Converts a ROS message object to a list"
  (cl:list 'HumanNaviDestination
    (cl:cons ':position (position msg))
    (cl:cons ':orientation (orientation msg))
    (cl:cons ':size (size msg))
))
