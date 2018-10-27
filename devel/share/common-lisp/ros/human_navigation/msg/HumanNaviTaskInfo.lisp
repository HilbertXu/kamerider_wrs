; Auto-generated. Do not edit!


(cl:in-package human_navigation-msg)


;//! \htmlinclude HumanNaviTaskInfo.msg.html

(cl:defclass <HumanNaviTaskInfo> (roslisp-msg-protocol:ros-message)
  ((environment_id
    :reader environment_id
    :initarg :environment_id
    :type cl:string
    :initform "")
   (target_object
    :reader target_object
    :initarg :target_object
    :type human_navigation-msg:HumanNaviObjectInfo
    :initform (cl:make-instance 'human_navigation-msg:HumanNaviObjectInfo))
   (destination
    :reader destination
    :initarg :destination
    :type human_navigation-msg:HumanNaviDestination
    :initform (cl:make-instance 'human_navigation-msg:HumanNaviDestination))
   (non_target_objects
    :reader non_target_objects
    :initarg :non_target_objects
    :type (cl:vector human_navigation-msg:HumanNaviObjectInfo)
   :initform (cl:make-array 0 :element-type 'human_navigation-msg:HumanNaviObjectInfo :initial-element (cl:make-instance 'human_navigation-msg:HumanNaviObjectInfo)))
   (furniture
    :reader furniture
    :initarg :furniture
    :type (cl:vector human_navigation-msg:HumanNaviObjectInfo)
   :initform (cl:make-array 0 :element-type 'human_navigation-msg:HumanNaviObjectInfo :initial-element (cl:make-instance 'human_navigation-msg:HumanNaviObjectInfo))))
)

(cl:defclass HumanNaviTaskInfo (<HumanNaviTaskInfo>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <HumanNaviTaskInfo>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'HumanNaviTaskInfo)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name human_navigation-msg:<HumanNaviTaskInfo> is deprecated: use human_navigation-msg:HumanNaviTaskInfo instead.")))

(cl:ensure-generic-function 'environment_id-val :lambda-list '(m))
(cl:defmethod environment_id-val ((m <HumanNaviTaskInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader human_navigation-msg:environment_id-val is deprecated.  Use human_navigation-msg:environment_id instead.")
  (environment_id m))

(cl:ensure-generic-function 'target_object-val :lambda-list '(m))
(cl:defmethod target_object-val ((m <HumanNaviTaskInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader human_navigation-msg:target_object-val is deprecated.  Use human_navigation-msg:target_object instead.")
  (target_object m))

(cl:ensure-generic-function 'destination-val :lambda-list '(m))
(cl:defmethod destination-val ((m <HumanNaviTaskInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader human_navigation-msg:destination-val is deprecated.  Use human_navigation-msg:destination instead.")
  (destination m))

(cl:ensure-generic-function 'non_target_objects-val :lambda-list '(m))
(cl:defmethod non_target_objects-val ((m <HumanNaviTaskInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader human_navigation-msg:non_target_objects-val is deprecated.  Use human_navigation-msg:non_target_objects instead.")
  (non_target_objects m))

(cl:ensure-generic-function 'furniture-val :lambda-list '(m))
(cl:defmethod furniture-val ((m <HumanNaviTaskInfo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader human_navigation-msg:furniture-val is deprecated.  Use human_navigation-msg:furniture instead.")
  (furniture m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <HumanNaviTaskInfo>) ostream)
  "Serializes a message object of type '<HumanNaviTaskInfo>"
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'environment_id))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'environment_id))
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'target_object) ostream)
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'destination) ostream)
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'non_target_objects))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'non_target_objects))
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'furniture))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'furniture))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <HumanNaviTaskInfo>) istream)
  "Deserializes a message object of type '<HumanNaviTaskInfo>"
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'environment_id) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'environment_id) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'target_object) istream)
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'destination) istream)
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
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'furniture) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'furniture)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'human_navigation-msg:HumanNaviObjectInfo))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<HumanNaviTaskInfo>)))
  "Returns string type for a message object of type '<HumanNaviTaskInfo>"
  "human_navigation/HumanNaviTaskInfo")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'HumanNaviTaskInfo)))
  "Returns string type for a message object of type 'HumanNaviTaskInfo"
  "human_navigation/HumanNaviTaskInfo")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<HumanNaviTaskInfo>)))
  "Returns md5sum for a message object of type '<HumanNaviTaskInfo>"
  "72a83c8dcb8252f4b5e97040679254a7")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'HumanNaviTaskInfo)))
  "Returns md5sum for a message object of type 'HumanNaviTaskInfo"
  "72a83c8dcb8252f4b5e97040679254a7")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<HumanNaviTaskInfo>)))
  "Returns full string definition for message of type '<HumanNaviTaskInfo>"
  (cl:format cl:nil "string environment_id~%HumanNaviObjectInfo target_object~%HumanNaviDestination destination~%HumanNaviObjectInfo[] non_target_objects~%HumanNaviObjectInfo[] furniture~%~%================================================================================~%MSG: human_navigation/HumanNaviObjectInfo~%string name~%geometry_msgs/Point position~%geometry_msgs/Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%================================================================================~%MSG: human_navigation/HumanNaviDestination~%geometry_msgs/Point position~%geometry_msgs/Quaternion orientation~%geometry_msgs/Point size~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'HumanNaviTaskInfo)))
  "Returns full string definition for message of type 'HumanNaviTaskInfo"
  (cl:format cl:nil "string environment_id~%HumanNaviObjectInfo target_object~%HumanNaviDestination destination~%HumanNaviObjectInfo[] non_target_objects~%HumanNaviObjectInfo[] furniture~%~%================================================================================~%MSG: human_navigation/HumanNaviObjectInfo~%string name~%geometry_msgs/Point position~%geometry_msgs/Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%================================================================================~%MSG: human_navigation/HumanNaviDestination~%geometry_msgs/Point position~%geometry_msgs/Quaternion orientation~%geometry_msgs/Point size~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <HumanNaviTaskInfo>))
  (cl:+ 0
     4 (cl:length (cl:slot-value msg 'environment_id))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'target_object))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'destination))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'non_target_objects) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'furniture) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <HumanNaviTaskInfo>))
  "Converts a ROS message object to a list"
  (cl:list 'HumanNaviTaskInfo
    (cl:cons ':environment_id (environment_id msg))
    (cl:cons ':target_object (target_object msg))
    (cl:cons ':destination (destination msg))
    (cl:cons ':non_target_objects (non_target_objects msg))
    (cl:cons ':furniture (furniture msg))
))
