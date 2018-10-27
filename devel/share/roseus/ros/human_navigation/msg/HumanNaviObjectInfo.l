;; Auto-generated. Do not edit!


(when (boundp 'human_navigation::HumanNaviObjectInfo)
  (if (not (find-package "HUMAN_NAVIGATION"))
    (make-package "HUMAN_NAVIGATION"))
  (shadow 'HumanNaviObjectInfo (find-package "HUMAN_NAVIGATION")))
(unless (find-package "HUMAN_NAVIGATION::HUMANNAVIOBJECTINFO")
  (make-package "HUMAN_NAVIGATION::HUMANNAVIOBJECTINFO"))

(in-package "ROS")
;;//! \htmlinclude HumanNaviObjectInfo.msg.html
(if (not (find-package "GEOMETRY_MSGS"))
  (ros::roseus-add-msgs "geometry_msgs"))


(defclass human_navigation::HumanNaviObjectInfo
  :super ros::object
  :slots (_name _position _orientation ))

(defmethod human_navigation::HumanNaviObjectInfo
  (:init
   (&key
    ((:name __name) "")
    ((:position __position) (instance geometry_msgs::Point :init))
    ((:orientation __orientation) (instance geometry_msgs::Quaternion :init))
    )
   (send-super :init)
   (setq _name (string __name))
   (setq _position __position)
   (setq _orientation __orientation)
   self)
  (:name
   (&optional __name)
   (if __name (setq _name __name)) _name)
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
  (:serialization-length
   ()
   (+
    ;; string _name
    4 (length _name)
    ;; geometry_msgs/Point _position
    (send _position :serialization-length)
    ;; geometry_msgs/Quaternion _orientation
    (send _orientation :serialization-length)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; string _name
       (write-long (length _name) s) (princ _name s)
     ;; geometry_msgs/Point _position
       (send _position :serialize s)
     ;; geometry_msgs/Quaternion _orientation
       (send _orientation :serialize s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; string _name
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _name (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; geometry_msgs/Point _position
     (send _position :deserialize buf ptr-) (incf ptr- (send _position :serialization-length))
   ;; geometry_msgs/Quaternion _orientation
     (send _orientation :deserialize buf ptr-) (incf ptr- (send _orientation :serialization-length))
   ;;
   self)
  )

(setf (get human_navigation::HumanNaviObjectInfo :md5sum-) "65b1250e41281e50a63b24196f1ad619")
(setf (get human_navigation::HumanNaviObjectInfo :datatype-) "human_navigation/HumanNaviObjectInfo")
(setf (get human_navigation::HumanNaviObjectInfo :definition-)
      "string name
geometry_msgs/Point position
geometry_msgs/Quaternion orientation

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



(provide :human_navigation/HumanNaviObjectInfo "65b1250e41281e50a63b24196f1ad619")


