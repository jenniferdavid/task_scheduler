/* Cargo-ANTs software prototype.
 *
 * Copyright (C) 2014 Roland Philippsen. All rights reserved.
 *
 * BSD license:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of
 *    contributors to this software may be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR THE CONTRIBUTORS TO THIS SOFTWARE BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef CARGO_ANTS_TASK_SCHEDULER_UTILITY_HPP
#define CARGO_ANTS_TASK_SCHEDULER_UTILITY_HPP

#include <sfl/gplan/GridFrame.hpp>
#include <estar2/estar.h>
#include <cargo_ants_msgs/MockupMap.h>


namespace task_scheduler {
  
  
  class DistanceTransform
  {
  public:
    enum {
      SUCCESS,
      NOT_IN_GRID,
      IN_OBSTACLE
    };
    
    DistanceTransform (sfl::GridFrame const & world_to_estar,
		       estar_grid_t const & grid);
    ~DistanceTransform ();
    
    double getGoalDistance (double robotx, double roboty, int * opt_errcode = 0);
    
  protected:
    sfl::GridFrame const world_to_estar_;
    estar_grid_t estar_grid_;
  };
  
  
  class EstarHelper
  {
  public:
    enum {
      SUCCESS,
      NOT_IN_GRID,
      NO_MAP
    };
    
    EstarHelper ();
    ~EstarHelper ();
    
    void readMap (cargo_ants_msgs::MockupMap::ConstPtr const & site_map,
		  std::string const & world_name,
		  double x0, double y0, double x1, double y1);
    
    DistanceTransform * createDistanceTransform (double goalx, double goaly, int * opt_errcode);

  protected:
    double const map_resolution_;// (1.0);
    double const robot_radius_;// (1.5);
    double const buffer_zone_;// (3.0);
    double const decay_power_;// (2.0);
    sfl::GridFrame const world_to_mapper2d_;
    sfl::GridFrame world_to_estar_;
    estar_t * estar_;
  };
  
}

#endif // CARGO_ANTS_TASK_SCHEDULER_UTILITY_HPP
