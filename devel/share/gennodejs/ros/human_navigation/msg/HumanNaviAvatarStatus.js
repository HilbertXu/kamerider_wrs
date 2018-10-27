// Auto-generated. Do not edit!

// (in-package human_navigation.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let geometry_msgs = _finder('geometry_msgs');

//-----------------------------------------------------------

class HumanNaviAvatarStatus {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.head = null;
      this.body = null;
      this.left_hand = null;
      this.right_hand = null;
      this.object_in_left_hand = null;
      this.object_in_right_hand = null;
      this.is_target_object_in_left_hand = null;
      this.is_target_object_in_right_hand = null;
    }
    else {
      if (initObj.hasOwnProperty('head')) {
        this.head = initObj.head
      }
      else {
        this.head = new geometry_msgs.msg.Pose();
      }
      if (initObj.hasOwnProperty('body')) {
        this.body = initObj.body
      }
      else {
        this.body = new geometry_msgs.msg.Pose();
      }
      if (initObj.hasOwnProperty('left_hand')) {
        this.left_hand = initObj.left_hand
      }
      else {
        this.left_hand = new geometry_msgs.msg.Pose();
      }
      if (initObj.hasOwnProperty('right_hand')) {
        this.right_hand = initObj.right_hand
      }
      else {
        this.right_hand = new geometry_msgs.msg.Pose();
      }
      if (initObj.hasOwnProperty('object_in_left_hand')) {
        this.object_in_left_hand = initObj.object_in_left_hand
      }
      else {
        this.object_in_left_hand = '';
      }
      if (initObj.hasOwnProperty('object_in_right_hand')) {
        this.object_in_right_hand = initObj.object_in_right_hand
      }
      else {
        this.object_in_right_hand = '';
      }
      if (initObj.hasOwnProperty('is_target_object_in_left_hand')) {
        this.is_target_object_in_left_hand = initObj.is_target_object_in_left_hand
      }
      else {
        this.is_target_object_in_left_hand = false;
      }
      if (initObj.hasOwnProperty('is_target_object_in_right_hand')) {
        this.is_target_object_in_right_hand = initObj.is_target_object_in_right_hand
      }
      else {
        this.is_target_object_in_right_hand = false;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type HumanNaviAvatarStatus
    // Serialize message field [head]
    bufferOffset = geometry_msgs.msg.Pose.serialize(obj.head, buffer, bufferOffset);
    // Serialize message field [body]
    bufferOffset = geometry_msgs.msg.Pose.serialize(obj.body, buffer, bufferOffset);
    // Serialize message field [left_hand]
    bufferOffset = geometry_msgs.msg.Pose.serialize(obj.left_hand, buffer, bufferOffset);
    // Serialize message field [right_hand]
    bufferOffset = geometry_msgs.msg.Pose.serialize(obj.right_hand, buffer, bufferOffset);
    // Serialize message field [object_in_left_hand]
    bufferOffset = _serializer.string(obj.object_in_left_hand, buffer, bufferOffset);
    // Serialize message field [object_in_right_hand]
    bufferOffset = _serializer.string(obj.object_in_right_hand, buffer, bufferOffset);
    // Serialize message field [is_target_object_in_left_hand]
    bufferOffset = _serializer.bool(obj.is_target_object_in_left_hand, buffer, bufferOffset);
    // Serialize message field [is_target_object_in_right_hand]
    bufferOffset = _serializer.bool(obj.is_target_object_in_right_hand, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type HumanNaviAvatarStatus
    let len;
    let data = new HumanNaviAvatarStatus(null);
    // Deserialize message field [head]
    data.head = geometry_msgs.msg.Pose.deserialize(buffer, bufferOffset);
    // Deserialize message field [body]
    data.body = geometry_msgs.msg.Pose.deserialize(buffer, bufferOffset);
    // Deserialize message field [left_hand]
    data.left_hand = geometry_msgs.msg.Pose.deserialize(buffer, bufferOffset);
    // Deserialize message field [right_hand]
    data.right_hand = geometry_msgs.msg.Pose.deserialize(buffer, bufferOffset);
    // Deserialize message field [object_in_left_hand]
    data.object_in_left_hand = _deserializer.string(buffer, bufferOffset);
    // Deserialize message field [object_in_right_hand]
    data.object_in_right_hand = _deserializer.string(buffer, bufferOffset);
    // Deserialize message field [is_target_object_in_left_hand]
    data.is_target_object_in_left_hand = _deserializer.bool(buffer, bufferOffset);
    // Deserialize message field [is_target_object_in_right_hand]
    data.is_target_object_in_right_hand = _deserializer.bool(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += object.object_in_left_hand.length;
    length += object.object_in_right_hand.length;
    return length + 234;
  }

  static datatype() {
    // Returns string type for a message object
    return 'human_navigation/HumanNaviAvatarStatus';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'f9190ca25b6cd6192693cc3f2a3156de';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    geometry_msgs/Pose head
    geometry_msgs/Pose body
    geometry_msgs/Pose left_hand
    geometry_msgs/Pose right_hand
    string object_in_left_hand
    string object_in_right_hand
    bool is_target_object_in_left_hand
    bool is_target_object_in_right_hand
    
    ================================================================================
    MSG: geometry_msgs/Pose
    # A representation of pose in free space, composed of position and orientation. 
    Point position
    Quaternion orientation
    
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
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new HumanNaviAvatarStatus(null);
    if (msg.head !== undefined) {
      resolved.head = geometry_msgs.msg.Pose.Resolve(msg.head)
    }
    else {
      resolved.head = new geometry_msgs.msg.Pose()
    }

    if (msg.body !== undefined) {
      resolved.body = geometry_msgs.msg.Pose.Resolve(msg.body)
    }
    else {
      resolved.body = new geometry_msgs.msg.Pose()
    }

    if (msg.left_hand !== undefined) {
      resolved.left_hand = geometry_msgs.msg.Pose.Resolve(msg.left_hand)
    }
    else {
      resolved.left_hand = new geometry_msgs.msg.Pose()
    }

    if (msg.right_hand !== undefined) {
      resolved.right_hand = geometry_msgs.msg.Pose.Resolve(msg.right_hand)
    }
    else {
      resolved.right_hand = new geometry_msgs.msg.Pose()
    }

    if (msg.object_in_left_hand !== undefined) {
      resolved.object_in_left_hand = msg.object_in_left_hand;
    }
    else {
      resolved.object_in_left_hand = ''
    }

    if (msg.object_in_right_hand !== undefined) {
      resolved.object_in_right_hand = msg.object_in_right_hand;
    }
    else {
      resolved.object_in_right_hand = ''
    }

    if (msg.is_target_object_in_left_hand !== undefined) {
      resolved.is_target_object_in_left_hand = msg.is_target_object_in_left_hand;
    }
    else {
      resolved.is_target_object_in_left_hand = false
    }

    if (msg.is_target_object_in_right_hand !== undefined) {
      resolved.is_target_object_in_right_hand = msg.is_target_object_in_right_hand;
    }
    else {
      resolved.is_target_object_in_right_hand = false
    }

    return resolved;
    }
};

module.exports = HumanNaviAvatarStatus;
