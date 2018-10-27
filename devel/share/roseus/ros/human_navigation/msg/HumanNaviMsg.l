;; Auto-generated. Do not edit!


(when (boundp 'human_navigation::HumanNaviMsg)
  (if (not (find-package "HUMAN_NAVIGATION"))
    (make-package "HUMAN_NAVIGATION"))
  (shadow 'HumanNaviMsg (find-package "HUMAN_NAVIGATION")))
(unless (find-package "HUMAN_NAVIGATION::HUMANNAVIMSG")
  (make-package "HUMAN_NAVIGATION::HUMANNAVIMSG"))

(in-package "ROS")
;;//! \htmlinclude HumanNaviMsg.msg.html


(defclass human_navigation::HumanNaviMsg
  :super ros::object
  :slots (_message _detail ))

(defmethod human_navigation::HumanNaviMsg
  (:init
   (&key
    ((:message __message) "")
    ((:detail __detail) "")
    )
   (send-super :init)
   (setq _message (string __message))
   (setq _detail (string __detail))
   self)
  (:message
   (&optional __message)
   (if __message (setq _message __message)) _message)
  (:detail
   (&optional __detail)
   (if __detail (setq _detail __detail)) _detail)
  (:serialization-length
   ()
   (+
    ;; string _message
    4 (length _message)
    ;; string _detail
    4 (length _detail)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; string _message
       (write-long (length _message) s) (princ _message s)
     ;; string _detail
       (write-long (length _detail) s) (princ _detail s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; string _message
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _message (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; string _detail
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _detail (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;;
   self)
  )

(setf (get human_navigation::HumanNaviMsg :md5sum-) "83c3ad4b113aebdb7a85eba9ba595d50")
(setf (get human_navigation::HumanNaviMsg :datatype-) "human_navigation/HumanNaviMsg")
(setf (get human_navigation::HumanNaviMsg :definition-)
      "string message
string detail

")



(provide :human_navigation/HumanNaviMsg "83c3ad4b113aebdb7a85eba9ba595d50")


