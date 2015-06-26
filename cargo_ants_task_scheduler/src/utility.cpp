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

#include "ros/ros.h"
#include "cargo_ants_task_scheduler/utility.hpp"
#include <sfl/gplan/Mapper2d.hpp>
#include <sfl/util/numeric.hpp>

using namespace cargo_ants_msgs;


namespace task_scheduler {
  
  
  DistanceTransform::
  DistanceTransform (sfl::GridFrame const & world_to_estar,
		     estar_grid_t const & grid)
    : world_to_estar_ (world_to_estar)
  {
    estar_grid_init (&estar_grid_, grid.dimx, grid.dimy);
    memcpy (estar_grid_.cell, grid.cell, grid.dimx * grid.dimy);
  }
  
  
  DistanceTransform::
  ~DistanceTransform ()
  {
    estar_grid_fini (&estar_grid_);
  }
  
  
  double DistanceTransform::
  getGoalDistance (double robotx, double roboty, int * opt_errcode)
  {
    sfl::GridFrame::index_t const poseidx (world_to_estar_.GlobalIndex (robotx, roboty));
    if (poseidx.v0 < 0 || poseidx.v0 >= estar_grid_.dimx || 
	poseidx.v1 < 0 || poseidx.v1 >= estar_grid_.dimy) {
      if (opt_errcode) {
	*opt_errcode = NOT_IN_GRID;
      }
      return -1.0;
    }
    
    estar_cell_t * cell (estar_grid_at (&estar_grid_, poseidx.v0, poseidx.v1));
    if (cell->flags & ESTAR_FLAG_OBSTACLE) {
      if (opt_errcode) {
	*opt_errcode = IN_OBSTACLE;
      }
      return -1.0;
    }
    
    if (opt_errcode) {
      *opt_errcode = SUCCESS;
    }
    return cell->phi;
  }
  
  
  EstarHelper::
  EstarHelper ()
    : map_resolution_ (1.0),
      robot_radius_ (1.5),
      buffer_zone_ (3.0),
      decay_power_ (2.0),
      world_to_mapper2d_ (0.0, 0.0, 0.0, map_resolution_),
      world_to_estar_ (0.0, 0.0, 0.0, map_resolution_),
      estar_ (0)
  {
  }
  
  
  EstarHelper::
  ~EstarHelper ()
  {
    if (estar_) {
      estar_fini (estar_);
    }
    delete estar_;
  }
  
  
  void EstarHelper::
  readMap (MockupMap::ConstPtr const & site_map,
	   std::string const & world_name,
	   double x0, double y0, double x1, double y1)
  {
    boost::shared_ptr <sfl::Mapper2d>
      m2d (sfl::Mapper2d::Create (world_to_mapper2d_, robot_radius_, buffer_zone_, decay_power_));
    boost::shared_ptr <sfl::TraversabilityMap> travmap (m2d->GetTravmap());
    int const freespace (travmap->freespace);
    travmap->Autogrow (x0, y0, freespace);
    travmap->Autogrow (x1, y1, freespace);
    
    for (size_t ie (0); ie < site_map->entries.size(); ++ie) {
      MockupMapEntry const & entry (site_map->entries[ie]);
      if (entry.name == world_name) {
	for (size_t il (0); il < entry.x0.size(); ++il) {
	  if (il >= entry.y0.size() || il >= entry.x1.size() || il >= entry.y1.size()) {
	    ROS_WARN ("MockupMap entries have mismatching sizes");
	    break;
	  }
	  m2d->AddObstacleLine (entry.x0[il], entry.y0[il], entry.x1[il], entry.y1[il], false);
	}
      }
    }
    
    ROS_INFO ("created costmap %zd x %zd",
	      travmap->grid.xend() - travmap->grid.xbegin(),
	      travmap->grid.yend() - travmap->grid.ybegin());
    
    ssize_t const xbegin (travmap->grid.xbegin());
    ssize_t const ybegin (travmap->grid.ybegin());
    ssize_t const xend (travmap->grid.xend());
    ssize_t const yend (travmap->grid.yend());
    ssize_t const dimx (xend - xbegin);
    ssize_t const dimy (yend - ybegin);
    int const obstacle (travmap->obstacle);
    double const scale (1.0 / (obstacle - freespace));
    sfl::TraversabilityMap::grid_t const & grid (travmap->grid);
    
    if (estar_) {
      estar_fini (estar_);
    }
    else {
      estar_ = new estar_t();
    }
    estar_init (estar_, dimx, dimy);
    
    for (ssize_t ix (xbegin); ix < xend; ++ix) {
      for (ssize_t iy (ybegin); iy < yend; ++iy) {
	estar_set_speed (estar_, ix - xbegin, iy - ybegin,
			 sfl::boundval (0.0, scale * (obstacle - grid.at(ix, iy)), 1.0));
      }
    }
    
    ROS_INFO ("initialized E* speed map");
    
    sfl::GridFrame::position_t const org (world_to_mapper2d_.GlobalPoint (xbegin, ybegin));
    world_to_estar_.Configure (org.v0, org.v1,
			       world_to_mapper2d_.Theta(), world_to_mapper2d_.Delta());
  }
  
  
  DistanceTransform * EstarHelper::
  createDistanceTransform (double goalx, double goaly, int * opt_errcode)
  {
    if ( ! estar_) {
      if (opt_errcode) {
	*opt_errcode = NO_MAP;
      }
      return 0;
    }
    
    sfl::GridFrame::index_t const goal_estar (world_to_estar_.GlobalIndex (goalx, goaly));
    if (goal_estar.v0 < 0 || goal_estar.v0 >= estar_->grid.dimx || 
	goal_estar.v1 < 0 || goal_estar.v1 >= estar_->grid.dimy) {
      if (opt_errcode) {
	*opt_errcode = NOT_IN_GRID;
      }
      return 0;
    }
    
    ROS_INFO ("goal %g  %g  has E* indices %zd  %zd", goalx, goaly, goal_estar.v0, goal_estar.v1);
    estar_reset (estar_);
    estar_set_goal (estar_, goal_estar.v0, goal_estar.v1);
    while (estar_->pq.len != 0) {
      estar_propagate (estar_);
    }
    
    ROS_INFO ("computed distance transform from E* speed map");
    
    if (opt_errcode) {
      *opt_errcode = SUCCESS;
    }
    return new DistanceTransform (world_to_estar_, estar_->grid);
  }

}
