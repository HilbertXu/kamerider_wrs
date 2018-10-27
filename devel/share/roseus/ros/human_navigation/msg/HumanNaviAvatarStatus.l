;; Auto-generated. Do not edit!


(when (boundp 'human_navigation::HumanNaviAvatarStatus)
  (if (not (find-package "HUMAN_NAVIGATION"))
    (make-package "HUMAN_NAVIGATION"))
  (shadow 'HumanNaviAvatarStatus (find-package "HUMAN_NAVIGATION")))
(unless (find-package "HUMAN_NAVIGATION::HUMANNAVIAVATARSTATUS")
  (make-package "HUMAN_NAVIGATION::HUMANNAVIAVATARSTATUS"))

(in-package "ROS")
;;//! \htmlinclude HumanNaviAvatarStatus.msg.html
(if (not (find-package "GEOMETRY_MSGS"))
  (ros::roseus-add-msgs "geometry_msgs"))


(defclass human_navigation::HumanNaviAvatarStatus
  :super ros::object
  :slots (_head _body _left_hand _right_hand _object_in_left_hand _object_in_right_hand _is_target_object_in_left_hand _is_target_object_in_right_hand ))

(defmethod human_navigation::HumanNaviAvatarStatus
  (:init
   (&key
    ((:head __head) (instance geometry_msgs::Pose :init))
    ((:body __body) (instance geometry_msgs::Pose :init))
    ((:left_hand __left_hand) (instance geometry_msgs::Pose :init))
    ((:right_hand __right_hand) (instance geometry_msgs::Pose :init))
    ((:object_in_left_hand __object_in_left_hand) "")
    ((:object_in_right_hand __object_in_right_hand) "")
    ((:is_target_object_in_left_hand __is_target_object_in_left_hand) nil)
    ((:is_target_object_in_right_hand __is_target_object_in_right_hand) nil)
    )
   (send-super :init)
   (setq _head __head)
   (setq _body __body)
   (setq _left_hand __left_hand)
   (setq _right_hand __right_hand)
   (setq _object_in_left_hand (string __object_in_left_hand))
   (setq _object_in_right_hand (string __object_in_right_hand))
   (setq _is_target_object_in_left_hand __is_target_object_in_left_hand)
   (setq _is_target_object_in_right_hand __is_target_object_in_right_hand)
   self)
  (:head
   (&rest __head)
   (if (keywordp (car __head))
       (send* _head __head)
     (progn
       (if __head (setq _head (car __head)))
       _head)))
  (:body
   (&rest __body)
   (if (keywordp (car __body))
       (send* _body __body)
     (progn
       (if __body (setq _body (car __body)))
       _body)))
  (:left_hand
   (&rest __left_hand)
   (if (keywordp (car __left_hand))
       (send* _left_hand __left_hand)
     (progn
       (if __left_hand (setq _left_hand (car __left_hand)))
       _left_hand)))
  (:right_hand
   (&rest __right_hand)
   (if (keywordp (car __right_hand))
       (send* _right_hand __right_hand)
     (progn
       (if __right_hand (setq _right_hand (car __right_hand)))
       _right_hand)))
  (:object_in_left_hand
   (&optional __object_in_left_hand)
   (if __object_in_left_hand (setq _object_in_left_hand __object_in_left_hand)) _object_in_left_hand)
  (:object_in_right_hand
   (&optional __object_in_right_hand)
   (if __object_in_right_hand (setq _object_in_right_hand __object_in_right_hand)) _object_in_right_hand)
  (:is_target_object_in_left_hand
   (&optional __is_target_object_in_left_hand)
   (if __is_target_object_in_left_hand (setq _is_target_object_in_left_hand __is_target_object_in_left_hand)) _is_target_object_in_left_hand)
  (:is_target_object_in_right_hand
   (&optional __is_target_object_in_right_hand)
   (if __is_target_object_in_right_hand (setq _is_target_object_in_right_hand __is_target_object_in_right_hand)) _is_target_object_in_right_hand)
  (:serialization-length
   ()
   (+
    ;; geometry_msgs/Pose _head
    (send _head :serialization-length)
    ;; geometry_msgs/Pose _body
    (send _body :serialization-length)
    ;; geometry_msgs/Pose _left_hand
    (send _left_hand :serialization-length)
    ;; geometry_msgs/Pose _right_hand
    (send _right_hand :serialization-length)
    ;; string _object_in_left_hand
    4 (length _object_in_left_hand)
    ;; string _object_in_right_hand
    4 (length _object_in_right_hand)
    ;; bool _is_target_object_in_left_hand
    1
    ;; bool _is_target_object_in_right_hand
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; geometry_msgs/Pose _head
       (send _head :serialize s)
     ;; geometry_msgs/Pose _body
       (send _body :serialize s)
     ;; geometry_msgs/Pose _left_hand
       (send _left_hand :serialize s)
     ;; geometry_msgs/Pose _right_hand
       (send _right_hand :serialize s)
     ;; string _object_in_left_hand
       (write-long (length _object_in_left_hand) s) (princ _object_in_left_hand s)
     ;; string _object_in_right_hand
       (write-long (length _object_in_right_hand) s) (princ _object_in_right_hand s)
     ;; bool _is_target_object_in_left_hand
       (if _is_target_object_in_left_hand (write-byte -1 s) (write-byte 0 s))
     ;; bool _is_target_object_in_right_hand
       (if _is_target_object_in_right_hand (write-byte -1 s) (write-byte 0 s))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; geometry_msgs/Pose _head
     (send _head :deserialize buf ptr-) (incf ptr- (send _head :serialization-length))
   ;; geometry_msgs/Pose _body
     (send _body :deserialize buf ptr-) (incf ptr- (send _body :serialization-length))
   ;; geometry_msgs/Pose _left_hand
     (send _left_hand :deserialize buf ptr-) (incf ptr- (send _left_hand :serialization-length))
   ;; geometry_msgs/Pose _right_hand
     (send _right_hand :deserialize buf ptr-) (incf ptr- (send _right_hand :serialization-length))
   ;; string _object_in_left_hand
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _object_in_left_hand (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; string _object_in_right_hand
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _object_in_right_hand (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; bool _is_target_object_in_left_hand
     (setq _is_target_object_in_left_hand (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; bool _is_target_object_in_right_hand
     (setq _is_target_object_in_right_hand (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;;
   self)
  )

(setf (get human_navigation::HumanNaviAvatarStatus :md5sum-) "f9190ca25b6cd6192693cc3f2a3156de")
(setf (get human_navigation::HumanNaviAvatarStatus :datatype-) "human_navigation/HumanNaviAvatarStatus")
(setf (get human_navigation::HumanNaviAvatarStatus :definition-)
      "geometry_msgs/Pose head
geometry_msgs/Pose body
geometry_msgs/Pose left_hand
geometry_msgs/Pose right_hand
string object_in_left_hand
string object_in_right_hand
bool is_target_object_in_left_hand
bool is_target_object_in_right_hand

================================================================================
MSG: geometry_msgs/Pose
# A representation of pose in free space, composed of position and orientation. 
Point position
Quaternion orientation

================================================================================
MSG: geometry_msgs/Point
# This contains the position of a point in free space
float64 x
float64 y
float64 z

================================================================================
MSG: geometry_msgs/Quaternion
# This represents an orientation in free space in quaternion form.

float64 x
float64 y
float64 z
float64 w

")



(provide :human_navigation/HumanNaviAvatarStatus "f9190ca25b6cd6192693cc3f2a3156de")


