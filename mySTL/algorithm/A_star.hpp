//https://blog.csdn.net/u012234115/article/details/47152137
// https://www.cnblogs.com/technology/archive/2011/05/26/2058842.html
#pragma once

#include <vector>
#include <list>
#include <math>

struct Point{

    Point(int __x, int __y, int __F=0, int __G=0, int __H=0):x(__x),y(__y),F(__F),G(__G),H(__H){}
    int x,y;//点坐标
    int F,G,H;//损失函数
    Point* parent;

};

class Astar{
public:
    void InitAstar(std::vector<std::vector<int> > &_maze){maze=_maze;};
    std::list<Point *> GetPath(Point &startPoint, Point &endPoint, bool isIgnoreCorner);
    bool isCanreach(const Point *point,const Point *target,bool isIgnoreCorner) const; //判断某点是否可以用于下一步判断
	Point *isInList(const std::list<Point *> &list,const Point *point) const; //判断开启/关闭列表中是否包含某点
	Point *getLeastFpoint(); //从开启列表中返回F值最小的节点
    //计算FGH值
	int calcG(Point *temp_start,Point *point);
	int calcH(Point *point,Point *end);
	int calcF(Point *point);

private:
	std::vector<std::vector<int>> maze;
	std::list<Point *> openList;  //开启列表
	std::list<Point *> closeList; //关闭列表

};

// 曼哈顿距离
int Astar::calcG(Point *temp_start,Point *point){
   int cost;
    int dx = abs(point->x - temp->x);
    int dy = abs(point->y - temp->y);
    cost = (dx + dy) + (sqrt(2) -1)* min(dx,dy);
}


// 欧几里得距离
int calcH(Point *point,Point *end){
    int cost;
    int dx = abs(point->x - temp->x);
    int dy = abs(point->y - temp->y);
    cost = sqrt(dx*dx + dy*dy);
}

