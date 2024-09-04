#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>
#include <vector>
#include <ctime>
#include <atomic>
#include <thread>

using namespace cv;
using namespace std;


string get_current_time() {
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);
    string current_time = to_string(1900 + ltm->tm_year) + "-" + to_string(1 + ltm->tm_mon) + "-" +
                          to_string(ltm->tm_mday) + "_" + to_string(ltm->tm_hour) + "-" +
                          to_string(ltm->tm_min) + "-" + to_string(ltm->tm_sec);
    return current_time;
}

int main(int argc, char **argv) {
    string input_path = "./";
    vector<Mat> images;

    const time_t start_program = time(nullptr);

    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <Input_Path>\n";
        return -1;
    } else {
        input_path = input_path + argv[1];
    }


    try {
        auto files = filesystem::directory_iterator(input_path);
        for (const auto &entry: files) {
            images.push_back(imread(entry.path()));
            cout << entry.path() << endl;
        }
    } catch (const filesystem::filesystem_error &e) {
        cerr << "이미지 로드 중에 오류가 발생했습니다 : " << e.what() << endl;
        cerr << "Error: " << e.what() << endl;
    }

    const time_t start_stitch = time(nullptr);

    double image_load_elapsed_time = difftime(start_stitch, start_program);
    cout << "이미지 로드 완료 [" << images.size() << "]" << endl;
    cout << "이미지 로드 소요 시간 : " << image_load_elapsed_time << "s" << endl;

    cout << "Image is loaded [" << images.size() << "]" << endl;
    cout << "Elapsed time : " << image_load_elapsed_time << "s" << endl;


    if (images.empty()) {
        cerr << "이미지가 로드되지 않았습니다." << endl;
        cerr << "images are not loaded." << endl;
        return -1;
    }

    string output_name = "pano_" + get_current_time() + ".jpg";

    if (argc != 3) {
        cout << "출력 파일 이름이 제공되지 않았습니다. 기본 이름 '" << output_name << "'를 사용합니다.\n";
        cout << "Output file name is not provided. Using default name '" << output_name << "'\n";
    } else {
        output_name = string(argv[2]) + "jpg";
        cout << "출력 파일 이름 : " << argv[2] << endl;
        cout << "Output file name : " << argv[2] << endl;
    }

    try {
        cout << "이미지 정합 시작, 이 작업은 많은 시간이 걸립니다." << endl;
        cout << "Stitching images, this process takes a lot of time." << endl;
        Mat stitched;
        Ptr<Stitcher> stitcher = Stitcher::create(Stitcher::SCANS);

        Ptr<cv::Feature2D> feature_finder = cv::ORB::create();
        stitcher->setFeaturesFinder(feature_finder);

        Stitcher::Status status = stitcher->stitch(images, stitched);

        cout << "Stitch elapsed time : " << difftime(time(nullptr), start_stitch) << endl;
        imwrite(output_name, stitched);
        cout << "이미지 정합 완료" << endl;
    } catch (const Exception &e) {
        cerr << "정합 도중에 오류가 발생했습니다 : " << e.what() << endl;
        cerr << "Error: " << e.what() << endl;
    }

    // vector<MAT> images;

    // Ptr<Stitcher> stitcher = Stitcher::create(Stitcher::SCAN, true);
    // Stitcher::Status status = stitcher->stitch(images, pano);

    // // 이미지 파일 경로에서 이미지 읽기
    // cv::Mat image = cv::imread(argv[1], cv::IMREAD_COLOR);

    // // 이미지가 제대로 로드되지 않았을 경우
    // if (image.empty()) {
    //     cout << "Could not open or find the image\n";
    //     return -1;
    // }

    // // 이미지 창 만들기
    // cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE);

    // // 이미지 표시
    // cv::imshow("Display Image", image);

    // // 키 입력 대기
    // cv::waitKey(0);

    return 0;
}
