#include <experimental/filesystem>

#include "MPCAlgorithm.hpp"
#include "simulation.hpp"
#include "timing.hpp"

namespace fs = std::experimental::filesystem;

fs::path getOutputPath() { return fs::path("..") / "output" / Model::getModelName(); }

// calculate an exponential moving average
double expMovingAverage(double previousAverage, double period, double newValue)
{
    const double factor = 2. / (period + 1.);
    const double result = (newValue - previousAverage) * factor + previousAverage;
    return result;
}

int main()
{
    auto model = std::make_shared<Model>();
    model->loadParameters();

    mpc::MPCAlgorithm solver(model);

    const double sim_time = 15.;

    solver.initialize(true, false);

    Model::state_vector_v_t X;
    Model::input_vector_v_t U;

    Model::state_vector_v_t X_sim;
    Model::input_vector_v_t U_sim;
    std::vector<double> times;

    Model::input_vector_t u;
    u.setZero();
    Model::state_vector_t x = model->p.x_init;

    solver.setFinalState(model->p.x_final);

    double t = 0.;
    size_t sim_step = 0;

    const double min_timestep = 0.010;
    double avg_solve_time = min_timestep;

    const double run_timer = tic();
    while (t < sim_time)
    {
        fmt::print("{:=^{}}\n", fmt::format("<SIMULATION STEP {}>", sim_step), 60);

        // forward estimation
        // Model::state_vector_t x_expected;
        // sim::simulate(model, avg_solve_time, x, u, u, x_expected);
        solver.setInitialState(x);

        X_sim.push_back(x);
        U_sim.push_back(u);
        times.push_back(t);

        // solve with current state
        const double t_start_solve = tic();
        solver.solve();
        const double solve_time = std::max(toc(t_start_solve) * 0.001, min_timestep);
        avg_solve_time = expMovingAverage(avg_solve_time, 20, solve_time);
        fmt::print("{:<{}}{:.2f}ms\n", "Average solve time:", 50, avg_solve_time * 1000);

        // move solve_time forward
        sim::simulate(model, solve_time, x, u, u, x);
        t += solve_time;

        // get the calculated input
        solver.getSolution(X, U);
        u = U.at(0);

        sim_step++;

        if ((x - model->p.x_final).norm() < 0.02)
        {
            break;
        }
    }
    fmt::print("\n");
    fmt::print("{:=^{}}\n", fmt::format("<SIMULATION FINISHED>"), 60);
    fmt::print("{:<{}}{:.2f}s\n", "Runtime:", 50, 0.001 * toc(run_timer));
    fmt::print("{:<{}}{:.2f}s\n", "Simulation time:", 50, t);
    const double freq = double(sim_step) / t;
    fmt::print("{:<{}}{:.2f}Hz\n", "Average frequency:", 50, freq);
    fmt::print("\n");

    // write solution to files
    double write_timer = tic();
    fs::path outputPath = getOutputPath() / std::to_string(0);
    if (not fs::exists(outputPath) and not fs::create_directories(outputPath))
    {
        throw std::runtime_error("Could not create output directory!");
    }

    const Eigen::IOFormat CSVFormat(Eigen::StreamPrecision,
                                    Eigen::DontAlignCols,
                                    ", ", "\n");

    {
        std::ofstream f(outputPath / "X_sim.txt");
        for (auto &x : X_sim)
        {
            f << x.transpose().format(CSVFormat) << "\n";
        }
    }
    {
        std::ofstream f(outputPath / "U_sim.txt");
        for (auto &u : U_sim)
        {
            f << u.transpose().format(CSVFormat) << "\n";
        }
    }
    {
        std::ofstream f(outputPath / "t_sim.txt");
        for (auto &t : times)
        {
            f << t << "\n";
        }
    }
    fmt::print("{:<{}}{:.2f}ms\n", "Time, solution files:", 50, toc(write_timer));
}