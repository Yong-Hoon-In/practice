#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
void drawHistogram(Mat &image, Mat hist);
int main()
{
	Mat srcImg = imread("15.jpg");
	if (srcImg.empty())
		return -1;

						Mat srcImg1(512, 512, CV_8U);
						Mat srcImg2(512, 512, CV_8U);
						Scalar mean1 = Scalar(128);
						Scalar stddev1 = Scalar(10);
						RNG &rng1 = theRNG();
						rng1.fill(srcImg1, RNG::NORMAL, mean1, stddev1);
						Scalar mean2 = Scalar(118);
						Scalar stddev2 = Scalar(10);
						RNG &rng2 = theRNG();
						rng2.fill(srcImg2, RNG::NORMAL, mean2, stddev2);
						int histSize = 256;
						float valueRange[] = { 0,256 };
						const float* ranges[] = { valueRange };
						int channels = 0;
						int dims = 1;
						Mat H1;
						calcHist(&srcImg, 1, &channels, Mat(), H1, dims, &histSize, ranges);		//정규화,히스토그램분포
						normalize(H1, H1, 1, 0, NORM_L1, CV_32F);
						Mat H2;
						calcHist(&srcImg2, 1, &channels, Mat(), H2, dims, &histSize, ranges);		//정규화,히스토그램분포
						normalize(H2, H2, 1, 0, NORM_L1, CV_32F);
						Mat S1(H1.rows, 2, CV_32F);
						Mat S2(H2.rows, 2, CV_32F);
						for (int i = 0; i < H1.rows; i++) {
							S1.at<float>(i, 0) = H1.at<float>(i);
							S1.at<float>(i, 1) = i + 1;
						}
						for (int i = 0; i < H2.rows; i++) {
							S2.at<float>(i, 0) = H2.at<float>(i);
							S2.at<float>(i, 1) = i + 1;
						}
						float emdDist = EMD(S1, S2, DIST_L2);
						cout << "emdDist" << emdDist << endl;
						Mat histImg1;
						drawHistogram(histImg1, H1);
						imshow("histImg1", histImg1);
						Mat histImg2;
						drawHistogram(histImg2, H2);

						imshow("histImg2", histImg2);
						waitKey();
						


	return 0;
}

void drawHistogram(Mat &image, Mat hist) {
	if (image.empty())
		image.create(512, 512, CV_8U);
	normalize(hist, hist, 0, image.rows, NORM_MINMAX, CV_32F);
	image = Scalar(255);
	int binW = cvRound((double)image.cols / hist.rows);
	int x1, y1, x2, y2;
	for (int i = 0; i < hist.rows; i++) {
		x1 = i * binW;
		y1 = image.rows;
		x2 = (i + 1)*binW;
		y2 = image.rows - cvRound(hist.at<float>(i));
		rectangle(image, Point(x1, y1), Point(x2, y2), Scalar(0), -1);
	}
}
