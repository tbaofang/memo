#include "local.mapping.h"

int main(int argc, char** argv){
    ros::init(argc, argv, "local_mapping");
    ros::NodeHandle nh;

    LocalMapping lm;

    return 0;
}