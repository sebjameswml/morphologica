#include <utility>
#include <iostream>
#include <unistd.h>
#include <cmath>

#include "Visual.h"
using morph::Visual;
#include "HexGridVisual.h"
using morph::HexGridVisual;
#include "ColourMap.h"
using morph::ColourMapType;
#include "tools.h"
using morph::Tools;
#include "HexGrid.h"
using morph::HexGrid;
using morph::HexDomainShape;
#include "ReadCurves.h"

using namespace std;

int main (int argc, char** argv)
{
    int rtn = 0;

    Visual v(1024, 768, "Ellipse");
    v.zNear = 0.001;
    v.showCoordArrows = false;

    bool holdVis = false;
    if (argc > 1) {
        string a1(argv[1]);
        if (a1.size() > 0) {
            holdVis = true;
        }
    }
    cout << "NB: Provide a cmd line arg (anything) to see the graphical window for this program" << endl;

    try {
        HexGrid hg(0.01, 3, 0, HexDomainShape::Boundary);
        hg.setCircularBoundary (1);

        cout << hg.extent() << endl;
        cout << "Number of hexes in grid:" << hg.num() << endl;
        cout << "Last vector index:" << hg.lastVectorIndex() << endl;

        if (hg.num() != 36624) {
            rtn = -1;
        }

        vector<float> data;
        unsigned int nhex = hg.num();
        data.resize(nhex, 0.0);

        // Make some dummy data (a sine wave)
        for (unsigned int hi=0; hi<nhex; ++hi) {
            data[hi] = 0.5 + 0.5*sin(hg.d_x[hi]); // Range 0->1
        }
        cout << "Created " << data.size() << " floats in data" << endl;

        array<float, 3> offset = { 0.0, 0.0, 0.0 };
        array<float, 4> scale = { 0.0, 0.0, 1.0, 0.0};
        v.addVisualModel (new HexGridVisual<float> (v.shaderprog, &hg, offset, &data, scale, ColourMapType::Rainbow));
        v.render();

        if (holdVis == true) {
            while (v.readyToFinish == false) {
                glfwWaitEventsTimeout (0.018);
                v.render();
            }
        }

    } catch (const exception& e) {
        cerr << "Caught exception reading svg: " << e.what() << endl;
        cerr << "Current working directory: " << Tools::getPwd() << endl;
        rtn = -1;
    }

    return rtn;
}
