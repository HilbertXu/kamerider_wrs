;; Auto-generated. Do not edit!


(when (boundp 'human_navigation::HumanNaviTaskInfo)
  (if (not (find-package "HUMAN_NAVIGATION"))
    (make-package "HUMAN_NAVIGATION"))
  (shadow 'HumanNaviTaskInfo (find-package "HUMAN_NAVIGATION")))
(unless (find-package "HUMAN_NAVIGATION::HUMANNAVITASKINFO")
  (make-package "HUMAN_NAVIGATION::HUMANNAVITASKINFO"))

(in-package "ROS")
;;//! \htmlinclude HumanNaviTaskInfo.msg.html


(defclass human_navigation::HumanNaviTaskInfo
  :super ros::object
  :slots (_environment_id _target_object _destination _non_target_objects _furniture ))

(defmethod human_navigation::HumanNaviTaskInfo
  (:init
   (&key
    ((:environment_id __environment_id) "")
    ((:target_object __target_object) (instance human_navigation::HumanNaviObjectInfo :init))
    ((:destination __destination) (instance human_navigation::HumanNaviDestination :init))
    ((:non_target_objects __non_target_objects) (let (r) (dotimes (i 0) (push (instance human_navigation::HumanNaviObjectInfo :init) r)) r))
    ((:furniture __furniture) (let (r) (dotimes (i 0) (push (instance human_navigation::HumanNaviObjectInfo :init) r)) r))
    )
   (send-super :init)
   (setq _environment_id (string __environment_id))
   (setq _target_object __target_object)
   (setq _destination __destination)
   (setq _non_target_objects __non_target_objects)
   (setq _furniture __furniture)
   self)
  (:environment_id
   (&optional __environment_id)
   (if __environment_id (setq _environment_id __environment_id)) _environment_id)
  (:target_object
   (&rest __target_object)
   (if (keywordp (car __target_object))
       (send* _target_object __target_object)
     (progn
       (if __target_object (setq _target_object (car __target_object)))
       _target_object)))
  (:destination
   (&rest __destination)
   (if (keywordp (car __destination))
       (send* _destination __destination)
     (progn
       (if __destination (setq _destination (car __destination)))
       _destination)))
  (:non_target_objects
   (&rest __non_target_objects)
   (if (keywordp (car __non_target_objects))
       (send* _non_target_objects __non_target_objects)
     (progn
       (if __non_target_objects (setq _non_target_objects (car __non_target_objects)))
       _non_target_objects)))
  (:furniture
   (&rest __furniture)
   (if (keywordp (car __furniture))
       (send* _furniture __furniture)
     (progn
       (if __furniture (setq _furniture (car __furniture)))
       _furniture)))
  (:serialization-length
   ()
   (+
    ;; string _environment_id
    4 (length _environment_id)
    ;; human_navigation/HumanNaviObjectInfo _target_object
    (send _target_object :serialization-length)
    ;; human_navigation/HumanNaviDestination _destination
    (send _destination :serialization-length)
    ;; human_navigation/HumanNaviObjectInfo[] _non_target_objects
    (apply #'+ (send-all _non_target_objects :serialization-length)) 4
    ;; human_navigation/HumanNaviObjectInfo[] _furniture
    (apply #'+ (send-all _furniture :serialization-length)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; string _environment_id
       (write-long (length _environment_id) s) (princ _environment_id s)
     ;; human_navigation/HumanNaviObjectInfo _target_object
       (send _target_object :serialize s)
     ;; human_navigation/HumanNaviDestination _destination
       (send _destination :serialize s)
     ;; human_navigation/HumanNaviObjectInfo[] _non_target_objects
     (write-long (length _non_target_objects) s)
     (dolist (elem _non_target_objects)
       (send elem :serialize s)
       )
     ;; human_navigation/HumanNaviObjectInfo[] _furniture
     (write-long (length _furniture) s)
     (dolist (elem _furniture)
       (send elem :serialize s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; string _environment_id
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _environment_id (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; human_navigation/HumanNaviObjectInfo _target_object
     (send _target_object :deserialize buf ptr-) (incf ptr- (send _target_object :serialization-length))
   ;; human_navigation/HumanNaviDestination _destination
     (send _destination :deserialize buf ptr-) (incf ptr- (send _destination :serialization-length))
   ;; human_navigation/HumanNaviObjectInfo[] _non_target_objects
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _non_target_objects (let (r) (dotimes (i n) (push (instance human_navigation::HumanNaviObjectInfo :init) r)) r))
     (dolist (elem- _non_target_objects)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;; human_navigation/HumanNaviObjectInfo[] _furniture
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _furniture (let (r) (dotimes (i n) (push (instance human_navigation::HumanNaviObjectInfo :init) r)) r))
     (dolist (elem- _furniture)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;;
   self)
  )

(setf (get human_navigation::HumanNaviTaskInfo :md5sum-) "72a83c8dcb8252f4b5e97040679254a7")
(setf (get human_navigation::HumanNaviTaskInfo :datatype-) "human_navigation/HumanNaviTaskInfo")
(setf (get human_navigation::HumanNaviTaskInfo :definition-)
      "string environment_id
HumanNaviObjectInfo target_object
HumanNaviDestination destination
HumanNaviObjectInfo[] non_target_objects
HumanNaviObjectInfo[] furniture

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

================================================================================
MSG: human_navigation/HumanNaviDestination
geometry_msgs/Point position
geometry_msgs/Quaternion orientation
geometry_msgs/Point size

")



(provide :human_navigation/HumanNaviTaskInfo "72a83c8dcb8252f4b5e97040679254a7")


