#include "accelerator.h"
#include "accelerators/acc_force_v1.hpp"
#include "accelerators/acc_force_v2.hpp"
#include "accelerators/acc_force_v3.hpp"

#include <random>

auto l2norm = [=](float x, float y, float z)
{
    return x*x+y*y+z*z;
};


update_message make_random_update_message(std::mt19937 &rng)
{

    std::uniform_real_distribution<> udist;
    
    auto next_float=[&](float low, float high)
    {
        return udist(rng)*(high-low)+low;
    };

    auto next_uint32=[&]()
    {
        return rng();
    };

    auto next_sphere=[&](float &x, float &y, float &z, float scale)
    {
        while(1){
            x=next_float(-1,1);
            y=next_float(-1,1);
            z=next_float(-1,1);
            auto n=l2norm(x,y,z);
            if(n < 1){
                break;
            }
        }
        x*=scale;
        y*=scale;
        z*=scale;
    };

    const float dt=0.02f;
    const float r_c=1.0f;

    update_message res;

    res.i_pos_x=next_float(0.0f, 1.0f);
    res.i_pos_y=next_float(0.0f, 1.0f);
    res.i_pos_z=next_float(0.0f, 1.0f);
    // Position of bead j
    res.j_pos_x=next_float(-1.0f, 2.0f);
    res.j_pos_y=next_float(-1.0f, 2.0f);
    res.j_pos_z=next_float(-1.0f, 2.0f);
    // Velocity of bead i and j
     // Beads can have speed of at most 1/dt, as it means that within one time-step
     // they can move (1/dt)*dt = 1.
    next_sphere(res.i_vel_x, res.i_vel_y, res.i_vel_z, 1/dt);
    next_sphere(res.j_vel_x, res.j_vel_y, res.j_vel_z, 1/dt);
    
    // ID of bead i
    res.i_id=next_uint32();
    // ID of bead j
    res.j_id=next_uint32();
    // Square of eculidian distance between beads
    res.r_ij_dist_sq=l2norm(res.i_pos_x-res.j_pos_x, res.i_pos_y-res.j_pos_y, res.i_pos_z-res.j_pos_z);
    // Cutoff radius 
    res.r_c=r_c; // 4 bytes
    // Interaction between bead type of i and bead type of j
    res.a_ij=next_float(1,100); // TODO: This range is based on the ranges used in dpd.h
    // Drag coefficient constant
    res.drag_coef = next_float(1,10); // TODO: No idea on the range of this
    // Sigma constant used in random force 
    res.sigma_ij = next_float(10,200); // TODO: No idea on the range of this
    // Randomly generated number
    res.grand = next_uint32();
    // RAND_MAX / 2
    res.rand_max_div_2 = 4294967295/2;
    // Square root of the timestep (0.02)
    res.sqrt_dt = sqrtf(dt);

    return res;
}

template<class A, class B>
void compare(
    const char *prefix,
    unsigned n,
    A f_ref,
    B f_dut
){
    std::mt19937 rng;

    double sumAbsErr=0;
    double sumAbsErrSqr=0;
    double maxAbsErr=0;
    double sumRelErr=0;
    double sumRelErrSqr=0;
    double maxRelErr=0;

    for(unsigned i=0; i<n; i++){
        update_message in=make_random_update_message(rng);
        return_message r_ref=f_ref(&in);
        return_message r_dut=f_dut(&in);

        double err_abs=l2norm(r_ref.x-r_dut.x, r_ref.y-r_dut.y, r_ref.z-r_dut.z);
        double mag_ref=l2norm(r_ref.x, r_ref.y, r_ref.z);
        double err_rel=err_abs / mag_ref;

        sumAbsErr += err_abs;
        sumAbsErrSqr += err_abs*err_abs;
        maxAbsErr = std::max(maxAbsErr, err_abs);
        sumRelErr += err_rel;
        sumRelErrSqr += err_rel*err_rel;
        maxRelErr = std::max(maxRelErr, err_rel);
    }

    double avgAbsErr = sumAbsErr / n;
    double avgRelErr = sumRelErr / n;
    double rmseAbsErr = sqrt(sumAbsErrSqr / n);
    double rmseRelErr = sqrt(sumRelErrSqr / n);

    fprintf(stderr, "%s maxAbsE=%g, avgAbsE=%g, rmseAbsE=%g, maxRelE=%g, avgRelE=%g, rmseRelE=%g\n",
        prefix, maxAbsErr, avgAbsErr, rmseAbsErr,  maxRelErr, avgRelErr, rmseAbsErr
    );
}

int main()
{
    compare("sanity (self-check):", 10000, accelerator, accelerator);
    compare("acc_force_v1:", 10000, accelerator, acc_force_v1);
    compare("acc_force_v2:", 10000, accelerator, acc_force_v2);
    compare("acc_force_v3:", 10000, accelerator, acc_force_v3);
}
