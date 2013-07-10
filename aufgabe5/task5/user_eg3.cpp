#//! user_eg3.cpp -- Invoke rbox and qhull from C++

#include "RboxPoints.h"
#include "QhullError.h"
#include "QhullQh.h"
#include "QhullFacet.h"
#include "QhullFacetList.h"
#include "QhullLinkedList.h"
#include "QhullVertex.h"
#include "Qhull.h"

#include <cstdio>   /* for printf() of help message */
#include <iostream>
#include <time.h>
#include <sstream>
#include <fstream>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;

using namespace std;

using orgQhull::Qhull;
using orgQhull::QhullError;
using orgQhull::QhullFacet;
using orgQhull::QhullFacetList;
using orgQhull::QhullQh;
using orgQhull::RboxPoints;
using orgQhull::QhullVertex;
using orgQhull::QhullVertexSet;


static clock_t start_time;
static clock_t stop_time;

void reset_timer() {

    start_time = 0;
    stop_time = 0;

}

bool start_timer() {

    reset_timer();
    start_time = clock();

    return true;
}

double stop_timer() {

    double timeDiff = 0.0;

    stop_time = clock();
    timeDiff = ((double)(stop_time - start_time)) / CLOCKS_PER_SEC;

    return timeDiff;
}

int main(int argc, char **argv)
{
    unsigned int POINT_MAX;
    const unsigned int DIM_MAX = 8;

    for(unsigned int dim = 2; dim <= DIM_MAX; dim++) {

        ostringstream ostream;
        ostream << "DIM" << dim << ".txt";
        ofstream file(ostream.str().c_str());
        if(file.is_open() != true) {

            cerr << "File " << ostream.str().c_str() << " could not be opened" << endl;
        }

        if(dim <6)
            POINT_MAX = 1000000;
        else
            POINT_MAX = 1000;

        // Zunehmende Punkte der konvexen Hülle
        for (unsigned int points = 0; points <= POINT_MAX; ) {

            RboxPoints rbox;
            Qhull qhull;
            ostringstream ostream;

            ostream << points << " " << "D" << dim;
            rbox.appendPoints(ostream.str().c_str());
            // Start Zeitmessung
            start_timer();
            qhull.runQhull(rbox, "");
            // Stop Zeitmessung
            double timeDiff = stop_timer();
            file << points << "\t" << timeDiff << endl;
            cout << "Time Diff: " << timeDiff << "\tHull: " << ostream.str().c_str() << endl;

            if(dim <6)
                points+=100000;
            else
                points+=100;
        }

    }

    return 0;
}// main

