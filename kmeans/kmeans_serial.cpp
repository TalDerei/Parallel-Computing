// Implementation of the KMeans Algorithm
// reference: http://mnemstudio.org/clustering-k-means-example-1.htm

#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <chrono>
#include <tbb/parallel_for.h>

using namespace std;
using namespace tbb;

class Point
{
private:
	int id_point, id_cluster;
	vector<double> values;
	int total_values;
	string name;

public:
	Point(int id_point, vector<double>& values, string name = "")
	{
		// cout << "id_point is: " << id_point << endl;
		// cout << "vector values are: " << values[0] << " and " << values[1] << endl;
		// cout << "name is: " << name << endl;
		this->id_point = id_point;
		total_values = values.size();

		for(int i = 0; i < total_values; i++)
			this->values.push_back(values[i]);

		this->name = name;
		id_cluster = -1;
	}

	int getID()
	{
		return id_point;
	}

	void setCluster(int id_cluster)
	{
		this->id_cluster = id_cluster;
	}

	int getCluster()
	{
		return id_cluster;
	}

	double getValue(int index)
	{
		return values[index];
	}

	int getTotalValues()
	{
		return total_values;
	}

	void addValue(double value)
	{
		values.push_back(value);
	}

	string getName()
	{
		return name;
	}
};

class Cluster
{
private:
	int id_cluster;
	vector<double> central_values;
	vector<Point> points;

public:
	Cluster(int id_cluster, Point point)
	{
		this->id_cluster = id_cluster;


		// cout << "? cluster id is: " << id_cluster << endl;

		int total_values = point.getTotalValues();

		// cout << "? total_values is: " << total_values << endl;

		for(int i = 0; i < total_values; i++) {
			// contains central values of the centroid
			central_values.push_back(point.getValue(i));
			// cout << "? point.getValue(i) is: " << point.getValue(i) << endl;
		}

		points.push_back(point);
	}

	void addPoint(Point point)
	{
		points.push_back(point);
	}

	bool removePoint(int id_point)
	{
		int total_points = points.size();

		for(int i = 0; i < total_points; i++)
		{
			if(points[i].getID() == id_point)
			{
				points.erase(points.begin() + i);
				return true;
			}
		}
		return false;
	}

	double getCentralValue(int index)
	{
		return central_values[index];
	}

	void setCentralValue(int index, double value)
	{
		central_values[index] = value;
	}

	Point getPoint(int index)
	{
		return points[index];
	}

	int getTotalPoints()
	{
		return points.size();
	}

	int getID()
	{
		return id_cluster;
	}
};

class KMeans
{
private:
	int K; // number of clusters
	int total_values, total_points, max_iterations;
	vector<Cluster> clusters;

	// return ID of nearest center (uses euclidean distance)
	int getIDNearestCenter(Point point)
	{
		double sum = 0.0, min_dist;
		int id_cluster_center = 0;

		for(int i = 0; i < total_values; i++)
		{
			// cout << "tot values is: " << total_values << endl;
			// cout << "clusters[0].getCentralValue(i) is: " << clusters[0].getCentralValue(i) <<endl;
			// cout << "point.getValue(i) is: " << point.getValue(i) << endl;

			sum += pow(clusters[0].getCentralValue(i) -
					   point.getValue(i), 2.0);
		}

		// cout << "?!?!?!?!?!?!??!?!?!?!?!? sum is: " << sum << endl;

		min_dist = sqrt(sum);

		// cout << "min_dist is: " << min_dist << endl;

		for(int i = 1; i < K; i++)
		{
			double dist;
			sum = 0.0;

			for(int j = 0; j < total_values; j++)
			{
				sum += pow(clusters[i].getCentralValue(j) -
						   point.getValue(j), 2.0);
			}

			dist = sqrt(sum);

			// cout << "dist is: " << dist << endl;

			if(dist < min_dist)
			{
				min_dist = dist;
				id_cluster_center = i;
			}
		}

		// cout << "id center is: !!!!!!!!!!!!!!!!!! " << id_cluster_center << endl;

		return id_cluster_center;
	}

public:
	KMeans(int K, int total_points, int total_values, int max_iterations) {
		this->K = K;
		this->total_points = total_points;
		this->total_values = total_values;
		this->max_iterations = max_iterations;
	}

	void run(vector<Point> & points) {
        auto begin = chrono::high_resolution_clock::now();
        
		if (K > total_points) {
			return;
		}

		vector<int> prohibited_indexes;

		// choose K distinct values for the centers of the clusters
		// parallel_for(tbb::blocked_range<size_t>(0, K), [&](tbb::blocked_range<size_t> r) {
		for(int i = 0; i < K; i++) {
			while(true) {
				int index_point = rand() % total_points;

				// multiple threads can calculate the index point independetkly, but what if they get the same index point?

				// cout << "!!!!!!!!!!!!! index point is: " << index_point << endl;

				// check if the point exists and hasn't been used before 
				if(find(prohibited_indexes.begin(), prohibited_indexes.end(), index_point) == prohibited_indexes.end()) {
					prohibited_indexes.push_back(index_point);
					// cluster with id = i
					points[index_point].setCluster(i);
					// cout << "cluster center is: " << points[index_point].getValue(i) << " " << points[index_point].getValue(i+1) << endl;
					// chose 2D points as a cluster 
					// can do in parallel, can calculate seperate cluster at the same time since they're operating on different indices!
					Cluster cluster(i, points[index_point]);
					clusters.push_back(cluster);
					break;
				}
			}
		}
		// }, auto_partitioner());
		
        auto end_phase1 = chrono::high_resolution_clock::now();
        
		int iter = 1;

		while(true)
		{
			bool done = true;

			// cout << "total number of poiunts is: !!!!!!!!!!!!!!!!!!!!!!" << total_points << endl;

			// you can parallelize this for loop using map!
			// The second step likewise has two possibilities for parallelism. Each point’s new cluster can be computed independently, 
			//and the distances from it to each centroid can be computed separately

			// associates each point to the nearest center
			
			for(int i = 0; i < total_points; i++)
			{
				int id_old_cluster = points[i].getCluster();

				int id_nearest_center = getIDNearestCenter(points[i]);

				if(id_old_cluster != id_nearest_center)
				{
					if(id_old_cluster != -1)
						clusters[id_old_cluster].removePoint(points[i].getID());

					points[i].setCluster(id_nearest_center);
					clusters[id_nearest_center].addPoint(points[i]);
					done = false;
				}
			}

			// recalculating the center of each cluster
			cout << "total values is: " << total_values << endl;
			cout << "K is: " << K << endl;
			for(int i = 0; i < K; i++)
			{
				for(int j = 0; j < total_values; j++)
				{
					int total_points_cluster = clusters[i].getTotalPoints();
					cout << "total_points_cluster is: " << total_points_cluster << endl;
					double sum = 0.0;

					if(total_points_cluster > 0)
					{
						// reduce!
						for(int p = 0; p < total_points_cluster; p++)
							sum += clusters[i].getPoint(p).getValue(j);
						clusters[i].setCentralValue(j, sum / total_points_cluster);
					}
					cout << "sum is: !!!!!!!!!!!!!!! " << sum << endl; 
				}
			}

			if(done == true || iter >= max_iterations)
			{
				// cout << "Break in iteration " << iter << "\n\n";
				break;
			}

			iter++;
		}
		cout << "iterations!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! " << iter << endl;

        auto end = chrono::high_resolution_clock::now();

		// shows elements of clusters
		for(int i = 0; i < K; i++)
		{
			int total_points_cluster =  clusters[i].getTotalPoints();

			cout << "Cluster " << clusters[i].getID() + 1 << endl;
			for(int j = 0; j < total_points_cluster; j++)
			{
				cout << "Point " << clusters[i].getPoint(j).getID() + 1 << ": ";
				for(int p = 0; p < total_values; p++)
					cout << clusters[i].getPoint(j).getValue(p) << " ";

				string point_name = clusters[i].getPoint(j).getName();

				if(point_name != "")
					cout << "- " << point_name;

				cout << endl;
			}

			cout << "Cluster values: ";

			for(int j = 0; j < total_values; j++)
				cout << clusters[i].getCentralValue(j) << " ";

			cout << "\n\n";
            cout << "TOTAL EXECUTION TIME = "<<std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count()<<"\n";
            
            cout << "TIME PHASE 1 = "<<std::chrono::duration_cast<std::chrono::microseconds>(end_phase1-begin).count()<<"\n";
            
            cout << "TIME PHASE 2 = "<<std::chrono::duration_cast<std::chrono::microseconds>(end-end_phase1).count()<<"\n";
		}
	}
};

int main(int argc, char *argv[])
{
	srand (time(NULL));

	int total_points, total_values, K, max_iterations, has_name;

	cin >> total_points >> total_values >> K >> max_iterations >> has_name;

	vector<Point> points;
	string point_name;

	// can parallelize by creating pointers to vectors of atomics
	for(int i = 0; i < total_points; i++)
	{
		vector<double> values;

		for(int j = 0; j < total_values; j++)
		{
			double value;
			cin >> value;
			values.push_back(value);
		}

		if(has_name)
		{
			cin >> point_name;
			Point p(i, values, point_name);
			points.push_back(p);
		}
		else
		{
			Point p(i, values);
			points.push_back(p);
		}
	}

	KMeans kmeans(K, total_points, total_values, max_iterations);
	kmeans.run(points);

	return 0;
}


/**
 * Step 1: choose k random clusters. Intution tells me that you can parallelize this by creating pointers to vectors of atomics?
 * This would be a simple fork-join paradigm
 * 
 * Step 2: associates each point to the nearest center would be a map operation 
 * 
 * Step 3: recalculating the center of each cluster is a reduce operation
 * 
 */