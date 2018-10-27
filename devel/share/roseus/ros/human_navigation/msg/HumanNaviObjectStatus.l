;; Auto-generated. Do not edit!


(when (boundp 'human_navigation::HumanNaviObjectStatus)
  (if (not (find-package "HUMAN_NAVIGATION"))
    (make-package "HUMAN_NAVIGATION"))
  (shadow 'HumanNaviObjectStatus (find-package "HUMAN_NAVIGATION")))
(unless (find-package "HUMAN_NAVIGATION::HUMANNAVIOBJECTSTATUS")
  (make-package "HUMAN_NAVIGATION::HUMANNAVIOBJECTSTATUS"))

(in-package "ROS")
;;//! \htmlinclude HumanNaviObjectStatus.msg.html


(defclass human_navigation::HumanNaviObjectStatus
  :super ros::object
  :slots (_target_object _non_target_objects ))

(defmethod human_navigation::HumanNaviObjectStatus
  (:init
   (&key
    ((:target_object __target_object) (instance human_navigation::HumanNaviObjectInfo :init))
    ((:non_target_objects __non_target_objects) (let (r) (dotimes (i 0) (push (instance human_navigation::HumanNaviObjectInfo :init) r)) r))
    )
   (send-super :init)
   (setq _target_object __target_object)
   (setq _non_target_objects __non_target_objects)
   self)
  (:target_object
   (&rest __target_object)
   (if (keywordp (car __target_object))
       (send* _target_object __target_object)
     (progn
       (if __target_object (setq _target_object (car __target_object)))
       _target_object)))
  (:non_target_objects
   (&rest __non_target_objects)
   (if (keywordp (car __non_target_objects))
       (send* _non_target_objects __non_target_objects)
     (progn
       (if __non_target_objects (setq _non_target_objects (car __non_target_objects)))
       _non_target_objects)))
  (:serialization-length
   ()
   (+
    ;; human_navigation/HumanNaviObjectInfo _target_object
    (send _target_object :serialization-length)
    ;; human_navigation/HumanNaviObjectInfo[] _non_target_objects
    (apply #'+ (send-all _non_target_objects :serialization-length)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; human_navigation/HumanNaviObjectInfo _target_object
       (send _target_object :serialize s)
     ;; human_navigation/HumanNaviObjectInfo[] _non_target_objects
     (write-long (length _non_target_objects) s)
     (dolist (elem _non_target_objects)
       (send elem :serialize s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; human_navigation/HumanNaviObjectInfo _target_object
     (send _target_object :deserialize buf ptr-) (incf ptr- (send _target_object :serialization-length))
   ;; human_navigation/HumanNaviObjectInfo[] _non_target_objects
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _non_target_objects (let (r) (dotimes (i n) (push (instance human_navigation::HumanNaviObjectInfo :init) r)) r))
     (dolist (elem- _non_target_objects)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;;
   self)
  )

(setf (get human_navigation::HumanNaviObjectStatus :md5sum-) "dc923dcd95df576c03a3a3f61c038230")
(setf (get human_navigation::HumanNaviObjectStatus :datatype-) "human_navigation/HumanNaviObjectStatus")
(setf (get human_navigation::HumanNaviObjectStatus :definition-)
      "HumanNaviObjectInfo target_object
HumanNaviObjectInfo[] non_target_objects

================================================================================
MSG: human_navigation/HumanNaviObjectInfo
string name
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



(provide :human_navigation/HumanNaviObjectStatus "dc923dcd95df576c03a3a3f61c038230")


