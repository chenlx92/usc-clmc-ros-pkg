/*********************************************************************
  Computational Learning and Motor Control Lab
  University of Southern California
  Prof. Stefan Schaal 
 *********************************************************************
  \remarks		...
 
  \file		task_recorder_manager.h

  \author	Peter Pastor
  \date		Jun 6, 2011

 *********************************************************************/

#ifndef TASK_RECORDER_MANAGER_H_
#define TASK_RECORDER_MANAGER_H_

// system includes
#include <vector>
#include <map>
#include <ros/ros.h>

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>

#include <task_recorder2_msgs/DataSample.h>

#include <task_recorder2/joint_states_recorder.h>
#include <task_recorder2/audio_recorder.h>
#include <task_recorder2/task_recorder_client.h>

#include <task_recorder2/StartStreaming.h>
#include <task_recorder2/StopStreaming.h>
#include <task_recorder2/StartRecording.h>
#include <task_recorder2/StopRecording.h>
#include <task_recorder2/InterruptRecording.h>
#include <task_recorder2/GetInfo.h>
#include <task_recorder2/GetDataSample.h>
#include <task_recorder2/AddDataSamples.h>
#include <task_recorder2/ReadDataSamples.h>

#include <task_recorder2_msgs/TaskRecorderSpecification.h>

// local includes
#include <task_recorder2/task_recorder_io.h>
#include <task_recorder2/task_recorder.h>

namespace task_recorder2
{

class TaskRecorderManager
{

public:

  /*! Constructor
   * @param node_handle
   */
  TaskRecorderManager(ros::NodeHandle node_handle);
  /*! Destructor
   */
  virtual ~TaskRecorderManager() {};

  /*!
   * @return True on success, False otherwise
   */
  bool initialize();

  /*!
   * @param task_recorders
   * @return True on success, otherwise False
   */
  virtual bool read(std::vector<boost::shared_ptr<task_recorder2::TaskRecorderBase> >& task_recorders)
  {
    return true;
  }

  /*!
   * @return the number of used task recorders
   */
  int getNumberOfTaskRecorders();

  /*!
   * @param request
   * @param response
   * @return True on success, False otherwise
   */
  bool startStreaming(task_recorder2::StartStreaming::Request& request,
                      task_recorder2::StartStreaming::Response& response);

  /*!
   * @param request
   * @param response
   * @return True on success, False otherwise
   */
  bool stopStreaming(task_recorder2::StopStreaming::Request& request,
                     task_recorder2::StopStreaming::Response& response);

  /*!
   * @param request
   * @param response
   * @return True on success, False otherwise
   */
  bool startRecording(task_recorder2::StartRecording::Request& request,
                      task_recorder2::StartRecording::Response& response);

  /*!
   * @param request
   * @param response
   * @return True on success, False otherwise
   */
  bool stopRecording(task_recorder2::StopRecording::Request& request,
                     task_recorder2::StopRecording::Response& response);

  /*!
   * @param request
   * @param response
   * @return True on success, False otherwise
   */
  bool interruptRecording(task_recorder2::InterruptRecording::Request& request,
                          task_recorder2::InterruptRecording::Response& response);

  /*!
   * @param request
   * @param response
   * @return True on success, False otherwise
   */
  bool getInfo(task_recorder2::GetInfo::Request& request,
               task_recorder2::GetInfo::Response& response);

  /*!
   * @param request
   * @param response
   * @return True on success, False otherwise
   */
  bool getDataSample(task_recorder2::GetDataSample::Request& request,
                     task_recorder2::GetDataSample::Response& response);

  /*!
   * @param request
   * @param response
   * @return True on success, False otherwise
   */
  bool addDataSamples(task_recorder2::AddDataSamples::Request& request,
                      task_recorder2::AddDataSamples::Response& response);

  /*!
   * @param request
   * @param response
   * @return True on success, False otherwise
   */
  bool readDataSamples(task_recorder2::ReadDataSamples::Request& request,
                       task_recorder2::ReadDataSamples::Response& response);

protected:

  /*!
   */
  bool initialized_;

  /*!
   */
  TaskRecorderIO<task_recorder2_msgs::DataSample> recorder_io_;

private:

  /*!
   */
  static const int DATA_SAMPLE_PUBLISHER_BUFFER_SIZE = 10000;

  double sampling_rate_;
  ros::Timer timer_;
  int counter_;

  /*! task recorders
   */
  std::vector<boost::shared_ptr<task_recorder2::TaskRecorderBase> > task_recorders_;
  /*! data samples
   */
  std::vector<task_recorder2_msgs::DataSample> data_samples_;

  task_recorder2_msgs::DataSample last_combined_data_sample_;
  boost::mutex last_combined_data_sample_mutex_;

  /*!
   */
  std::vector<task_recorder2::StartStreaming::Request> start_streaming_requests_;
  std::vector<task_recorder2::StartStreaming::Response> start_streaming_responses_;
  std::vector<task_recorder2::StopStreaming::Request> stop_streaming_requests_;
  std::vector<task_recorder2::StopStreaming::Response> stop_streaming_responses_;
  std::vector<task_recorder2::StartRecording::Request> start_recording_requests_;
  std::vector<task_recorder2::StartRecording::Response> start_recording_responses_;
  std::vector<task_recorder2::StopRecording::Request> stop_recording_requests_;
  std::vector<task_recorder2::StopRecording::Response> stop_recording_responses_;
  std::vector<task_recorder2::InterruptRecording::Request> interrupt_recording_requests_;
  std::vector<task_recorder2::InterruptRecording::Response> interrupt_recording_responses_;

  /*!
   */
  ros::Publisher data_sample_publisher_;
  ros::ServiceServer start_streaming_service_server_;
  ros::ServiceServer stop_streaming_service_server_;
  ros::ServiceServer start_recording_service_server_;
  ros::ServiceServer stop_recording_service_server_;
  ros::ServiceServer interrupt_recording_service_server_;
  ros::ServiceServer get_info_service_server_;
  ros::ServiceServer get_data_sample_service_server_;
  ros::ServiceServer add_data_samples_service_server_;
  ros::ServiceServer read_data_samples_service_server_;

  /*!
   */
  ros::Publisher stop_recording_publisher_;

  /*!
   * @param timer_event
   */
  void timerCB(const ros::TimerEvent& timer_event);

  /*!
   * @param time_stamp
   * @return True if last sample is updated, otherwise False
   */
  bool setLastDataSample(const ros::Time& time_stamp);

};

}

#endif /* TASK_RECORDER_MANAGER_H_ */
