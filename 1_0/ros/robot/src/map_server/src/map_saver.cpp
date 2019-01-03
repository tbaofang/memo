/*
 * map_saver
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
 *     * Neither the name of the <ORGANIZATION> nor the names of its
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

#include "map_server/map_saver.h"

namespace map_server
{
using namespace std;
 
/**
 * @brief Map generation node.
 */

    MapSaver::MapSaver(const std::string& mapname) : mapname_(mapname), saved_map_(false)
    {
      ros::NodeHandle n;
      ROS_INFO("Waiting for the map");
      map_sub_ = n.subscribe("map", 1, &map_server::MapSaver::mapCallback, this);
    }

    void MapSaver::mapCallback(const nav_msgs::OccupancyGridConstPtr& map)
    {
      ROS_INFO("Received a %d X %d map @ %.3f m/pix",
               map->info.width,
               map->info.height,
               map->info.resolution);


      std::string mapdatafile = mapname_ + ".pgm";
      ROS_INFO("Writing map occupancy data to %s", mapdatafile.c_str());
      FILE* out = fopen(mapdatafile.c_str(), "w");
      if (!out)
      {
        ROS_ERROR("Couldn't save map file to %s", mapdatafile.c_str());
        return;
      }

      fprintf(out, "P5\n# CREATOR: Map_generator.cpp %.3f m/pix\n%d %d\n255\n",
              map->info.resolution, map->info.width, map->info.height);
      for(unsigned int y = 0; y < map->info.height; y++) {
        for(unsigned int x = 0; x < map->info.width; x++) {
          unsigned int i = x + (map->info.height - y - 1) * map->info.width;
          if (map->data[i] == 0) { //occ [0,0.1)
            fputc(254, out);
          } else if (map->data[i] == +100) { //occ (0.65,1]
            fputc(000, out);
          } else { //occ [0.1,0.65]
            fputc(205, out);
          }
        }
      }

      fclose(out);


      std::string mapmetadatafile = mapname_ + ".yaml";
      ROS_INFO("Writing map occupancy data to %s", mapmetadatafile.c_str());
      FILE* yaml = fopen(mapmetadatafile.c_str(), "w");


      /*
resolution: 0.100000
origin: [0.000000, 0.000000, 0.000000]
#
negate: 0
occupied_thresh: 0.65
free_thresh: 0.196

       */

      geometry_msgs::Quaternion orientation = map->info.origin.orientation;
      tf::Matrix3x3 mat(tf::Quaternion(orientation.x, orientation.y, orientation.z, orientation.w));
      double yaw, pitch, roll;
      mat.getEulerYPR(yaw, pitch, roll);

      fprintf(yaml, "image: %s\nresolution: %f\norigin: [%f, %f, %f]\nnegate: 0\noccupied_thresh: 0.65\nfree_thresh: 0.196\n\n",
              mapdatafile.c_str(), map->info.resolution, map->info.origin.position.x, map->info.origin.position.y, yaw);

      fclose(yaml);

      ROS_INFO("Done\n");
      saved_map_ = true;
    }

    MapSaverServer::MapSaverServer()
    {
      ROS_INFO("MapSaverServer::MapSaverServer");
      service = n.advertiseService("saver_control", &MapSaverServer::cmdCallback, this);
      map_saver = NULL;
    }

    MapSaverServer::~MapSaverServer()
    {
      delete map_saver;
      map_saver = NULL;
    }

    bool MapSaverServer::cmdCallback(lsrobot::cmd::Request &req, lsrobot::cmd::Response &res)
    {
      // ROS_INFO("MapSaverServer::cmdCallback cmd = %d %s", req.cmd, req.str.c_str());
      ROS_INFO("saver callback is entered!");
      ROS_INFO("MapSaverServer::cmdCallback cmd = %lld", req.cmd);
      ROS_INFO("filename to save is %s", req.str.c_str());

      if (req.cmd == 1)
      {
        // start
        if (map_saver == NULL)
        {
          map_saver = new MapSaver(req.str);
          ROS_ASSERT(map_saver);
        }
        else
        {
          map_saver->mapname_ = req.str;
          map_saver->saved_map_ = false;
        }

        ROS_INFO("saved_map = %d", map_saver->saved_map_);

        while(!map_saver->saved_map_ && ros::ok())
        {
          ros::spinOnce();
          usleep(100000);
        }

        ROS_INFO("cmd = 1,done");

      }
      else if (req.cmd == 2)
      {
        // stop
        if (map_saver != NULL)
        {
          delete map_saver;
          map_saver = NULL;
          ROS_INFO("cmd = 2,done");
        }
      }
      else
      {
        ROS_WARN("cmd error!");
      }

      ROS_INFO("MapSaverServer::cmdCallback res = %lld", res.result);
      return true;
    }
}

#define USAGE "Usage: \n" \
              "  map_saver -h\n"\
              "  map_saver [-f <mapname>] [ROS remapping args]"

int main(int argc, char** argv)
{
  ros::init(argc, argv, "map_saver_server");
  map_server::MapSaverServer mss;
  ros::spin();

  return 0;
}

