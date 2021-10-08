// Implementation of the KMeans Algorithm
// reference: http://mnemstudio.org/clustering-k-means-example-1.htm

#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <chrono>
#include <thread>
#include <oneapi/tbb/parallel_for.h>
#include <oneapi/tbb/parallel_reduce.h>
#include <oneapi/tbb/task_arena.h>
#include <oneapi/tbb/global_control.h>
#include "/usr/local/opt/libomp/include/omp.h"

#define THREADS 8

using namespace std;
using namespace tbb;

/** Point */
class Point {
private:
	int id_point, id_cluster;
	vector<double> values;
	int total_values;
	string name;

public:
	Point(int id_point, vector<double>& values, string name = "") {
		this->id_point = id_point;
		total_values = values.size();

		for(int i = 0; i < total_values; i++)
			this->values.push_back(values[i]);

		this->name = name;
		id_cluster = -1;
	}

	int getID() {
		return id_point;
	}

	void setCluster(int id_cluster) {
		this->id_cluster = id_cluster;
	}

	int getCluster() {
		return id_cluster;
	}

	double getValue(int index) {
		return values[index];
	}

	int getTotalValues() {
		return total_values;
	}

	void addValue(double value) {
		values.push_back(value);
	}

	string getName() {
		return name;
	}
};

/** Cluster */
class Cluster {
private:
	int id_cluster;
	vector<double> central_values;
	vector<Point> points;

public:
	Cluster(int id_cluster, Point point) {
		this->id_cluster = id_cluster;

		int total_values = point.getTotalValues();

		for(int i = 0; i < total_values; i++) {
			central_values.push_back(point.getValue(i));
		}

		points.push_back(point);
	}

	void addPoint(Point point) {
		points.push_back(point);
	}

	bool removePoint(int id_point) {
		int total_points = points.size();

		for(int i = 0; i < total_points; i++) {
			if(points[i].getID() == id_point) {
				points.erase(points.begin() + i);
				return true;
			}
		}
		return false;
	}

	double getCentralValue(int index) {
		return central_values[index];
	}

	void setCentralValue(int index, double value) {
		central_values[index] = value;
	}

	Point getPoint(int index) {
		return points[index];
	}

	int getTotalPoints() {
		return points.size();
	}

	int getID() {
		return id_cluster;
	}
};

/** KMeans */
class KMeans {
private:
	int K; 
	int total_values, total_points, max_iterations;
	vector<Cluster> clusters;

	int getIDNearestCenter(Point point) {
		double sum = 0.0, min_dist;
		int id_cluster_center = 0;

		// #pragma omp simd
		for(int i = 0; i < total_values; i++) {
			sum += pow(clusters[0].getCentralValue(i) - point.getValue(i), 2.0);
		}

		min_dist = sqrt(sum);

		// #pragma omp simd
		for(int i = 1; i < K; i++) {
			double dist;
			sum = 0.0;

			for(int j = 0; j < total_values; j++) {
				sum += pow(clusters[i].getCentralValue(j) - point.getValue(j), 2.0);
			}

			dist = sqrt(sum);

			if(dist < min_dist) {
				min_dist = dist;
				id_cluster_center = i;
			}
		}

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
		/** Start timing execution */
        auto begin = chrono::high_resolution_clock::now();
        
		if (K > total_points) {
			return;
		}

		vector<int> prohibited_indexes;

		/** Choose K distinct points for the centroids of the clusters */
		for(int i = 0; i < K; i++) {
			while(true) {
				int index_point = rand() % total_points;

				/* Check if point exists and hasn't been used as the center of another cluster */
				if(find(prohibited_indexes.begin(), prohibited_indexes.end(), index_point) == prohibited_indexes.end()) {
					prohibited_indexes.push_back(index_point);
					points[index_point].setCluster(i);
					Cluster cluster(i, points[index_point]);
					clusters.push_back(cluster);
					break;
				}
			}
		}
		
        auto end_phase1 = chrono::high_resolution_clock::now();
        
		int iter = 1;

		while(true) {
			/** Number of dimensions/attributes associated with each point */
			int dimensions = total_values;

			/** Vector containing centroid counts */
            std::vector<int> centroids_count(K);

            /** N-dementional Matrix containing cluster sums */
            std::vector<std::vector<float>> cluster_matrix(K, std::vector<float>(dimensions));

			bool done = true;

			/** Number of TBB threads */
			tbb::global_control global_limit(tbb::global_control::max_allowed_parallelism, THREADS);

			/** Associate each point to the nearest center */
			parallel_for(tbb::blocked_range<size_t>(0, total_points), [&](tbb::blocked_range<size_t> r) {			
				for(int i = r.begin(); i < r.end(); i++) {
					int id_old_cluster = points[i].getCluster();
					int id_nearest_center = getIDNearestCenter(points[i]);
					
					if(id_old_cluster != id_nearest_center) {
						/** 
						 * Deleted removePoint() and addPoint() function calls that were throwing race conditions.
						 * Simply associating points with clusters, WITHOUT adding them to the cluster.
						 */
						points[i].setCluster(id_nearest_center);
						done = false;
					}
				}
			}, auto_partitioner());

			/** Recalulating the center of each cluster by rewriting the logic of evaluating points by clusters */

            /** Compute cluster counts */
			#pragma omp parallel
			for (int i = 0; i < total_points; i++) { 
				centroids_count[points[i].getCluster()]++;
			}
			
            /** Summation of values in the cluster */
            for (int i = 0; i < total_points; i++) { 
				#pragma omp parallel for reduction(+ : cluster_matrix[points[i].getCluster()][j])
                for (int j = 0; j < dimensions; j++) {
                    cluster_matrix[points[i].getCluster()][j] += points[i].getValue(j);
                }
            }

            /** Set cluster values by computing mean */
            for(int i = 0; i < K; i++) {
                for (int j = 0; j < dimensions; j++) {
					double mean = cluster_matrix[i][j] / centroids_count[i];
                    clusters[i].setCentralValue(j, mean);
                }
            }

            /** Reset contents of matrix for reuse */
			for(int i = 0 ; i < K; i++){
            	for( int j = 0; j < dimensions; j++){
                	cluster_matrix[i][j] = 0;
             	}
            }

			if(done == true || iter >= max_iterations) {
				/** Finally add points to clusters ONLY ONCE */
				cout << "Total points is: " << total_points << endl;
				for (int i = 0; i < total_points; i++) {
					int id = points[i].getCluster();
					clusters[id].addPoint(points[i]);
				}
				break;
			}

			iter++;
		}

		/** Finish timing execution */
        auto end = chrono::high_resolution_clock::now();

		/** shows elements of clusters */
		for(int i = 0; i < K; i++) {
			int total_points_cluster =  clusters[i].getTotalPoints();
			cout << "Total points in cluster " << i << " is: " << total_points_cluster << endl;
 
			cout << "Cluster " << clusters[i].getID() + 1 << endl;
			for(int j = 0; j < total_points_cluster; j++) {
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

			/** Print execution times */
			cout << "\n\n";
            cout << "TOTAL EXECUTION TIME = "<<std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count()<<"\n";
            
            cout << "TIME PHASE 1 = "<<std::chrono::duration_cast<std::chrono::microseconds>(end_phase1-begin).count()<<"\n";
            
            cout << "TIME PHASE 2 = "<<std::chrono::duration_cast<std::chrono::microseconds>(end-end_phase1).count()<<"\n";
		}
	}
};

int main(int argc, char *argv[]) {
	srand (time(NULL));

	int total_points, total_values, K, max_iterations, has_name;

	cin >> total_points >> total_values >> K >> max_iterations >> has_name;

	vector<Point> points;
	string point_name;

	/* Parallelizing this for loop introduces unnecessary additional thread overhead */
	for(int i = 0; i < total_points; i++) {
		vector<double> values;

		for(int j = 0; j < total_values; j++) {
			double value;
			cin >> value;
			values.push_back(value);
		}

		if(has_name) {
			cin >> point_name;
			Point p(i, values, point_name);
			points.push_back(p);
		} else {
			Point p(i, values);
			points.push_back(p);
		}
	}

	/** Kmeans constructor */
	KMeans kmeans(K, total_points, total_values, max_iterations);

	/** Execute Kmeans algorithm */
	kmeans.run(points);

	return 0;
}