
(cl:in-package :asdf)

(defsystem "interactive_cleanup-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "InteractiveCleanupMsg" :depends-on ("_package_InteractiveCleanupMsg"))
    (:file "_package_InteractiveCleanupMsg" :depends-on ("_package"))
  ))