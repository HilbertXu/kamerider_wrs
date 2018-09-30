/************************************
Author:  WangFeng
Date: 7/28/2018
*************************************/

#include "finding_auto.hpp"

void SIGVerseTb3OpenManipulatorGraspingAuto::rgbCameraInfoCallback(const sensor_msgs::CameraInfo::ConstPtr& camera_info)
{
  rgb_camera_height_ = camera_info->height;
  rgb_camera_width_  = camera_info->width;
  //ROS_INFO("rgb_camera_height=%d", (int)rgb_camera_height_);
}

void SIGVerseTb3OpenManipulatorGraspingAuto::graspableObjectCallback(const std_msgs::String::ConstPtr& msg)
{
   grasp_name = msg->data.c_str();
}

void  SIGVerseTb3OpenManipulatorGraspingAuto::trashboxCallback(const std_msgs::String::ConstPtr& msg_trash)
{
   trashbox_name = msg_trash->data.c_str();
}

void SIGVerseTb3OpenManipulatorGraspingAuto::boundingBoxesCallback(const darknet_ros_msgs::BoundingBoxes::ConstPtr& bounding_boxes)
{

  //ROS_INFO("boundingBoxesCallback size=%d", (int)bounding_boxes->boundingBoxes.size());

  int data_count = std::min<int>(bounding_boxes->boundingBoxes.size(), MAX_OBJECTS_NUM);

  bounding_boxes_data_.boundingBoxes.resize(data_count);

  for(int i=0; i<data_count; i++)
  {
    bounding_boxes_data_.boundingBoxes[i].Class       = bounding_boxes->boundingBoxes[i].Class;
    bounding_boxes_data_.boundingBoxes[i].probability = bounding_boxes->boundingBoxes[i].probability;
    bounding_boxes_data_.boundingBoxes[i].xmin        = bounding_boxes->boundingBoxes[i].xmin;
    bounding_boxes_data_.boundingBoxes[i].ymin        = bounding_boxes->boundingBoxes[i].ymin;
    bounding_boxes_data_.boundingBoxes[i].xmax        = bounding_boxes->boundingBoxes[i].xmax;
    bounding_boxes_data_.boundingBoxes[i].ymax        = bounding_boxes->boundingBoxes[i].ymax;
  }
}


void SIGVerseTb3OpenManipulatorGraspingAuto::pointCloudCallback(const sensor_msgs::PointCloud2::ConstPtr& point_cloud)
{
  //ROS_INFO("pointCloudCallback row_step=%d", (int)point_cloud->row_step);

  point_cloud_data_.header.seq        = point_cloud->header.seq;
  point_cloud_data_.header.stamp.sec  = point_cloud->header.stamp.sec;
  point_cloud_data_.header.stamp.nsec = point_cloud->header.stamp.nsec;
  point_cloud_data_.header.frame_id   = point_cloud->header.frame_id;

  point_cloud_data_.height = point_cloud->height;
  point_cloud_data_.width  = point_cloud->width;
  //ROS_INFO("pointCloudCallback height=%d", (int)point_cloud_data_.height);

  point_cloud_data_.fields.resize(point_cloud->fields.size());

  for(int i=0; i<point_cloud->fields.size(); i++)
  {
    point_cloud_data_.fields[i].name     = point_cloud->fields[i].name;
    point_cloud_data_.fields[i].offset   = point_cloud->fields[i].offset;
    point_cloud_data_.fields[i].datatype = point_cloud->fields[i].datatype;
    point_cloud_data_.fields[i].count    = point_cloud->fields[i].count;
  }

  point_cloud_data_.is_bigendian = point_cloud->is_bigendian;
  point_cloud_data_.point_step   = point_cloud->point_step;
  point_cloud_data_.row_step     = point_cloud->row_step;

  int data_count = point_cloud->row_step * point_cloud->height;
  //ROS_INFO("pointCloudCallback data_count=%d",(int)data_count);

  point_cloud_data_.data.resize(data_count);
  std::memcpy(&point_cloud_data_.data[0], &point_cloud->data[0], data_count * sizeof(uint8_t));

  point_cloud_data_.is_dense = point_cloud->is_dense;
}


bool SIGVerseTb3OpenManipulatorGraspingAuto::findGraspingTarget(geometry_msgs::Vector3 &point_cloud_pos, const std::string &target_name)
{
  for(int i=0; i<bounding_boxes_data_.boundingBoxes.size(); i++)
  {
    if(bounding_boxes_data_.boundingBoxes[i].Class == target_name &&
       bounding_boxes_data_.boundingBoxes[i].probability > PROBABILITY_THRESHOLD)
    {
      puts(("Found the target. name= " + target_name).c_str());

      int center_x = (bounding_boxes_data_.boundingBoxes[i].xmax + bounding_boxes_data_.boundingBoxes[i].xmin) / 2;
      int center_y = (bounding_boxes_data_.boundingBoxes[i].ymax + bounding_boxes_data_.boundingBoxes[i].ymin) / 2;

      puts(("x=" + std::to_string(center_x) + ", y=" + std::to_string(center_y)).c_str());

      int point_cloud_screen_x = clamp<int>((int)((float)center_x * point_cloud_data_.width  / rgb_camera_width_),  0, point_cloud_data_.width-1);
      int point_cloud_screen_y = clamp<int>((int)((float)center_y * point_cloud_data_.height / rgb_camera_height_), 0, point_cloud_data_.height-1);

      // the center
      bool is_succeeded = get3dPositionFromScreenPosition(point_cloud_pos, point_cloud_data_, point_cloud_screen_x, point_cloud_screen_y);

      puts(("x=" + std::to_string(point_cloud_pos.x) + ", y=" + std::to_string(point_cloud_pos.y) + ", z=" + std::to_string(point_cloud_pos.z)).c_str());

      if(is_succeeded) { return true; }

      // Around the center (1/4)
      int play_x = (bounding_boxes_data_.boundingBoxes[i].xmax - center_x) / 4;
      int play_y = (bounding_boxes_data_.boundingBoxes[i].ymax - center_y) / 4;

      for(int yi=-play_y; yi<=+play_y; yi+=play_y)
      {
        for(int xi=-play_x; xi<=+play_x; xi+=play_x)
        {
          is_succeeded = get3dPositionFromScreenPosition(point_cloud_pos, point_cloud_data_, point_cloud_screen_x + xi, point_cloud_screen_y + yi);
          puts(("x=" + std::to_string(point_cloud_pos.x) + ", y=" + std::to_string(point_cloud_pos.y) + ", z=" + std::to_string(point_cloud_pos.z)).c_str());
           if(is_succeeded) { return true; }
        }
      }

      // Around the center (1/2)
      play_x = (bounding_boxes_data_.boundingBoxes[i].xmax - center_x) / 2;
      play_y = (bounding_boxes_data_.boundingBoxes[i].ymax - center_y) / 2;

      for(int yi=-play_y; yi<=+play_y; yi+=play_y)
      {
        for(int xi=-play_x; xi<=+play_x; xi+=play_x)
        {
          is_succeeded = get3dPositionFromScreenPosition(point_cloud_pos, point_cloud_data_, point_cloud_screen_x + xi, point_cloud_screen_y + yi);
          puts(("x=" + std::to_string(point_cloud_pos.x) + ", y=" + std::to_string(point_cloud_pos.y) + ", z=" + std::to_string(point_cloud_pos.z)).c_str());
          if(is_succeeded) { return true; }
        }
      }

      puts("Failed to get point cloud data.");
      return false;
    }
  }

  //puts(("Couldn't find " + target_name + ". Or low probability.").c_str());
  //puts(("objects=" + getDetectedObjectsList()).c_str());

  return false;
}

bool SIGVerseTb3OpenManipulatorGraspingAuto::sensorToMap(tf::TransformBroadcaster &tf_broadcaster, tf::TransformListener &tf_listener, const std::string &target_name,geometry_msgs::Vector3 &map_target_pos)
{
  geometry_msgs::Vector3 target_pos;

  if(!findGraspingTarget(target_pos, target_name)){ return false; }

  tf::Transform target_transform;

  target_transform.setOrigin( tf::Vector3(target_pos.x, target_pos.y, target_pos.z));
  target_transform.setRotation( tf::Quaternion::getIdentity());

  tf_broadcaster.sendTransform(tf::StampedTransform(target_transform, ros::Time::now(),CAMERA_DEPTH_OPTICAL_FRAME_NAME,target_name));
  
  tf::StampedTransform map_transform;

  try
  {
  tf_listener.waitForTransform( MAP_NAME, target_name ,ros::Time(0), ros::Duration(0.3) );  
  tf_listener.lookupTransform(MAP_NAME, target_name ,ros::Time(0), map_transform);                 //ros中的坐标与虚拟中的坐标方向不全相符 rosrun tf tf_monitor  /map /rabbit
  }
  catch (tf::TransformException &ex)
  {
    puts(("Couldn't lookup the transform of TF."));
    ROS_ERROR("%s",ex.what());
    return false;
  }

  tf::Vector3 vec_map_transform = map_transform.getOrigin();
  map_target_pos.x =  vec_map_transform.x();
  map_target_pos.y = vec_map_transform.y();
  map_target_pos.z = vec_map_transform.z();
   puts(("vec_map_transform x=" + std::to_string(vec_map_transform.x()) + ", y=" + std::to_string(vec_map_transform.y()) + ", z=" + std::to_string(vec_map_transform.z())).c_str());

  return true;
} 



std::string SIGVerseTb3OpenManipulatorGraspingAuto::getDetectedObjectsList()
{
  std::string detected_objects_ = "";

  for(int i=0; i<bounding_boxes_data_.boundingBoxes.size(); i++)
  {
    detected_objects_
      += bounding_boxes_data_.boundingBoxes[i].Class + ": "
      + std::to_string((int)std::floor(bounding_boxes_data_.boundingBoxes[i].probability * 100)) + "%   ";
  }

  return detected_objects_;
}


void SIGVerseTb3OpenManipulatorGraspingAuto::showDetectedObjectsList()
{
  puts("\n");
  puts("---------------------------");
  puts("Detected objects Info");
  puts(("objects=" + getDetectedObjectsList()).c_str());
  puts("---------------------------");
}


void SIGVerseTb3OpenManipulatorGraspingAuto::keyLoop(int argc, char** argv)
{
    ros::init(argc, argv, "hsrb_finding", ros::init_options::NoSigintHandler);

    ros::NodeHandle node_handle;

    // Override the default ros sigint handler.
    // This must be set after the first NodeHandle is created.
    signal(SIGINT, rosSigintHandler);

    ros::Rate loop_rate(10);

    std::string sub_rgb_camera_info_topic_name;
    std::string sub_point_cloud_topic_name;
    std::string sub_bounding_boxes_topic_name;
    std::string sub_graspable_object_topic_name;
    std::string sub_trash_box_topic_name;

    node_handle.param<std::string>("sub_rgb_camera_info_topic_name",  sub_rgb_camera_info_topic_name,  "/hsrb/head_rgbd_sensor/rgb/camera_info");
    node_handle.param<std::string>("sub_point_cloud_topic_name",      sub_point_cloud_topic_name,      "/hsrb/head_rgbd_sensor/depth/points");
    node_handle.param<std::string>("sub_bounding_boxes_topic_name",   sub_bounding_boxes_topic_name,   "/darknet_ros/bounding_boxes");
    node_handle.param<std::string>("sub_graspable_object_topic_name",  sub_graspable_object_topic_name,   "GraspableObject");
    node_handle.param<std::string>("sub_trash_box_topic_name",  sub_trash_box_topic_name,   "TrashboxName");

    ros::Subscriber sub_rgb_camera_info = node_handle.subscribe(sub_rgb_camera_info_topic_name, 10, &SIGVerseTb3OpenManipulatorGraspingAuto::rgbCameraInfoCallback, this);
    ros::Subscriber sub_point_cloud     = node_handle.subscribe(sub_point_cloud_topic_name,     10, &SIGVerseTb3OpenManipulatorGraspingAuto::pointCloudCallback, this);
    ros::Subscriber sub_bounding_boxes  = node_handle.subscribe(sub_bounding_boxes_topic_name,  10, &SIGVerseTb3OpenManipulatorGraspingAuto::boundingBoxesCallback, this);
    ros::Subscriber sub_graspable_object  = node_handle.subscribe(sub_graspable_object_topic_name,  10, &SIGVerseTb3OpenManipulatorGraspingAuto::graspableObjectCallback, this);
    ros::Subscriber sub_trash_box  = node_handle.subscribe(sub_trash_box_topic_name,  10, &SIGVerseTb3OpenManipulatorGraspingAuto::trashboxCallback, this);
 
    tf::TransformBroadcaster tf_broadcaster;
    tf::TransformListener tf_listener;

   ros::Publisher pub = node_handle.advertise<geometry_msgs::Vector3>("/target/xyz", 100);
   ros::Publisher pub_trashbox = node_handle.advertise<geometry_msgs::Vector3>("/trashbox/xyz", 100);
    sleep(2);

    bool is_transforming = false;
    bool is_trashbox = false;
   
    while (ros::ok())
    { 
      if(!is_transforming)
      {
        geometry_msgs::Vector3 map_target_pos;
        std::string c = grasp_name;
        
       if(c==GRASPING_TARGET1_NAME)
       {
       GRASPING_TARGET = GRASPING_TARGET1_NAME;
       }
       if(c==GRASPING_TARGET2_NAME)
       {
       GRASPING_TARGET = GRASPING_TARGET2_NAME;
       }
       if(c==GRASPING_TARGET3_NAME)
       {
       GRASPING_TARGET = GRASPING_TARGET3_NAME;
       }
       if(c==GRASPING_TARGET4_NAME)
       {
       GRASPING_TARGET = GRASPING_TARGET4_NAME;
       }
       if(c==GRASPING_TARGET5_NAME)
       {
       GRASPING_TARGET = GRASPING_TARGET5_NAME;
       }
       if(c==GRASPING_TARGET6_NAME)
       {
       GRASPING_TARGET = GRASPING_TARGET6_NAME;
       }
       if(c==GRASPING_TARGET7_NAME)
       {
       GRASPING_TARGET = GRASPING_TARGET7_NAME;
       }
       if(c==GRASPING_TARGET8_NAME)
       {
       GRASPING_TARGET = GRASPING_TARGET8_NAME;
       }
       if(c==GRASPING_TARGET9_NAME)
       {
       GRASPING_TARGET = GRASPING_TARGET9_NAME;
       }
       if(c==GRASPING_TARGET10_NAME)
       {
       GRASPING_TARGET = GRASPING_TARGET10_NAME;
       }
       if(c==GRASPING_TARGET11_NAME)
       {
       GRASPING_TARGET = GRASPING_TARGET11_NAME;
       }
       if(c==GRASPING_TARGET12_NAME)
       {
       GRASPING_TARGET = GRASPING_TARGET12_NAME;
       }
       if(c==GRASPING_TARGET13_NAME)
       {
       GRASPING_TARGET = GRASPING_TARGET13_NAME;
       }
        is_transforming = sensorToMap(tf_broadcaster, tf_listener, GRASPING_TARGET,map_target_pos);
        pub.publish(map_target_pos); 
      }     
      else
      {
        is_transforming = false;
      }
      if(!is_trashbox)
      {
        geometry_msgs::Vector3 map_target_pos_trash;
        std::string d = trashbox_name;
       if(d==TRASH1_NAME)
       {
       TRASNBOX_TARGET = TRASH1_NAME;
       }
       if(d==TRASH2_NAME)
       {
       TRASNBOX_TARGET = TRASH2_NAME;
       }
       if(d==TRASH3_NAME)
       {
       TRASNBOX_TARGET = TRASH3_NAME;
       }
       is_trashbox = sensorToMap(tf_broadcaster, tf_listener, TRASNBOX_TARGET,map_target_pos_trash);
       pub_trashbox.publish(map_target_pos_trash); 
      }     
      else
      {
        is_trashbox = false;
      }
    ros::spinOnce();
    loop_rate.sleep();
    }
      return;
}


int main(int argc, char** argv)
{
  SIGVerseTb3OpenManipulatorGraspingAuto grasping_auto;

  grasping_auto.keyLoop(argc, argv);

  return(EXIT_SUCCESS);
}


