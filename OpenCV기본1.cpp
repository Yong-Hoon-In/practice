#include <opencv2/opencv.hpp>
#include <fstream>
using namespace cv;
using namespace std;
void onMouse(int event, int x, int y, int flags, void* param);
void OnChange(int pos, void *param);
int main()
{
	Mat srcImg=imread("result.jpg",IMREAD_GRAYSCALE);
	if (srcImg.empty())
		return -1;
	Mat srcImg1 = imread("ze.jpg", -1);
	if (srcImg1.empty())
		return -1;
	Mat dstImg(512, 512, CV_8UC3, Scalar(255, 255, 255));
/*	rectangle(dstImg, Point(100, 100), Point(400, 400), Scalar(0, 0, 255), 2);
	line(dstImg, Point(400, 100), Point(100, 400), Scalar(0, 255, 0));
	line(dstImg, Point(400, 100), Point(100, 400), Scalar(0, 255, 0),2, 8, 1);  //사각형,라인 그리기
	rectangle(dstImg, Point(400 / 2, 100 / 2), Point(100 / 2, 400 / 2), Scalar(255, 0, 0));
	imshow("dstimg", dstImg);*/
/*	Rect imgRect(100, 100, 300, 300);
	rectangle(dstImg, imgRect.tl(), imgRect.br(), Scalar(255, 0, 0), 2);
	Point pt1(120, 50), pt2(300, 500);
	line(dstImg, pt1, pt2, Scalar(0, 255, 0), 2);
	clipLine(imgRect,pt1,pt2);
	cout << "pt1=" << pt1 << endl;
	cout << "pt2=" << pt2 << endl;
	circle(dstImg, pt1, 5, Scalar(0, 0, 255), 2);
	circle(dstImg, pt2, 5, Scalar(0, 0, 255), 2);
	line(dstImg, pt1, pt2, Scalar(0, 0, 255), 2);*/
/*	Point pt1(10, 10 ), pt2(400, 400);
	LineIterator it(srcImg, pt1, pt2, 8);
	vector<uchar> buffer(it.count);
	for (int i = 0; i < it.count; i++, ++it) {
		buffer[i] = **it;
	}
	cout << (Mat)buffer << endl;
	line(srcImg, pt1, pt2, Scalar(255), 2);               //소스이미지의 화소값을 데스티네이션 이미지로 프로파일 하기
	pt1 = Point(0, dstImg.cols - buffer[0]);
	for (int i = 1; i < buffer.size(); i++, ++it) {
		pt2 = Point(i, dstImg.cols - buffer[i]);
		line(dstImg, pt1, pt2, Scalar(255), 2);
		pt1 = pt2;
	}
	*/
/*	rectangle(dstImg, Point(100, 100), Point(400, 400), Scalar(0, 0, 255));
	line(dstImg, Point(250, 100), Point(250, 400), Scalar(0, 0, 255));
	line(dstImg, Point(100,250), Point(400,250), Scalar(0, 0, 255));	//원그리기/라인그리기
	circle(dstImg, Point(250, 250), 150, Scalar(255, 0, 0));
	circle(dstImg, Point(250, 250 ),50, Scalar(0, 255, 0), -1);
	*/
/*	Point center(250,200);
	Size size(200, 100);
	rectangle(dstImg, Point(center.x-size.width, center.y - size.height), Point(center.x + size.width, center.y + size.height), Scalar(255, 0, 0));
	line(dstImg, Point(center.x - size.width, center.y), Point(center.x + size.width, center.y), Scalar(0, 255, 0));
	line(dstImg, Point(center.x, center.y-size.height), Point(center.x, center.y + size.height), Scalar(0, 255, 0));

	ellipse(dstImg, center, size, 0, 0, 360, Scalar(0, 0, 255));
	ellipse(dstImg, center, size, 90, 45, 360, Scalar(0, 0, 250), 2);
	RotatedRect box(center, size, 90);
	ellipse(dstImg, box, Scalar(255, 0, 0), 2);
	vector<Point> pts;
	ellipse2Poly(center, size, 90, 0, 360, 45, pts);
	polylines(dstImg, pts, true, Scalar(0, 255, 0), 4);				//타원그리기
	Point pt1,pt2;
	for (int i = 0; i < pts.size(); i++) {
		pt1 = pts[i];
		if (i == pts.size()-1)
			pt2 = pts[0];
		else
			pt2 = pts[i + 1];
		line(dstImg, pt1, pt2, Scalar(0, 0, 255));
	}
	*/
/*	Point pts1[4] = { Point(100,100),Point(200,100),Point(200,200),Point(100,200) };
	Point pts2[3] = { Point(300,200),Point(400,100),Point(400,200)}; 
	const Point *polygons[2] = { pts1,pts2 };			//다각형그리기
	int npts[2] = { 4,3 };
	polylines(dstImg, polygons, npts, 2, true, Scalar(255, 0, 0));
	*/
/*	string text = "OpenCV Programming";
	int fontFace = FONT_HERSHEY_COMPLEX;
	double fontScale = 1.0;
	int thickness = 1;
	int baseline;
	Point org(100, 100);
	putText(dstImg, text, org, fontFace, fontScale, Scalar(0, 0, 0));
	Size size= getTextSize(text, fontFace, fontScale, thickness, &baseline);		//문자열 출력
	rectangle(dstImg, org, Point(org.x + size.width, org.y - size.height), Scalar(0, 0, 255));
	circle(dstImg, org, 3, Scalar(255, 0, 0), 2);
	*/
/*	namedWindow("srcImg");
	
	int x = 100, y = 100;
	moveWindow("srcImg", x, y);
	int nKey;
	while (1) {
		nKey = waitKey(0);
	//	printf("%x\n", nKey);
		if (nKey == 0x1B)
			break;
		switch (nKey) {
		case 0x250000:
			x -= 10;			키보드 입력받기
			break;
		case 0x270000:
			x += 10;
		case 0x260000:
			y -= 10;
			break;
		case 0x280000:
			y += 10;
			break;
		}
		moveWindow("srcImg", x, y);
	}
	*/
/*	Mat image[2];
	image[0] = imread("ze.jpg", IMREAD_GRAYSCALE);
	if (image[0].empty())
		return -1;
	image[1].create(image[0].size(), CV_8U);		//트랙바로 그레이스케일 조절
	namedWindow("image");
	imshow("image", image[0]);
	int pos = 128;
	OnChange(pos, (void*)image);
	createTrackbar("threshold", "image", &pos, 255, OnChange, (void*)image);
	*/
//	imshow("dstImg", dstImg);
//	imshow("srcImg", srcImg);
//	setMouseCallback("dstImg", onMouse, (void*)&dstImg);	//키보드 입력 처리
/*	cout << "srcImg.type()=" << srcImg1.type() << endl;
	cout << "srcImg.depth()=" << srcImg1.depth() << endl;
	cout << "channel()" << srcImg1.channels() << endl;
	imwrite("lena.bmp", srcImg);		//imread,imwrite 활용하기
	vector<int> params;
	params.push_back(IMWRITE_JPEG_QUALITY);
	params.push_back(100);
	imshow("srcImg", srcImg);
	imshow("srcImg1", srcImg1);
	*/
	Mat srcImg2(512, 512, CV_8UC3, Scalar::all(255));
	rectangle(srcImg, Point(100, 100), Point(400, 400), Scalar(255, 0, 0), -1);
	vector<uchar> buf1;
	imencode(".bmp", srcImg, buf1);
	Mat dstImg1 = imdecode(buf1, IMREAD_COLOR);

	imshow("dstImg1", dstImg1);
	ofstream outfile1("test.bmp", ios::binary);
	outfile1.write((const char*)buf1.data(),buf1.size());
	outfile1.close();
	vector<int> params;
	params.push_back(IMWRITE_JPEG_QUALITY);
	params.push_back(90);

	vector<uchar> buf2;
	imencode(".jpg", srcImg2, buf2, params);

	Mat dstImg2 = imdecode(buf2, IMREAD_COLOR);			//imdecode,imencode
	imshow("dstImg", dstImg2);

	ofstream outfile2("test.jpg", ios::binary);
	outfile2.write((const char*)buf2.data(),buf2.size());
	outfile2.close();
	params.clear();
	params.push_back(IMWRITE_PNG_COMPRESSION);
	params.push_back(9);

	vector<uchar> buf3;
	imencode(".png", srcImg2, buf3, params);
	Mat dstImg3 = imdecode(buf3, IMREAD_COLOR);
	imshow("dstImg3",dstImg3);
	ofstream outfile3("test.png", ios::binary);
	outfile3.write((const char*)buf3.data(), buf3.size());
	outfile3.close();
	waitKey();




	return 0;
}
void onMouse(int event, int x, int y, int flags, void* param) {
	Mat *pMat = (Mat*)param;
	Mat image = Mat(*pMat);
	switch (event) {
	case EVENT_LBUTTONDOWN:
		if (flags&EVENT_FLAG_SHIFTKEY)
			rectangle(image, Point(x - 5, y - 5), Point(x + 5, y + 5), Scalar(255, 0, 0));
		else
			circle(image, Point(x, y), 5, Scalar(0,0,255), 5);
			break;
	case EVENT_RBUTTONDBLCLK:
		circle(image, Point(x, y), 5, Scalar(255, 0, 0), 5);
		break;
	case EVENT_LBUTTONDBLCLK:
		image = Scalar(255, 255, 255);
		break;
	}
	imshow("dstImg", image);
}
void OnChange(int pos, void *param) {
	Mat *pMat = (Mat*)param;
	Mat srcImg = Mat(pMat[0]);
	Mat dstImg = Mat(pMat[1]);
	int x, y, s, r;
	int nThreshold = pos;
	for (y = 0; y < srcImg.rows; y++)
		for (x = 0; x < srcImg.cols; x++) {
			r = srcImg.at<uchar>(y, x);
			if (r > nThreshold)
				s = 255;
			else
				s = 0;
			dstImg.at<uchar>(y, x) = s;
		}
	imshow("image", dstImg);
}
