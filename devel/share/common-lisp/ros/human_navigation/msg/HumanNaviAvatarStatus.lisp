; Auto-generated. Do not edit!


(cl:in-package human_navigation-msg)


;//! \htmlinclude HumanNaviAvatarStatus.msg.html

(cl:defclass <HumanNaviAvatarStatus> (roslisp-msg-protocol:ros-message)
  ((head
    :reader head
    :initarg :head
    :type geometry_msgs-msg:Pose
    :initform (cl:make-instance 'geometry_msgs-msg:Pose))
   (body
    :reader body
    :initarg :body
    :type geometry_msgs-msg:Pose
    :initform (cl:make-instance 'geometry_msgs-msg:Pose))
   (left_hand
    :reader left_hand
    :initarg :left_hand
    :type geometry_msgs-msg:Pose
    :initform (cl:make-instance 'geometry_msgs-msg:Pose))
   (right_hand
    :reader right_hand
    :initarg :right_hand
    :type geometry_msgs-msg:Pose
    :initform (cl:make-instance 'geometry_msgs-msg:Pose))
   (object_in_left_hand
    :reader object_in_left_hand
    :initarg :object_in_left_hand
    :type cl:string
    :initform "")
   (object_in_right_hand
    :reader object_in_right_hand
    :initarg :object_in_right_hand
    :type cl:string
    :initform "")
   (is_target_object_in_left_hand
    :reader is_target_object_in_left_hand
    :initarg :is_target_object_in_left_hand
    :type cl:boolean
    :initform cl:nil)
   (is_target_object_in_right_hand
    :reader is_target_object_in_right_hand
    :initarg :is_target_object_in_right_hand
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass HumanNaviAvatarStatus (<HumanNaviAvatarStatus>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <HumanNaviAvatarStatus>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'HumanNaviAvatarStatus)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name human_navigation-msg:<HumanNaviAvatarStatus> is deprecated: use human_navigation-msg:HumanNaviAvatarStatus instead.")))

(cl:ensure-generic-function 'head-val :lambda-list '(m))
(cl:defmethod head-val ((m <HumanNaviAvatarStatus>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader human_navigation-msg:head-val is deprecated.  Use human_navigation-msg:head instead.")
  (head m))

(cl:ensure-generic-function 'body-val :lambda-list '(m))
(cl:defmethod body-val ((m <HumanNaviAvatarStatus>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader human_navigation-msg:body-val is deprecated.  Use human_navigation-msg:body instead.")
  (body m))

(cl:ensure-generic-function 'left_hand-val :lambda-list '(m))
(cl:defmethod left_hand-val ((m <HumanNaviAvatarStatus>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader human_navigation-msg:left_hand-val is deprecated.  Use human_navigation-msg:left_hand instead.")
  (left_hand m))

(cl:ensure-generic-function 'right_hand-val :lambda-list '(m))
(cl:defmethod right_hand-val ((m <HumanNaviAvatarStatus>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader human_navigation-msg:right_hand-val is deprecated.  Use human_navigation-msg:right_hand instead.")
  (right_hand m))

(cl:ensure-generic-function 'object_in_left_hand-val :lambda-list '(m))
(cl:defmethod object_in_left_hand-val ((m <HumanNaviAvatarStatus>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader human_navigation-msg:object_in_left_hand-val is deprecated.  Use human_navigation-msg:object_in_left_hand instead.")
  (object_in_left_hand m))

(cl:ensure-generic-function 'object_in_right_hand-val :lambda-list '(m))
(cl:defmethod object_in_right_hand-val ((m <HumanNaviAvatarStatus>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader human_navigation-msg:object_in_right_hand-val is deprecated.  Use human_navigation-msg:object_in_right_hand instead.")
  (object_in_right_hand m))

(cl:ensure-generic-function 'is_target_object_in_left_hand-val :lambda-list '(m))
(cl:defmethod is_target_object_in_left_hand-val ((m <HumanNaviAvatarStatus>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader human_navigation-msg:is_target_object_in_left_hand-val is deprecated.  Use human_navigation-msg:is_target_object_in_left_hand instead.")
  (is_target_object_in_left_hand m))

(cl:ensure-generic-function 'is_target_object_in_right_hand-val :lambda-list '(m))
(cl:defmethod is_target_object_in_right_hand-val ((m <HumanNaviAvatarStatus>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader human_navigation-msg:is_target_object_in_right_hand-val is deprecated.  Use human_navigation-msg:is_target_object_in_right_hand instead.")
  (is_target_object_in_right_hand m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <HumanNaviAvatarStatus>) ostream)
  "Serializes a message object of type '<HumanNaviAvatarStatus>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'head) ostream)
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'body) ostream)
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'left_hand) ostream)
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'right_hand) ostream)
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'object_in_left_hand))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'object_in_left_hand))
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'object_in_right_hand))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'object_in_right_hand))
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'is_target_object_in_left_hand) 1 0)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'is_target_object_in_right_hand) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <HumanNaviAvatarStatus>) istream)
  "Deserializes a message object of type '<HumanNaviAvatarStatus>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'head) istream)
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'body) istream)
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'left_hand) istream)
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'right_hand) istream)
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'object_in_left_hand) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'object_in_left_hand) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'object_in_right_hand) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'object_in_right_hand) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
    (cl:setf (cl:slot-value msg 'is_target_object_in_left_hand) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:setf (cl:slot-value msg 'is_target_object_in_right_hand) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<HumanNaviAvatarStatus>)))
  "Returns string type for a message object of type '<HumanNaviAvatarStatus>"
  "human_navigation/HumanNaviAvatarStatus")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'HumanNaviAvatarStatus)))
  "Returns string type for a message object of type 'HumanNaviAvatarStatus"
  "human_navigation/HumanNaviAvatarStatus")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<HumanNaviAvatarStatus>)))
  "Returns md5sum for a message object of type '<HumanNaviAvatarStatus>"
  "f9190ca25b6cd6192693cc3f2a3156de")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'HumanNaviAvatarStatus)))
  "Returns md5sum for a message object of type 'HumanNaviAvatarStatus"
  "f9190ca25b6cd6192693cc3f2a3156de")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<HumanNaviAvatarStatus>)))
  "Returns full string definition for message of type '<HumanNaviAvatarStatus>"
  (cl:format cl:nil "geometry_msgs/Pose head~%geometry_msgs/Pose body~%geometry_msgs/Pose left_hand~%geometry_msgs/Pose right_hand~%string object_in_left_hand~%string object_in_right_hand~%bool is_target_object_in_left_hand~%bool is_target_object_in_right_hand~%~%================================================================================~%MSG: geometry_msgs/Pose~%# A representation of pose in free space, composed of position and orientation. ~%Point position~%Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'HumanNaviAvatarStatus)))
  "Returns full string definition for message of type 'HumanNaviAvatarStatus"
  (cl:format cl:nil "geometry_msgs/Pose head~%geometry_msgs/Pose body~%geometry_msgs/Pose left_hand~%geometry_msgs/Pose right_hand~%string object_in_left_hand~%string object_in_right_hand~%bool is_target_object_in_left_hand~%bool is_target_object_in_right_hand~%~%================================================================================~%MSG: geometry_msgs/Pose~%# A representation of pose in free space, composed of position and orientation. ~%Point position~%Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <HumanNaviAvatarStatus>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'head))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'body))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'left_hand))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'right_hand))
     4 (cl:length (cl:slot-value msg 'object_in_left_hand))
     4 (cl:length (cl:slot-value msg 'object_in_right_hand))
     1
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <HumanNaviAvatarStatus>))
  "Converts a ROS message object to a list"
  (cl:list 'HumanNaviAvatarStatus
    (cl:cons ':head (head msg))
    (cl:cons ':body (body msg))
    (cl:cons ':left_hand (left_hand msg))
    (cl:cons ':right_hand (right_hand msg))
    (cl:cons ':object_in_left_hand (object_in_left_hand msg))
    (cl:cons ':object_in_right_hand (object_in_right_hand msg))
    (cl:cons ':is_target_object_in_left_hand (is_target_object_in_left_hand msg))
    (cl:cons ':is_target_object_in_right_hand (is_target_object_in_right_hand msg))
))
