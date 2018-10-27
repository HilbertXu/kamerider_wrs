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
let HumanNaviDestination = require('./HumanNaviDestination.js');

//-----------------------------------------------------------

class HumanNaviTaskInfo {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.environment_id = null;
      this.target_object = null;
      this.destination = null;
      this.non_target_objects = null;
      this.furniture = null;
    }
    else {
      if (initObj.hasOwnProperty('environment_id')) {
        this.environment_id = initObj.environment_id
      }
      else {
        this.environment_id = '';
      }
      if (initObj.hasOwnProperty('target_object')) {
        this.target_object = initObj.target_object
      }
      else {
        this.target_object = new HumanNaviObjectInfo();
      }
      if (initObj.hasOwnProperty('destination')) {
        this.destination = initObj.destination
      }
      else {
        this.destination = new HumanNaviDestination();
      }
      if (initObj.hasOwnProperty('non_target_objects')) {
        this.non_target_objects = initObj.non_target_objects
      }
      else {
        this.non_target_objects = [];
      }
      if (initObj.hasOwnProperty('furniture')) {
        this.furniture = initObj.furniture
      }
      else {
        this.furniture = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type HumanNaviTaskInfo
    // Serialize message field [environment_id]
    bufferOffset = _serializer.string(obj.environment_id, buffer, bufferOffset);
    // Serialize message field [target_object]
    bufferOffset = HumanNaviObjectInfo.serialize(obj.target_object, buffer, bufferOffset);
    // Serialize message field [destination]
    bufferOffset = HumanNaviDestination.serialize(obj.destination, buffer, bufferOffset);
    // Serialize message field [non_target_objects]
    // Serialize the length for message field [non_target_objects]
    bufferOffset = _serializer.uint32(obj.non_target_objects.length, buffer, bufferOffset);
    obj.non_target_objects.forEach((val) => {
      bufferOffset = HumanNaviObjectInfo.serialize(val, buffer, bufferOffset);
    });
    // Serialize message field [furniture]
    // Serialize the length for message field [furniture]
    bufferOffset = _serializer.uint32(obj.furniture.length, buffer, bufferOffset);
    obj.furniture.forEach((val) => {
      bufferOffset = HumanNaviObjectInfo.serialize(val, buffer, bufferOffset);
    });
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type HumanNaviTaskInfo
    let len;
    let data = new HumanNaviTaskInfo(null);
    // Deserialize message field [environment_id]
    data.environment_id = _deserializer.string(buffer, bufferOffset);
    // Deserialize message field [target_object]
    data.target_object = HumanNaviObjectInfo.deserialize(buffer, bufferOffset);
    // Deserialize message field [destination]
    data.destination = HumanNaviDestination.deserialize(buffer, bufferOffset);
    // Deserialize message field [non_target_objects]
    // Deserialize array length for message field [non_target_objects]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.non_target_objects = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.non_target_objects[i] = HumanNaviObjectInfo.deserialize(buffer, bufferOffset)
    }
    // Deserialize message field [furniture]
    // Deserialize array length for message field [furniture]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.furniture = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.furniture[i] = HumanNaviObjectInfo.deserialize(buffer, bufferOffset)
    }
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += object.environment_id.length;
    length += HumanNaviObjectInfo.getMessageSize(object.target_object);
    object.non_target_objects.forEach((val) => {
      length += HumanNaviObjectInfo.getMessageSize(val);
    });
    object.furniture.forEach((val) => {
      length += HumanNaviObjectInfo.getMessageSize(val);
    });
    return length + 92;
  }

  static datatype() {
    // Returns string type for a message object
    return 'human_navigation/HumanNaviTaskInfo';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '72a83c8dcb8252f4b5e97040679254a7';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    string environment_id
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
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new HumanNaviTaskInfo(null);
    if (msg.environment_id !== undefined) {
      resolved.environment_id = msg.environment_id;
    }
    else {
      resolved.environment_id = ''
    }

    if (msg.target_object !== undefined) {
      resolved.target_object = HumanNaviObjectInfo.Resolve(msg.target_object)
    }
    else {
      resolved.target_object = new HumanNaviObjectInfo()
    }

    if (msg.destination !== undefined) {
      resolved.destination = HumanNaviDestination.Resolve(msg.destination)
    }
    else {
      resolved.destination = new HumanNaviDestination()
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

    if (msg.furniture !== undefined) {
      resolved.furniture = new Array(msg.furniture.length);
      for (let i = 0; i < resolved.furniture.length; ++i) {
        resolved.furniture[i] = HumanNaviObjectInfo.Resolve(msg.furniture[i]);
      }
    }
    else {
      resolved.furniture = []
    }

    return resolved;
    }
};

module.exports = HumanNaviTaskInfo;
