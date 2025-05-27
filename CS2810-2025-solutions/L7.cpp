#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;


// Comparator functor class that will be used for sorting
class Comparator {
	private:
		// A private boolean to indicate the expected sorting order
		bool isAscending;
	
	public:
		// The arugment to the constructor is a boolean flag which is used to determine whether the functor should sort in ascending or desceding order
		Comparator(bool ascendingFlag) : isAscending(ascendingFlag) {}
		
		// The functor implementation
		bool operator()(long long a, long long b) const {
			// If the flag denotes ascending order then we will send a true value if the first value is lesser than the second value (which indicates that the elements are in correct order)
			// If the flag denotes descending order then we send true when the first value is greater than the second value (which again indicates that the elements are in correct order)
			return isAscending ? a < b : a > b;
		}
};


// The class to represent the matrix class
class Matrix2D {
	private:
		// We will be maintaining two copies of the input array
		// This is done to optimize the running of the sort commands
		// We say that we will sort the array only if there is an INVERSION_2D or a DISPLAY_2D command
		// For other instances when there are just SORT_2D commands, we will just keep storing the sorting order
		
		// This matrix represents the original input array
		vector<vector<long long>> originalData;
		
		// This matrix represents the sorted array
		vector<vector<long long>> sortedData;
		
		// These variables represent the size of tha matrix and the count of inversions in the matrix
		long long size;
		long long inversionCount;
		
		
		// A method to run the recursive merge sort function
		void mergeSort(vector<long long>& arr, long long left, long long right, const Comparator& comparator) {
			// boundary condition
			if (left >= right) return;
			
			// Get the mid point (from where the split will take place)
			long long mid = left + (right - left) / 2;
			
			// Make the recursive calls on the right half and the left half of the array (thus creating two subarrays)
			mergeSort(arr, left, mid, comparator);
			mergeSort(arr, mid + 1, right, comparator);
			
			// Call the merge method which actually implements the merge logic
			merge(arr, left, mid, right, comparator);
		}
		
		
		// A method which takes two sorted subarrays and merges them into another sorted array (the sorting is done using the Comparator functor)
		void merge(vector<long long>& arr, long long left, long long mid, long long right, const Comparator& comparator) {
			// A temporary array to store the sorted array (after merging)
			vector<long long> temp;
			
			// Keep two indices for the two subarrays
			long long i = left, j = mid + 1;
			
			// Run a loop over the subarrays till both of them are within the range
			while (i <= mid && j <= right) {
				// Use the comparator to determine the sorting order and accordingly one of the two elements (from the two subarrays) into the temp array
				if (comparator(arr[i], arr[j])) 
					temp.push_back(arr[i++]);
				else 
					temp.push_back(arr[j++]);
			}
			
			// If either of the two subarrays are still having elements to be added , then add them
			while (i <= mid) temp.push_back(arr[i++]);
			while (j <= right) temp.push_back(arr[j++]);
			
			// Once the two subarrays are merged then transfer all the elements (which are now sorted) into the original array
			for (long long k = left; k <= right; ++k) 
				arr[k] = temp[k - left];
		}
		
		
		// A method to run the merge algorithm to count the number of inversions
		long long mergeSortCount(vector<long long>& arr, long long left, long long right) {
			// boundary condition
			if (left >= right) return 0;
			
			// Get the mid point (from where the split will take place)
			long long mid = left + (right - left) / 2;
			
			// Declare a variable to count the inversions
			long long inv = 0;
			
			// Make the recursive calls on the right half and the left half of the array and add the number of inversions returned from the two subarrays
			inv += mergeSortCount(arr, left, mid);
			inv += mergeSortCount(arr, mid + 1, right);
			
			// Call the merge method which actually calculates and returns the number of inversions for the current array
			inv += mergeCount(arr, left, mid, right);
			
			// Returned the total number of inversions calculated from the left subarray, the right subarray and the current merged array
			return inv;
		}
		
		
		// A method which takes the two sorted subarrays and counts the number of inversions in them while mergint the two subaraays into a single sorted array
		long long mergeCount(vector<long long>& arr, long long left, long long mid, long long right) {
			// A temporary array to store the sorted array (after merging)
			vector<long long> temp;
			
			// Keep two indices for the two subarrays
			long long i = left, j = mid + 1, inv = 0;
			
			// Run a loop over the subarrays till both of them are within the range
			while (i <= mid && j <= right) {
				if (arr[i] <= arr[j])
					temp.push_back(arr[i++]);
				
				// If the first element is greater than the second element then we can say that it is an inversion and we increase the count
				// Since the first subarray is sorted and the element at index i is greater than the element at index j, then all the elements to the right of index i in the first array
				// will be greater than the element in index j.
				// Hence all these indices will be counted for inversion w.r.t index j
				else {
					temp.push_back(arr[j++]);
					inv += (mid - i + 1);
				}
			}
			
			// If either of the two subarrays are sill having elements to be added , then add them
			while (i <= mid) temp.push_back(arr[i++]);
			while (j <= right) temp.push_back(arr[j++]);
			
			// Once the two subarrays are merged then transfer all the elements (which are now sorted) into the original array
			for (long long k = left; k <= right; ++k) 
				arr[k] = temp[k - left];
			
			// Return the count of inversions calculated
			return inv;
		}
	
	
	public:
		// Two strings to determine :
		// (1) the current sorted order of the array
		// (2) the sorted order that was passed along with the last SORT_2D command
		string curSortOrder, lastSortOrder;
		
		// Default constructor
		Matrix2D(){}
		
		// Constuctor for the matrix class which takes the input matrix as parameter
		Matrix2D(const vector<vector<long long>>& input) : 
			// currently both the sorted and the unsorted arrays are set to the input array
			originalData(input), sortedData(input), size(input.size()), 
			// the sort orders are set to none initially
			curSortOrder("none"), lastSortOrder("none"),
			// the inital inversion count is set to -1
			inversionCount(-1) {}
		
		
		// A method that will be used to call the merge sort function
		void sortRows(const Comparator& comparator, const string& order) {
			// sort the array row wise by passing each row to the merge sort function
			for (auto& row : sortedData) {
				// Call to merge sort function
				mergeSort(row, 0, row.size() - 1, comparator);
			}
		}
		
		
		// Function to call for counting inversions
		long long countInversions() {
			// We first check if the current sorted order of the array and the order passed along with the last SORT_2D command are same or not.
			// Also we check if the number of inversions have been calculated till now or not
			// If both satisfies then we directly return the currentlt stored value of inversions wihtout sorting the array again
			// This is done to save the unecessary time needed for sorting again and again
			if ( lastSortOrder == curSortOrder && inversionCount != -1)
				return inversionCount;
			
			// We sort the array if required
			if ( lastSortOrder != curSortOrder){
				// The comparator object is created by setting the sorting order
				Comparator comp(curSortOrder == "ascending");
				
				// sortRows method is called by passing the comparator object
				sortRows(comp, curSortOrder);
				
				// Since the array is now sorted, we set the sorted order
				lastSortOrder = curSortOrder;
			}
			
			// Flatten the array to count the inversions
			vector<long long> flat;
			for (const auto& row : sortedData)
				flat.insert(flat.end(), row.begin(), row.end());
			
			// Call the mergeSortCount method with this flattened array to calculate the number of inversions
			inversionCount = mergeSortCount(flat, 0, flat.size() - 1);
			
			return inversionCount;
		}
		
		
		// Method to display the matrix
		void display() {
			// Before displaying also, we will sort only if required (i.e. the current sorted order of the matrix and the order passed as argument 
			// with last SORT_2D command are different)
			if ( lastSortOrder != curSortOrder){
				// The comparator object is created by setting the sorting order
				Comparator comp(curSortOrder == "ascending");
				
				// sortRows method is called by passing the comparator object
				sortRows(comp, curSortOrder);
				
				// Since the array is now sorted, we set the sorted order
				lastSortOrder = curSortOrder;
			}
			
			// Print the matrix
			for (const auto& row : sortedData) {
				for (long long val : row) {
					cout << val << " ";
				}
				cout << endl;
			}
		} 
};



// From here, the code for the closest pair on 2D plane starts

// Each point is represented by a struct
struct Point {
	// corodinates of the point
    long long x, y;
	
	// index of the point in the point (needed during printing the closest point based on the input order)
    long long index;
	
	// constructor with default values as 0
    Point(long long x = 0, long long y = 0, long long index = 0) : x(x), y(y), index(index) {}
};


// Method to find the squared Euclidean distance between two points
long long squaredDistance(const Point& a, const Point& b) {
    return (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y);
}


// Method to compare two different pair of points (first by distance, then by x cooridnate and lastly by y coordinate)
bool comparePairs(const pair<Point, Point>& p1, const pair<Point, Point>& p2) {
    // Find the distance of the two points in the two pairs
	long long d1 = squaredDistance(p1.first, p1.second);
    long long d2 = squaredDistance(p2.first, p2.second);
	
	// If the distance is not equal then return the boolean value based on the distances
    if (d1 != d2) return d1 < d2;
	
	// If the distance is equal, then control reaches here
    
	// Compare and return the boolean values based on x coordinate and then by y coordinate (in case x ccordinates are equal)
	// This is done for the first point in the two pairs
    if (p1.first.x != p2.first.x) return p1.first.x < p2.first.x;
    if (p1.first.y != p2.first.y) return p1.first.y < p2.first.y;
	
    // If still tie is not broken then the comparison is done for the second point in the two pairs
    if (p1.second.x != p2.second.x) return p1.second.x < p2.second.x;
    if (p1.second.y != p2.second.y) return p1.second.y < p2.second.y;
	
    // If still tie not broken then the points are sorted based on the input order
    return (p1.first.index < p2.first.index) || (p1.first.index == p2.first.index && p1.second.index < p2.second.index);
}


// Method to calculate the closest distance by brute force (this is used if the number of points are less)
pair<Point, Point> bruteForceClosestPair(const vector<Point>& points, long long start, long long end) {
	// conisder the first two points as the closest points
    pair<Point, Point> bestPair = {points[start], points[start+1]};
	
	// calculate teh distance of this pair (the first two points)
    long long minDist = squaredDistance(bestPair.first, bestPair.second);
	
	// iterate over the points in pairwise manner
    for (long long i = start; i <= end; ++i) {
        for (long long j = i + 1; j <= end; ++j) {
			// calculate the distance of the current pair
            long long dist = squaredDistance(points[i], points[j]);
			
			// Update thhe best pair if the current distance is smaller
            if (dist < minDist || (dist == minDist && comparePairs({points[i], points[j]}, bestPair))) {
                minDist = dist;
                bestPair = {points[i], points[j]};
            }
        }
    }
	
	// return the best pair
    return bestPair;
}


// method to find the best pair from the strip (used for the divide and conquer algorithm)
pair<Point, Point> stripClosest(const vector<Point>& strip, const pair<Point, Point>& bestPair) {
	// consider the best pair and find the distance of the points
    pair<Point, Point> result = bestPair;
    long long minDist = squaredDistance(bestPair.first, bestPair.second);
	
	// iterate over the points in the strip
    for (size_t i = 0; i < strip.size(); ++i) {
		// we do not consider all the possible pairs
		// we take only those pairs whose vertical distance is lesser than the current minimum distance
		// this is done because the points are already sorted on x coordinate
        for (size_t j = i + 1; j < strip.size() && (strip[j].y - strip[i].y)*(strip[j].y - strip[i].y) < minDist; ++j) {
			// find the distance
            long long dist = squaredDistance(strip[i], strip[j]);
			// update the best pair
            if (dist < minDist || (dist == minDist && comparePairs({strip[i], strip[j]}, result))) {
                minDist = dist;
                result = {strip[i], strip[j]};
            }
        }
    }
	
	// return the best pair
    return result;
}


// Method to run the divide and conquer algorithm (the arguments to this method are the points sorted by x coordinate and sorted by y coordinate)
pair<Point, Point> closestUtil(const vector<Point>& pointsByX, const vector<Point>& pointsByY) {
	// if the number of points are 3 or less then we find the closest pair by brute force
    long long n = pointsByX.size();
    if (n <= 3) return bruteForceClosestPair(pointsByX, 0, n-1);
	
	// find the middle point according to the x coorindates
    long long mid = n / 2;
    Point midPoint = pointsByX[mid];
	
	// form the points on the left and the right half (by x ccodinate) of the middle point
    vector<Point> leftByX(pointsByX.begin(), pointsByX.begin() + mid);
    vector<Point> rightByX(pointsByX.begin() + mid, pointsByX.end());
	
	// parititon the set which is sorted by y coordinate into two sets (which is split by the middle x coordinate)
    vector<Point> leftByY, rightByY;
    for (const Point& p : pointsByY) {
        if (p.x < midPoint.x || (p.x == midPoint.x && p.index < midPoint.index))
            leftByY.push_back(p);
        else
            rightByY.push_back(p);
    }
	
	// make the recursive calls on the left and the right points
    pair<Point, Point> leftPair = closestUtil(leftByX, leftByY);
    pair<Point, Point> rightPair = closestUtil(rightByX, rightByY);
	
	// call the compare method on the result of the left and right calls and return the pair accordingly
	// this stores the best pair from the left and right subparts combined
	// now we need to find if the best pair is from the strip (i.e. one point is on the left subpart and the other point is in the right subpart)
    pair<Point, Point> bestPair = comparePairs(leftPair, rightPair) ? leftPair : rightPair;
	
	//calculate and store the distance
	// this distance will be used for finding the best pair
    long long minDist = squaredDistance(bestPair.first, bestPair.second);
	
	// collect the points that belong to the strip
    vector<Point> strip;
    for (const Point& p : pointsByY) {
        long long dx = p.x - midPoint.x;
        if (dx * dx < minDist)
            strip.push_back(p);
    }
	
	// find the closest pair in the strip
    pair<Point, Point> stripPair = stripClosest(strip, bestPair);
	
	// campre the best pair obtained from the recursive calls and the best pair obtained from the strip and return the best result
    return comparePairs(stripPair, bestPair) ? stripPair : bestPair;
}


// method to form the points, sort them and call the method to find the closest pair
void findClosestPair() {
	// taking input and forming a vector of points
    long long n;
    cin >> n;
    vector<Point> points(n);
    for (long long i = 0; i < n; ++i) {
        cin >> points[i].x >> points[i].y;
        points[i].index = i;
    }
	
	// sorting the points based on x coordinate
    vector<Point> pointsByX = points;
    sort(pointsByX.begin(), pointsByX.end(), [](const Point& a, const Point& b) {
        return a.x < b.x || (a.x == b.x && a.index < b.index);
    });
	
	// sorting the points based on y coordinate
    vector<Point> pointsByY = points;
    sort(pointsByY.begin(), pointsByY.end(), [](const Point& a, const Point& b) {
        return a.y < b.y || (a.y == b.y && a.index < b.index);
    });
	
	// call the closest pair method
    pair<Point, Point> bestPair = closestUtil(pointsByX, pointsByY);

    // maintain the correct order of the points based on the input order
    if (bestPair.first.index > bestPair.second.index)
        swap(bestPair.first, bestPair.second);
	
	// print the coordinates
    cout << bestPair.first.x << " " << bestPair.first.y << " "
         << bestPair.second.x << " " << bestPair.second.y << endl;
}


// Main method
int main() {
    string command = "";
    long long size;
    Matrix2D matrix;
    
    while (command != "END") {
        cin >> command;
        if (command == "SORT_2D") {
            string order;
            cin >> order;
			
			//set the current sort order instead of actually sorting the matrix
            matrix.curSortOrder = order;
        }
        else if (command == "CREATE_2D") {
            cin >> size;
            vector<vector<long long>> inputMatrix(size, vector<long long>(size));
            for (long long i = 0; i < size; ++i)
                for (long long j = 0; j < size; ++j)
                    cin >> inputMatrix[i][j];
				
			//create the matrix
            matrix = Matrix2D(inputMatrix);
        }
        else if (command == "INVERSION_2D") {
			// count the number of inversions
            cout << matrix.countInversions() << endl;
        } 
        else if (command == "DISPLAY_2D") {
			// display the matrix
            matrix.display();
        }
        else if (command == "CLOSEST_2D") {
			// find the closest pair
            findClosestPair();
        }
    }
    return 0;
}