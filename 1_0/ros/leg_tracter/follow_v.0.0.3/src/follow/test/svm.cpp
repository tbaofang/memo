#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc.hpp"

using namespace std;

int main(int argc, char** argv){
    int width = 512, height = 512;
    cv::Mat image = cv::Mat::zeros(width, height, CV_8UC3);
//
//        cout << image.size << endl;
//        cout << image.dims << endl;
//        cout << image.at<int>(10,10) << endl;
//        cout << image.channels() << endl;
//        cout << image.depth() << endl;
//        cout << image.total() << endl;

    //标签
//    float labels[4] = { 1.0,-1.0,-1.0,-1.0 };
//    cv::Mat labelsMat(4, 1, CV_32FC1, labels);

    int labels[5] = { 1, 1, -1, -1, -1 };
    cv::Mat labelsMat(5, 1, CV_32SC1, labels);

    //设置训练数据
    float trainingData[5][2] = {{500, 200}, {301,10},{255,10},{501,255},{10,501} };
    cv::Mat trainingDataMat(5, 2, CV_32FC1, trainingData);


    // initial SVM
    cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
    svm->setType(cv::ml::SVM::Types::C_SVC);
    svm->setKernel(cv::ml::SVM::KernelTypes::LINEAR);
    svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 100, 1e-6));

    // train operation
    svm->train(trainingDataMat, cv::ml::SampleTypes::ROW_SAMPLE, labelsMat);

    // prediction
    cv::Vec3b green(0, 255, 0);
    cv::Vec3b blue(255, 0, 0);
    for (int i = 0; i < image.rows; i++)
    {
        for (int j = 0; j < image.cols; j++)
        {
            cv::Mat sampleMat = (cv::Mat_<float>(1, 2) << j, i);
            float respose = svm->predict(sampleMat);
            if (respose == 1)
                image.at<cv::Vec3b>(i, j) = green;
            else if (respose == -1)
                image.at<cv::Vec3b>(i, j) = blue;
        }
    }



    int thickness = -1;
    int lineType = cv::LineTypes::LINE_8;
    cout << "line Type: " << lineType << endl;
    cv::circle(image, cv::Point(500, 200), 5, cv::Scalar(0, 0, 0), thickness, lineType);
    cv::circle(image, cv::Point(301, 10), 5, cv::Scalar(0, 0, 0), thickness, lineType);
    cv::circle(image, cv::Point(255, 10), 5, cv::Scalar(255, 255, 255), thickness, lineType);
    cv::circle(image, cv::Point(501, 255), 5, cv::Scalar(255, 255, 255), thickness, lineType);
    cv::circle(image, cv::Point(10, 501), 5, cv::Scalar(255, 255, 255), thickness, lineType);

    thickness = 2;
    lineType = cv::LineTypes::LINE_8;

    cv::Mat sv = svm->getSupportVectors();
    cout << "sv rows: " << sv.rows << endl;
    for (int i = 0; i < sv.rows; i++)
    {
        const float* v = sv.ptr<float>(i);
        cout << "v[0]: " << v[0] << endl;
        cout << "v[1]: " << v[1] << endl;
        cv::circle(image, cv::Point((int)v[0], (int)v[1]), 6, cv::Scalar(0, 0, 0), thickness, lineType);
    }


    cv::imshow("SVM Simple Example", image);


    cv::waitKey(0);
    return 0;


}