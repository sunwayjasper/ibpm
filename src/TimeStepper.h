#ifndef _TIMESTEPPER_H_
#define _TIMESTEPPER_H_

/*!
    \file TimeStepper.h
    \class TimeStepper

    \brief Abstract base class for advancing a flow field forward in time
    
    The governing equations are in the form
    \f{align}
       \frac{d\gamma}{dt} + Bf &= L\gamma + N(q)\\
       C\gamma &= b
    \f}
    where \f$ \gamma \f$ is a Scalar and \f$ f \f$ and \f$ b \f$ are
    BoundaryVectors.  The operators B, C, L, and N are determined by an
    associated NavierStokesModel.
    
    \author Clancy Rowley
    \author $LastChangedBy$
    \date  2 Aug 2008
    \date $LastChangedDate$
    \version $Revision$
*/

class TimeStepper {

//
// Public methods
//
public:
    
/*! \brief Setup all routines necessary to step the solution forward in time.

Note that creation of the ProjectionSolver should be deferred to the
subclasses, but determination of which type of solver to instantiate should be
handled by the base class.
\param[in] model    The associated NavierStokesModel instance
\param[in] timestep Timestep to use for the advance() routine
*/
TimeStepper(const NavierStokesModel& model, double timestep);

/*! \brief Advance the state forward in time.

Pure virtual method: must be overridden by subclasses
\param[inout] x State vector to be stepped forward in time
*/
virtual void advance(State& x) = 0;

//
// Protected methods
//
protected:
    
/*! \brief Return a new ProjectionSolver of the appropriate type

\param[in] alpha    Parameter passed to the appropriate ProjectionSolver (usually the timestep)
*/
ProjectionSolver* createSolver(double alpha);


//
// Protected data
//
	double _timestep;
	const NavierStokesModel& _model;
	
}


#endif /* _TIMESTEPPER_H_ */
