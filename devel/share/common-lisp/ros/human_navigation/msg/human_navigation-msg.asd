
(cl:in-package :asdf)

(defsystem "human_navigation-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :geometry_msgs-msg
)
  :components ((:file "_package")
    (:file "HumanNaviAvatarStatus" :depends-on ("_package_HumanNaviAvatarStatus"))
    (:file "_package_HumanNaviAvatarStatus" :depends-on ("_package"))
    (:file "HumanNaviDestination" :depends-on ("_package_HumanNaviDestination"))
    (:file "_package_HumanNaviDestination" :depends-on ("_package"))
    (:file "HumanNaviGuidanceMsg" :depends-on ("_package_HumanNaviGuidanceMsg"))
    (:file "_package_HumanNaviGuidanceMsg" :depends-on ("_package"))
    (:file "HumanNaviMsg" :depends-on ("_package_HumanNaviMsg"))
    (:file "_package_HumanNaviMsg" :depends-on ("_package"))
    (:file "HumanNaviObjectInfo" :depends-on ("_package_HumanNaviObjectInfo"))
    (:file "_package_HumanNaviObjectInfo" :depends-on ("_package"))
    (:file "HumanNaviObjectStatus" :depends-on ("_package_HumanNaviObjectStatus"))
    (:file "_package_HumanNaviObjectStatus" :depends-on ("_package"))
    (:file "HumanNaviTaskInfo" :depends-on ("_package_HumanNaviTaskInfo"))
    (:file "_package_HumanNaviTaskInfo" :depends-on ("_package"))
  ))