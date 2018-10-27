// Auto-generated. Do not edit!

// (in-package human_navigation.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let HumanNaviObjectInfo = require('./HumanNaviObjectInfo.js');

//-----------------------------------------------------------

class HumanNaviObjectStatus {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.target_object = null;
      this.non_target_objects = null;
    }
    else {
      if (initObj.hasOwnProperty('target_object')) {
        this.target_object = initObj.target_object
      }
      else {
        this.target_object = new HumanNaviObjectInfo();
      }
      if (initObj.hasOwnProperty('non_target_objects')) {
        this.non_target_objects = initObj.non_target_objects
      }
      else {
        this.non_target_objects = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type HumanNaviObjectStatus
    // Serialize message field [target_object]
    bufferOffset = HumanNaviObjectInfo.serialize(obj.target_object, buffer, bufferOffset);
    // Serialize message field [non_target_objects]
    // Serialize the length for message field [non_target_objects]
    bufferOffset = _serializer.uint32(obj.non_target_objects.length, buffer, bufferOffset);
    obj.non_target_objects.forEach((val) => {
      bufferOffset = HumanNaviObjectInfo.serialize(val, buffer, bufferOffset);
    });
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type HumanNaviObjectStatus
    let len;
    let data = new HumanNaviObjectStatus(null);
    // Deserialize message field [target_object]
    data.target_object = HumanNaviObjectInfo.deserialize(buffer, bufferOffset);
    // Deserialize message field [non_target_objects]
    // Deserialize array length for message field [non_target_objects]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.non_target_objects = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.non_target_objects[i] = HumanNaviObjectInfo.deserialize(buffer, bufferOffset)
    }
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += HumanNaviObjectInfo.getMessageSize(object.target_object);
    object.non_target_objects.forEach((val) => {
      length += HumanNaviObjectInfo.getMessageSize(val);
    });
    return length + 4;
  }

  static datatype() {
    // Returns string type for a message object
    return 'human_navigation/HumanNaviObjectStatus';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'dc923dcd95df576c03a3a3f61c038230';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    HumanNaviObjectInfo target_object
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
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new HumanNaviObjectStatus(null);
    if (msg.target_object !== undefined) {
      resolved.target_object = HumanNaviObjectInfo.Resolve(msg.target_object)
    }
    else {
      resolved.target_object = new HumanNaviObjectInfo()
    }

    if (msg.non_target_objects !== undefined) {
      resolved.non_target_objects = new Array(msg.non_target_objects.length);
      for (let i = 0; i < resolved.non_target_objects.length; ++i) {
        resolved.non_target_objects[i] = HumanNaviObjectInfo.Resolve(msg.non_target_objects[i]);
      }
    }
    else {
      resolved.non_target_objects = []
    }

    return resolved;
    }
};

module.exports = HumanNaviObjectStatus;
