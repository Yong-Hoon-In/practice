#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;
void drawHistogram(Mat &image, Mat hist);
int main()
{
	Mat srcImg = imread("15.jpg");
	if (srcImg.empty())
		return -1;
/*	Mat_<uchar>image(srcImg);
	Mat_<uchar>dstImg(srcImg.size());
	for (int y = 0; y < image.rows; y++) {
		for (int x = 0; x < image.cols; x++) {		//반전영상
			uchar r = image(y,x);
			dstImg(y, x) = 255 - r;
		}
	}
	imshow("dstImg", srcImg);
	waitKey();
	return 0;
*/
/*	Mat_ <uchar> lut(1, 256);
	for (int i = 0; i < 256; i++)
		lut(i) = 256 - i;
	Mat dstImg;				//LUT함수로 반전영상
	LUT(srcImg, lut, dstImg);
	imshow("dstImg", dstImg);
	waitKey();
	*/
/*	Mat dstImg1;
	double th1 = threshold(srcImg, dstImg1, 100, 255, THRESH_BINARY);
	cout << "th1 =" << th1 << endl;
	Mat dstImg2;
	double th2 = threshold(srcImg, dstImg2, 100, 255, THRESH_BINARY + THRESH_OTSU);		//threshold
	cout << "th2=" << th2 << endl;
	imshow("dstImg1", dstImg1);
	imshow("dstImg2", dstImg2);
	waitKey();
	*/
/*	Mat dstImg1;
	adaptiveThreshold(srcImg, dstImg1, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 21, 5);
	Mat dstImg2;
	adaptiveThreshold(srcImg, dstImg2, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 21, 5);//이미지 임계처리
	imshow("dstImg1", dstImg1);
	imshow("dstImg2", dstImg2);
	waitKey();
	*/
/*	double minVal, maxVal;
	Point minLoc, maxLoc;
	minMaxLoc(srcImg, &minVal, &maxVal, &minLoc, &maxLoc);
	cout << "In srcImg" << endl;
	cout << "minVal" << minVal << endl;
	cout << "maxVal" << maxVal << endl;
	cout << "minLoc" << minLoc << endl;
	cout << "maxLoc" << maxLoc << endl;
	Mat dstImg;
	double scale = 100.0;
	srcImg.convertTo(dstImg, -1, scale, -scale * minVal);			//선형변환
	minMaxLoc(dstImg, &minVal, &maxVal, &minLoc, &maxLoc);
	cout << "In srcImg" << endl;
	cout << "minVal" << minVal << endl;
	cout << "maxVal" << maxVal << endl;
	cout << "minLoc" << minLoc << endl;
	cout << "maxLoc" << maxLoc << endl;
	imshow("dstImg", dstImg);
	waitKey();
	*/
/*	Mat srcImg1(512, 512, CV_8U);
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
	calcHist(&srcImg1, 1, &channels, Mat(), H1, dims, &histSize, ranges);
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
	waitKey();*/
/*		Mat hsvImg;
		cvtColor(srcImg, hsvImg, COLOR_BGR2HSV);
		vector<Mat> planes;
		split(hsvImg, planes);
		Mat hueImg = planes[0];
		Rect roi(100, 100, 100, 100);
		rectangle(srcImg, roi, Scalar(0, 0, 255), 2);
		Mat roiImg = hueImg(roi);
		int histSize = 256;
		float hValue[] = { 0,256 };
		const float* ranges[] = { hValue };
		int channels = 0;
		int dims = 1;
		Mat hist;
		calcHist(&roiImg, 1, &channels, Mat(), hist, dims, &histSize, ranges);		//역투영,관심영역히스토리
		Mat hueImg2;
		hueImg.convertTo(hueImg2, CV_32F);
		Mat backProject;
		calcBackProject(&hueImg2, 1, &channels, hist, backProject, ranges);
		double minVal , maxVal;
		minMaxLoc(backProject, &minVal, &maxVal);
		cout << minVal << endl;
		cout << maxVal << endl;
		Mat backProject2;
		normalize(backProject, backProject2, 0, 255, NORM_MINMAX, CV_8U);
		imshow("backPt2", backProject2);
		imshow("src", srcImg);
		waitKey();*/
	Mat grayImg;
	cvtColor(srcImg, grayImg, COLOR_BGR2GRAY);
	imshow("gray", grayImg);
	Mat hsvImg;
	cvtColor(srcImg, hsvImg, COLOR_BGR2HSV);
	imshow("hsv", hsvImg);
	Mat yCbcrImg;
	cvtColor(srcImg, yCbcrImg, COLOR_BGR2YCrCb);
	imshow("ycb", yCbcrImg);
	Mat luvImg;
	cvtColor(srcImg, luvImg, COLOR_BGR2Luv);
	imshow("luv", luvImg);
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
