;; Auto-generated. Do not edit!


(when (boundp 'follow::LegArray)
  (if (not (find-package "FOLLOW"))
    (make-package "FOLLOW"))
  (shadow 'LegArray (find-package "FOLLOW")))
(unless (find-package "FOLLOW::LEGARRAY")
  (make-package "FOLLOW::LEGARRAY"))

(in-package "ROS")
;;//! \htmlinclude LegArray.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass follow::LegArray
  :super ros::object
  :slots (_header _legs ))

(defmethod follow::LegArray
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:legs __legs) (let (r) (dotimes (i 0) (push (instance follow::Leg :init) r)) r))
    )
   (send-super :init)
   (setq _header __header)
   (setq _legs __legs)
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:legs
   (&rest __legs)
   (if (keywordp (car __legs))
       (send* _legs __legs)
     (progn
       (if __legs (setq _legs (car __legs)))
       _legs)))
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; follow/Leg[] _legs
    (apply #'+ (send-all _legs :serialization-length)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; follow/Leg[] _legs
     (write-long (length _legs) s)
     (dolist (elem _legs)
       (send elem :serialize s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; follow/Leg[] _legs
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _legs (let (r) (dotimes (i n) (push (instance follow::Leg :init) r)) r))
     (dolist (elem- _legs)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;;
   self)
  )

(setf (get follow::LegArray :md5sum-) "9e932f47090c69cbf30f535c2ab02e32")
(setf (get follow::LegArray :datatype-) "follow/LegArray")
(setf (get follow::LegArray :definition-)
      "std_msgs/Header header
Leg[] legs
================================================================================
MSG: std_msgs/Header
# Standard metadata for higher-level stamped data types.
# This is generally used to communicate timestamped data 
# in a particular coordinate frame.
# 
# sequence ID: consecutively increasing ID 
uint32 seq
#Two-integer timestamp that is expressed as:
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
# time-handling sugar is provided by the client library
time stamp
#Frame this data is associated with
# 0: no frame
# 1: global frame
string frame_id

================================================================================
MSG: follow/Leg
geometry_msgs/Point position
float32 confidence
================================================================================
MSG: geometry_msgs/Point
# This contains the position of a point in free space
float64 x
float64 y
float64 z

")



(provide :follow/LegArray "9e932f47090c69cbf30f535c2ab02e32")


