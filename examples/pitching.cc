// plunging - example of main routine for a pitching flat plate
//
// Clancy Rowley
// 30 Aug 2008
// Princeton University
//
// $Revision$
// $LastChangedDate$
// $LastChangedBy$
// $HeadURL$

#include <iostream>
#include "ibpm.h"

using namespace std;
using namespace ibpm;

int main(int argc, char* argv[]) {
    cout << "Pitching flat plate example\n";

    // lift and drag
    double lift = 0.;
    double drag = 0.;
    
    // Setup grid
    int nx = 100;
    int ny = 100;
    double length = 4.0;
    double xOffset = -1;
    double yOffset = -2;
    Grid grid( nx, ny, length, xOffset, yOffset );
    
    
    // Make a flat plate, length 1, with center at 1/4 chord
    RigidBody plate;
    int numPoints = 1. / grid.getDx();
    plate.addLine( 0, 0, 1, 0, numPoints );
    plate.setCenter( 0.25, 0 );
    
    // Set the motion to plunging: amplitude = 0.25, period 0.1 time unit
    double amplitude = 0.25;
    double freq = 0.1;
    PitchPlunge motion( amplitude, freq, 0, 0 );
    plate.setMotion( motion );
    Geometry geom;
    geom.addBody( plate );
    geom.moveBodies(0);

    // Setup equations to solve
    double Reynolds=10;
    double magnitude = 1;
    double alpha = 0;  // angle of background flow
    Flux q_potential = Flux::UniformFlow( grid, magnitude, alpha );
    cout << "Setting up Navier Stokes model..." << flush;
    NonlinearNavierStokes model( grid, geom, Reynolds, q_potential );
    // model.init();
    cout << "done" << endl;

    // Setup timestepper
    double dt = 0.005;
    // Euler solver(model, dt);
    RungeKutta2 solver(model, dt);

    // Build the state variable, zero initial conditions
    State x(grid, geom);
    x.gamma = 0.;

    // Setup output routines
    OutputTecplot tecplot( "tecplot/pitch%03d.plt", "Pitching plate, step %03d" );
    Logger logger;
    // Output Tecplot file every few timesteps
    logger.addOutput( &tecplot, 25 );
    logger.init();
    logger.doOutput( x );
    
    // Step
    const double PI = 4. * atan(1.);
    int numSteps = 250;
    for(int i=1; i <= numSteps; ++i) {
        double theta = amplitude * sin( 2 * PI * freq * x.time );
        cout << "step " << setw(4) << i
            << "  time = " << setw(5) << x.time
            << "  theta = " << theta << endl;
        solver.advance( x );
        computeNetForce(x.f, drag, lift);
        cout << " x force : " << setw(16) << drag/dt << " , y force : "
            << setw(16) << lift/dt << "\n";
        logger.doOutput( x );
    }
    logger.cleanup();
    return 0;
}