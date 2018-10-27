// Auto-generated. Do not edit!

// (in-package human_navigation.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class HumanNaviGuidanceMsg {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.message = null;
      this.display_type = null;
    }
    else {
      if (initObj.hasOwnProperty('message')) {
        this.message = initObj.message
      }
      else {
        this.message = '';
      }
      if (initObj.hasOwnProperty('display_type')) {
        this.display_type = initObj.display_type
      }
      else {
        this.display_type = '';
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type HumanNaviGuidanceMsg
    // Serialize message field [message]
    bufferOffset = _serializer.string(obj.message, buffer, bufferOffset);
    // Serialize message field [display_type]
    bufferOffset = _serializer.string(obj.display_type, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type HumanNaviGuidanceMsg
    let len;
    let data = new HumanNaviGuidanceMsg(null);
    // Deserialize message field [message]
    data.message = _deserializer.string(buffer, bufferOffset);
    // Deserialize message field [display_type]
    data.display_type = _deserializer.string(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += object.message.length;
    length += object.display_type.length;
    return length + 8;
  }

  static datatype() {
    // Returns string type for a message object
    return 'human_navigation/HumanNaviGuidanceMsg';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '61cc6c45b4669a8bb054d2fc90a5dff7';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    string message
    string display_type
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new HumanNaviGuidanceMsg(null);
    if (msg.message !== undefined) {
      resolved.message = msg.message;
    }
    else {
      resolved.message = ''
    }

    if (msg.display_type !== undefined) {
      resolved.display_type = msg.display_type;
    }
    else {
      resolved.display_type = ''
    }

    return resolved;
    }
};

module.exports = HumanNaviGuidanceMsg;
