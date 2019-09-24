#include <iostream>
#include <vector>
#include <omp.h>
#include <cassert>
#include <stdlib.h>
using namespace std;

// macro for linear indexing into 2D table (column-major)
inline int li(int w, int j, int W) {return w+j*(W+1);}

void build_table(vector<int>& K, const vector<int>& wts, const vector<int>& vals, int W);
int compute_table(vector<int>& K, const vector<int>& wts, const vector<int>& vals, int W);
void backtrack(const vector<int>& K, const vector<int>& wts, vector<bool>& used, int W);
void backtrack_implicit(vector<int>&K, vector<int>&M, const vector<int>& wts, const vector<int>& vals, int W, 
                        int first, int last, vector<int>& path);
int findk(vector<int>&K, vector<int>&M, const vector<int>& wts, const vector<int>& vals, int W, 
                        int first, int last);
int convert_path_to_used(vector<bool>& used, const vector<int>& path, const vector<int>& vals, int W);

int main(int argc, char* argv[]) {
    int best_value_serial, total_weight, nused, total_value;
    double tic, seconds[3];

    const int MAX_VALUE  = 1000;
    const int MAX_WEIGHT = 1000;

    // set the problem size from command line
    int capacity   = atoi(argv[1]);
    int nitems     = atoi(argv[2]);

    cout << "Capacity: " << capacity << " Items: " << nitems << endl;

    // initialize vectors
    vector<int> weight(nitems,0);
    vector<int> value(nitems,0);

    // initialize weights and values randomly
    srand( time(0) );
    for(int i = 0; i < nitems; i++)
    {
        // weight should not exceed capcity
        weight[i] = 1 + (lrand48() % min(MAX_WEIGHT,capacity)); 
        value[i]  = 1 + (lrand48() % MAX_VALUE);
    }

    // initialize output
    vector<bool> used(nitems,false);
    vector<bool> used2(nitems,false);

    /* ************************** */
    /* Memory inefficient version */
    /* ************************** */

    // time the solution
    tic = omp_get_wtime();

    // initialize 2D dynamic programming table
    vector<int> table(nitems*(capacity+1));

    // run algorithm
    build_table(table, weight, value, capacity);
    backtrack(table, weight, used, capacity);

    seconds[0] = omp_get_wtime() - tic;

    /* ************************************* */
    /* Memory efficient version (value only) */
    /* ************************************* */

    // time the solution
    tic = omp_get_wtime();

    // initialize 1D dynamic programming table
    vector<int> table2(2*(capacity+1));

    // run algorithm
    int knapsackValue = compute_table(table2, weight, value, capacity);

    seconds[1] = omp_get_wtime() - tic;;

    /* ************************ */
    /* Memory efficient version */
    /* ************************ */

    // time the solution
    tic = omp_get_wtime();

    // initialize 1D dynamic programming table
        // iniitialize 1D tables
    vector<int> table3(2*(capacity+1));
    vector<int> midpts(2*(capacity+1));
    vector<int> path(nitems);
    
    // run algorithm
    backtrack_implicit(table3, midpts, weight, value, capacity, 0, nitems-1, path);
    int knapsackValue2 = convert_path_to_used(used2, path, value, capacity);

    seconds[2] = omp_get_wtime() - tic;;

    // check mem ineff value against mem eff value
    assert(table.back() == knapsackValue);

    // check mem ineff solution against mem eff solution
    assert(table.back() == knapsackValue2);
    assert(used == used2);

    cout << "Mem inef sol = \t" << seconds[0] << " seconds" << endl;
    cout << "Mem eff val = \t" << seconds[1] << " seconds" << endl;
    cout << "Mem eff sol = \t" << seconds[2] << " seconds" << endl;

    return 0;
}

void build_table(vector<int>& K, const vector<int>& wts, const vector<int>& vals, int W) {

    int w, j, numItems = wts.size();

    // initialize first column
    for(w = 0; w <= W; w++) {
        if( w < wts[0]) {
            K[w] = 0;
        } else {
            K[w] = vals[0];
        }
    }

    // loop over 2D table
    for(w = 0; w <= W; w++) {
        for(j = 1; j < numItems; j++) {
            // if item doesn't fit or not including it is better
            if(w < wts[j] || K[li(w,j-1,W)] >= vals[j] + K[li(w-wts[j],j-1,W)]) {
                K[li(w,j,W)] = K[li(w,j-1,W)];
            } else { // else including it is better
                K[li(w,j,W)] = vals[j] + K[li(w-wts[j],j-1,W)];
            }
        }
    }

}

void backtrack(const vector<int>& K, const vector<int>& wts, vector<bool>& used, int W) {
    
    int j, w = W, n = used.size();
    
    // backtrack starting from bottom right corner
    for(j = n-1; j > 0; j--) {
        // see if item is used
        used[j] = (K[li(w,j,W)] != K[li(w,j-1,W)]);
        // if so decrease remaining capacity
        if(used[j]) {
            w -= wts[j];
        }
    }
    // see if first item is used
    used[0] = (w >= wts[0]);
}

int compute_table(vector<int>& K, const vector<int>& wts, const vector<int>& vals, int W) {
    
    int w, j, numItems = wts.size(), curr, prev;

    // initialize first column
    for(w = 0; w <= W; w++) {
        if(w < wts[0]) {
            K[w] = 0;
        } else {
            K[w] = vals[0];
        }
    }

    // loop over columns of implicit 2D table
    for(j = 1; j < numItems; j++) {
        // switch curr and prev columns each iteration to avoid copy
        curr = j%2;
        prev = (j+1)%2;
        for(w = 0; w <= W; w++) {
            // if item doesn't fit or not including it is better
            if(w < wts[j] || K[li(w,prev,W)] >= vals[j] + K[li(w-wts[j],prev,W)]) {
                K[li(w,curr,W)] = K[li(w,prev,W)];
            // else including it is better
            } else {
                K[li(w,curr,W)] = vals[j] + K[li(w-wts[j],prev,W)];
            }
        }
    }
    
    // return last entry in current column
    return K[li(W,curr,W)];

}

void backtrack_implicit(vector<int>&K, vector<int>&M, const vector<int>& wts, const vector<int>& vals, int W, 
                        int first, int last, vector<int>& path) {
    // base case
    if(first == last) return; 

    // recursive case
    int mid = (first+last) / 2;
    int k = findk(K,M,wts,vals,W,first,last);
    // set middle element of path
    if(first) {
        // need to offset based on capacity already used
        path[mid] = k + path[first-1]; 
    } else {
        path[mid] = k;
    }
    backtrack_implicit(K,M,wts,vals,k,first,mid,path);
    backtrack_implicit(K,M,wts,vals,W-k,mid+1,last,path);
     
}

int findk(vector<int>&K, vector<int>&M, const vector<int>& wts, const vector<int>& vals, int W, 
                        int first, int last) {
    
    int w, j, numItems = last-first+1, mid = (first+last) / 2, curr, prev;

    // initialize columns for 1st and mid element
    for(w = 0; w <= W; w++) {
        if(w < wts[first]) {
            K[li(w,first%2,W)] = 0;
            M[li(w,mid%2,W)] = w;
        } else {
            K[li(w,first%2,W)] = vals[first];
            M[li(w,mid%2,W)] = w;
        }
    }

    // loop from 2nd element to last
    for(j = first+1; j <= last; j++) {
        // switch curr and prev columns each iteration to avoid copy
        curr = j%2;
        prev = (j+1)%2;

        for(w = 0; w <= W; w++) {
        
            // if item doesn't fit or not including it is better
            if(w < wts[j] || K[li(w,prev,W)] >= vals[j] + K[li(w-wts[j],prev,W)]) {
                K[li(w,curr,W)] = K[li(w,prev,W)];
                // update midpoint value
                if(j > mid) {
                    M[li(w,curr,W)] = M[li(w,prev,W)];
                }
            }
            // else including it is better
            else {
                K[li(w,curr,W)] = vals[j] + K[li(w-wts[j],prev,W)];
                // update midpt value
                if(j > mid) {
                    M[li(w,curr,W)] = M[li(w-wts[j],prev,W)];
                }
            }
        }
    }

    // return last element in current column 
    // (value is row index of path through middle column)
    return M[li(W,curr,W)];

}

int convert_path_to_used(vector<bool>& used, const vector<int>& path, const vector<int>& vals, int W) {
    int n = path.size(), kValue = 0;
    // handle last item (last entry in path is implicitly W)
    if(path[n-2] == W) {
        used[n-1] = false;
    } else {
        used[n-1] = true;
        kValue += vals[n-1];
    }
    // handle other items
    for(int j = n-2; j > 0; j--) {
        // if capacity index didn't change, we didn't include item
        if(path[j] == path[j-1]) {
            used[j] = false;
        // else we included it, count up its value
        } else {
            used[j] = true;
            kValue += vals[j];
        }
    }
    // handle first item (first entry in path is implicitly 0)
    if(path[0]) {
        used[0] = true;
        kValue += vals[0];
    } else {
        used[0] = false;
    }

    return kValue;
}

