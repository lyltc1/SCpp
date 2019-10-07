<p align="center">
  <img width=500 src="https://user-images.githubusercontent.com/1352472/66313599-ef995f80-e912-11e9-85fd-fc18f0ed328d.png">
</p>

This library implements various optimal control algorithms that are particularly suited for aerospace applications.

## Guidance and Control Algorithms

 * Efficient Successive Convexification, a real-time guidance algorithm for optimal trajectory planning of constrained dynamical systems
 * Generic linear receding-horizon SOCP MPC algorithm
 * Linear Quadratic Regulator

## Features

 * JIT derivative code generation with CppAD
 * Intuitive interface to implement custom models
 * Rapid iteration with parameters files
 

## Current Models

 * Generic Rocket Model
 * SpaceX Starship Landing Model
 

## Dependencies

 * C++17
 * Eigen
 * Boost (odeint and ptree)
 * fmt (included as submodule)
 * ECOS (included as submodule)

## Instructions

### Install

``` 
git clone --recurse-submodules https://github.com/EmbersArc/SCpp.git
cd SCpp
mkdir build
cd build
cmake ..
make
```

### Run

Available executables are:

* **LQR_sim** to simulate a trajectory with the classic MPC controller

* **MPC_sim** to simulate a trajectory with the classic MPC controller

* **SC_oneshot** to calculate one trajectory with Successive Convexification

* **SC_sim** to simulate a trajectory with Successive Convexification

Calculated trajectories are written to the `output/<modelname>` directory.

### Create a Custom Model

See existing models in the `socp_mpc/models` folder for some examples.

## Papers

* [Successive Convexification: A Superlinearly Convergent Algorithm for Non-convex Optimal Control Problems](https://arxiv.org/abs/1804.06539)

* [Successive Convexification for 6-DoF Mars Rocket Powered Landing with Free-Final-Time](https://arxiv.org/abs/1802.03827)

## Examples

### Rocket Trajectory Model with Free-Final-Time

<p align="center">
  <img src="https://thumbs.gfycat.com/DeliriousCandidAldabratortoise-size_restricted.gif">
</p>

### SpaceX Starship Landing Trajectory

<p align="center">
  <img width="400" src="https://user-images.githubusercontent.com/1352472/66057427-f736be00-e538-11e9-8078-727282910f54.png">
</p>

### 2D Rocket Landing Problem

feed-forward input tested in a box2d physics simulation

<p align="center">
  <img width="300" src="https://thumbs.gfycat.com/DaringPortlyBlacklab-small.gif">
</p>


### Cartpole

<p align="center">
  <img src="https://thumbs.gfycat.com/KnobbyFlatCanvasback-small.gif">
</p>

## Contributing

I'm looking forward to contributions, both problem formulations and improvements to the core library.

