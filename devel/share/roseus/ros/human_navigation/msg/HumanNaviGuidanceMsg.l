;; Auto-generated. Do not edit!


(when (boundp 'human_navigation::HumanNaviGuidanceMsg)
  (if (not (find-package "HUMAN_NAVIGATION"))
    (make-package "HUMAN_NAVIGATION"))
  (shadow 'HumanNaviGuidanceMsg (find-package "HUMAN_NAVIGATION")))
(unless (find-package "HUMAN_NAVIGATION::HUMANNAVIGUIDANCEMSG")
  (make-package "HUMAN_NAVIGATION::HUMANNAVIGUIDANCEMSG"))

(in-package "ROS")
;;//! \htmlinclude HumanNaviGuidanceMsg.msg.html


(defclass human_navigation::HumanNaviGuidanceMsg
  :super ros::object
  :slots (_message _display_type ))

(defmethod human_navigation::HumanNaviGuidanceMsg
  (:init
   (&key
    ((:message __message) "")
    ((:display_type __display_type) "")
    )
   (send-super :init)
   (setq _message (string __message))
   (setq _display_type (string __display_type))
   self)
  (:message
   (&optional __message)
   (if __message (setq _message __message)) _message)
  (:display_type
   (&optional __display_type)
   (if __display_type (setq _display_type __display_type)) _display_type)
  (:serialization-length
   ()
   (+
    ;; string _message
    4 (length _message)
    ;; string _display_type
    4 (length _display_type)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; string _message
       (write-long (length _message) s) (princ _message s)
     ;; string _display_type
       (write-long (length _display_type) s) (princ _display_type s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; string _message
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _message (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; string _display_type
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _display_type (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;;
   self)
  )

(setf (get human_navigation::HumanNaviGuidanceMsg :md5sum-) "61cc6c45b4669a8bb054d2fc90a5dff7")
(setf (get human_navigation::HumanNaviGuidanceMsg :datatype-) "human_navigation/HumanNaviGuidanceMsg")
(setf (get human_navigation::HumanNaviGuidanceMsg :definition-)
      "string message
string display_type

")



(provide :human_navigation/HumanNaviGuidanceMsg "61cc6c45b4669a8bb054d2fc90a5dff7")


