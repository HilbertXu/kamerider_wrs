; Auto-generated. Do not edit!


(cl:in-package human_navigation-msg)


;//! \htmlinclude HumanNaviGuidanceMsg.msg.html

(cl:defclass <HumanNaviGuidanceMsg> (roslisp-msg-protocol:ros-message)
  ((message
    :reader message
    :initarg :message
    :type cl:string
    :initform "")
   (display_type
    :reader display_type
    :initarg :display_type
    :type cl:string
    :initform ""))
)

(cl:defclass HumanNaviGuidanceMsg (<HumanNaviGuidanceMsg>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <HumanNaviGuidanceMsg>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'HumanNaviGuidanceMsg)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name human_navigation-msg:<HumanNaviGuidanceMsg> is deprecated: use human_navigation-msg:HumanNaviGuidanceMsg instead.")))

(cl:ensure-generic-function 'message-val :lambda-list '(m))
(cl:defmethod message-val ((m <HumanNaviGuidanceMsg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader human_navigation-msg:message-val is deprecated.  Use human_navigation-msg:message instead.")
  (message m))

(cl:ensure-generic-function 'display_type-val :lambda-list '(m))
(cl:defmethod display_type-val ((m <HumanNaviGuidanceMsg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader human_navigation-msg:display_type-val is deprecated.  Use human_navigation-msg:display_type instead.")
  (display_type m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <HumanNaviGuidanceMsg>) ostream)
  "Serializes a message object of type '<HumanNaviGuidanceMsg>"
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'message))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'message))
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'display_type))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'display_type))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <HumanNaviGuidanceMsg>) istream)
  "Deserializes a message object of type '<HumanNaviGuidanceMsg>"
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'message) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'message) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'display_type) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'display_type) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<HumanNaviGuidanceMsg>)))
  "Returns string type for a message object of type '<HumanNaviGuidanceMsg>"
  "human_navigation/HumanNaviGuidanceMsg")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'HumanNaviGuidanceMsg)))
  "Returns string type for a message object of type 'HumanNaviGuidanceMsg"
  "human_navigation/HumanNaviGuidanceMsg")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<HumanNaviGuidanceMsg>)))
  "Returns md5sum for a message object of type '<HumanNaviGuidanceMsg>"
  "61cc6c45b4669a8bb054d2fc90a5dff7")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'HumanNaviGuidanceMsg)))
  "Returns md5sum for a message object of type 'HumanNaviGuidanceMsg"
  "61cc6c45b4669a8bb054d2fc90a5dff7")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<HumanNaviGuidanceMsg>)))
  "Returns full string definition for message of type '<HumanNaviGuidanceMsg>"
  (cl:format cl:nil "string message~%string display_type~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'HumanNaviGuidanceMsg)))
  "Returns full string definition for message of type 'HumanNaviGuidanceMsg"
  (cl:format cl:nil "string message~%string display_type~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <HumanNaviGuidanceMsg>))
  (cl:+ 0
     4 (cl:length (cl:slot-value msg 'message))
     4 (cl:length (cl:slot-value msg 'display_type))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <HumanNaviGuidanceMsg>))
  "Converts a ROS message object to a list"
  (cl:list 'HumanNaviGuidanceMsg
    (cl:cons ':message (message msg))
    (cl:cons ':display_type (display_type msg))
))
