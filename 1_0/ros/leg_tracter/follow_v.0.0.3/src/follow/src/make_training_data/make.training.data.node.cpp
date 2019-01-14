#include "make.training.data.h"

int main(int argc, char** argv){
    ros::init(argc, argv, "make_training_data");
    MakeTrainingData mtd;
    mtd.making();
}