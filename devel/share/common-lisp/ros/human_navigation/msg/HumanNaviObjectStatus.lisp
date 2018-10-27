; Auto-generated. Do not edit!


(cl:in-package human_navigation-msg)


;//! \htmlinclude HumanNaviObjectStatus.msg.html

(cl:defclass <HumanNaviObjectStatus> (roslisp-msg-protocol:ros-message)
  ((target_object
    :reader target_object
    :initarg :target_object
    :type human_navigation-msg:HumanNaviObjectInfo
    :initform (cl:make-instance 'human_navigation-msg:HumanNaviObjectInfo))
   (non_target_objects
    :reader non_target_objects
    :initarg :non_target_objects
    :type (cl:vector human_navigation-msg:HumanNaviObjectInfo)
   :initform (cl:make-array 0 :element-type 'human_navigation-msg:HumanNaviObjectInfo :initial-element (cl:make-instance 'human_navigation-msg:HumanNaviObjectInfo))))
)

(cl:defclass HumanNaviObjectStatus (<HumanNaviObjectStatus>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <HumanNaviObjectStatus>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'HumanNaviObjectStatus)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name human_navigation-msg:<HumanNaviObjectStatus> is deprecated: use human_navigation-msg:HumanNaviObjectStatus instead.")))

(cl:ensure-generic-function 'target_object-val :lambda-list '(m))
(cl:defmethod target_object-val ((m <HumanNaviObjectStatus>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader human_navigation-msg:target_object-val is deprecated.  Use human_navigation-msg:target_object instead.")
  (target_object m))

(cl:ensure-generic-function 'non_target_objects-val :lambda-list '(m))
(cl:defmethod non_target_objects-val ((m <HumanNaviObjectStatus>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader human_navigation-msg:non_target_objects-val is deprecated.  Use human_navigation-msg:non_target_objects instead.")
  (non_target_objects m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <HumanNaviObjectStatus>) ostream)
  "Serializes a message object of type '<HumanNaviObjectStatus>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'target_object) ostream)
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'non_target_objects))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'non_target_objects))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <HumanNaviObjectStatus>) istream)
  "Deserializes a message object of type '<HumanNaviObjectStatus>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'target_object) istream)
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'non_target_objects) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'non_target_objects)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'human_navigation-msg:HumanNaviObjectInfo))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<HumanNaviObjectStatus>)))
  "Returns string type for a message object of type '<HumanNaviObjectStatus>"
  "human_navigation/HumanNaviObjectStatus")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'HumanNaviObjectStatus)))
  "Returns string type for a message object of type 'HumanNaviObjectStatus"
  "human_navigation/HumanNaviObjectStatus")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<HumanNaviObjectStatus>)))
  "Returns md5sum for a message object of type '<HumanNaviObjectStatus>"
  "dc923dcd95df576c03a3a3f61c038230")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'HumanNaviObjectStatus)))
  "Returns md5sum for a message object of type 'HumanNaviObjectStatus"
  "dc923dcd95df576c03a3a3f61c038230")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<HumanNaviObjectStatus>)))
  "Returns full string definition for message of type '<HumanNaviObjectStatus>"
  (cl:format cl:nil "HumanNaviObjectInfo target_object~%HumanNaviObjectInfo[] non_target_objects~%~%================================================================================~%MSG: human_navigation/HumanNaviObjectInfo~%string name~%geometry_msgs/Point position~%geometry_msgs/Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'HumanNaviObjectStatus)))
  "Returns full string definition for message of type 'HumanNaviObjectStatus"
  (cl:format cl:nil "HumanNaviObjectInfo target_object~%HumanNaviObjectInfo[] non_target_objects~%~%================================================================================~%MSG: human_navigation/HumanNaviObjectInfo~%string name~%geometry_msgs/Point position~%geometry_msgs/Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <HumanNaviObjectStatus>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'target_object))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'non_target_objects) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <HumanNaviObjectStatus>))
  "Converts a ROS message object to a list"
  (cl:list 'HumanNaviObjectStatus
    (cl:cons ':target_object (target_object msg))
    (cl:cons ':non_target_objects (non_target_objects msg))
))
