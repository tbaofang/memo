/*
 * Copyright (c) 2008, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/* Author: Brian Gerkey */

#define USAGE "\nUSAGE: map_server <map.yaml>\n" \
  "  map.yaml: map description file\n" \
  "DEPRECATED USAGE: map_server <map> <resolution>\n" \
  "  map: image file to load\n"\
  "  resolution: map resolution [meters/pixel]"
#include "map_server/ls.h"
#include "map_server/map_server.h"


#ifdef HAVE_NEW_YAMLCPP
// The >> operator disappeared in yaml-cpp 0.5, so this function is
// added to provide support for code written under the yaml-cpp 0.3 API.
template<typename T>
void operator >> (const YAML::Node& node, T& i)
{
  i = node.as<T>();
}
#endif


/** Trivial constructor */
MapServer::MapServer(const std::string &file_name, double resolution): fname(file_name), res(resolution)
{
  vwall_sub_ = n.subscribe("/lsvwalls", 1, &MapServer::vwallCallback, this);
}

int MapServer::loadMap()
{
  ROS_INFO("file_name = %s, res = %f", fname.c_str(), res);
  int error_code = 0;
  std::string mapfname = "";
  double origin[3];
  int negate;
  double occ_th, free_th;
  MapMode mode = TRINARY;
  std::string frame_id;
  ros::NodeHandle private_nh("~");
  private_nh.param("frame_id", frame_id, std::string("map"));
  deprecated = (res != 0);

  if (!deprecated) {
    ROS_INFO("In if");
    if (fname.size() <= 4)
    {
      fname += ".yaml";
    }
    else
    {
      std::string file_type = fname.substr(fname.size() - 4);
      if (file_type != ".yaml")
      {
        fname += ".yaml";
      }
    }

    std::ifstream fin(fname.c_str());

    if (fin.fail()) {
      ROS_ERROR("Map_server could not open %s.", fname.c_str());
      error_code = LSSV_LOAD_MAP_ERROR;
      return error_code;
    }
#ifdef HAVE_NEW_YAMLCPP
    // The document loading process changed in yaml-cpp 0.5.
    YAML::Node doc = YAML::Load(fin);
#else
    YAML::Parser parser(fin);
    YAML::Node doc;
    parser.GetNextDocument(doc);
#endif
    try {
      doc["resolution"] >> res;
    } catch (YAML::InvalidScalar) {
      ROS_ERROR("The map does not contain a resolution tag or it is invalid.");
      error_code = LSSV_LOAD_MAP_ERROR;
      return error_code;
    }
    try {
      doc["negate"] >> negate;
    } catch (YAML::InvalidScalar) {
      ROS_ERROR("The map does not contain a negate tag or it is invalid.");
      error_code = LSSV_LOAD_MAP_ERROR;
      return error_code;
    }
    try {
      doc["occupied_thresh"] >> occ_th;
    } catch (YAML::InvalidScalar) {
      ROS_ERROR("The map does not contain an occupied_thresh tag or it is invalid.");
      error_code = LSSV_LOAD_MAP_ERROR;
      return error_code;
    }
    try {
      doc["free_thresh"] >> free_th;
    } catch (YAML::InvalidScalar) {
      ROS_ERROR("The map does not contain a free_thresh tag or it is invalid.");
      error_code = LSSV_LOAD_MAP_ERROR;
      return error_code;
    }
    try {
      std::string modeS = "";
      doc["mode"] >> modeS;

      if(modeS=="trinary")
        mode = TRINARY;
      else if(modeS=="scale")
        mode = SCALE;
      else if(modeS=="raw")
        mode = RAW;
      else{
        ROS_ERROR("Invalid mode tag \"%s\".", modeS.c_str());
        error_code = LSSV_LOAD_MAP_ERROR;
        return error_code;
      }
    } catch (YAML::Exception) {
      ROS_DEBUG("The map does not contain a mode tag or it is invalid... assuming Trinary");
      mode = TRINARY;
    }
    try {
      doc["origin"][0] >> origin[0];
      doc["origin"][1] >> origin[1];
      doc["origin"][2] >> origin[2];
    } catch (YAML::InvalidScalar) {
      ROS_ERROR("The map does not contain an origin tag or it is invalid.");
      error_code = LSSV_LOAD_MAP_ERROR;
      return error_code;
    }
    try {
      doc["image"] >> mapfname;
      // TODO: make this path-handling more robust
      if(mapfname.size() == 0)
      {
        ROS_ERROR("The image tag cannot be an empty string.");
        error_code = LSSV_LOAD_MAP_ERROR;
        return error_code;
      }
      if(mapfname[0] != '/')
      {
        // dirname can modify what you pass it
        char* fname_copy = strdup(fname.c_str());
        mapfname = std::string(dirname(fname_copy)) + '/' + mapfname;
        free(fname_copy);
      }
    } catch (YAML::InvalidScalar) {
      ROS_ERROR("The map does not contain an image tag or it is invalid.");
      error_code = LSSV_LOAD_MAP_ERROR;
      return error_code;
    }
  } else {
    ROS_INFO("In else");
    private_nh.param("negate", negate, 0);
    private_nh.param("occupied_thresh", occ_th, 0.65);
    private_nh.param("free_thresh", free_th, 0.196);
    mapfname = fname;
    origin[0] = origin[1] = origin[2] = 0.0;
  }

  ROS_INFO("Loading map from image \"%s\"", mapfname.c_str());
  //      map_server::loadMapFromFile(&map_resp_,mapfname.c_str(),res,negate,occ_th,free_th, origin, mode);
  map_server::loadCVMapFromFile(&map_resp_,mapfname.c_str(),res,negate,occ_th,free_th, origin, mode);
  map_resp_.map.info.map_load_time = ros::Time::now();
  map_resp_.map.header.frame_id = frame_id;
  map_resp_.map.header.stamp = ros::Time::now();
  ROS_INFO("Read a %d X %d map @ %.3lf m/cell",
           map_resp_.map.info.width,
           map_resp_.map.info.height,
           map_resp_.map.info.resolution);
  meta_data_message_ = map_resp_.map.info;

  service = n.advertiseService("static_map", &MapServer::mapCallback, this);

  //pub = n.advertise<nav_msgs::MapMetaData>("map_metadata", 1,

  // Latched publisher for metadata
  metadata_pub= n.advertise<nav_msgs::MapMetaData>("map_metadata", 1, true);
  metadata_pub.publish( meta_data_message_ );

  // Latched publisher for data
  map_pub = n.advertise<nav_msgs::OccupancyGrid>("map", 1, true);
  map_pub.publish( map_resp_.map );
  ROS_INFO("map_server pub map");
  return error_code;
}

void MapServer::vwallCallback(const lsrobot::LSVirtualWallsConstPtr &vwalls)
{
  ROS_INFO("VWallCallback");

  if (map_resp_.map.info.width <=0 || map_resp_.map.info.height <=0)
  {
    ROS_ERROR("Map is empty! Cannot draw virtual wall on an empty map!!");
    return;
  }

  if (map_resp_.map.data.size() <=0 )
  {
    ROS_ERROR("map_data_.size() <=0");
    return;
  }

  nav_msgs::OccupancyGrid map;
  map.data.resize(map_resp_.map.data.size());



  // copy the origin map data
  for (int i = 0; i < map_resp_.map.data.size(); i++)
  {
    map.data[i] = map_resp_.map.data[i];
  }

  //  map_name_ = vwalls->map_name;
  //  ROS_INFO("map_name = %s", map_name_.c_str());

  if (vwalls->walls.size() > 0)
  {
    // ROS_INFO("walls size = %d", vwalls->walls.size());
    // draw the line wall
    cv::Mat virtual_img(map_resp_.map.info.height, map_resp_.map.info.width, CV_8UC1, cv::Scalar(0));
    cv::Point start_pt, end_pt;

    for (int i = 0; i < vwalls->walls.size(); i++)
    {
      start_pt = world2Map(vwalls->walls[i].start_x, vwalls->walls[i].start_y);
      end_pt = world2Map(vwalls->walls[i].end_x, vwalls->walls[i].end_y);

      cv::line(virtual_img, start_pt, end_pt, cv::Scalar(255), 2);
    }

    //cv::imwrite("/home/fu/map/line.png", virtual_img);
    int nl = virtual_img.rows;
    int nc = virtual_img.cols* virtual_img.channels();
    if (virtual_img.isContinuous())
    {
      nc = nc*nl;
      nl = 1;
    }

    for (int j = 0; j < nl; j++)
    {
      uchar * data = virtual_img.ptr<uchar>(j);
      for (int i = 0; i < nc; i++)
      {
        if(data[i] == 255)
        {
          int index = j*nc + i;
          map.data[index] = +100;
        }
      }
    }
  }

  map.info.map_load_time = ros::Time::now();
  map.header.frame_id = map_resp_.map.header.frame_id;
  map.header.stamp = ros::Time::now();
  map.info.resolution = map_resp_.map.info.resolution;
  map.info.width = map_resp_.map.info.width;
  map.info.height = map_resp_.map.info.height;
  map.info.origin.position.x = map_resp_.map.info.origin.position.x;
  map.info.origin.position.y = map_resp_.map.info.origin.position.y;
  map.info.origin.position.z = map_resp_.map.info.origin.position.z;
  map.info.origin.orientation.x = map_resp_.map.info.origin.orientation.x;
  map.info.origin.orientation.y = map_resp_.map.info.origin.orientation.y;
  map.info.origin.orientation.z = map_resp_.map.info.origin.orientation.z;
  map.info.origin.orientation.w = map_resp_.map.info.origin.orientation.w;
  map_pub.publish(map);
}


/** Callback invoked when someone requests our service */
bool MapServer::mapCallback(nav_msgs::GetMap::Request  &req,
                            nav_msgs::GetMap::Response &res )
{
  // request is empty; we ignore it

  // = operator is overloaded to make deep copy (tricky!)
  res = map_resp_;
  ROS_INFO("Sending map");

  return true;
}

cv::Point MapServer::world2Map(double x, double y)
{
  cv::Point pt(0,0);
  if (map_resp_.map.info.resolution <= 0)
    return pt;

  pt.x = (x - map_resp_.map.info.origin.position.x)/ map_resp_.map.info.resolution;
  pt.y = (y - map_resp_.map.info.origin.position.y)/ map_resp_.map.info.resolution;
  return pt;
}

MapServerControl::MapServerControl()
{
  ROS_INFO("MapServerControl::MapServerControl");
  service = n.advertiseService("map_server_control", &MapServerControl::cmdCallback, this);
  map_server = NULL;
}

MapServerControl::~MapServerControl()
{
  delete map_server;
  map_server = NULL;
}

bool MapServerControl::cmdCallback(lsrobot::cmd::Request &req, lsrobot::cmd::Response &res)
{
  ROS_INFO("MapServerControl::cmdCallback cmd = %d", req.cmd);
  if (req.cmd == 1)
  {
    // stop
    if (map_server != NULL)
    {
      delete map_server;
      map_server = NULL;
      ROS_INFO("delet map_server");
    }

    // start
    //    if (map_server == NULL)
    //    {
    map_server = new MapServer(req.str, 0.0);
    ROS_ASSERT(map_server);
    res.result = map_server->loadMap();
    if (res.result != 0)
    {
      // load map error
      ROS_ERROR("loadMap error");
      delete map_server;
      map_server = NULL;
    }
    ROS_INFO("cmd = 1,done");
    //    }
  }
  else if (req.cmd == 2)
  {
    // stop
    if (map_server != NULL)
    {
      delete map_server;
      map_server = NULL;
      ROS_INFO("cmd = 2,done");
    }
  }
  else
  {
    ROS_WARN("cmd error!");
  }

  ROS_INFO("MapServerControl::cmdCallback res = %d", res.result);
  return true;
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "map_server", ros::init_options::AnonymousName);

  try
  {
    MapServerControl msc;
    ros::spin();
  }
  catch(std::runtime_error& e)
  {
    ROS_ERROR("map_server exception: %s", e.what());
    return -1;
  }

  return 0;
}
