;; Auto-generated. Do not edit!


(when (boundp 'interactive_cleanup::InteractiveCleanupMsg)
  (if (not (find-package "INTERACTIVE_CLEANUP"))
    (make-package "INTERACTIVE_CLEANUP"))
  (shadow 'InteractiveCleanupMsg (find-package "INTERACTIVE_CLEANUP")))
(unless (find-package "INTERACTIVE_CLEANUP::INTERACTIVECLEANUPMSG")
  (make-package "INTERACTIVE_CLEANUP::INTERACTIVECLEANUPMSG"))

(in-package "ROS")
;;//! \htmlinclude InteractiveCleanupMsg.msg.html


(defclass interactive_cleanup::InteractiveCleanupMsg
  :super ros::object
  :slots (_message _detail ))

(defmethod interactive_cleanup::InteractiveCleanupMsg
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

(setf (get interactive_cleanup::InteractiveCleanupMsg :md5sum-) "83c3ad4b113aebdb7a85eba9ba595d50")
(setf (get interactive_cleanup::InteractiveCleanupMsg :datatype-) "interactive_cleanup/InteractiveCleanupMsg")
(setf (get interactive_cleanup::InteractiveCleanupMsg :definition-)
      "string message
string detail

")



(provide :interactive_cleanup/InteractiveCleanupMsg "83c3ad4b113aebdb7a85eba9ba595d50")


