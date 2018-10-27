; Auto-generated. Do not edit!


(cl:in-package human_navigation-msg)


;//! \htmlinclude HumanNaviObjectInfo.msg.html

(cl:defclass <HumanNaviObjectInfo> (roslisp-msg-protocol:ros-message)
  ((name
    :reader name
    :initarg :name
    :type cl:string
    :initform "")
   (position
    :reader position
    :initarg :position
    :type geometry_msgs-msg:Point
    :initform (cl:make-instance 'geometry_msgs-msg:Point))
   (orientation
    :reader orientation
    :initarg :orientation
    :type geometry_msgs-msg:Quaternion
    :initform (cl:make-instance 'geometry_msgs-msg:Quaternion)))
)

(cl:defclass HumanNaviObjectInfo (<HumanNaviObjectInfo>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <HumanNaviObjectInfo>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'HumanNaviObjectInfo)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name human_navigation-msg:<HumanNaviObjectInfo> is deprecated: use human_navigation-msg:HumanNaviObjectInfo instead.")))

(cl:ensure-generic-function 'name-val :lambda-list '(m))
(cl:defmethod name-val ((m <HumanNaviObjectInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader human_navigation-msg:name-val is deprecated.  Use human_navigation-msg:name instead.")
  (name m))

(cl:ensure-generic-function 'position-val :lambda-list '(m))
(cl:defmethod position-val ((m <HumanNaviObjectInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader human_navigation-msg:position-val is deprecated.  Use human_navigation-msg:position instead.")
  (position m))

(cl:ensure-generic-function 'orientation-val :lambda-list '(m))
(cl:defmethod orientation-val ((m <HumanNaviObjectInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader human_navigation-msg:orientation-val is deprecated.  Use human_navigation-msg:orientation instead.")
  (orientation m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <HumanNaviObjectInfo>) ostream)
  "Serializes a message object of type '<HumanNaviObjectInfo>"
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'name))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'name))
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'position) ostream)
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'orientation) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <HumanNaviObjectInfo>) istream)
  "Deserializes a message object of type '<HumanNaviObjectInfo>"
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'name) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'name) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'position) istream)
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'orientation) istream)
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<HumanNaviObjectInfo>)))
  "Returns string type for a message object of type '<HumanNaviObjectInfo>"
  "human_navigation/HumanNaviObjectInfo")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'HumanNaviObjectInfo)))
  "Returns string type for a message object of type 'HumanNaviObjectInfo"
  "human_navigation/HumanNaviObjectInfo")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<HumanNaviObjectInfo>)))
  "Returns md5sum for a message object of type '<HumanNaviObjectInfo>"
  "65b1250e41281e50a63b24196f1ad619")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'HumanNaviObjectInfo)))
  "Returns md5sum for a message object of type 'HumanNaviObjectInfo"
  "65b1250e41281e50a63b24196f1ad619")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<HumanNaviObjectInfo>)))
  "Returns full string definition for message of type '<HumanNaviObjectInfo>"
  (cl:format cl:nil "string name~%geometry_msgs/Point position~%geometry_msgs/Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'HumanNaviObjectInfo)))
  "Returns full string definition for message of type 'HumanNaviObjectInfo"
  (cl:format cl:nil "string name~%geometry_msgs/Point position~%geometry_msgs/Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <HumanNaviObjectInfo>))
  (cl:+ 0
     4 (cl:length (cl:slot-value msg 'name))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'position))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'orientation))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <HumanNaviObjectInfo>))
  "Converts a ROS message object to a list"
  (cl:list 'HumanNaviObjectInfo
    (cl:cons ':name (name msg))
    (cl:cons ':position (position msg))
    (cl:cons ':orientation (orientation msg))
))
