#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include<bits/stdc++.h>
using namespace std;
using namespace cv;
int Isvalid(int x,int y,Mat *img){
	if(x<0||x>=img->rows||y<0||y>img->cols)
		return 0;
	if(img->at<uchar>(x,y)==0)
		return 0;		
	return 1;
}
int main(){
	Mat img=imread("rrt.png",0);
	Mat a(img.rows,img.cols,CV_8UC1,Scalar(0));
	a=img.clone();
	float distance[img.rows][img.cols];
	int i,j;
	int visited[img.rows][img.cols];
	for(i=0;i<img.rows;i++){
		for(j=0;j<img.cols;j++){
			distance[i][j]=INT_MAX;
			visited[i][j]=0;
		}
	}
	int parent[img.rows][img.cols];
	for(i=0;i<img.rows;i++){
		for(j=0;j<img.cols;j++){
			parent[i][j]=i*(img.cols)+j;
		}
	}
	int k;
	int m=img.rows;
	int n=img.cols;
	distance[0][0]=0;
	for(i=0;i<m*n;i++){
		float min=INT_MAX;
		int min_index;
		for(j=0;j<img.rows;j++){
			for(k=0;k<img.cols;k++){
				if(!visited[j][k])
				if(distance[j][k]<min){
					min=distance[j][k];
					min_index=j*(img.cols)+k;
				}
			}
		}
		int x=min_index/(img.cols),y=min_index%(img.cols);
		distance[x][y]=min;
		visited[x][y]=1;
		a.at<uchar>(min_index/(img.cols),min_index%(img.cols))=0;
		namedWindow("window1",WINDOW_NORMAL);
		imshow("window1",a);
		waitKey(1);
		if(visited[x][y+1]==0 && Isvalid(x,y+1,&img)){
			if(distance[x][y]+1<distance[x][y+1]){
				distance[x][y+1]=distance[x][y]+1;
				parent[x][y+1]=min_index;
			}
		}
		if(visited[x+1][y]==0 && Isvalid(x+1,y,&img)){
			if(distance[x][y]+1<distance[x+1][y]){
				distance[x+1][y]=distance[x][y]+1;
				parent[x+1][y]=min_index;
			}
		}
		if(visited[x][y-1]==0 && Isvalid(x,y-1,&img)){
			if(distance[x][y]+1<distance[x][y-1]){
				distance[x][y-1]=distance[x][y]+1;
				parent[x][y-1]=min_index;
			}
		}
		if(visited[x-1][y]==0 && Isvalid(x-1,y,&img)){
			if(distance[x][y]+1<distance[x-1][y]){
				distance[x-1][y]=distance[x][y]+1;
				parent[x-1][y]=min_index;
			}
		}
		if(visited[x+1][y+1]==0 && Isvalid(x+1,y+1,&img)){
			if(distance[x][y]+1.414<distance[x+1][y+1]){
				distance[x+1][y+1]=distance[x][y]+1.414;
				parent[x+1][y+1]=min_index;
			}
		}
		if(visited[x-1][y+1]==0 && Isvalid(x-1,y+1,&img)){
			if(distance[x][y]+1.414<distance[x-1][y+1]){
				distance[x-1][y+1]=distance[x][y]+1.414;
				parent[x-1][y+1]=min_index;
			}
		}
		if(visited[x+1][y-1]==0 && Isvalid(x+1,y-1,&img)){
			if(distance[x][y]+1.414<distance[x+1][y-1]){
				distance[x+1][y-1]=distance[x][y]+1.414;
				parent[x+1][y-1]=min_index;
			}
		}
		if(visited[x-1][y-1]==0 && Isvalid(x-1,y-1,&img)){
			if(distance[x][y]+1.414<distance[x-1][y-1]){
				distance[x-1][y-1]=distance[x][y]+1.414;
				parent[x-1][y-1]=min_index;
			}
		}
	}
	cout<<distance[289][289];
	int current=289*(img.cols)+289;
	while(Isvalid(current/img.cols,current%img.cols,&img)==1){
		img.at<uchar>(current/(img.cols),current%(img.cols))=0;
		current=parent[current/(img.cols)][current%(img.cols)];
	}
	namedWindow("window",WINDOW_NORMAL);
	imshow("window",img);
	waitKey(0);
	return 0;
}