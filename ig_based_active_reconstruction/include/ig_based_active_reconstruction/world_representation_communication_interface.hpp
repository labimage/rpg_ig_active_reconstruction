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

#include "movements/core"

namespace ig_based_active_reconstruction
{
  
namespace world_representation
{

  /*! Abstract interface definition for world representation modules plus data structures used
   * for communication.
   */
  class CommunicationInterface
  {
  public:
    /*! Returned status for a specific ig metric calculation
     */
    enum struct IgRetrievalStatus
    {
      SUCCEEDED, //! IG was successfully calculated.
      FAILED, //! IG could not be calculated, an error occured.
      UNKNOWN_METRIC //! The IG name was unknown and hence the IG could not be calculated
    };
    
    /*! Result of an information gain calculation call.
     */
    struct IgRetrievalResult
    {
      IgRetrievalStatus status; //! Status.
      double predicted_gain; //! Calculated information gain if the call succeeded, undefined otherwise.
    };
    
    /*! Command structure for information gain retrieval computation. The struct features a constructor that sets all members
     * to default values. See member descriptions for details.
     */
    struct IgRetrievalCommand
    {
    public:
      /*! Describes a subwindow
       */
      struct SubWindow
      {
	double min_x_perc, max_x_perc; //! x-coordinates-window through which rays will be cast. [percentage of image width, 0.0-1.0]
	double min_y_perc, max_y_perc; //! y-coordinates-window through which rays will be cast. [percentage of image height, 0.0-1.0]
      };
      
    public:
      /*! Constructor loads default values.
       */
      IgRetrievalCommand();
      
    public:      
      movements::PoseVector path; //! Describes the path for which the information gain shall be calculated. Note that in the current octomap-based implementation provided with the framework this is not yet implemented: Only the first pose will be considered and no casts into the future attempted.
      std::vector<std::string> metric_names; //! Vector with the names of all metrics that shall be calculated.
      
      double ray_resolution_x; //! How many rays are cast per pixel on the image's x-axis to obtain the information. [rays/px] Default: 1.0
      double ray_resolution_y; //! How many rays are cast per pixel on the image's y-axis to obtain the information. [rays/px] Default: 1.0
      uint32_t ray_step_size; //! Each i-th voxel on the ray will be included for calculation (if set to zero, 1 will be used instead as default). Default: 1
      
      SubWindow ray_window; //! Defines a subwindow of the image on which the rays shall be cast. Defaults to the complete window. Default: [0.0, 1.0] for both x- and y-coordinate windows, ie the complete image.
      
      double min_ray_depth; //! Minimal ray depth to start ig computation. [World representation units, usually m] Default: 0
      double max_ray_depth; //! Maximal ray depth for the ig computation. [World representation units, usually m] Default: 10.0
      
      double occupied_passthrough_threshold; //! If the occupancy likelihood of the end point of a ray is lower than this threshold, the ray is continued until either an endpoint is found that has a higher occupancy likelihood or the maximal ray depth is reached. Default: 0.0
    };
    
  public:
    virtual ~CommunicationInterface(){};
    
    /*! Calculates a set of information gains for a given view.
     * @param command Specifies which information gains have to be calculated and for which pose along with further parameters that define how the ig('s) will be collected.
     * @param output_ig (Output) Vector with the results of the information gain calculation. The indices correspond to the indices of the names in the metric_names array within the passed command.
     */
    virtual IgRetrievalStatus getViewInformationGain(IgRetrievalCommand& command, std::vector<IgRetrievalResult>& output_ig)=0;
  };
  
  
}


}