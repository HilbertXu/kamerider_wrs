;; Auto-generated. Do not edit!


(when (boundp 'human_navigation::HumanNaviDestination)
  (if (not (find-package "HUMAN_NAVIGATION"))
    (make-package "HUMAN_NAVIGATION"))
  (shadow 'HumanNaviDestination (find-package "HUMAN_NAVIGATION")))
(unless (find-package "HUMAN_NAVIGATION::HUMANNAVIDESTINATION")
  (make-package "HUMAN_NAVIGATION::HUMANNAVIDESTINATION"))

(in-package "ROS")
;;//! \htmlinclude HumanNaviDestination.msg.html
(if (not (find-package "GEOMETRY_MSGS"))
  (ros::roseus-add-msgs "geometry_msgs"))


(defclass human_navigation::HumanNaviDestination
  :super ros::object
  :slots (_position _orientation _size ))

(defmethod human_navigation::HumanNaviDestination
  (:init
   (&key
    ((:position __position) (instance geometry_msgs::Point :init))
    ((:orientation __orientation) (instance geometry_msgs::Quaternion :init))
    ((:size __size) (instance geometry_msgs::Point :init))
    )
   (send-super :init)
   (setq _position __position)
   (setq _orientation __orientation)
   (setq _size __size)
   self)
  (:position
   (&rest __position)
   (if (keywordp (car __position))
       (send* _position __position)
     (progn
       (if __position (setq _position (car __position)))
       _position)))
  (:orientation
   (&rest __orientation)
   (if (keywordp (car __orientation))
       (send* _orientation __orientation)
     (progn
       (if __orientation (setq _orientation (car __orientation)))
       _orientation)))
  (:size
   (&rest __size)
   (if (keywordp (car __size))
       (send* _size __size)
     (progn
       (if __size (setq _size (car __size)))
       _size)))
  (:serialization-length
   ()
   (+
    ;; geometry_msgs/Point _position
    (send _position :serialization-length)
    ;; geometry_msgs/Quaternion _orientation
    (send _orientation :serialization-length)
    ;; geometry_msgs/Point _size
    (send _size :serialization-length)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; geometry_msgs/Point _position
       (send _position :serialize s)
     ;; geometry_msgs/Quaternion _orientation
       (send _orientation :serialize s)
     ;; geometry_msgs/Point _size
       (send _size :serialize s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; geometry_msgs/Point _position
     (send _position :deserialize buf ptr-) (incf ptr- (send _position :serialization-length))
   ;; geometry_msgs/Quaternion _orientation
     (send _orientation :deserialize buf ptr-) (incf ptr- (send _orientation :serialization-length))
   ;; geometry_msgs/Point _size
     (send _size :deserialize buf ptr-) (incf ptr- (send _size :serialization-length))
   ;;
   self)
  )

(setf (get human_navigation::HumanNaviDestination :md5sum-) "e9adffd11b4bc3effe2908e9f95d5bea")
(setf (get human_navigation::HumanNaviDestination :datatype-) "human_navigation/HumanNaviDestination")
(setf (get human_navigation::HumanNaviDestination :definition-)
      "geometry_msgs/Point position
geometry_msgs/Quaternion orientation
geometry_msgs/Point size

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



(provide :human_navigation/HumanNaviDestination "e9adffd11b4bc3effe2908e9f95d5bea")


