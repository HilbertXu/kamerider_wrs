;; Auto-generated. Do not edit!


(when (boundp 'darknet_ros_msgs::BoundingBoxes)
  (if (not (find-package "DARKNET_ROS_MSGS"))
    (make-package "DARKNET_ROS_MSGS"))
  (shadow 'BoundingBoxes (find-package "DARKNET_ROS_MSGS")))
(unless (find-package "DARKNET_ROS_MSGS::BOUNDINGBOXES")
  (make-package "DARKNET_ROS_MSGS::BOUNDINGBOXES"))

(in-package "ROS")
;;//! \htmlinclude BoundingBoxes.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass darknet_ros_msgs::BoundingBoxes
  :super ros::object
  :slots (_header _boundingBoxes ))

(defmethod darknet_ros_msgs::BoundingBoxes
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:boundingBoxes __boundingBoxes) (let (r) (dotimes (i 0) (push (instance darknet_ros_msgs::BoundingBox :init) r)) r))
    )
   (send-super :init)
   (setq _header __header)
   (setq _boundingBoxes __boundingBoxes)
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:boundingBoxes
   (&rest __boundingBoxes)
   (if (keywordp (car __boundingBoxes))
       (send* _boundingBoxes __boundingBoxes)
     (progn
       (if __boundingBoxes (setq _boundingBoxes (car __boundingBoxes)))
       _boundingBoxes)))
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; darknet_ros_msgs/BoundingBox[] _boundingBoxes
    (apply #'+ (send-all _boundingBoxes :serialization-length)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; darknet_ros_msgs/BoundingBox[] _boundingBoxes
     (write-long (length _boundingBoxes) s)
     (dolist (elem _boundingBoxes)
       (send elem :serialize s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; darknet_ros_msgs/BoundingBox[] _boundingBoxes
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _boundingBoxes (let (r) (dotimes (i n) (push (instance darknet_ros_msgs::BoundingBox :init) r)) r))
     (dolist (elem- _boundingBoxes)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;;
   self)
  )

(setf (get darknet_ros_msgs::BoundingBoxes :md5sum-) "468565c3a90acd3f0e4aa67421b39e1f")
(setf (get darknet_ros_msgs::BoundingBoxes :datatype-) "darknet_ros_msgs/BoundingBoxes")
(setf (get darknet_ros_msgs::BoundingBoxes :definition-)
      "Header header
BoundingBox[] boundingBoxes

================================================================================
MSG: std_msgs/Header
# Standard metadata for higher-level stamped data types.
# This is generally used to communicate timestamped data 
# in a particular coordinate frame.
# 
# sequence ID: consecutively increasing ID 
uint32 seq
#Two-integer timestamp that is expressed as:
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
# time-handling sugar is provided by the client library
time stamp
#Frame this data is associated with
# 0: no frame
# 1: global frame
string frame_id

================================================================================
MSG: darknet_ros_msgs/BoundingBox
string Class
float64 probability
int64 xmin
int64 ymin
int64 xmax
int64 ymax


")



(provide :darknet_ros_msgs/BoundingBoxes "468565c3a90acd3f0e4aa67421b39e1f")


