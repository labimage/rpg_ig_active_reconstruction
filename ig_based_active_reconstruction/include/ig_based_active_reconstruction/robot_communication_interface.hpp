/* Copyright (c) 2015, Stefan Isler, islerstefan@bluewin.ch
*
This file is part of ig_based_active_reconstruction, a ROS package for...well,

ig_based_active_reconstruction is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
ig_based_active_reconstruction is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.
You should have received a copy of the GNU Lesser General Public License
along with ig_based_active_reconstruction. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "ig_based_active_reconstruction/view_space.hpp"
#include "ig_based_active_reconstruction/view.hpp"
#include "ig_based_active_reconstruction/robot_movement_cost.hpp"

namespace ig_based_active_reconstruction
{
  
namespace robot
{

/*! Abstract interface definition for robot interface implementations: Those can be communication units
 * (e.g. RosRemote and RosReceiver in the ig_based_active_reconstruction_ros package) or implementations that
 * forward the commands to a "robot" (e.g. FlyingStereoCamCommUnit in the flying_stereo_cam package)
 */
class CommunicationInterface
{
public:
  /*! Reception info
   */
  enum struct ReceptionInfo
  { 
    SUCCEEDED, 
    FAILED
  };
  
public:  
  virtual ~CommunicationInterface(){};
  
  /*! returns the current view */
  virtual View getCurrentView()=0;
  
  /*! Returns the view space that is available for planning (the idea is that it consists exclusively of poses that are considered to be reachable aforehand by the robot, considering its restraints
   * @param _space pointer to the ViewSpace object that should be filled
   * @return false if it failed or the robot does not provide such a service.
   */
  virtual bool getPlanningSpace( ViewSpace* _space )=0;
  
  /*! Commands robot to retrieve new data.
   * @return information about what happened (data received, receival failed )
   */
  virtual ReceptionInfo retrieveData()=0;
  
  /*! Returns the cost to move from the current view to the indicated view
   * @param _target_view the next view
   * @return cost to move to that view
   */
  virtual MovementCost movementCost( View& _target_view )=0;
  
  /*! returns the cost to move from start view to target view
   * @param _start_view the start view
   * @param _target_view the target view
   * @param _fill_additional_information if true then the different parts of the cost will be included in the additional fields as well
   * @return cost for the movement
   */
  virtual MovementCost movementCost( View& _start_view, View& _target_view, bool _fill_additional_information  )=0;
  
  /*! Tells the robot to get the camera to a new view
   * @param _target_view where to move to
   * @return false if the operation failed
   */
  virtual bool moveTo( View& _target_view )=0;
};

}

}