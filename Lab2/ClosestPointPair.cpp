#include <fstream>
#include <iostream>

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <ctime>

#include <string>
#include <tuple>
#include <utility>
#include <vector>

using namespace std;

typedef tuple<unsigned, double, double> Point;
typedef tuple<Point, Point, double>		PointPair;

#define serial 0
#define loc_X 1
#define loc_Y 2

#define _p1 0
#define _p2 1
#define _dist 2

class ClosestPair {
public:
	ClosestPair(string data_path);
	static double Distance(Point &P1, Point &P2);
	PointPair	  FindClosestPair();
	PointPair	  NormalFindClosestPair();

private:
	void	  PreSort();
	PointPair ClosestPointPair(vector<Point> &Point_X_Sort,
							   vector<Point> &Point_Y_Sort, unsigned p,
							   unsigned r);
	PointPair Merge(vector<Point> &Points);

private:
	static bool CompLocX(const Point &P1, const Point &P2);
	static bool CompLocY(const Point &P1, const Point &P2);

private:
	vector<Point> Points;
	unsigned	  PointSize;
	vector<Point> Points_X_Sort;
	vector<Point> Points_Y_Sort;
};

ClosestPair::ClosestPair(string data_path) {
	ifstream f;
	double	 _loc_X, _loc_Y;
	unsigned _serial;
	// load data into vector
	f.open(data_path, ios::in);
	if (f.is_open()) {
		while (!f.eof()) {
			f >> _serial >> _loc_X >> _loc_Y;
			// cout << _serial << ' ' << _loc_X << ' ' << _loc_Y << endl;
			Points.push_back(make_tuple(_serial, _loc_X, _loc_Y));
		}
	}
	// Points.pop_back();
	PointSize = Points.size();
	f.close();
}

double ClosestPair::Distance(Point &P1, Point &P2) {
	double DistPowX = pow(get<loc_X>(P1) - get<loc_X>(P2), 2);
	double DistPowY = pow(get<loc_Y>(P1) - get<loc_Y>(P2), 2);
	return sqrt(DistPowX + DistPowY);
}

PointPair ClosestPair::FindClosestPair() {
	PreSort();
	// base on the sorted array X and array Y, find the closest point pair
	PointPair closest_pair =
		ClosestPointPair(Points_X_Sort, Points_Y_Sort, 0, PointSize);
	if (get<serial>(get<_p1>(closest_pair)) >
		get<serial>(get<_p2>(closest_pair)))
		return make_tuple(get<_p2>(closest_pair), get<_p1>(closest_pair),
						  get<_dist>(closest_pair));
	else
		return closest_pair;
}

PointPair ClosestPair::NormalFindClosestPair() {
	Point  point1		= make_tuple(INT_MAX, DBL_MAX, DBL_MAX);
	Point  point2		= make_tuple(INT_MAX, DBL_MAX, DBL_MAX);
	double closest_dist = DBL_MAX;
	for (size_t i = 0; i < PointSize; i++) {
		for (size_t j = i + 1; j < PointSize; j++) {
			double dist = Distance(Points[i], Points[j]);
			if (dist < closest_dist) {
				point1		 = Points[i];
				point2		 = Points[j];
				closest_dist = dist;
			}
		}
	}
	return make_tuple(point1, point2, closest_dist);
}

PointPair ClosestPair::ClosestPointPair(vector<Point> &Points_X_Sort,
										vector<Point> &Points_Y_Sort,
										unsigned p, unsigned r) {
	unsigned points_size = r - p;
	if (points_size < 0)
		exit(-1);
	else if (points_size == 0 || points_size == 1) {
		Point	  NonePoint = make_tuple(INT_MAX, DBL_MAX, DBL_MAX);
		PointPair NonePair	= make_tuple(NonePoint, NonePoint, DBL_MAX);
		return NonePair;
	} else if (points_size <= 3) {
		Point  point1		= make_tuple(INT_MAX, DBL_MAX, DBL_MAX);
		Point  point2		= make_tuple(INT_MAX, DBL_MAX, DBL_MAX);
		double closest_dist = DBL_MAX;
		for (int i = p; i < r; i++) {
			for (int j = i + 1; j < r; j++) {
				double dist = Distance(Points_X_Sort[i], Points_X_Sort[j]);
				if (dist < closest_dist) {
					point1		 = Points_X_Sort[i];
					point2		 = Points_X_Sort[j];
					closest_dist = dist;
				}
			}
		}
		return make_tuple(point1, point2, closest_dist);
	}
	// * Divide and Recursion
	unsigned	  m			= (p + r - 1) / 2;
	double		  loc_X_div = get<loc_X>(Points_X_Sort[m]);
	vector<Point> YL, YR;
	for (unsigned i = 0; i < points_size; i++) {
		if (get<loc_X>(Points_Y_Sort[i]) <= loc_X_div)
			//* if Point_Y_Sort.loc_X > Point_X_Sort[middle].loc_X, add this
			//* point to YL
			YL.push_back(Points_Y_Sort[i]);

		else
			YR.push_back(Points_Y_Sort[i]);
	}

	PointPair pair1 = ClosestPointPair(Points_X_Sort, YL, p, m + 1);
	PointPair pair2 = ClosestPointPair(Points_X_Sort, YR, m + 1, r);
	// * Compare ret
	PointPair closest_pair =
		get<_dist>(pair1) < get<_dist>(pair2) ? pair1 : pair2;
	double closest_dist = get<_dist>(closest_pair); //* 一般不会为 DBL_MAX
	// * Merge
	double		  l_loc_X_limit = (closest_dist == DBL_MAX)
									  ? get<loc_X>(Points_X_Sort[p])
									  : loc_X_div - closest_dist;
	double		  r_loc_X_limit = (closest_dist == DBL_MAX)
									  ? get<loc_X>(Points_X_Sort[r])
									  : loc_X_div + closest_dist;
	vector<Point> Points_Y_Sort_limit;
	for (int i = 0; i < points_size; i++) {
		if (get<loc_X>(Points_Y_Sort[i]) >= l_loc_X_limit &&
			get<loc_X>(Points_Y_Sort[i]) <= r_loc_X_limit) {
			Points_Y_Sort_limit.push_back(Points_Y_Sort[i]);
		}
	}
	PointPair merge_closest_pair = Merge(Points_Y_Sort_limit);
	double	  merge_closest_dist = get<_dist>(merge_closest_pair);

	return (closest_dist <= merge_closest_dist) ? closest_pair
												: merge_closest_pair;
}

void ClosestPair::PreSort() {
	Points_X_Sort = Points;
	Points_Y_Sort = Points;
	sort(Points_X_Sort.begin(), Points_X_Sort.end(), this->CompLocX);
	sort(Points_Y_Sort.begin(), Points_Y_Sort.end(), this->CompLocY);
}

PointPair ClosestPair::Merge(vector<Point> &Points) {
	unsigned points_size = Points.size();
	if (points_size == 0) {
		Point	  NonePoint = make_tuple(INT_MAX, DBL_MAX, DBL_MAX);
		PointPair NonePair	= make_tuple(NonePoint, NonePoint, DBL_MAX);
		return NonePair;
	}
	Point  point1		= make_tuple(INT_MAX, DBL_MAX, DBL_MAX);
	Point  point2		= make_tuple(INT_MAX, DBL_MAX, DBL_MAX);
	double closest_dist = DBL_MAX;
	for (int i = 0; i < Points.size(); i++) {
		for (int j = i + 1; j < Points.size() && j < i + 7; j++) {
			double dist = Distance(Points[i], Points[j]);
			if (dist < closest_dist) {
				point1		 = Points[i];
				point2		 = Points[j];
				closest_dist = dist;
			}
		}
	}
	return make_tuple(point1, point2, closest_dist);
}

bool ClosestPair::CompLocX(const Point &P1, const Point &P2) {
	return get<loc_X>(P1) < get<loc_X>(P2);
}

bool ClosestPair::CompLocY(const Point &P1, const Point &P2) {
	return get<loc_Y>(P1) < get<loc_Y>(P2);
}

int main() {
	string		data_path = "data.txt";
	ClosestPair points(data_path);

    clock_t start, end;

	start = clock();
	PointPair	point_pair = points.FindClosestPair();
    end = clock();
    cout << "duration of find closest pair is " << end - start << "ms" << endl;

	Point		point1	   = get<_p1>(point_pair);
	Point		point2	   = get<_p2>(point_pair);
	double		dist	   = get<_dist>(point_pair);
	cout << "The closest point pair is" << endl;
	cout << get<serial>(point1) << ' ' << get<loc_X>(point1) << ' '
		 << get<loc_Y>(point1) << endl;
	cout << get<serial>(point2) << ' ' << get<loc_X>(point2) << ' '
		 << get<loc_Y>(point2) << endl;
	cout << "The distance of the point pair is " << dist << endl << endl;

    start = clock();
    PointPair normal_point_pair = points.NormalFindClosestPair();
    end = clock();
    cout << "duration of normal find closest pair is " << end - start << "ms" << endl;

	Point	  normal_point1		= get<_p1>(normal_point_pair);
	Point	  normal_point2		= get<_p2>(normal_point_pair);
	double	  normal_dist		= get<_dist>(normal_point_pair);
	cout << "The closest point pair is" << endl;
	cout << get<serial>(normal_point1) << ' ' << get<loc_X>(normal_point1)
		 << ' ' << get<loc_Y>(normal_point1) << endl;
	cout << get<serial>(normal_point2) << ' ' << get<loc_X>(normal_point2)
		 << ' ' << get<loc_Y>(normal_point2) << endl;
	cout << "The distance of the point pair is " << normal_dist << endl;
}